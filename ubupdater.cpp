/*
 * Copyright (C) 2016 The Ubports Project
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
 *
 * Authored by: Marius Gripsgard <mariogrip@ubports.com>
 */
 #include <unistd.h>

 #include "ui.h"
 #include "install.h"

 extern "C" {
 #include "libcrecovery/common.h"
 }

 static const char *UBUNTU_COMMAND_FILE = "/cache/recovery/ubuntu_command";
 static const char *UBUNTU_UPDATE_SCRIPT = "/sbin/system-image-upgrader";

//TODO: add error handeling
//TODO: show status (text or progressbar) under update

int do_ubuntu_update(RecoveryUI *ui){
  ui->ShowText(true);
  ui->SetBackground(RecoveryUI::INSTALLING_UPDATE);
  ui->Print("Installing Ubuntu update...\n");
  char tmp[PATH_MAX];
  sprintf(tmp, "%s %s &> /cache/install.log", UBUNTU_UPDATE_SCRIPT, UBUNTU_COMMAND_FILE );
  __system("mount /cache");
  __system(tmp);
  ui->Print("Ubuntu update complete...\n");
  __system("reboot");
  return INSTALL_SUCCESS;
}

int do_test_update(RecoveryUI *ui){
  ui->ShowText(true);
  ui->SetBackground(RecoveryUI::INSTALLING_UPDATE);
  ui->Print("Installing Ubuntu update...\n");
  char tmp[PATH_MAX];
  sprintf(tmp, "%s %s", UBUNTU_UPDATE_SCRIPT, UBUNTU_COMMAND_FILE );
  sleep(10);
  ui->Print("Slept 10");
  sleep(100);
  ui->Print("Ubuntu update complete...\n");
  return INSTALL_SUCCESS;
}
