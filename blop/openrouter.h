#ifndef __BLOP_OPENROUTER_H__
#define __BLOP_OPENROUTER_H__

#ifndef OPENROUTER_NAME
  #define OPENROUTER_NAME orapi
#endif /* OPENROUTER_NAME */

#ifndef OPENROUTER_FN_PREFIX
  #define OPENROUTER_FN_PREFIX OPENROUTER_NAME  
#endif /* OPENROUTER_FN_PREFIX */

#define struct_openrouter OPENROUTER_NAME

#define fn_openrouter_create  CONCAT2(OPENROUTER_FN_PREFIX, _create)
#define fn_openrouter_destroy CONCAT2(OPENROUTER_FN_PREFIX, _destroy)
#define fn_openrouter_chat    CONCAT2(OPENROUTER_FN_PREFIX, _chat)

#ifdef __cplusplus
extern "C" {
#endif

struct struct_openrouter;
typedef struct struct_openrouter struct_openrouter;

struct_openrouter* fn_openrouter_create(struct_openrouter* openr);
void               fn_openrouter_destroy(struct_openrouter* openr);
const char*        fn_openrouter_chat(struct_openrouter* openr);

struct struct_openrouter {
  int allocated;

  CURL* curl;
  struct curl_slist* headers;

  cJSON* root;
  cJSON* messages;

  char* last;
  char* response;
  size_t response_size;
};

#ifdef OPENROUTER_IMPLEMENTATION

static size_t curl_write_cb(void* data, size_t size, size_t nmemb, void* userp) {
  ASSERT_PTR(userp);
  ASSERT_PTR(data);

  OR_API* api = userp;

  size_t total = size * nmemb;
  
  char* tmp = NULL;
  REALLOC(tmp, char, api->response, api->response_size + total + 1);

  api->response = tmp;
  memcpy(api->response + api->response_size, data, total);
  api->response_size += total;
  api->response[api->response_size] = '\0';

  return total;
}

struct_openrouter* fn_openrouter_create(struct_openrouter* openr) {
  OR_API* api = NULL;
  CALLOC(api, OR_API, 1);

  if (curl_global_init(CURL_GLOBAL_ALL) != 0) {
    LOG_FATAL("curl_global_init failed");
    FREE(api);
    ABORT();
  }

  api->curl = curl_easy_init();
  if (!api->curl) {
    LOG_FATAL("curl_easy_init failed");
    goto fail;
  }

  /* Headers */
  char auth[512];
  int n = snprintf(auth, sizeof(auth), "Authorization: Bearer %s", api_key);
  if (n < 0 || n >= (int)sizeof(auth)) {
    LOG_FATAL("Authorization header truncated");
    goto fail;
  }

#define APPEND_HDR(h) do { api->headers = curl_slist_append(api->headers, h); if (!api->headers) { LOG_FATALF("curl_slist_append failed: %s", h); goto fail; } } while(false)

  api->headers = NULL;
  APPEND_HDR(auth);
  APPEND_HDR("Content-Type: application/json");
  APPEND_HDR("HTTP-Referer: OR-API");
  APPEND_HDR("X-Title: OR-API");

  api->root = cJSON_CreateObject();
  if (!api->root) {
    LOG_FATAL("cJSON_CreateObject failed");
    goto fail;
  }

  if (!cJSON_AddStringToObject(api->root, "model", model)) {
    LOG_FATAL("Failed to add model");
    goto fail;
  }

  api->messages = cJSON_AddArrayToObject(api->root, "messages");
  if (!api->messages) {
    LOG_FATAL("Failed to create messages array");
    goto fail;
  }

  cJSON* sys = cJSON_CreateObject();
  if (!sys) {
    LOG_FATAL("Failed to create system message");
    goto fail;
  }

  cJSON_AddStringToObject(sys, "role", "system");
  cJSON_AddStringToObject(sys, "content", "You are a helpful assistant.");
  cJSON_AddItemToArray(api->messages, sys);

  curl_easy_setopt(api->curl, CURLOPT_URL, "https://openrouter.ai/api/v1/chat/completions");
  curl_easy_setopt(api->curl, CURLOPT_HTTPHEADER, api->headers);
  curl_easy_setopt(api->curl, CURLOPT_POST, 1L);
  curl_easy_setopt(api->curl, CURLOPT_WRITEFUNCTION, write_cb);
  curl_easy_setopt(api->curl, CURLOPT_WRITEDATA, api);
  curl_easy_setopt(api->curl, CURLOPT_CAINFO, "cacert.pem");

  return api;

fail:
    or_destroy(api);
    ABORT();
}

DLL_EXPORT const char* or_chat(OR_API* api, const char* user_text) {
  ASSERT_PTR(api);
  ASSERT_PTR(user_text);

  free(api->response);
  api->response = NULL;
  api->response_size = 0;

  cJSON* user = cJSON_CreateObject();
  ASSERT_FORCED(user != NULL, "Failed to create user message");

  cJSON_AddStringToObject(user, "role", "user");
  cJSON_AddStringToObject(user, "content", user_text);
  cJSON_AddItemToArray(api->messages, user);

  char* payload = cJSON_PrintUnformatted(api->root);
  ASSERT_FORCED(payload != NULL, "cJSON_PrintUnformatted failed");
  curl_easy_setopt(api->curl, CURLOPT_POSTFIELDS, payload);

  CURLcode res = curl_easy_perform(api->curl);
  free(payload);

  if (res != CURLE_OK) {
    LOG_FATALF("curl_easy_perform failed: %s", curl_easy_strerror(res));
    ABORT();
  }

  long http_code = 0;
  curl_easy_getinfo(api->curl, CURLINFO_RESPONSE_CODE, &http_code);

  if (http_code != 200) {
    LOG_FATALF("HTTP %ld: %s", http_code, api->response ? api->response : "<no body>");
    ABORT();
  }

  ASSERT_FORCED(api->response && api->response_size > 0, "Empty response body");

  cJSON* json = cJSON_Parse(api->response);
  if (!json) {
    LOG_FATALF("JSON parse failed: %s", api->response);
    ABORT();
  }

  cJSON* err = cJSON_GetObjectItemCaseSensitive(json, "error");
  if (err) {
    cJSON* msg = cJSON_GetObjectItemCaseSensitive(err, "message");
    LOG_FATALF("OpenRouter error: %s", cJSON_IsString(msg) ? msg->valuestring : "<unknown>");
    cJSON_Delete(json);
    ABORT();
  }

  cJSON* choices = cJSON_GetObjectItemCaseSensitive(json, "choices");
  ASSERT_FORCED(choices && cJSON_IsArray(choices), "`choices` missing or not array");
  
  ASSERT_FORCED(cJSON_GetArraySize(choices) > 0, "`choices` array empty");
  
  cJSON* choice0 = cJSON_GetArrayItem(choices, 0);
  ASSERT_FORCED(choice0 && cJSON_IsObject(choice0), "`choices[0]` invalid");
  
  cJSON* message = cJSON_GetObjectItemCaseSensitive(choice0, "message");
  ASSERT_FORCED(message && cJSON_IsObject(message), "`message` missing");
  
  cJSON* content = cJSON_GetObjectItemCaseSensitive(message, "content");
  ASSERT_FORCED(content && cJSON_IsString(content), "`content` missing or not string");


  const char* reply = content->valuestring;

  if (api->last) {
    free(api->last);
  }
  api->last = strdup(reply);

  cJSON* assistant = cJSON_CreateObject();
  cJSON_AddStringToObject(assistant, "role", "assistant");
  cJSON_AddStringToObject(assistant, "content", reply);
  cJSON_AddItemToArray(api->messages, assistant);

  cJSON_Delete(json);
  return api->last;
}

DLL_EXPORT void or_destroy(OR_API* api) {
  ASSERT_PTR(api);

  curl_easy_cleanup(api->curl);
  curl_slist_free_all(api->headers);
  cJSON_Delete(api->root);
  if (api->last) {
    free(api->last);
  }
  free(api->response);
  free(api);

  curl_global_cleanup();
}

#endif /* OPENROUTER_IMPLEMENTATION */

#ifdef __cplusplus
}
#endif

#endif /* __BLOP_OPENROUTER_H__ */