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
#pragma once

#include <FreeRTOS.h>
#include <lvgl/src/lv_core/lv_obj.h>
#include <string>
#include "displayapp/screens/Screen.h"

namespace Pinetime {
  namespace Controllers {
    class MIDIService;
  }

  namespace Applications {
    namespace Screens {
      class Midi : public Screen {
      public:
        Midi(DisplayApp* app, Pinetime::Controllers::MIDIService& midi);

        ~Midi() override;

        void Refresh() override;

        void OnObjectEvent(lv_obj_t* obj, lv_event_t event);

      private:
        bool OnTouchEvent(TouchEvents event) override;

        lv_obj_t* key1;
        lv_obj_t* key2;
        lv_obj_t* key3;
        lv_obj_t* key4;
        lv_obj_t* key5;
        lv_obj_t* key6;
        
        lv_obj_t* key1_label;
        lv_obj_t* key2_label;
        lv_obj_t* key3_label;
        lv_obj_t* key4_label;
        lv_obj_t* key5_label;
        lv_obj_t* key6_label;

        lv_style_t btn_style, pressed_btn_style;

        Pinetime::Controllers::MIDIService& midiService;

        lv_task_t* taskRefresh;

        /** Watchapp */
      };
    }
  }
}
