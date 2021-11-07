#include "Cut.h"
#include "includes.h"

const MENUITEMS cutItems = {
  // title
  LABEL_CUT,
  // icon                          label
  {
    // {ICON_HOME,                    LABEL_HOME},
    // {ICON_X_HOME,                  LABEL_X},
    // {ICON_Y_HOME,                  LABEL_Y},
    // {ICON_Z_HOME,                  LABEL_Z},
    // {ICON_BACKGROUND,              LABEL_BACKGROUND},
    // {ICON_BACKGROUND,              LABEL_BACKGROUND},
    // {ICON_BACKGROUND,              LABEL_BACKGROUND},
    {ICON_BACK,                    LABEL_BACK},
  }
};

void menuCut(void)
{
  KEY_VALUES key_num = KEY_IDLE;

  menuDrawPage(&cutItems);

  while (MENU_IS(menuCut))
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