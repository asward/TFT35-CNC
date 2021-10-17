#include "Spindle.h"
#include "includes.h"

const MENUITEMS spindleItems = {
  // title
  LABEL_LASER,
  // icon                          label
  {
    // {ICON_HOME,                    LABEL_HOME},
    // {ICON_X_HOME,                  LABEL_X},
    // {ICON_Y_HOME,                  LABEL_Y},
    // {ICON_Z_HOME,                  LABEL_Z},
    // {ICON_BACKGROUND,              LABEL_BACKGROUND},
    // {ICON_BACKGROUND,              LABEL_BACKGROUND},
    // {ICON_BACKGROUND,              LABEL_BACKGROUND},
    [KEY_ICON_7]=BACK_ITEM,
  }
};
void spindleHandler(void){

};


void menuSpindle(void)
{
  KEY_VALUES key_num = KEY_IDLE;

  menuDrawPage(&spindleItems);

  while (MENU_IS(menuSpindle))
  {
    key_num = menuKeyGetValue();
    switch (key_num)
    {
    //   case KEY_ICON_0: storeCmd("G28\n");   break;
    //   case KEY_ICON_1: storeCmd("G28 X\n"); break;
    //   case KEY_ICON_2: storeCmd("G28 Y\n"); break;
    //   case KEY_ICON_3: storeCmd("G28 Z\n"); break;
      case KEY_ICON_7: CLOSE_MENU();      break;
      default: break;
    }

    loopProcess();
  }
}
