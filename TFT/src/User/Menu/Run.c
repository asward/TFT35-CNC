#include "Run.h"
#include "includes.h"

// #define NUM_PER_PAGE 5
// static bool list_mode = true;

void selectEnd(void){
  // prevent reset if printing from other source
  if (!infoHost.printing)  
  {
    resetInfoFile();
    powerFailedSetDriverSource(getCurFileSource());
  }
}
void tftSDMenuHandler(KEY_VALUES selectedSetting){
  // list_mode = infoSettings.files_list_mode;  // follow list mode setting in TFT sd card
  infoFile.source = TFT_SD;
  OPEN_MENU(menuLoadGCodeFromSource);
  OPEN_MENU(menuPrintRestore);
  selectEnd();
}
void uDiskMenuHandler(KEY_VALUES selectedSetting){
  
  // list_mode = infoSettings.files_list_mode;  // follow list mode setting in usb disk
  infoFile.source = TFT_UDISK;
  OPEN_MENU(menuLoadGCodeFromSource);
  OPEN_MENU(menuPrintRestore);
  selectEnd();
}

void onBoardSDHandler(KEY_VALUES selectedSetting){
  
    // list_mode = true;  // force list mode in Onboard sd card
    infoFile.source = BOARD_SD;
    OPEN_MENU(menuLoadGCodeFromSource);  // TODO: fix here,  onboard sd card PLR feature
    selectEnd();
}

void printInfoHander(KEY_VALUES selectedSetting){
  if (infoPrintSummary.name[0] != 0)
    printInfoPopup();
}
ITEM tftSDMenuItem =       {ICON_ONTFT_SD, LABEL_TFTSD, tftSDMenuHandler};
ITEM uDiskMenuItem =       {ICON_U_DISK, LABEL_U_DISK, uDiskMenuHandler};
ITEM onBoardSDMenuItem =       {ICON_ONBOARD_SD, LABEL_ONBOARDSD, onBoardSDHandler};
ITEM printInfoMenuItem =       {ICON_SCREEN_INFO, LABEL_INFO, printInfoHander};

void menuRun(void)
{
  MENUITEMS runItems;

  initDefaultMenu(&runItems, LABEL_CUT, true);

  runItems.items[KEY_ICON_0] = tftSDMenuItem;
  runItems.items[KEY_ICON_1] = uDiskMenuItem;
  if (infoMachineSettings.onboardSD == ENABLED)
  {
    runItems.items[KEY_ICON_2] = onBoardSDMenuItem;
  }
  
  runItems.items[KEY_ICON_4] = printInfoMenuItem;


  menuDrawPage(&runItems);

  while (MENU_IS(menuRun))
  {
    menuRunItemDefaultHandler();

    loopProcess();
  }
}
void runMenuHander(KEY_VALUES selectedSetting){
  
  if (infoMachineSettings.firmwareType == FW_REPRAPFW)
  {
    // // NO REPRAP
    // list_mode = infoSettings.files_list_mode;
    // infoFile.source = BOARD_SD;
    // REPLACE_MENU(menuPrintFromSource);
    // selectEnd();

  } else {
    OPEN_MENU(menuRun);
  }
}

ITEM runMenuItem = {ICON_FILE, LABEL_CUT, runMenuHander};