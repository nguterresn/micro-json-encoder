#include "encoder.h"
#include <errno.h>
#include <stdarg.h>

static void json_write(struct json_ctx *ctx, char const *fmt, ...);

void json_init(struct json_ctx *ctx, char *buf, int length) {
  ctx->buf = buf;
  ctx->ptr = buf;
  ctx->remaining = length;
  ctx->error = 0;
}

void json_start(struct json_ctx *ctx) { json_write(ctx, "{"); }
void json_close(struct json_ctx *ctx) { json_write(ctx, "}"); }

void json_start_object(struct json_ctx *ctx, char const *key) {
  json_write(ctx, "\"%s\":{", key);
}

void json_close_object(struct json_ctx *ctx, enum json_format format) {
  json_write(ctx, format == JSON_FORMAT_COMMA ? "}," : "}");
}

void json_start_array(struct json_ctx *ctx, char const *key) {
  json_write(ctx, "\"%s\":[", key);
}

void json_close_array(struct json_ctx *ctx, enum json_format format) {
  json_write(ctx, format == JSON_FORMAT_COMMA ? "]," : "]");
}

void json_set_array_integer(struct json_ctx *ctx, int const value,
                            enum json_format format) {
  json_write(ctx, format == JSON_FORMAT_COMMA ? "%d," : "%d", value);
}

void json_set_array_string(struct json_ctx *ctx, char const *value,
                           enum json_format format) {
  json_write(ctx, format == JSON_FORMAT_COMMA ? "\"%s\"," : "\"%s\"", value);
}

void json_set_object_integer(struct json_ctx *ctx, char const *key, int value,
                             enum json_format format) {
  json_write(ctx, format == JSON_FORMAT_COMMA ? "\"%s\":%d," : "\"%s\":%d", key,
             value);
}

void json_set_object_string(struct json_ctx *ctx, char const *key,
                            char const *value, enum json_format format) {
  json_write(ctx,
             format == JSON_FORMAT_COMMA ? "\"%s\":\"%s\"," : "\"%s\":\"%s\"",
             key, value);
}

void json_set_object_boolean(struct json_ctx *ctx, char const *key, bool value,
                             enum json_format format) {
  json_write(ctx,
             format == JSON_FORMAT_COMMA
                 ? (value ? "\"%s\":true," : "\"%s\":false,")
                 : (value ? "\"%s\":true" : "\"%s\":false"),
             key, value);
}

void json_set_object_empty(struct json_ctx *ctx, char const *key,
                           enum json_format format) {
  json_start_object(ctx, key);
  json_close_object(ctx, format);
}

static void json_write(struct json_ctx *ctx, char const *fmt, ...) {
  if (ctx->error) {
    return;
  }

  va_list args;
  va_start(args, fmt);
  int written = vsnprintf(ctx->ptr, ctx->remaining, fmt, args);
  va_end(args);

  if (written < 0 || written >= ctx->remaining) {
    ctx->error = -ENOBUFS;
    return;
  }

  ctx->ptr += written;
  ctx->remaining -= written;
}