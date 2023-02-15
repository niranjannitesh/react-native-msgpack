import { NativeModules } from 'react-native';

const Msgpack = NativeModules.Msgpack;

if (Msgpack == null || typeof Msgpack.install !== 'function') {
  throw new Error('Msgpack native module is not available.');
}

if (Msgpack.install() !== true) {
  throw new Error('Msgpack native module is not available.');
}

declare global {
  function msgpackEncode(data: any): Uint8Array;
  function msgpackDecode(data: ArrayBuffer): any;
}

export function encode(data: any): Uint8Array {
  return global.msgpackEncode(data);
}

export function decode(data: Uint8Array): any {
  return global.msgpackDecode(data.buffer);
}
