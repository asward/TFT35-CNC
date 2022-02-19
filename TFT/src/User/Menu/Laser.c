#include "Laser.h"
#include "includes.h"
#define LASER_IO_ID BED
#define LASER_PWR_ID 1

static bool laserOn = false;

void setLaserPower(uint8_t pwrPercent){
  fanSetPercent(LASER_PWR_ID,pwrPercent);
}

uint8_t getLaserPower(){
  return fanGetCurPercent(LASER_PWR_ID);
}

void setLaserIO(bool on){
    heatSetTargetTemp(LASER_IO_ID, on ? 0 : 255);
}

void laserPowerUp5Handler(KEY_VALUES selectedItem){
  if(laserOn){
    //Find laser tool
    //Use SpeedControl.c to set tool speed
    //Do nothing if no laser tool defined
    uint16_t curPower =  getLaserPower();
    curPower = curPower + 5;
    setLaserPower(curPower);

    LABELCHAR(tempMsg, LABEL_LASER);
    sprintf(&tempMsg[strlen(tempMsg)], " %s", textSelect(LABEL_UP));
    addToast(DIALOG_TYPE_INFO, tempMsg);
  }
}
void laserPowerDown5Handler(KEY_VALUES selectedItem){
  if(laserOn){
    //Find laser tool
    //Use SpeedControl.c to set tool speed
    //Do nothing if no laser tool defined
    
    uint16_t curPower =  getLaserPower();
    curPower = curPower - 5;
    setLaserPower(curPower);
    curPower =  getLaserPower();

    LABELCHAR(tempMsg, LABEL_LASER);
    sprintf(&tempMsg[strlen(tempMsg)], " %s", textSelect(LABEL_DOWN));
    addToast(DIALOG_TYPE_INFO, tempMsg);
  }
}
void laserOnOffToggleHandler(KEY_VALUES selectedItem){
  //Find laser tool
  laserOn = !laserOn;
  setLaserIO(laserOn);
  
  LABELCHAR(tempMsg, LABEL_LASER);
  if(laserOn){
    sprintf(&tempMsg[strlen(tempMsg)], " %s", textSelect(LABEL_ON));
  } else{
    sprintf(&tempMsg[strlen(tempMsg)], " %s", textSelect(LABEL_OFF));
  };

  addToast(DIALOG_TYPE_INFO, tempMsg);

  getCurMenuItems()->items[selectedItem].icon = laserOn ? ICON_RGB_RED : ICON_RGB_OFF;
}


ITEM laserOnOffToggleItem = {ICON_RGB_OFF, "OFF", laserOnOffToggleHandler};
void updateLaserOnOffToggleItem(){
  laserOnOffToggleItem.icon = laserOn ? ICON_RGB_RED : ICON_RGB_OFF;
  // laserOnOffToggleItem.label = laserOn ? LABEL_ON : LABEL_OFF;
}
// On select of laser item
void laserMenuHandler(KEY_VALUES selectedItem){
   OPEN_MENU(menuLaser);
}

ITEM laserMenuItem = {ICON_LASER, LABEL_LASER, laserMenuHandler};
ITEM laserPowerUp5Item = {ICON_INC, "+5%", laserPowerUp5Handler};
ITEM laserPowerDown5Item = {ICON_DEC, "-5%", laserPowerDown5Handler};

//  Define laser menu event handling
void menuLaser(void)
{
  MENUITEMS laserItems;

  initDefaultMenu(&laserItems, LABEL_LASER, true);
  
  laserItems.items[KEY_ICON_0] = laserPowerUp5Item;
  laserItems.items[KEY_ICON_3] = laserPowerDown5Item;
  laserItems.items[KEY_ICON_4] = laserOnOffToggleItem;

  menuDrawPage(&laserItems);

  while (MENU_IS(menuLaser))
  {
    menuRunItemDefaultHandler();

    loopProcess();
  }
}
