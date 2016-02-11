/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <linux/input.h>

#include "common.h"
#include "device.h"
#include "screen_ui.h"

#include "roots.h"

static const char* HEADERS[] = { "Swipe up/down to change selections;",
                                 "swipe right to select, or left to go back.",
                                 "",
                                 NULL };
                                 
static const char* ITEMS[] = {"Reboot system now", 
							   "Ubuntu actions",
							   "Android actions",
#ifdef DOWNLOAD_MODE
                               "Reboot to download mode",
#else
                               "Reboot to bootloader",
#endif
                               "Power down",
                               "View recovery logs",
                               NULL };

static const char* ANDROID_ITEMS[] =  { "Reboot system now",
                               "Apply update",
                               "Wipe data/factory reset",
                               "Wipe cache partition",
                               "Wipe media",
							   "Go back",
                               NULL };
                               
static const char* UBUNTU_ITEMS[] = { "Reboot system now",
								"Replace ubuntu android system",
								"Install ubuntu zip",
								"Install ubuntu preinstalled",
								"Wipe data/factory reset",
								"Go back",
								NULL};

static Device::BuiltinAction ACTIONS[] = {
    Device::REBOOT,
    Device::ACTION_MODE_UBUNTU,
    Device::ACTION_MODE_ANDROID,
    Device::REBOOT_BOOTLOADER,
    Device::SHUTDOWN,
    Device::READ_RECOVERY_LASTLOG,
    Device::NO_ACTION
};

static Device::BuiltinAction ANDROID_ACTIONS[] = {
    Device::REBOOT,
    Device::APPLY_UPDATE,
    Device::WIPE_DATA,
    Device::WIPE_CACHE,
    Device::WIPE_MEDIA,
    Device::GO_BACK,
    Device::NO_ACTION
};

static Device::BuiltinAction UBUNTU_ACTIONS[] = {
    Device::REBOOT,
    Device::REPLACE_SYSTEM,
    Device::INSTALL_UBUNTU_ZIP,
    Device::INSTALL_UBUNTU_ROOTSTOCK,
    Device::WIPE_DATA_UBUNTU,
    Device::GO_BACK,
    Device::NO_ACTION
};

extern int ui_root_menu;

class DefaultDevice : public Device {
  public:
    DefaultDevice() :
        ui(new ScreenRecoveryUI) {
        // Remove "wipe media" option for non-datamedia devices
        if (!is_data_media()) {
            int i;
            for (i = 4; ITEMS[i+1] != NULL; ++i) {
                ITEMS[i] = ITEMS[i+1];
                ACTIONS[i] = ACTIONS[i+1];
            }
            ITEMS[i] = NULL;
            ACTIONS[i] = NO_ACTION;
        }
    }

    RecoveryUI* GetUI() { return ui; }

    int HandleMenuKey(int key, int visible) {
        if (visible) {
            if (key & KEY_FLAG_ABS) {
                return key;
            }
            switch (key) {
              case KEY_RIGHTSHIFT:
              case KEY_DOWN:
              case KEY_VOLUMEDOWN:
              case KEY_MENU:
                return kHighlightDown;

              case KEY_LEFTSHIFT:
              case KEY_UP:
              case KEY_VOLUMEUP:
              case KEY_SEARCH:
                return kHighlightUp;

              case KEY_ENTER:
              case KEY_POWER:
              case BTN_MOUSE:
              case KEY_HOME:
              case KEY_HOMEPAGE:
              case KEY_SEND:
                return kInvokeItem;

              case KEY_BACKSPACE:
              case KEY_BACK:
                if (!ui_root_menu)
                  return kGoBack;
            }
        }

        return kNoAction;
    }

    BuiltinAction InvokeMenuItem(int menu_position, Device::Mode mode) {
		switch(mode){
			case MODE_UBUNTU:
				return MenuItemSelectUbuntu(menu_position);
			case MODE_ANDROID:
				return MenuItemSelectAndroid(menu_position);
			default:
				return MenuItemSelect(menu_position);
		}

    }
    BuiltinAction InvokeMenuItem(int menu_position) {
		return InvokeMenuItem(menu_position, Device::MODE_MENU);
	} 

    const char* const* GetMenuHeaders() { return HEADERS; }
    const char* const* GetMenuItems() { return ITEMS; }
    const char* const* GetMenuItems(Device::Mode mode) { 
		switch(mode){
			case MODE_UBUNTU:
				return UBUNTU_ITEMS;
			case MODE_ANDROID:
				return ANDROID_ITEMS;
			default:
				return ITEMS;
		}
		return ITEMS; 
	}

  private:
    RecoveryUI* ui;
    
    BuiltinAction MenuItemSelect(int menu_position){
		if (menu_position < 0 ||
                menu_position >= (int)(sizeof(ITEMS)/sizeof(ITEMS[0])) ||
                ITEMS[menu_position] == NULL) {
            return NO_ACTION;
        }
        return ACTIONS[menu_position];
		}
		
	    BuiltinAction MenuItemSelectUbuntu(int menu_position){
		if (menu_position < 0 ||
                menu_position >= (int)(sizeof(UBUNTU_ITEMS)/sizeof(UBUNTU_ITEMS[0])) ||
                UBUNTU_ITEMS[menu_position] == NULL) {
            return NO_ACTION;
        }
        return UBUNTU_ACTIONS[menu_position];
		}
		
	    BuiltinAction MenuItemSelectAndroid(int menu_position){
		if (menu_position < 0 ||
                menu_position >= (int)(sizeof(ANDROID_ITEMS)/sizeof(ANDROID_ITEMS[0])) ||
                ANDROID_ITEMS[menu_position] == NULL) {
            return NO_ACTION;
        }
        return ANDROID_ACTIONS[menu_position];
		}
};

Device* make_device() {
    return new DefaultDevice();
}
