// MessagePack for jsi::Object and other jsi::format
// Created by Nitesh Kumar Niranjan on 2023-02-14
// Copyright (c) 2023 Nitesh Kumar Niranjan. All rights reserved.

#include <vector>
#include <jsi/jsi.h>
#include "cmp.h"

using namespace facebook;

void writeValue(cmp_ctx_t *ctx, jsi::Runtime &rt, const jsi::Value &value);
void writeObject(cmp_ctx_t *ctx, jsi::Runtime &rt, const jsi::Object &object);
void writeArray(cmp_ctx_t *ctx, jsi::Runtime &rt, const jsi::Object &object);
static bool _mp_reader(cmp_ctx_t *ctx, void *data, size_t limit);
static size_t _mp_writer(cmp_ctx_t *ctx, const void *data, size_t count);
static size_t mp_writer(cmp_ctx_t *ctx, const void *data, size_t count);
static bool mp_reader(cmp_ctx_t *ctx, void *data, size_t limit);

jsi::Value readObject(cmp_ctx_t *ctx, jsi::Runtime &rt, uint32_t length);
jsi::Value readArray(cmp_ctx_t *ctx, jsi::Runtime &rt, uint32_t length);



void writeValue(cmp_ctx_t *ctx, jsi::Runtime &rt, const jsi::Value &value)
{
  if (value.isBool())
  {
    cmp_write_bool(ctx, value.getBool());
    return;
  }

  if (value.isNumber())
  {
    cmp_write_double(ctx, value.getNumber());
    return;
  }

  if (value.isString())
  {
    auto v = value.getString(rt).utf8(rt);
    cmp_write_str(ctx, v.c_str(), v.size());
    return;
  }

  if (value.isObject())
  {
    auto object = std::move(value.getObject(rt));
    if (object.isArray(rt))
    {
      writeArray(ctx, rt, object);
      return;
    }
    else
    {
      writeObject(ctx, rt, object);
      return;
    }
  }

  cmp_write_nil(ctx);
  return;
}

void writeArray(cmp_ctx_t *ctx, jsi::Runtime &rt, const jsi::Object &object)
{
  cmp_write_array(ctx, object.getPropertyNames(rt).size(rt));
  auto array = object.asArray(rt);
  for (size_t i = 0; i < array.size(rt); i++)
  {
    writeValue(ctx, rt, std::move(array.getValueAtIndex(rt, i)));
  }
}

void writeObject(cmp_ctx_t *ctx, jsi::Runtime &rt, const jsi::Object &object)
{
  cmp_write_map(ctx, object.getPropertyNames(rt).size(rt));
  auto names = object.getPropertyNames(rt);
  for (size_t i = 0; i < names.size(rt); i++)
  {
    auto name = names.getValueAtIndex(rt, i).asString(rt);
    cmp_write_str(ctx, name.utf8(rt).c_str(), name.utf8(rt).size());
    writeValue(ctx, rt, (object.getProperty(rt, name)));
  }
}

static bool mp_reader(cmp_ctx_t *ctx, void *data, size_t limit)
{
  return 0;
}

struct MessagePackWriter
{
  std::vector<uint8_t> data;

  size_t write(void *data, size_t count)
  {
    this->data.insert(this->data.end(), (uint8_t *)data, (uint8_t *)data + count);
    return count;
  }
};

static size_t mp_writer(cmp_ctx_t *ctx, const void *data, size_t count)
{
  auto *mp = (MessagePackWriter *)ctx->buf;
  return mp->write(const_cast<void *>(data), count);
}

std::vector<uint8_t> write(jsi::Runtime &rt, const jsi::Value &value)
{
  std::vector<uint8_t> data;
    MessagePackWriter writer;
    std::unique_ptr<cmp_ctx_s> ctx(new cmp_ctx_s);
  cmp_init(ctx.get(), &writer, mp_reader, NULL, mp_writer);
  writeValue(ctx.get(), rt, value);
  return std::move(writer.data);
}


jsi::Value readValue(cmp_ctx_t * ctx, jsi::Runtime &rt)
{
  cmp_object_t obj;
  if (!cmp_read_object(ctx, &obj))
  {
    return jsi::Value::null();
  }

  switch (obj.type)
  {
  case CMP_TYPE_NIL:
    return jsi::Value(jsi::Value::null());

  case CMP_TYPE_BOOLEAN:
    return jsi::Value(obj.as.boolean);

  case CMP_TYPE_FIXMAP:
  case CMP_TYPE_MAP16:
  case CMP_TYPE_MAP32:
  {
    uint32_t length = obj.as.map_size;
    return readObject(ctx, rt, length);
  }

  case CMP_TYPE_FIXARRAY:
  case CMP_TYPE_ARRAY16:
  case CMP_TYPE_ARRAY32:
  {
    uint32_t length = obj.as.array_size;
    return readArray(ctx, rt, length);
  }

  case CMP_TYPE_FIXSTR:
  case CMP_TYPE_STR8:
  case CMP_TYPE_STR16:
  case CMP_TYPE_STR32:
  {
    uint32_t length = obj.as.str_size;
    if (length == 0)
    {
      return jsi::Value(rt, jsi::String::createFromAscii(rt, ""));
    }
    std::vector<uint8_t> data(length);
    auto readSuccess = ctx->read(ctx, data.data(), length);

    if (readSuccess)
    {
      return jsi::Value(rt, jsi::String::createFromUtf8(rt, (const uint8_t *)data.data(), length));
    }

    return jsi::Value(rt, jsi::String::createFromAscii(rt, ""));
  }

  case CMP_TYPE_POSITIVE_FIXNUM:
    return jsi::Value((double) obj.as.u8);
  case CMP_TYPE_NEGATIVE_FIXNUM:
    return jsi::Value((double) obj.as.s8);
  case CMP_TYPE_FLOAT:
    return jsi::Value((double) obj.as.flt);
  case CMP_TYPE_DOUBLE:
    return jsi::Value((double) obj.as.dbl);
  case CMP_TYPE_UINT8:
    return jsi::Value((double) obj.as.u8);
  case CMP_TYPE_UINT16:
    return jsi::Value((double) obj.as.u16);
  case CMP_TYPE_UINT32:
    return jsi::Value((double) obj.as.u32);
  case CMP_TYPE_UINT64:
    return jsi::Value((double) obj.as.u64);
  case CMP_TYPE_SINT8:
    return jsi::Value((double) obj.as.s8);
  case CMP_TYPE_SINT16:
    return jsi::Value((double) obj.as.s16);
  case CMP_TYPE_SINT32:
    return jsi::Value((double) obj.as.s32);
  case CMP_TYPE_SINT64:
    return jsi::Value((double) obj.as.s64);

  default:
    return jsi::Value(jsi::Value::null());
  }
}

jsi::Value readObject(cmp_ctx_t *ctx, jsi::Runtime &rt, uint32_t length)
{
  jsi::Object result = jsi::Object(rt);
  for (uint32_t i = 0; i < length; i++)
  {
    auto key = readValue(ctx, rt).toString(rt);
    auto value = readValue(ctx, rt);
    if (value.isString()) {
        result.setProperty(rt, key, value.getString(rt));
    } else if (value.isNumber()) {
        result.setProperty(rt, key, value.getNumber());
    } else if (value.isBool()) {
        result.setProperty(rt, key, value.getBool());
    } else if (value.isNull() || value.isUndefined()) {
        result.setProperty(rt, key, value.null());
    } else if (value.isObject()) {
        result.setProperty(rt, key, value.getObject(rt));
    }
  }
  return result;
}

jsi::Value readArray(cmp_ctx_t *ctx, jsi::Runtime &rt, uint32_t length)
{
  jsi::Array result = jsi::Array(rt, length);
  for (uint32_t i = 0; i < length; i++)
  {
    auto value = readValue(ctx, rt);
    result.setValueAtIndex(rt, i, value);
  }
  return result;
}


class MessagePackReader {
public:
    MessagePackReader(const char* data, size_t length) {
        this->data = new char[length];
        std::memcpy(this->data, data, length);
        this->size = length;
        index = 0;
    }

    ~MessagePackReader() {
        delete[] data;
    }

    size_t read(void *data, size_t limit)
    {
        if (this->index + limit > this->size)
        {
            return 0;
        }

        memcpy(data, this->data + this->index, limit);

        this->index += limit;
        return limit;
    }

    jsi::Value read(jsi::Runtime &rt)
    {
        cmp_ctx_t ctx;
        cmp_init(&ctx, this, _mp_reader, NULL, _mp_writer);
        return readValue(&ctx, rt);
    }

private:
    char* data;
    size_t index;
    size_t size;
};


static bool _mp_reader(cmp_ctx_t *ctx, void *data, size_t limit)
{
    auto *mp = (MessagePackReader *)ctx->buf;
    return mp->read(data, limit);
}

static size_t _mp_writer(cmp_ctx_t *ctx, const void *data, size_t count)
{
    return 0;
}

