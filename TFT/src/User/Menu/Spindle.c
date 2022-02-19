#include "Spindle.h"
#include "includes.h"

ITEM spindleMenuItem = {ICON_SPINDLE, LABEL_SPINDLE,spindleHandler};

void spindleHandler(KEY_VALUES selectedItem){
  OPEN_MENU(menuSpindle);
};

void menuSpindle(void)
{
  MENUITEMS spindleItems;

  initDefaultMenu(&spindleItems, LABEL_SPINDLE, true);

  menuDrawPage(&spindleItems);

  while (MENU_IS(menuSpindle))
  {menuRunItemDefaultHandler();
    loopProcess();
  }
}
