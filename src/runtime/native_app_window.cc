// Copyright 2015 Samsung Electronics Co, Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "runtime/native_app_window.h"

#include <Elementary.h>

namespace wrt {

NativeAppWindow::NativeAppWindow() {
}

NativeAppWindow::~NativeAppWindow() {
}

Evas_Object* NativeAppWindow::CreateWindowInternal() {
  elm_config_accel_preference_set("opengl");
  return elm_win_add(NULL, "wrt-widget", ELM_WIN_BASIC);
}


}  // namespace wrt
