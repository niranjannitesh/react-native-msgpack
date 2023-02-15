#include <jsi/jsi.h>
#include <vector>
#include "msgpack.hpp"

using namespace facebook;

void install(jsi::Runtime &rt)
{
  auto encodePropId = jsi::PropNameID::forAscii(rt, "msgpack_encode");
  auto encodeLamda = [](jsi::Runtime &rt,
                        const jsi::Value &thisValue,
                        const jsi::Value *args,
                        size_t count) -> jsi::Value
  {
    auto data = write(rt, std::move(args[0]));
    auto size = data.size();
    jsi::ArrayBuffer arrayBuffer =
        rt.global()
            .getPropertyAsFunction(rt, "ArrayBuffer")
            .callAsConstructor(rt, static_cast<int>(size))
            .asObject(rt)
            .getArrayBuffer(rt);
    std::memcpy(arrayBuffer.data(rt), data.data(), size);
    auto typedArray = rt.global()
                          .getPropertyAsFunction(rt, "Uint8Array")
                          .callAsConstructor(rt, std::move(arrayBuffer))
                          .asObject(rt);

    return std::move(typedArray);
  };
  jsi::Function encode =
      jsi::Function::createFromHostFunction(rt, encodePropId, 1, encodeLamda);
  rt.global().setProperty(rt, "msgpackEncode", encode);

  auto decodePropId = jsi::PropNameID::forAscii(rt, "msgpack_decode");
  auto decodeLamda = [](jsi::Runtime &rt,
                        const jsi::Value &thisValue,
                        const jsi::Value *args,
                        size_t count) -> jsi::Value
  {
    auto byteResult = args[0].asObject(rt).getArrayBuffer(rt);
    uint8_t *data = std::move(byteResult.data(rt));
    auto size = byteResult.size(rt);
    MessagePackReader mp = MessagePackReader(reinterpret_cast<const char *>(data), size);
    return mp.read(rt);
  };
  jsi::Function decode =
      jsi::Function::createFromHostFunction(rt, decodePropId, 1, decodeLamda);
  rt.global().setProperty(rt, "msgpackDecode", decode);
}
