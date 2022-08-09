#include "displayapp/widgets/StatusIcons.h"
#include "displayapp/screens/Symbols.h"

using namespace Pinetime::Applications::Widgets;

StatusIcons::StatusIcons(Controllers::Battery& batteryController, Controllers::Ble& bleController, Controllers::AlarmController& alarmController)
  : batteryController {batteryController}, bleController {bleController}, alarmController { alarmController } {
}

void StatusIcons::Create() {
  container = lv_cont_create(lv_scr_act(), nullptr);
  lv_cont_set_layout(container, LV_LAYOUT_ROW_TOP);
  lv_cont_set_fit(container, LV_FIT_TIGHT);
  lv_obj_set_style_local_pad_inner(container, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 5);
  lv_obj_set_style_local_bg_opa(container, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);

  alarmIcon = lv_label_create(container, nullptr);
  lv_label_set_text_static(alarmIcon, Screens::Symbols::clock);

  bleIcon = lv_label_create(container, nullptr);
  lv_obj_set_style_local_text_color(bleIcon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x0082FC));
  lv_label_set_text_static(bleIcon, Screens::Symbols::bluetooth);

  batteryPlug = lv_label_create(container, nullptr);
  lv_obj_set_style_local_text_color(batteryPlug, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xFF0000));
  lv_label_set_text_static(batteryPlug, Screens::Symbols::plug);

  batteryIcon.Create(container);

  lv_obj_align(container, nullptr, LV_ALIGN_IN_TOP_RIGHT, 0, 0);
}

void StatusIcons::Update() {
  powerPresent = batteryController.IsPowerPresent();
  if (powerPresent.IsUpdated()) {
    lv_obj_set_hidden(batteryPlug, !powerPresent.Get());
  }

  batteryPercentRemaining = batteryController.PercentRemaining();
  if (batteryPercentRemaining.IsUpdated()) {
    auto batteryPercent = batteryPercentRemaining.Get();
    batteryIcon.SetBatteryPercentage(batteryPercent);
  }

  bleState = bleController.IsConnected();
  bleRadioEnabled = bleController.IsRadioEnabled();
  if (bleState.IsUpdated() || bleRadioEnabled.IsUpdated()) {
    lv_obj_set_hidden(bleIcon, !bleState.Get());
  }

  alarmEnabled = alarmController.State() == Controllers::AlarmController::AlarmState::Set;
  if(alarmEnabled.IsUpdated()){
    lv_obj_set_hidden(alarmIcon, !alarmEnabled.Get());
  }

  lv_obj_realign(container);
}
