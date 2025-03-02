// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_SHELL_PLATFORM_WINDOWS_TESTING_WINDOWS_TEST_CONFIG_BUILDER_H_
#define FLUTTER_SHELL_PLATFORM_WINDOWS_TESTING_WINDOWS_TEST_CONFIG_BUILDER_H_

#include <string>
#include <string_view>
#include <vector>

#include "flutter/fml/macros.h"
#include "flutter/fml/unique_object.h"
#include "flutter/shell/platform/windows/public/flutter_windows.h"
#include "flutter/shell/platform/windows/testing/windows_test_context.h"

namespace flutter {
namespace testing {

// Deleter for FlutterDesktopEngineRef objects.
struct EngineDeleter {
  typedef FlutterDesktopEngineRef pointer;
  void operator()(FlutterDesktopEngineRef engine) {
    FML_CHECK(FlutterDesktopEngineDestroy(engine));
  }
};

// Unique pointer wrapper for FlutterDesktopEngineRef.
using EnginePtr = std::unique_ptr<FlutterDesktopEngine, EngineDeleter>;

// Deleter for FlutterViewControllerRef objects.
struct ViewControllerDeleter {
  typedef FlutterDesktopViewControllerRef pointer;
  void operator()(FlutterDesktopViewControllerRef engine) {
    FlutterDesktopViewControllerDestroy(engine);
  }
};

// Unique pointer wrapper for FlutterDesktopViewControllerRef.
using ViewControllerPtr =
    std::unique_ptr<FlutterDesktopViewControllerState, ViewControllerDeleter>;

// Test configuration builder for WindowsTests.
//
// Utility class for configuring engine and view controller launch arguments,
// and launching the engine to run a test fixture.
class WindowsConfigBuilder {
 public:
  explicit WindowsConfigBuilder(WindowsTestContext& context);
  ~WindowsConfigBuilder();

  // Returns the desktop engine properties configured for this test.
  FlutterDesktopEngineProperties GetEngineProperties() const;

  // Adds an argument to the Dart entrypoint arguments List<String>.
  void AddDartEntrypointArgument(std::string_view arg);

  // Returns a configured and initialized view controller running the default
  // Dart entrypoint.
  ViewControllerPtr LaunchEngine() const;

 private:
  // Initialize COM, so that it is available for use in the library and/or
  // plugins.
  void InitializeCOM() const;

  // Returns a configured and initialized engine.
  EnginePtr InitializeEngine() const;

  WindowsTestContext& context_;
  std::vector<std::string> dart_entrypoint_arguments_;

  FML_DISALLOW_COPY_AND_ASSIGN(WindowsConfigBuilder);
};

}  // namespace testing
}  // namespace flutter

#endif  // FLUTTER_SHELL_PLATFORM_WINDOWS_TESTING_WINDOWS_TEST_CONFIG_BUILDER_H_
