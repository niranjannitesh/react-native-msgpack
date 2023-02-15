#ifdef __cplusplus
#import "react-native-msgpack.hpp"
#endif

#ifdef RCT_NEW_ARCH_ENABLED
#import "RNMsgpackSpec.h"

@interface Msgpack : NSObject <NativeMsgpackSpec>
#else
#import <React/RCTBridgeModule.h>

@interface Msgpack : NSObject <RCTBridgeModule>
#endif

@end
