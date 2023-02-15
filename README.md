# react-native-msgpack

Native MessagePack for React Native

## Installation

```sh
npm install react-native-msgpack
```

## Usage

```js
import { encode, decode } from 'react-native-msgpack';

// ...

const result = encode({ hello: 'world' });
const decoded = decode(result);
```

## Contributing

See the [contributing guide](CONTRIBUTING.md) to learn how to contribute to the repository and the development workflow.

## License

MIT

---

Made with [create-react-native-library](https://github.com/callstack/react-native-builder-bob)
