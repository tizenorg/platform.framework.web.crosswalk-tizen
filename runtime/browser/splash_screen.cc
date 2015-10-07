#include "runtime/browser/splash_screen.h"

#include "runtime/browser/native_window.h"
#include "common/logger.h"

#if defined(HAVE_X11)
#include <Ecore_X.h>
#elif defined(HAVE_WAYLAND)
#include <Ecore_Wayland.h>
#endif
#include <manifest_handlers/splash_screen_handler.h>
#include <map>

namespace runtime {
SplashScreen::SplashScreen(runtime::NativeWindow* window,
                           std::shared_ptr<const wgt::parse::SplashScreenInfo>
                           ss_info) :
                           is_active_(false) {
  CreateSplashScreen(window, ss_info);
}

void SplashScreen::HideSplashScreen() {
  if (is_active_)
    evas_object_hide(background_);
}

void SplashScreen::CreateSplashScreen(runtime::NativeWindow* window,
                                      std::shared_ptr
                                      <const wgt::parse::SplashScreenInfo>
                                      ss_info) {

  LOGGER(INFO) << "start of create splash screen";
  if (ss_info == nullptr)
    return;
  auto splash_map = ss_info->splash_screen_data();
  auto used_orientation = ChooseOrientation(splash_map,
                                            window->natural_orientation());
  if (used_orientation == wgt::parse::ScreenOrientation::NONE)
    return;
  is_active_ = true;
  auto dimensions = GetDimensions();
  SetBackgroundColor(splash_map[used_orientation], window->evas_object(),
                     dimensions);
}

std::pair<int,int> SplashScreen::GetDimensions() {
  int w,h;
#if defined(HAVE_X11)
  uint16_t pid = getpid();
  ecore_x_window_prop_property_set(
    elm_win_xwindow_get(window),
    ECORE_X_ATOM_NET_WM_PID,
    ECORE_X_ATOM_CARDINAL, 32, &pid, 1);
  ecore_x_vsync_animator_tick_source_set(elm_win_xwindow_get(window));
  ecore_x_window_size_get(ecore_x_window_root_first_get(), &w, &h);
#elif defined(HAVE_WAYLAND)
  ecore_wl_screen_size_get(&w, &h);
#endif
  evas_object_resize(background_, w, h);
  return std::make_pair(w, h);
}

void SplashScreen::SetBackgroundColor(const wgt::parse::SplashScreenData&
                                       splash_data,
                                      Evas_Object* parent,
                                      const std::pair<int,int>& pair) {
  background_ = elm_bg_add(parent);
  evas_object_resize(background_, pair.first, pair.second);
  elm_bg_color_set(background_, splash_data.color.red, splash_data.color.green,
                   splash_data.color.blue);
  evas_object_show(background_);
}

wgt::parse::ScreenOrientation SplashScreen::ChooseOrientation(
    const std::map<wgt::parse::ScreenOrientation,
    wgt::parse::SplashScreenData>& splash_map,
    runtime::NativeWindow::ScreenOrientation screen_orientation) {
  auto orientation_pair = splash_map.end();
  if (screen_orientation ==
      runtime::NativeWindow::ScreenOrientation::PORTRAIT_PRIMARY) {
    orientation_pair = splash_map.find(wgt::parse::ScreenOrientation::PORTRAIT);
    if (orientation_pair == splash_map.end())
      orientation_pair = splash_map.find(wgt::parse::ScreenOrientation::AUTO);
  } else {
    orientation_pair =
        splash_map.find(wgt::parse::ScreenOrientation::LANDSCAPE);
    if (orientation_pair == splash_map.end())
      orientation_pair = splash_map.find(wgt::parse::ScreenOrientation::AUTO);
  }
  if (orientation_pair != splash_map.end())
      return orientation_pair->first;
  return wgt::parse::ScreenOrientation::NONE;
}
}  // namespace runtime
