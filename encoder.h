#pragma once
#include <stdbool.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

struct json_ctx {
  char const *buf;
  char *ptr;
  int remaining;
  int error;
};

enum json_format { JSON_FORMAT_END, JSON_FORMAT_COMMA };

void json_test(struct json_ctx *ctx);

void json_init(struct json_ctx *ctx, char *buf, int length);

void json_start(struct json_ctx *ctx);
void json_close(struct json_ctx *ctx);
void json_start_object(struct json_ctx *ctx, char const *key);
void json_close_object(struct json_ctx *ctx, enum json_format format);
void json_start_array(struct json_ctx *ctx, char const *key);
void json_close_array(struct json_ctx *ctx, enum json_format format);
void json_set_object_integer(struct json_ctx *ctx, char const *key, int value,
                             enum json_format format);
void json_set_object_string(struct json_ctx *ctx, char const *key,
                            char const *value, enum json_format format);
void json_set_object_boolean(struct json_ctx *ctx, char const *key, bool value,
                             enum json_format format);
void json_set_object_empty(struct json_ctx *ctx, char const *key,
                           enum json_format format);
void json_set_array_integer(struct json_ctx *ctx, int const value,
                            enum json_format format);
void json_set_array_string(struct json_ctx *ctx, char const *value,
                           enum json_format format);

#ifdef __cplusplus
}
#endif