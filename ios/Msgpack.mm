#import "Msgpack.h"

#import <React/RCTBridge+Private.h>
#import <jsi/jsi.h>

@implementation Msgpack
@synthesize bridge = _bridge;
@synthesize methodQueue = _methodQueue;


RCT_EXPORT_MODULE(Msgpack)


+ (BOOL)requiresMainQueueSetup {
  return YES;
}

RCT_EXPORT_BLOCKING_SYNCHRONOUS_METHOD(install)
{
  RCTBridge *bridge = [RCTBridge currentBridge];
  RCTCxxBridge *cxxBridge = (RCTCxxBridge *) bridge;
  if (cxxBridge == nil) {
    return @false;
  }
  if (cxxBridge.runtime == nil) {
    return @false;
  }
  install(*(facebook::jsi::Runtime *) cxxBridge.runtime);
  return @true;
}

@end
