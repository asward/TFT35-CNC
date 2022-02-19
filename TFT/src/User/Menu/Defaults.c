#include "Defaults.h"
#include "includes.h"


// On select of laser item
void backHandler(KEY_VALUES selectedKey){
   CLOSE_MENU();
}
ITEM backItem = {ICON_BACK, LABEL_BACK, backHandler};

// On select of laser item
void emptyHandler(KEY_VALUES selectedKey){
}
void initDefaultMenu(MENUITEMS *menuItems, int32_t title, bool backButton) {
  LABEL titleLabel = {.index = title};
  ITEM backItem = {ICON_BACK, LABEL_BACK,backHandler};
  ITEM emptyItem = {ICON_BACKGROUND, LABEL_BACKGROUND, emptyHandler};

  menuItems->title = titleLabel;
  
  menuItems->items[KEY_ICON_0] = emptyItem;
  menuItems->items[KEY_ICON_1] = emptyItem;
  menuItems->items[KEY_ICON_2] = emptyItem;
  menuItems->items[KEY_ICON_3] = emptyItem;
  menuItems->items[KEY_ICON_4] = emptyItem;
  menuItems->items[KEY_ICON_5] = emptyItem;
  menuItems->items[KEY_ICON_6] = emptyItem;
  if(backButton) {
      menuItems->items[KEY_ICON_7] = backItem;
  }else{
      menuItems->items[KEY_ICON_7] = emptyItem;
  }
}