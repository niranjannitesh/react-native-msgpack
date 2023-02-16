import * as React from 'react';

import { StyleSheet, View } from 'react-native';
import { encode, decode } from 'react-native-msgpack';
import { unpack, pack } from 'msgpackr/dist';

const obj = {
  _id: {
    $oid: '5e8b3d2ce1a9cd1cd8e26e15',
  },
  username: 'niranjannitesh',
  verified: false,
  password: '$2b$14$SjifJFfAqPROD1.QrVpksuuqxXvpbKhoemPyXbwlFjjDp8kxqQo3W',
  profile: {
    pronouns: ['he/him/his'],
    name: 'Nitesh Kumar Niranjan',
    bio: 'Finding magic in a mundane world 🍁',
    location: 'India',
    date_of_birth: {
      $date: {
        $numberLong: '1012867200000',
      },
    },
    profile_pic: 'user_profile_pic/ibnkiavqvlsjiaij9wlw',
    cover_image:
      'https://images.unsplash.com/photo-1493329025335-18542a61595f?crop=entropy&cs=tinysrgb&fit=max&fm=jpg&ixid=MnwxNDcyNzd8MHwxfHNlYXJjaHwzfHxnbGFjaWVyfGVufDB8fHx8MTY0NjMwMzM5MA&ixlib=rb-1.2.1&q=80&w=1920',
  },
  deactivated: false,
  banned_at: null,
  banned_by: null,
  created_at: {
    $date: {
      $numberLong: '1580551775000',
    },
  },
  updated_at: {
    $date: {
      $numberLong: '1660935853175',
    },
  },
  reputation: {
    absolute: 2990,
    cumulative: 3003,
  },
  phone: {
    iso_alpha_2: 'IN',
    number: '+917004153058',
  },
  email: 'niteshkumarniranjan@gmail.com',
  email_verified: true,
  google_id: '108045490346839687273',
  status: {
    emoji: '🤧',
    created_at: {
      $date: {
        $numberLong: '1656247348549',
      },
    },
  },
  opted_old_feed: false,
  shown_ratings_popup: false,
  username_changed_count: 0,
  online_status: false,
};

const RUN_CYCLES = 10000;
const n = () => {
  console.log('comuting with react-native-msgpack');
  const startTime = Date.now();
  for (let i = 0; i < RUN_CYCLES; i++) {
    const resp = encode(obj);
    decode(resp);
  }
  const endTime = Date.now();
  console.log('Time taken: ', endTime - startTime, 'ms');
};

const d = () => {
  console.log('comuting with msgpackr');
  const startTime = Date.now();
  for (let i = 0; i < RUN_CYCLES; i++) {
    const resp = pack(obj);
    unpack(resp);
  }
  const endTime = Date.now();
  console.log('Time taken: ', endTime - startTime, 'ms');
};

export default function App() {
  React.useEffect(() => {
    n();
  }, []);

  React.useEffect(() => {
    d();
  }, []);

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
