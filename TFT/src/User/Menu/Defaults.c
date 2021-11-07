#include "Defaults.h"
#include "includes.h"

ITEM backItem = {ICON_BACK, LABEL_BACK, backHandler};

static const MENUITEMS EMPTY_MENU = {
  .title = LABEL_BACKGROUND,
  .items = {    
    {ICON_BACKGROUND, LABEL_BACKGROUND},
    {ICON_BACKGROUND, LABEL_BACKGROUND},
    {ICON_BACKGROUND, LABEL_BACKGROUND},
    {ICON_BACKGROUND, LABEL_BACKGROUND},

    {ICON_BACKGROUND, LABEL_BACKGROUND},
    {ICON_BACKGROUND, LABEL_BACKGROUND},
    {ICON_BACKGROUND, LABEL_BACKGROUND},
    {ICON_BACKGROUND, LABEL_BACKGROUND}
    // {ICON_BACK, LABEL_BACK,backHandler}
  }
};
// On select of laser item
void backHandler(void){
   CLOSE_MENU();
}

// On select of laser item
void emptyHandler(void){
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