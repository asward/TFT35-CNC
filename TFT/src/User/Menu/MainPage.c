#include "MainPage.h"
#include "includes.h"


void mainHandler(KEY_VALUES selectedItem){
  OPEN_MENU(menuMain);
}

void menuMain(void)
{
  MENUITEMS mainPageItems;
  initDefaultMenu(&mainPageItems, LABEL_MAINMENU, infoSettings.status_screen == 1);
  
  mainPageItems.items[KEY_ICON_0] = runMenuItem; //RUN
  mainPageItems.items[KEY_ICON_1] = moveMenuItem; //MOVE
  mainPageItems.items[KEY_ICON_2] = homeMenuItem; //HOME
  mainPageItems.items[KEY_ICON_3] = emerStopItem ; //EMER STOP

  mainPageItems.items[KEY_ICON_4] = settingsMenuItem; //SETTINGS
  mainPageItems.items[KEY_ICON_5] = laserMenuItem; //LASER
  mainPageItems.items[KEY_ICON_6] = spindleMenuItem; //SPINDLE
  // mainPageItems.items[KEY_ICON_7] = backItem; //BACK

 
  menuDrawPage(&mainPageItems);

  while (MENU_IS(menuMain))
  {menuRunItemDefaultHandler();

    loopProcess();
  }
}

ITEM mainMenuItem = {ICON_MAINMENU,LABEL_MAINMENU,mainHandler};