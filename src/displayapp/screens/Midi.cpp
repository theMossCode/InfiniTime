/*  Copyright (C) 2020 JF, Adam Pigg, Avamander

    This file is part of InfiniTime.

    InfiniTime is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    InfiniTime is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "displayapp/screens/Midi.h"
#include "displayapp/screens/Symbols.h"
#include <cstdint>
#include "displayapp/DisplayApp.h"
#include "components/ble/MIDIService.h"

using namespace Pinetime::Applications::Screens;

static void event_handler(lv_obj_t* obj, lv_event_t event) {
  Midi* screen = static_cast<Midi*>(obj->user_data);
  screen->OnObjectEvent(obj, event);
}


Midi::Midi(Pinetime::Applications::DisplayApp* app, Pinetime::Controllers::MIDIService& midi) : Screen(app), midiService(midi) {
  lv_style_init(&btn_style);
  lv_style_set_radius(&btn_style, LV_STATE_DEFAULT, 20);
  lv_style_set_bg_color(&btn_style, LV_STATE_DEFAULT, LV_COLOR_AQUA);
  lv_style_set_bg_opa(&btn_style, LV_STATE_DEFAULT, LV_OPA_50);

  lv_style_init(&pressed_btn_style);
  lv_style_set_bg_color(&pressed_btn_style, LV_STATE_PRESSED, LV_COLOR_GREEN);

  key1 = lv_btn_create(lv_scr_act(), nullptr);
  key1->user_data = this;
  lv_obj_set_event_cb(key1, event_handler);
  lv_obj_set_size(key1, 115, 50);
  lv_obj_align(key1, nullptr, LV_ALIGN_IN_TOP_LEFT, 0, 0);
  lv_obj_add_style(key1, LV_BTN_PART_MAIN, &btn_style);
  lv_obj_add_style(key1, LV_BTN_PART_MAIN, &pressed_btn_style);
  key1_label = lv_label_create(key1, nullptr);
  lv_label_set_text_static(key1_label, "1");  

  key2 = lv_btn_create(lv_scr_act(), nullptr);
  key2->user_data = this;
  lv_obj_set_event_cb(key2, event_handler);
  lv_obj_set_size(key2, 115, 50);
  lv_obj_align(key2, nullptr, LV_ALIGN_IN_TOP_LEFT, 125, 0);
  lv_obj_add_style(key2, LV_BTN_PART_MAIN, &btn_style);
  lv_obj_add_style(key2, LV_BTN_PART_MAIN, &pressed_btn_style);
  key2_label = lv_label_create(key2, nullptr);
  lv_label_set_text_static(key2_label, "2");  

  key3 = lv_btn_create(lv_scr_act(), nullptr);
  key3->user_data = this;
  lv_obj_set_event_cb(key3, event_handler);
  lv_obj_set_size(key3, 115, 50);
  lv_obj_align(key3, nullptr, LV_ALIGN_IN_TOP_LEFT, 0, 60);
  lv_obj_add_style(key3, LV_BTN_PART_MAIN, &btn_style);
  lv_obj_add_style(key3, LV_BTN_PART_MAIN, &pressed_btn_style);
  key3_label = lv_label_create(key3, nullptr);
  lv_label_set_text_static(key3_label, "3");  

  key4 = lv_btn_create(lv_scr_act(), nullptr);
  key4->user_data = this;
  lv_obj_set_event_cb(key4, event_handler);
  lv_obj_set_size(key4, 115, 50);
  lv_obj_align(key4, nullptr, LV_ALIGN_IN_TOP_LEFT, 125, 60);
  lv_obj_add_style(key4, LV_BTN_PART_MAIN, &btn_style);
  lv_obj_add_style(key4, LV_BTN_PART_MAIN, &pressed_btn_style);
  key4_label = lv_label_create(key4, nullptr);
  lv_label_set_text_static(key4_label, "4");  

  key5 = lv_btn_create(lv_scr_act(), nullptr);
  key5->user_data = this;
  lv_obj_set_event_cb(key5, event_handler);
  lv_obj_set_size(key5, 115, 50);
  lv_obj_align(key5, nullptr, LV_ALIGN_IN_TOP_LEFT, 0, 120);
  lv_obj_add_style(key5, LV_BTN_PART_MAIN, &btn_style);
  lv_obj_add_style(key5, LV_BTN_PART_MAIN, &pressed_btn_style);
  key5_label = lv_label_create(key5, nullptr);
  lv_label_set_text_static(key5_label, "5");  

  key6 = lv_btn_create(lv_scr_act(), nullptr);
  key6->user_data = this;
  lv_obj_set_event_cb(key6, event_handler);
  lv_obj_set_size(key6, 115, 50);
  lv_obj_align(key6, nullptr, LV_ALIGN_IN_TOP_LEFT, 125, 120);
  lv_obj_add_style(key6, LV_BTN_PART_MAIN, &btn_style);
  lv_obj_add_style(key6, LV_BTN_PART_MAIN, &pressed_btn_style);
  key6_label = lv_label_create(key6, nullptr);
  lv_label_set_text_static(key6_label, "6");  

  taskRefresh = lv_task_create(RefreshTaskCallback, LV_DISP_DEF_REFR_PERIOD, LV_TASK_PRIO_MID, this);
}

Midi::~Midi() {
  lv_task_del(taskRefresh);
  lv_style_reset(&btn_style);
  lv_obj_clean(lv_scr_act());
}

void Midi::Refresh() {
    ;;
}

void Midi::OnObjectEvent(lv_obj_t* obj, lv_event_t event) {
  if (event == LV_EVENT_PRESSED) {
    if (obj == key1) {
      midiService.AppEvent(Pinetime::Controllers::MIDIService::NOTE_ON, 0, 0x3c);
    }
    else if(obj == key2) {
      midiService.AppEvent(Pinetime::Controllers::MIDIService::NOTE_ON, 0, 0x3e);
    }
    else if(obj == key3) {
      midiService.AppEvent(Pinetime::Controllers::MIDIService::NOTE_ON, 0, 0x40);
    }
    else if(obj == key4) {
      midiService.AppEvent(Pinetime::Controllers::MIDIService::NOTE_ON, 0, 0x41);
    }
    else if(obj == key5) {
      midiService.AppEvent(Pinetime::Controllers::MIDIService::NOTE_ON, 0, 0x43);
    }
    else if(obj == key6) {
      midiService.AppEvent(Pinetime::Controllers::MIDIService::NOTE_ON, 0, 0x45);
    }
  }
  if (event == LV_EVENT_RELEASED){
    if (obj == key1) {
      midiService.AppEvent(Pinetime::Controllers::MIDIService::NOTE_OFF, 0, 0x3c);
    }    
    else if(obj == key2) {
      midiService.AppEvent(Pinetime::Controllers::MIDIService::NOTE_OFF, 0, 0x3e);
    }
    else if(obj == key3) {
      midiService.AppEvent(Pinetime::Controllers::MIDIService::NOTE_OFF, 0, 0x40);
    }
    else if(obj == key4) {
      midiService.AppEvent(Pinetime::Controllers::MIDIService::NOTE_OFF, 0, 0x41);
    }
    else if(obj == key5) {
      midiService.AppEvent(Pinetime::Controllers::MIDIService::NOTE_OFF, 0, 0x43);
    }
    else if(obj == key6) {
      midiService.AppEvent(Pinetime::Controllers::MIDIService::NOTE_OFF, 0, 0x45);
    }
  }
}

bool Midi::OnTouchEvent(Pinetime::Applications::TouchEvents event) {
    return false;
}
