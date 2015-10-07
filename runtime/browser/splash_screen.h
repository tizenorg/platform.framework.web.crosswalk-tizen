#ifndef SPLASH_SCREEN_H
#define SPLASH_SCREEN_H

#include "runtime/browser/native_window.h"

#include <Elementary.h>
#include <manifest_handlers/splash_screen_handler.h>
#include <manifest_handlers/setting_handler.h>
#include <map>
#include <memory>

namespace runtime {
class SplashScreen
{
 public:
  SplashScreen(runtime::NativeWindow* window,
               std::shared_ptr<const wgt::parse::SplashScreenInfo> ss_info);
  void HideSplashScreen();

 private:
  void CreateSplashScreen(runtime::NativeWindow* window,
                          std::shared_ptr
                          <const wgt::parse::SplashScreenInfo>
                          ss_info);
  wgt::parse::ScreenOrientation ChooseOrientation(
      const std::map<wgt::parse::ScreenOrientation,
      wgt::parse::SplashScreenData>& splash_map,
      runtime::NativeWindow::ScreenOrientation screen_orientation);
  std::pair<int,int> GetDimensions();
  void SetBackgroundColor(const wgt::parse::SplashScreenData&
                                         splash_data,
                                        Evas_Object* parent,
                                        const std::pair<int,int>& pair);


  Evas_Object *image_;
  Evas_Object *background_;
  bool is_active_;

};
}  // namespace runtime
#endif // SPLASH_SCREEN_H
