#include "Laser.h"
#include "includes.h"


// #define LASER_POWER_UP_5_ITEM {ICON_E_5_PERCENT, "5%%", laserPowerUp5}

void laserPowerUp5(void){
  //Find laser tool
  //Use SpeedControl.c to set tool speed
   //Do nothing if no laser tool defined
  LABELCHAR(tempMsg, LABEL_MBL);

  sprintf(&tempMsg[strlen(tempMsg)], " %s", textSelect(LABEL_ON));

  addToast(DIALOG_TYPE_INFO, tempMsg);
}

const MENUITEMS laserItems = {
  // title
  LABEL_LASER,
  // icon                          label
  {
    // LASER_POWER_UP_5_ITEM,
    // LASER_POWER_UP_5_ITEM,
    // LASER_POWER_UP_5_ITEM,
    // LASER_POWER_UP_5_ITEM,

    {ICON_BACKGROUND,              LABEL_BACKGROUND},
    {ICON_BACKGROUND,              LABEL_BACKGROUND},
    {ICON_BACKGROUND,              LABEL_BACKGROUND},
    BACK_ITEM
  }
};

// On select of laser item
void laserHandler(void){
   OPEN_MENU(menuLaser);
}

//  Define laser menu event handling
void menuLaser(void)
{
  KEY_VALUES key_num = KEY_IDLE;

  menuDrawPage(&laserItems);

  while (MENU_IS(menuLaser))
  {
    key_num = menuRunItemDefaultHandler();

    loopProcess();
  }
}
