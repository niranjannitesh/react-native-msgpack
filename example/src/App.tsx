import * as React from 'react';

import { StyleSheet, View } from 'react-native';
import { encode, decode } from 'react-native-msgpack';

export default function App() {
  React.useEffect(() => {
    const resp = encode({
      name: 'Tester',
      a: {
        b: 'test',
      },
      c: [1, 2, 3, 4, 5],
      d: false,
    });
    const t = decode(resp);
    console.log(t);
  });

  return <View style={styles.container} />;
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
  },
  box: {
    width: 60,
    height: 60,
    marginVertical: 20,
  },
});
