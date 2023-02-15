package com.msgpack;

import android.util.Log;

import androidx.annotation.NonNull;

import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.module.annotations.ReactModule;
import com.facebook.react.bridge.JavaScriptContextHolder;

@ReactModule(name = MsgpackModule.NAME)
public class MsgpackModule extends ReactContextBaseJavaModule {
  public static final String NAME = "Msgpack";

  public MsgpackModule(ReactApplicationContext reactContext) {
    super(reactContext);
  }

  @Override
  @NonNull
  public String getName() {
    return NAME;
  }

  static {
    Log.i(NAME, "Loading C++ library...");
    System.loadLibrary("cpp");
  }

  private native void nativeInstall(long jsi);


  @ReactMethod(isBlockingSynchronousMethod = true)
  public boolean install() {
    try {
      ReactApplicationContext reactContext = getReactApplicationContext();
      JavaScriptContextHolder contextHolder = reactContext.getJavaScriptContextHolder();
      this.nativeInstall(contextHolder.get());
      Log.i(NAME, "Successfully installed!");
      return true;
    } catch (Exception exception) {
      Log.e(NAME, "Failed to install JSI Bindings!", exception);
      return false;
    }
  }
}
