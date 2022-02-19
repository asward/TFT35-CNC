#ifndef _DEFAULTS_H_
#define _DEFAULTS_H_

#include "includes.h"
#ifdef __cplusplus
extern "C" {
#endif

extern ITEM backItem;
void initDefaultMenu(MENUITEMS *menuItems, int32_t title, bool backButton);
void backHandler(KEY_VALUES selectedKey);
void emptyHandler(KEY_VALUES selectedKey);

#ifdef __cplusplus
}
#endif

#endif
