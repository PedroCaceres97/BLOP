/*!
 * \file openai.h
 * \brief High-level, portable C wrapper for LLM chat completions with tool support
 *
 * ===========================================================================
 * Overview
 * ===========================================================================
 *
 * This header provides a compact, assertion-heavy API to interact with a variety
 * of chat-completion backends (OpenAI API, OpenRouter, and llama.cpp compatible
 * endpoints). The goal is to supply a simple C-friendly interface that builds
 * JSON payloads, manages a short "conversation history", exposes a lightweight
 * "tool" system (functions that can be called by the model), and performs
 * HTTP communication through libcurl. The implementation includes defensive
 * checks (assertions) to catch and fail fast on invalid state or runtime
 * errors.
 *
 * The file is intentionally self-contained as a single-header library when
 * compiled with OPENAI_IMPLEMENTATION defined. It depends on the following
 * third-party libraries (make sure they are available at link-time):
 *  - libcurl (curl_easy_init / curl_easy_perform)
 *  - cJSON (JSON building / parsing)
 *  - The "blop" macro/assertion framework (blop/blop.h) for allocation and
 *    assertion helpers (CALLOC, REALLOC, FREE, BLOP_ASSERT_PTR, etc.).
 *
 * The design follows a simple pattern:
 *  - struct_openai_config collects options and feature flags.
 *  - fn_openai_create builds a base JSON request object (api->root) and sets
 *    defaults. It also initializes curl and headers appropriate for the
 *    selected backend.
 *  - fn_openai_send repeatedly sends api->root until a non-tool response is
 *    received. If the model requests tool calls, the library dispatches the
 *    calls to attached tools, posts their results back into history, and
 *    re-sends the request.
 *  - Tools are lightweight structs providing a name, description, a JSON
 *    parameters object (JSON schema) and a callback (type_openai_tool_fn) that
 *    accepts JSON arguments and returns a newly allocated JSON result.
 *
 * ===========================================================================
 * Key Concepts & Terminology
 * ===========================================================================
 *
 * - "api" / struct_openai: The main handle. Holds curl state, headers, the
 *   request JSON tree (api->root), the local in-memory conversation (api->messages),
 *   tool metadata and configuration.
 *
 * - "tools": Small C callbacks registered into the API. The model can request
 *   to call a tool, providing arguments as JSON. The callback must return a
 *   newly allocated cJSON* that will be taken by the API and posted back to
 *   the model as a tool response. The API deletes the returned cJSON* after use.
 *
 * - "history": An array of messages sent and received. Bounded by
 *   config.max_history and pruned based on config.max_remove_idx.
 *
 * - "backend": Implementation-specific behavior for headers, argument parsing
 *   and whether the backend supports "seed". Backends are pluggable through
 *   a small ops table struct_openai_backend_ops.
 *
 * ===========================================================================
 * Error handling policy
 * ===========================================================================
 *
 * This library follows a fail-fast policy using the "blop" assertion macros.
 * All allocation failures, libcurl errors, JSON creation/parsing errors, and
 * invalid usage states are asserted so the program terminates on misbehavior.
 * The rationale is to keep the code paths simple and avoid complex error
 * recovery. If you prefer to convert assertions into error returns, remove or
 * adapt the BLOP_ASSERT_* macros accordingly.
 *
 * ===========================================================================
 * JSON layout used by the API (conceptual)
 * ===========================================================================
 *
 * api->root   : root JSON for the outgoing request. Typical fields:
 *    - model
 *    - temperature, top_p, max_tokens, seed (if supported)
 *    - tools   : array of available tools (each entry describes the function and its parameters)
 *    - messages: array of message objects forming the conversation history
 *
 * message object (within api->messages):
 *    {
 *      "role": "system"|"user"|"assistant"|"tool",
 *      "content": "..." | cJSON object/array, // content may be string or structured
 *      // tool messages may include: "tool_call_id": "..." , "name": "..."
 *    }
 *
 * Tool function contract:
 *   - Accepts a cJSON* 'args' which the backend parsing code prepares from the
 *     value supplied by the model. If 'args' is NULL or malformed, the tool
 *     should handle it gracefully or assert, per your policy.
 *   - Must return a newly allocated cJSON* describing the result. Ownership is
 *     transferred to the API which will delete it after posting.
 *
 * ===========================================================================
 * Configuration and Defaults
 * ===========================================================================
 *
 * The struct_openai_config fields control behavior. Important defaults:
 *  - url: per-backend default set if left NULL
 *  - model: REQUIRED (asserted)
 *  - api_key: REQUIRED for OpenAI and OpenRouter backends (asserted)
 *  - max_history: default 15
 *  - max_remove_idx: default 1 (which allows preserving a system message at idx 0)
 *  - max_tokens: default 200 (-1 to omit sending max_tokens)
 *  - temperature: default 0.5f
 *  - top_p: default 0.5f
 *  - seed: ignored if backend does not support it
 *
 * ===========================================================================
 * Backend specifics
 * ===========================================================================
 *
 * Each supported backend implements a small set of operations declared in
 * struct_openai_backend_ops:
 *  - init_request(struct_openai*): optional, prepare per-request modifications
 *  - init_headers(struct_openai*): required to populate api->headers
 *  - parse_tool_args(cJSON* raw, cJSON** out): parse an argument value from the
 *      model into a cJSON* object the tool can consume
 *  - push_tool_result(api, id, name, result): what to do when a tool returns
 *  - supports_seed(): whether the backend accepts a numeric "seed" param
 *
 * Implemented backends:
 *  - OPENAI_API
 *    - Default url: https://api.openai.com/v1/chat/completions
 *    - Expects API key, sets Authorization header
 *    - parse_tool_args: expects stringified JSON arguments
 *  - OPENROUTER_API
 *    - Default url: https://openrouter.ai/api/v1/chat/completions
 *    - Sets custom headers X-Title and HTTP-Referer if provided
 *    - parse_tool_args: accepts object or string
 *  - LLAMA_CPP_API
 *    - Backend for llama.cpp-like HTTP endpoints
 *    - Does not require API key but URL must be explicitly provided
 *    - parse_tool_args: expects stringified JSON
 *
 * ===========================================================================
 * Memory management notes
 * ===========================================================================
 *
 * - The API duplicates or creates cJSON structures when constructing requests
 *   and when inserting messages into history. The API frees those objects
 *   within fn_openai_destroy or when they are pruned from history.
 * - Tool callbacks must return a freshly allocated cJSON* (ownership transferred).
 * - The API keeps an internal char* response buffer that is grown with REALLOC
 *   as the HTTP body is received.
 * - On assertion failures the program will terminate; therefore leaking memory
 *   on assertion paths is acceptable for this project's error policy.
 *
 * ===========================================================================
 * Typical usage example (pseudo-C)
 * ===========================================================================
 *
 *  struct_openai_config cfg = {0};
 *  cfg.backend = OPENAI_API;
 *  cfg.api_key = getenv("OPENAI_API_KEY");
 *  cfg.model = "gpt-4o-mini";
 *  cfg.max_history = 20;
 *
 *  struct_openai* api = fn_openai_create(NULL, cfg);
 *  // build a user message
 *  cJSON* msg = cJSON_CreateObject();
 *  cJSON_AddStringToObject(msg, "role", "user");
 *  cJSON_AddStringToObject(msg, "content", "Say hello");
 *  cJSON* reply = fn_openai_send(api, msg); // ownership of msg transferred
 *  // inspect reply (string or structured content)
 *  fn_openai_destroy(api);
 * 
 * ===========================================================================
 * Quick full struct
 * ===========================================================================
 * 
 *  struct_openai_config cfg = {
 *    .backend            = LLAMA_CPP_API,
 *    .url                = "http://127.0.0.1:8080/v1/chat/completions",
 *    .model              = "local-model",
 *    .api_key            = NULL,
 *    .x_title            = NULL,
 *    .http_referer       = NULL,
 *    .cainfo_path        = NULL,
 *    .seed               = -1,
 *    .reasoning          = false,
 *    .max_history        = 15,   
 *    .max_remove_idx     = 1,    
 *    .max_tokens         = 200,  
 *    .timeout_ms         = 60000,
 *    .temperature        = 0.5f, 
 *    .top_p              = 0.0f, 
 *    .presence_penalty   = 0,    
 *    .frequency_penalty  = 0,    
 *    .stop               = NULL, 
 *    .response_format    = NULL, 
 *    .logit_bias         = NULL  
 *  };
 *
 * ===========================================================================
 * Tools example (pseudo-C)
 * ===========================================================================
 *
 *  // Define a tool that echoes incoming args under the key 'echoed'
 *  cJSON* my_tool_fn(cJSON* args) {
 *    cJSON* out = cJSON_CreateObject();
 *    cJSON_AddItemToObject(out, "echoed", cJSON_Duplicate(args, 1));
 *    return out; // returned object will be freed by the API
 *  }
 *
 *  // Attach tool
 *  struct_openai_tool tool;
 *  fn_openai_tool_create(&tool, "echo_tool", "Echo supplied arguments", my_tool_fn);
 *  fn_openai_tool_add_param(&tool, "text", "string");
 *  fn_openai_tool_attach(api, 0, tool);
 *
 * ===========================================================================
 * Limitations & Caveats
 * ===========================================================================
 *
 * - Not thread-safe by itself. Caller must serialize access when using the
 *   same api handle from multiple threads.
 * - Error handling is assert-based by design. If you want to convert it to
 *   a return-code style API that gracefully reports errors, you will need to
 *   remove or adapt the assertion macros.
 * - The header assumes that cJSON and libcurl are compiled and linked into
 *   the final program; make sure your build system links against -lcurl and
 *   the cJSON object file/library.
 *
 * ===========================================================================
 * Implementation notes & guidelines for contributors
 * ===========================================================================
 *
 * - All allocation and error checks should use the BLOP_* and ASSERT_* macros
 *   to keep the library consistent with project-wide policies.
 * - When adding a new backend, implement a struct_openai_backend_ops and
 *   populate openai_ops appropriately in fn_openai_create.
 * - When modifying the tool format, ensure openai_push_tool constructs a
 *   stable, validated JSON schema for the backend to include in the "tools"
 *   array.
 *
 * ===========================================================================
 * Change log (high level)
 * ===========================================================================
 * - 2026-01: Added thorough assertion coverage and defensive checks for JSON
 *   operations and curl option results. Documented API usage and contracts.
 * - Original: Single-header chat-completion wrapper with tool dispatch.
 *
 * ===========================================================================
 */

#ifndef __BLOP_OPENAI_H__
#define __BLOP_OPENAI_H__

#include <blop/blop.h>
#include <curl/curl.h>
#include <cJSON/cJSON.h>

#ifndef OPENAI_NAME
  #define OPENAI_NAME openai
#endif /* OPENAI_NAME */

#ifndef OPENAI_FN_PREFIX
  #define OPENAI_FN_PREFIX OPENAI_NAME
#endif /* OPENAI_FN_PREFIX */

#ifndef OPENAI_TOOL_COUNT
  #define OPENAI_TOOL_COUNT 32
#endif /* OPENAI_TOOL_COUNT */

#define struct_openai             OPENAI_NAME
#define struct_openai_tool        CONCAT2(OPENAI_NAME, _tool)
#define struct_openai_config      CONCAT2(OPENAI_NAME, _config)
#define struct_openai_backend_ops CONCAT2(OPENAI_NAME, _backend_ops)

#define enum_openai_backend       CONCAT2(OPENAI_NAME, _backend)

#define type_openai_tool_fn       CONCAT2(OPENAI_NAME, _tool_fn)

#define fn_openai_create          CONCAT2(OPENAI_FN_PREFIX, _create)
#define fn_openai_destroy         CONCAT2(OPENAI_FN_PREFIX, _destroy)
#define fn_openai_send            CONCAT2(OPENAI_FN_PREFIX, _send)

#define fn_openai_rdlock          CONCAT2(OPENAI_FN_PREFIX, _rdlock)
#define fn_openai_wrlock          CONCAT2(OPENAI_FN_PREFIX, _wrlock)
#define fn_openai_rdunlock        CONCAT2(OPENAI_FN_PREFIX, _rdunlock)
#define fn_openai_wrunlock        CONCAT2(OPENAI_FN_PREFIX, _wrunlock)

#define fn_openai_tool_create     CONCAT2(OPENAI_FN_PREFIX, _tool_create)
#define fn_openai_tool_add_param  CONCAT2(OPENAI_FN_PREFIX, _tool_add_param)
#define fn_openai_tool_attach     CONCAT2(OPENAI_FN_PREFIX, _tool_attach)
#define fn_openai_tool_deattach   CONCAT2(OPENAI_FN_PREFIX, _tool_deattach)

#define fn_openai_history_set     CONCAT2(OPENAI_FN_PREFIX, _history_set)
#define fn_openai_history_get     CONCAT2(OPENAI_FN_PREFIX, _history_get)
#define fn_openai_history_erase   CONCAT2(OPENAI_FN_PREFIX, _history_erase)
#define fn_openai_history_insert  CONCAT2(OPENAI_FN_PREFIX, _history_insert)
#define fn_openai_history_size    CONCAT2(OPENAI_FN_PREFIX, _history_size)

#ifdef __cplusplus
extern "C" {
#endif

typedef enum enum_openai_backend {
  OPENAI_API,
  OPENROUTER_API,
  LLAMA_CPP_API
} enum_openai_backend;

struct struct_openai;
struct struct_openai_tool;
struct struct_openai_config;
struct struct_openai_backend_ops;
typedef struct struct_openai struct_openai;
typedef struct struct_openai_tool struct_openai_tool;
typedef struct struct_openai_config struct_openai_config;
typedef struct struct_openai_backend_ops struct_openai_backend_ops;

/* 
 * Tool function must return a newly allocated cJSON*.
 * Ownership is transferred to Openai and it will be deleted.
 * Do not cJSON_Delete(args)
 */
typedef cJSON* (*type_openai_tool_fn)(cJSON* args);

struct_openai*      fn_openai_create          (struct_openai* api, struct_openai_config config);
void                fn_openai_destroy         (struct_openai* api);
cJSON*              fn_openai_send            (struct_openai* api, cJSON* message);

void                fn_openai_rdlock          (struct_openai* api);
void                fn_openai_wrlock          (struct_openai* api);
void                fn_openai_rdunlock        (struct_openai* api);
void                fn_openai_wrunlock        (struct_openai* api);

void                fn_openai_tool_create     (struct_openai_tool* tool, const char* name, const char* description, type_openai_tool_fn fn);
void                fn_openai_tool_add_param  (struct_openai_tool* tool, const char* name, const char* type);
void                fn_openai_tool_attach     (struct_openai* api, size_t idx, struct_openai_tool tool);
void                fn_openai_tool_deattach   (struct_openai* api, size_t idx);

void                fn_openai_history_set     (struct_openai* api, size_t idx, cJSON* message);
cJSON*              fn_openai_history_get     (struct_openai* api, size_t idx);
void                fn_openai_history_erase   (struct_openai* api, size_t idx);
void                fn_openai_history_insert  (struct_openai* api, size_t idx, cJSON* message);
int                 fn_openai_history_size    (struct_openai* api);

struct struct_openai_backend_ops {
  void (*init_request)(struct_openai*);
  void (*init_headers)(struct_openai*);
  int  (*parse_tool_args)(cJSON* raw, cJSON** out);
  void (*push_tool_result)(struct_openai*, const char*, const char*, cJSON*);
  int  (*supports_seed)(void);
};

struct struct_openai_config {
  enum_openai_backend backend;

  const char* url;          /* API endpoint; default provided below */
  const char* model;        /* model id */
  const char* api_key;      /* required */
  const char* x_title;      /* optional X-Title header */
  const char* http_referer; /* optional Referer header */
  const char* cainfo_path;  /* optional path to cacert.pem */

  int   seed;               /* -1 = unset */
  int   reasoning;          /* -1 = no data send */
  int   max_history;        /* -1     = default (15 messages) */
  int   max_remove_idx;     /* idx to remove message when history gets to its max, this allows to keep system message without weird logic */
  int   max_tokens;         /* 0      = default (200 tokens) | -1 = no data send*/
  long  timeout_ms;         /*  0     = default (no explicit timeout) */
  float temperature;        /* -1.0f  = default (0.5f) */
  float top_p;              /* -1.0f  = default (0.5f) */
  float presence_penalty;   /*  0     = no effect */
  float frequency_penalty;  /*  0     = no effect */
  cJSON* stop;              /* array or string (owned by caller) */
  cJSON* response_format;   /* JSON schema / type */
  cJSON* logit_bias;        /* object */
};

struct struct_openai_tool {
  int         in_use;
  const char* name;
  const char* description;
  cJSON*      parameters;   /* JSON Schema */
  type_openai_tool_fn fn;
};

struct struct_openai {
  int allocated;
  RWLOCK_TYPE lock;

  cJSON* root;
  cJSON* tools;
  cJSON* messages;

  CURL* curl;
  struct curl_slist* headers;

  char* response;
  size_t response_size;

  struct_openai_tool attached[OPENAI_TOOL_COUNT];
  struct_openai_config config;
};

#ifdef OPENAI_IMPLEMENTATION

/* --------------------------------------------------------------------------
 * Default macros
 * -------------------------------------------------------------------------- */

#define OPENAI_DEFAULT_URL          "https://api.openai.com/v1/chat/completions"
#define OPENAI_DEFAULT_HISTORY      15
#define OPENAI_DEFAULT_REMOVE_IDX   1
#define OPENAI_DEFAULT_MAXTOKENS    200
#define OPENAI_DEFAULT_TEMPERATURE  0.5f
#define OPENAI_DEFAULT_TOP_P        0.5f

#define OPENROUTER_DEFAULT_URL      "https://openrouter.ai/api/v1/chat/completions"
#define OPENROUTER_DEFAULT_X_TITLE  "Blop default x-title"

/* --------------------------------------------------------------------------
 * Helpers
 * -------------------------------------------------------------------------- */

static size_t openai_write_callback(void* data, size_t size, size_t nmemb, void* userp) {
  BLOP_ASSERT_PTR(data);
  BLOP_ASSERT_PTR(userp);

  struct_openai* api = (struct_openai*)userp;

  size_t total = size * nmemb;
  
  REALLOC(api->response, char, api->response, api->response_size + total + 1);
  memcpy(api->response + api->response_size, data, total);
  api->response_size += total;
  api->response[api->response_size] = '\0';

  return total;
}

static void   openai_append_hdr(struct curl_slist** header, const char* format, ...) {
  BLOP_ASSERT_PTR(header);
  BLOP_ASSERT_PTR(format);

  char openai_temp_buffer[1024] = {0};
  va_list args;
  va_start(args, format);
  vsnprintf(openai_temp_buffer, sizeof(openai_temp_buffer), format, args);
  va_end(args);
  
  *header = curl_slist_append(*header, openai_temp_buffer);
  BLOPF_ASSERT(*header != NULL, "curl_slist_append failed -> %s", openai_temp_buffer);
}

static void   openai_push_tool(struct_openai* api, const char* name, const char* description, const cJSON* parameters) {
  BLOP_ASSERT_PTR(api);
  BLOP_ASSERT_PTR(name);
  BLOP_ASSERT_PTR(description);
  BLOP_ASSERT_PTR(parameters);

  cJSON* tool = cJSON_CreateObject();
  BLOP_ASSERT_PTR(tool);

  cJSON* fn = cJSON_AddObjectToObject(tool, "function");
  BLOP_ASSERT_PTR(fn);

  BLOP_ASSERT_PTR(cJSON_AddStringToObject(tool, "type", "function"));
  BLOP_ASSERT_PTR(cJSON_AddStringToObject(fn, "description", description));
  BLOP_ASSERT_PTR(cJSON_AddStringToObject(fn, "name", name));

  cJSON* dup_params = NULL;
  if (parameters) {
    dup_params = cJSON_Duplicate(parameters, true);
    BLOP_ASSERT_PTR(dup_params);
  } else {
    dup_params = cJSON_CreateObject();
    BLOP_ASSERT_PTR(dup_params);
  }

  cJSON_AddItemToObject(fn, "parameters", dup_params); /* ownership transferred here */
  cJSON_AddItemToArray(api->tools, tool);
}

static struct_openai_tool* openai_find_tool(struct_openai* api, const char* name) {
  BLOP_ASSERT_PTR(api);
  BLOP_ASSERT_PTR(name);
  
  for (size_t i = 0; i < OPENAI_TOOL_COUNT; ++i) {
    if (api->attached[i].in_use && strcmp(api->attached[i].name, name) == 0) {
      return &api->attached[i];
    }
  }
  return NULL;
}

/* --------------------------------------------------------------------------
 * Backends
 * -------------------------------------------------------------------------- */

static const struct_openai_backend_ops* openai_ops = NULL;

  /* --------------------------------------------------------------------------
   * Openai
   * -------------------------------------------------------------------------- */

static int openai_parse_args(cJSON* raw, cJSON** out) {
  if (!cJSON_IsString(raw)) { return 0; }
  *out = cJSON_Parse(raw->valuestring);
  return *out != NULL;
}

static void openai_push_result(struct_openai* api, const char* id, const char* name, cJSON* result) {
  cJSON* msg = cJSON_CreateObject();
  cJSON_AddStringToObject(msg, "role", "tool");
  cJSON_AddStringToObject(msg, "tool_call_id", id);

  char* printed = cJSON_PrintUnformatted(result);
  if (printed) {
    BLOP_ASSERT_PTR(cJSON_AddStringToObject(msg, "content", printed));
    FREE_IF(printed);
  } else {
    BLOP_ASSERT_PTR(cJSON_AddStringToObject(msg, "content", "{}"));
  }

  fn_openai_history_insert(api, fn_openai_history_size(api), msg);
}

static void openai_init_headers(struct_openai* api) {
  openai_append_hdr(&api->headers, "Authorization: Bearer %s", api->config.api_key);
  openai_append_hdr(&api->headers, "Content-Type: application/json");
}

static int openai_supports_seed(void) { return 1; }

static const struct openai_backend_ops OPENAI_BACKEND = {
  .init_request      = NULL,
  .init_headers      = openai_init_headers,
  .parse_tool_args   = openai_parse_args,
  .push_tool_result  = openai_push_result,
  .supports_seed     = openai_supports_seed
};

  /* --------------------------------------------------------------------------
   * Openrouter
   * -------------------------------------------------------------------------- */

static int openrouter_parse_args(cJSON* raw, cJSON** out) {
  if (cJSON_IsString(raw)) {
    *out = cJSON_Parse(raw->valuestring);
  } else if (cJSON_IsObject(raw)) {
    *out = cJSON_Duplicate(raw, true);
  } else return 0;
  return *out != NULL;
}

static void openrouter_push_result(struct_openai* api, const char* id, const char* name, cJSON* result) {
  cJSON* msg = cJSON_CreateObject();
  cJSON_AddStringToObject(msg, "role", "tool");
  cJSON_AddStringToObject(msg, "tool_call_id", id);
  cJSON_AddStringToObject(msg, "name", name);

  char* printed = cJSON_PrintUnformatted(result);
  if (printed) {
    BLOP_ASSERT_PTR(cJSON_AddStringToObject(msg, "content", printed));
    FREE_IF(printed);
  } else {
    BLOP_ASSERT_PTR(cJSON_AddStringToObject(msg, "content", "{}"));
  }

  fn_openai_history_insert(api, fn_openai_history_size(api), msg);
}

static void openrouter_init_headers(struct_openai* api) {
  openai_append_hdr(&api->headers, "Authorization: Bearer %s", api->config.api_key);
  openai_append_hdr(&api->headers, "Content-Type: application/json");

  if (api->config.http_referer) {
    openai_append_hdr(&api->headers, "HTTP-Referer: %s", api->config.http_referer);
  }
  if (api->config.x_title) {
    openai_append_hdr(&api->headers, "X-Title: %s", api->config.x_title);
  }
}

static int openrouter_supports_seed(void) { return 1; }

static const struct openai_backend_ops OPENROUTER_BACKEND = {
  .init_request      = NULL,
  .init_headers      = openrouter_init_headers,
  .parse_tool_args   = openrouter_parse_args,
  .push_tool_result  = openrouter_push_result,
  .supports_seed     = openrouter_supports_seed
};

  /* --------------------------------------------------------------------------
   * Llama.cpp
   * -------------------------------------------------------------------------- */

static int llama_parse_args(cJSON* raw, cJSON** out) {
  if (!cJSON_IsString(raw)) { return 0; }
  *out = cJSON_Parse(raw->valuestring);
  return *out != NULL;
}

static void llama_init_headers(struct_openai* api) {
  openai_append_hdr(&api->headers, "Content-Type: application/json");
}

static int llama_supports_seed(void) { return 0; }

static const struct openai_backend_ops LLAMA_BACKEND = {
  .init_request      = NULL,
  .init_headers      = llama_init_headers,
  .parse_tool_args   = llama_parse_args,
  .push_tool_result  = openai_push_result, /* same as OpenAI */
  .supports_seed     = llama_supports_seed
};

/* --------------------------------------------------------------------------
 * Implementation
 * -------------------------------------------------------------------------- */

struct_openai*  fn_openai_create(struct_openai* api, struct_openai_config config) {
  BLOP_ASSERT_PTR(config.model);

  if (!api) {
    CALLOC(api, struct struct_openai, 1);
    api->allocated = true;
  }

  RWLOCK_INIT(api->lock);

  memset(api->attached, 0, sizeof(api->attached));
  api->config = config;

  /* ------------------------------------------------------------------ */
  /* Defaults                                                           */
  /* ------------------------------------------------------------------ */

  api->config.max_history    = config.max_history    > 0  ? config.max_history    : OPENAI_DEFAULT_HISTORY;
  api->config.max_remove_idx = config.max_remove_idx >= 0 ? config.max_remove_idx : OPENAI_DEFAULT_REMOVE_IDX;
  api->config.max_tokens     = (config.max_tokens > 0 || config.max_tokens == -1) ? config.max_tokens : OPENAI_DEFAULT_MAXTOKENS;
  api->config.temperature    = config.temperature >= 0.f ? config.temperature : OPENAI_DEFAULT_TEMPERATURE;
  api->config.top_p          = config.top_p >= 0.f ? config.top_p : OPENAI_DEFAULT_TOP_P;

  BLOP_ASSERT_BOUNDS(api->config.max_remove_idx, api->config.max_history - 2);

  /* ------------------------------------------------------------------ */
  /* Backend selection                                                  */
  /* ------------------------------------------------------------------ */

  switch (config.backend) {
    case OPENAI_API:
      openai_ops = &OPENAI_BACKEND;
      if (!api->config.url) { api->config.url = OPENAI_DEFAULT_URL; }
      BLOP_ASSERT_PTR(api->config.api_key);
      break;

    case OPENROUTER_API:
      openai_ops = &OPENROUTER_BACKEND;
      if (!api->config.url) { api->config.url = OPENROUTER_DEFAULT_URL; }
      if (!api->config.x_title) { api->config.x_title = OPENROUTER_DEFAULT_X_TITLE; }
      BLOP_ASSERT_PTR(api->config.api_key);
      break;

    case LLAMA_CPP_API:
      openai_ops = &LLAMA_BACKEND;
      /* llama MUST provide url */
      BLOP_ASSERT_PTR(api->config.url);
      break;

    default:
      BLOP_ASSERT_FORCED(0, "Unknown backend");
  }

  BLOP_ASSERT_PTR(openai_ops);

  /* ------------------------------------------------------------------ */
  /* Root JSON                                                          */
  /* ------------------------------------------------------------------ */

  api->root = cJSON_CreateObject();
  BLOP_ASSERT_PTR(api->root);

  api->tools    = cJSON_AddArrayToObject(api->root, "tools");
  api->messages = cJSON_AddArrayToObject(api->root, "messages");

  cJSON_AddStringToObject(api->root, "model", api->config.model);
  cJSON_AddNumberToObject(api->root, "temperature", api->config.temperature);
  cJSON_AddNumberToObject(api->root, "top_p", api->config.top_p);

  if (api->config.max_tokens != -1) { 
    BLOP_ASSERT_PTR(cJSON_AddNumberToObject(api->root, "max_tokens", api->config.max_tokens));
  }
  
  if (api->config.seed >= 0 && openai_ops->supports_seed()) { 
    BLOP_ASSERT_PTR(cJSON_AddNumberToObject(api->root, "seed", api->config.seed));
  }

  if (api->config.reasoning != -1 && config.backend == OPENROUTER_API) {
    cJSON* reasoning = cJSON_CreateObject();
    BLOP_ASSERT_PTR(reasoning);
    BLOP_ASSERT_PTR(cJSON_AddBoolToObject(reasoning, "enabled", !!api->config.reasoning));
    cJSON_AddItemToObject(api->root, "reasoning", reasoning);
  }

  if (api->config.presence_penalty != 0.f) {
    BLOP_ASSERT_PTR(cJSON_AddNumberToObject(api->root, "presence_penalty", api->config.presence_penalty));
  }

  if (api->config.frequency_penalty != 0.f) {
    BLOP_ASSERT_PTR(cJSON_AddNumberToObject(api->root, "frequency_penalty", api->config.frequency_penalty));
  }

  if (api->config.stop) {
    BLOP_ASSERT_FORCED(
      cJSON_IsString(api->config.stop) || cJSON_IsArray(api->config.stop),
      "`stop` must be string or array"
    );
    cJSON_AddItemToObject(api->root, "stop", cJSON_Duplicate(api->config.stop, true));
  }

  if (api->config.response_format) {
    cJSON* dup_resp = cJSON_Duplicate(api->config.response_format, true);
    BLOP_ASSERT_PTR(dup_resp);
    cJSON_AddItemToObject(api->root, "response_format", dup_resp);
  }

  if (api->config.logit_bias) {
    BLOP_ASSERT_FORCED(cJSON_IsObject(api->config.logit_bias), "logit_bias must be object");
    cJSON* dup_lb = cJSON_Duplicate(api->config.logit_bias, true);
    BLOP_ASSERT_PTR(dup_lb);
    cJSON_AddItemToObject(api->root, "logit_bias", dup_lb);
  }

  /* ------------------------------------------------------------------ */
  /* CURL                                                               */
  /* ------------------------------------------------------------------ */

  api->curl = curl_easy_init();
  BLOP_ASSERT_FORCED(api->curl, "curl_easy_init failed");

  api->headers = NULL;
  openai_ops->init_headers(api);

  CURLcode cres = curl_easy_setopt(api->curl, CURLOPT_POST, 1L);
  BLOPF_ASSERT(cres == CURLE_OK, "curl_easy_setopt CURLOPT_POST failed: %s", curl_easy_strerror(cres));

  return api;
}

void            fn_openai_destroy(struct_openai* api) {
  ASSERT_PTR(api);

  if (api->root) { cJSON_Delete(api->root); }
  if (api->headers) { curl_slist_free_all(api->headers); }
  if (api->curl) { curl_easy_cleanup(api->curl); }

  FREE_IF(api->response);

  RWLOCK_DESTROY(api->lock);

  if (api->allocated) {
    FREE(api);
  }
}

cJSON*          fn_openai_send(struct_openai* api, cJSON* message) {
  BLOP_ASSERT_PTR(api);
  BLOP_ASSERT_PTR(message);
  BLOP_ASSERT_PTR(openai_ops);
  BLOP_ASSERT_PTR(api->curl);
  BLOP_ASSERT_PTR(api->root);
  BLOP_ASSERT_PTR(api->messages);

  fn_openai_history_insert(
    api,
    fn_openai_history_size(api),
    cJSON_Duplicate(message, true)
  );

  while (true) {
    FREE_IF(api->response);
    api->response_size = 0;

    char* payload = cJSON_PrintUnformatted(api->root);
    BLOP_ASSERT_PTR(payload);

    CURLcode cres;
    cres = curl_easy_setopt(api->curl, CURLOPT_URL, api->config.url);
    BLOPF_ASSERT(cres == CURLE_OK, "curl_easy_setopt CURLOPT_URL failed: %s", curl_easy_strerror(cres));
    cres = curl_easy_setopt(api->curl, CURLOPT_HTTPHEADER, api->headers);
    BLOPF_ASSERT(cres == CURLE_OK, "curl_easy_setopt CURLOPT_HTTPHEADER failed: %s", curl_easy_strerror(cres));
    cres = curl_easy_setopt(api->curl, CURLOPT_POSTFIELDS, payload);
    BLOPF_ASSERT(cres == CURLE_OK, "curl_easy_setopt CURLOPT_POSTFIELDS failed: %s", curl_easy_strerror(cres));
    cres = curl_easy_setopt(api->curl, CURLOPT_WRITEFUNCTION, openai_write_callback);
    BLOPF_ASSERT(cres == CURLE_OK, "curl_easy_setopt CURLOPT_WRITEFUNCTION failed: %s", curl_easy_strerror(cres));
    cres = curl_easy_setopt(api->curl, CURLOPT_WRITEDATA, api);
    BLOPF_ASSERT(cres == CURLE_OK, "curl_easy_setopt CURLOPT_WRITEDATA failed: %s", curl_easy_strerror(cres));

    if (api->config.timeout_ms > 0) {
      cres = curl_easy_setopt(api->curl, CURLOPT_TIMEOUT_MS, api->config.timeout_ms);
      BLOPF_ASSERT(cres == CURLE_OK, "curl_easy_setopt CURLOPT_TIMEOUT_MS failed: %s", curl_easy_strerror(cres));
    }

    if (api->config.cainfo_path) {
      cres = curl_easy_setopt(api->curl, CURLOPT_CAINFO, api->config.cainfo_path);
      BLOPF_ASSERT(cres == CURLE_OK, "curl_easy_setopt CURLOPT_CAINFO failed: %s", curl_easy_strerror(cres));
    }

    CURLcode res = curl_easy_perform(api->curl);
    FREE(payload);

    BLOPF_ASSERT(res == CURLE_OK, "curl_easy_perform failed: %s", curl_easy_strerror(res));
    BLOP_ASSERT_FORCED(api->response_size > 0, "Empty HTTP response");

    cJSON* root = cJSON_Parse(api->response);
    BLOP_ASSERT_PTR(root);

    /* -------------------------------------------------------------- */
    /* ERROR                                                          */
    /* -------------------------------------------------------------- */

    cJSON* err = cJSON_GetObjectItem(root, "error");
    if (err) {
      char* e = cJSON_Print(err);
      BLOPF_ASSERT_FORCED(false, "API error:\n%s", e ? e : "<unprintable>");
    }

    /* -------------------------------------------------------------- */
    /* CHOICES                                                        */
    /* -------------------------------------------------------------- */

    cJSON* choices = cJSON_GetObjectItem(root, "choices");
    BLOPF_ASSERT_FORCED(choices && cJSON_IsArray(choices), "Missing choices\n%s", api->response);

    cJSON* choice = cJSON_GetArrayItem(choices, 0);
    BLOP_ASSERT_PTR(choice);

    /* -------------------------------------------------------------- */
    /* MESSAGE (OpenAI / OpenRouter / llama.cpp compatible)           */
    /* -------------------------------------------------------------- */

    cJSON* msg = cJSON_GetObjectItem(choice, "message");

    if (!msg) {
      /* llama.cpp fallback */
      cJSON* text = cJSON_GetObjectItem(choice, "text");
      BLOP_ASSERT_FORCED(text && cJSON_IsString(text), "Invalid llama.cpp response");

      msg = cJSON_CreateObject();
      BLOP_ASSERT_PTR(msg);
      BLOP_ASSERT_PTR(cJSON_AddStringToObject(msg, "role", "assistant"));
      BLOP_ASSERT_PTR(cJSON_AddStringToObject(msg, "content", text->valuestring));
    }

    BLOP_ASSERT_PTR(msg);

    /* -------------------------------------------------------------- */
    /* TOOL CALLS                                                     */
    /* -------------------------------------------------------------- */

    cJSON* tool_calls = cJSON_GetObjectItem(msg, "tool_calls");

    if (tool_calls && cJSON_IsArray(tool_calls)) {
      cJSON* dup_msg = cJSON_Duplicate(msg, true);
      BLOP_ASSERT_PTR(dup_msg);
      fn_openai_history_insert(api, fn_openai_history_size(api), dup_msg);

      cJSON* call;
      cJSON_ArrayForEach(call, tool_calls) {
        cJSON* id = cJSON_GetObjectItem(call, "id");
        cJSON* fn = cJSON_GetObjectItem(call, "function");
        BLOP_ASSERT_PTR(fn);

        cJSON* name = cJSON_GetObjectItem(fn, "name");
        cJSON* args_raw = cJSON_GetObjectItem(fn, "arguments");
        BLOP_ASSERT_PTR(name);
        BLOP_ASSERT_PTR(args_raw);

        cJSON* args = NULL;

        if (cJSON_IsString(args_raw)) {
          args = cJSON_Parse(args_raw->valuestring);
        } else if (cJSON_IsObject(args_raw)) {
          args = cJSON_Duplicate(args_raw, true);
        }

        BLOP_ASSERT_FORCED(args != NULL, "Failed to parse tool arguments");

        struct_openai_tool* tool = openai_find_tool(api, name->valuestring);
        BLOPF_ASSERT_FORCED(tool != NULL, "Unknown tool requested: %s", name->valuestring);
        BLOP_ASSERT_PTR(tool->fn);
        BLOP_ASSERT_PTR(tool->parameters); /* tool should have parameters object */

        cJSON* result = tool->fn(args);
        cJSON_Delete(args);
        BLOP_ASSERT_PTR(result);

        openai_ops->push_tool_result(api, id ? id->valuestring : "", name->valuestring, result);
        cJSON_Delete(result);
      }

      cJSON_Delete(root);
      continue; /* 🔁 tool loop */
    }

    /* -------------------------------------------------------------- */
    /* FINAL MESSAGE                                                  */
    /* -------------------------------------------------------------- */

    cJSON* response = cJSON_Duplicate(msg, true);
    BLOP_ASSERT_PTR(response);
    fn_openai_history_insert(api, fn_openai_history_size(api), response);

    cJSON_Delete(root);
    return response;
  }
}

void            fn_openai_rdlock(struct_openai* api) {
  BLOP_ASSERT_PTR(api);
  RWLOCK_RDLOCK(api->lock);
}

void            fn_openai_wrlock(struct_openai* api) {
  BLOP_ASSERT_PTR(api);
  RWLOCK_WRLOCK(api->lock);
}

void            fn_openai_rdunlock(struct_openai* api) {
  BLOP_ASSERT_PTR(api);
  RWLOCK_RDUNLOCK(api->lock);
}

void            fn_openai_wrunlock(struct_openai* api) {
  BLOP_ASSERT_PTR(api);
  RWLOCK_WRUNLOCK(api->lock);
}

void            fn_openai_tool_create(struct_openai_tool* tool, const char* name, const char* description, type_openai_tool_fn fn) {
  BLOP_ASSERT_PTR(tool);
  BLOP_ASSERT_PTR(name);
  BLOP_ASSERT_PTR(description);
  BLOP_ASSERT_PTR(fn);

  tool->fn = fn;
  tool->name = name;
  tool->description = description;
  tool->parameters = cJSON_CreateObject();
  BLOP_ASSERT_PTR(tool->parameters);

  cJSON* properties = cJSON_AddObjectToObject(tool->parameters, "properties");
  BLOP_ASSERT_PTR(properties);
  BLOP_ASSERT_PTR(cJSON_AddStringToObject(tool->parameters, "type", "object"));
}

void            fn_openai_tool_add_param(struct_openai_tool* tool, const char* name, const char* type) {
  BLOP_ASSERT_PTR(tool);
  BLOP_ASSERT_PTR(name);
  BLOP_ASSERT_PTR(type);
  BLOP_ASSERT_PTR(tool->parameters);

  cJSON* properties = cJSON_GetObjectItem(tool->parameters, "properties");
  BLOP_ASSERT_PTR(properties);

  cJSON* parameter = cJSON_AddObjectToObject(properties, name);
  BLOP_ASSERT_PTR(parameter);
  BLOP_ASSERT_PTR(cJSON_AddStringToObject(parameter, "type", type));
}

void            fn_openai_tool_attach(struct_openai* api, size_t idx, struct_openai_tool tool) {
  BLOP_ASSERT_PTR(api);
  BLOP_ASSERT_BOUNDS(idx, OPENAI_TOOL_COUNT);
  BLOP_ASSERT_FORCED(!api->attached[idx].in_use, "Trying to attach a tool in an already attached slot");

  BLOP_ASSERT_PTR(tool.fn);
  BLOP_ASSERT_PTR(tool.name);
  BLOP_ASSERT_PTR(tool.parameters);

  tool.in_use = true;
  if (tool.description == NULL) {
    tool.description = "No description provided";
  }

  for (size_t i = 0; i < OPENAI_TOOL_COUNT; ++i) {
    if (api->attached[i].in_use) {
      BLOPF_ASSERT_FORCED(strcmp(api->attached[i].name, tool.name) != 0, "Tool name already attached: %s", tool.name);
    }
  }

  openai_push_tool(api, tool.name, tool.description, tool.parameters);
  api->attached[idx] = tool;
}

void            fn_openai_tool_deattach(struct_openai* api, size_t idx) {
  BLOP_ASSERT_PTR(api);
  BLOP_ASSERT_BOUNDS(idx, OPENAI_TOOL_COUNT);
  BLOP_ASSERT_FORCED(api->attached[idx].in_use, "Trying to deattach a tool in a free slot");

  cJSON_DeleteItemFromObject(api->root, "tools");
  api->tools = cJSON_AddArrayToObject(api->root, "tools");
  BLOP_ASSERT_PTR(api->tools);
  memset(&api->attached[idx], 0, sizeof(struct_openai_tool));

  for (size_t i = 0; i < OPENAI_TOOL_COUNT; i++) {
    if (api->attached[i].in_use) {
      openai_push_tool(api, api->attached[i].name, api->attached[i].description, api->attached[i].parameters);
    }
  }
}

void            fn_openai_history_set(struct_openai* api, size_t idx, cJSON* message) {
  BLOP_ASSERT_PTR(api);
  BLOP_ASSERT_PTR(message);
  BLOP_ASSERT_PTR(api->messages);
  BLOP_ASSERT_BOUNDS(idx, cJSON_GetArraySize(api->messages));
  cJSON_ReplaceItemInArray(api->messages, idx, message);
}

cJSON*          fn_openai_history_get(struct_openai* api, size_t idx) {
  BLOP_ASSERT_PTR(api);
  BLOP_ASSERT_PTR(api->messages);
  BLOP_ASSERT_BOUNDS(idx, cJSON_GetArraySize(api->messages));
  return cJSON_GetArrayItem(api->messages, idx);
}

void            fn_openai_history_erase(struct_openai* api, size_t idx) {
  BLOP_ASSERT_PTR(api);
  BLOP_ASSERT_PTR(api->messages);
  BLOP_ASSERT_BOUNDS(idx, cJSON_GetArraySize(api->messages));
  cJSON_DeleteItemFromArray(api->messages, idx);
}

void            fn_openai_history_insert(struct_openai* api, size_t idx, cJSON* message) {
  BLOP_ASSERT_PTR(api);
  BLOP_ASSERT_PTR(message);
  BLOP_ASSERT_PTR(api->messages);

  BLOP_ASSERT_BOUNDS(idx, cJSON_GetArraySize(api->messages) + 1);

  cJSON_InsertItemInArray(api->messages, idx, message);

  while (cJSON_GetArraySize(api->messages) > api->config.max_history) {
    cJSON_DeleteItemFromArray(api->messages, api->config.max_remove_idx);
  }
}

int             fn_openai_history_size(struct_openai* api) {
  BLOP_ASSERT_PTR(api);
  BLOP_ASSERT_PTR(api->messages);
  return cJSON_GetArraySize(api->messages);
}

#endif /* OPENAI_IMPLEMENTATION */

#ifdef __cplusplus
}
#endif

#endif /* __BLOP_OPENAI_H__ */