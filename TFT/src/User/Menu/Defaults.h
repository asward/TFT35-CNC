#ifndef _DEFAULTS_H_
#define _DEFAULTS_H_

#include "includes.h"
#ifdef __cplusplus
extern "C" {
#endif

extern ITEM backItem;
void initDefaultMenu(MENUITEMS *menuItems, int32_t title, bool backButton);
void backHandler(void);
void emptyHandler(void);

#ifdef __cplusplus
}
#endif

#endif
