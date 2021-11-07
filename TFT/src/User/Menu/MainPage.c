#include "MainPage.h"
#include "includes.h"


void mainHandler(KEY_VALUES selectedItem){
  OPEN_MENU(menuMain);
}

void menuMain(void)
{
  MENUITEMS mainPageItems;

  initDefaultMenu(&mainPageItems, LABEL_MAINMENU, infoSettings.status_screen == 1);
  
  mainPageItems.items[KEY_ICON_0] = moveMenuItem; //MOVE
  mainPageItems.items[KEY_ICON_1] = homeMenuItem; //HOME
  mainPageItems.items[KEY_ICON_2] = terminalMenuItem; //TERMINAL
  mainPageItems.items[KEY_ICON_3] = emerStopItem ; //EMER STOP
  mainPageItems.items[KEY_ICON_4] = laserMenuItem; //LASER
  mainPageItems.items[KEY_ICON_5] = spindleMenuItem; //SPINDLE
  mainPageItems.items[KEY_ICON_6] = settingsMenuItem; //SETTINGS
 
  KEY_VALUES key_num = KEY_IDLE;

  menuDrawPage(&mainPageItems);

  while (MENU_IS(menuMain))
  {
    key_num = menuRunItemDefaultHandler();

    loopProcess();
  }
}

ITEM mainMenuItem = {ICON_MAINMENU,LABEL_MAINMENU,mainHandler};