#include "MainPage.h"
#include "includes.h"

void mainHandler(void){
  OPEN_MENU(menuMain);
}

void menuMain(void)
{
  // 1 title, ITEM_PER_PAGE items (icon + label)
  MENUITEMS mainPageItems = {
    // title
    LABEL_MAINMENU,
    // icon                          label
    {
      [KEY_ICON_0]={ICON_MOVE, LABEL_MOVE},
      {ICON_HOME,                    LABEL_HOME}, 
      {ICON_GCODE,                   LABEL_TERMINAL},
      [KEY_ICON_3]=EMERSTOP_ITEM,
      {ICON_LASER,                   LABEL_LASER},
      {ICON_SPINDLE,                 LABEL_SPINDLE},
      {ICON_SETTINGS,                LABEL_SETTINGS},
      [KEY_ICON_7]=BACK_ITEM    
    }
  };
 
  KEY_VALUES key_num = KEY_IDLE;

  // CHANGE 'BACK' TO 'CUT' WITHOUT STATUS SCREEN
  if (infoSettings.status_screen != 1)
  {
    mainPageItems.items[7].icon = ICON_PRINT;
    mainPageItems.items[7].label.index = LABEL_PRINT;
  }

  menuDrawPage(&mainPageItems);

  while (MENU_IS(menuMain))
  {
    key_num = menuRunItemDefaultHandler();

    //After switch
    switch (key_num)
    {
      // case KEY_ICON_0:
      //   OPEN_MENU(menuMove);
      //   break;

      // case KEY_ICON_1:
      //   OPEN_MENU(menuHome);
      //   break;

      // case KEY_ICON_2:
      //   OPEN_MENU(menuTerminal);
      //   break;

      // case KEY_ICON_3:
      //   // Emergency Stop : Used for emergency stopping, a reset is required to return to operational mode.
      //   // it may need to wait for a space to open up in the command queue.
      //   // Enable EMERGENCY_PARSER in Marlin Firmware for an instantaneous M112 command.
      //   Serial_Puts(SERIAL_PORT, "M112\n");
      //   break;

      // case KEY_ICON_4:
      //   OPEN_MENU(menuLaser);
      //   break;

      // case KEY_ICON_5:
      //   OPEN_MENU(menuSpindle);
      //   break;

      // case KEY_ICON_6:
      //   OPEN_MENU(menuSettings);
      //   break;

      // case KEY_ICON_7:
      //   // OPEN CUT IF NO STATUS SCREEN
      //   if (infoSettings.status_screen != 1)
      //     OPEN_MENU(menuCut);
      //   else
      //     CLOSE_MENU();
      //   break;

      default:
        break;
    }

    loopProcess();
  }
}

