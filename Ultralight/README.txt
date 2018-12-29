Hey thanks for trying out the Beta!

To link against the library you'll need to link against:
  - Ultralight.lib
  - UltralightCore.lib
  - WebCore.lib
  
You'll also need to distribute the following DLLs with your binary:
  - Ultralight.dll
  - UltralightCore.dll
  - WebCore.dll
  
When compiling applications you'll need to disable RTTI since we
compiled our library without this option.

Also please note that we have only supplied x64 binaries in this
release so you might want to disable any x86 build configurations.

If you use any of the Framework code in your app, make sure to define
the following in your preprocessor settings:
  - FRAMEWORK_PLATFORM_WIN
  
The framework code depends on DirectX 11 so make sure it's installed.
  
Please give any feedback at: https://discuss.ultralig.ht

Thanks!