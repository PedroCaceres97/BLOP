#define OPENAI_IMPLEMENTATION
#include <blop/openai.h>

#include <stdio.h>

/* -------------------------------------------------------------------------- */
/* Example tool: add two numbers                                              */
/* -------------------------------------------------------------------------- */

void chat(openai* api, const char* message) {
  cJSON* user = cJSON_CreateObject();
  ASSERT_PTR(user); 
  
  cJSON_AddStringToObject(user, "role", "user");
  cJSON_AddStringToObject(user, "content", message);

  cJSON* response = openai_send(api, user);
  cJSON* content = cJSON_GetObjectItem(response, "content");

  char* out = cJSON_Print(content);
  printf("AI: %s\n", out);
  FREE(out);
}

/* -------------------------------------------------------------------------- */
/* Main                                                                        */
/* -------------------------------------------------------------------------- */

int main(void) {
  curl_global_init(CURL_GLOBAL_DEFAULT);

  /* ---------------------------------------------------------------------- */
  /* Create API                                                             */
  /* ---------------------------------------------------------------------- */

  struct_openai_config cfg = {
    .backend            = LLAMA_CPP_API,

    .url                = "http://127.0.0.1:8080/v1/chat/completions",
    .model              = "local-model",
    .api_key            = NULL,
    .x_title            = NULL,
    .http_referer       = NULL,
    .cainfo_path        = "C:/Dev/Libs/curl-8.17.0_5-win64-mingw/cacert.pem",

    .seed               = -1,
    .reasoning          = false,
    .max_history        = 15,   
    .max_remove_idx     = 1,    
    .max_tokens         = 200,  
    .timeout_ms         = 60000,
    .temperature        = 0.5f, 
    .top_p              = 0.0f, 
    .presence_penalty   = 0,    
    .frequency_penalty  = 0,    
    .stop               = NULL, 
    .response_format    = NULL, 
    .logit_bias         = NULL  
  };

  struct_openai* api = fn_openai_create(NULL, cfg);

  /* ---------------------------------------------------------------------- */
  /* Attach tool                                                             */
  /* ---------------------------------------------------------------------- */

/*
  struct_openai_tool say = {0};
  openai_tool_create(&say, "say_to_system", "Prints a message to the system", system_say);
  openai_tool_add_param(&say, "msg", "string");
  openai_tool_attach(api, 0, say);
*/

  /* ---------------------------------------------------------------------- */
  /* System message                                                          */
  /* ---------------------------------------------------------------------- */

  cJSON* system = cJSON_CreateObject();
  cJSON_AddStringToObject(system, "role", "system");
  cJSON_AddStringToObject(
    system,
    "content",
    "You are a helpful and funny assistant. You only respond with plain text (No markdown, XML, no nothing and no special characters )."
  );

  openai_history_insert(api, 0, system);

  /* ---------------------------------------------------------------------- */
  /* User message                                                            */
  /* ---------------------------------------------------------------------- */

  char user[256] = {0};
  
  while(true) {
    printf("PROMPT: ");
    gets(user);
    if (strcmp(user, "exit") == 0) {
      break;
    }
    chat(api, user);
  }

  /* ---------------------------------------------------------------------- */
  /* Cleanup                                                                 */
  /* ---------------------------------------------------------------------- */

  openai_destroy(api);
  curl_global_cleanup();

  return 0;
}