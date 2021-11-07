#ifndef _SPINDLE_H_
#define _SPINDLE_H_

#include "menu.h"

#ifdef __cplusplus
extern "C" {
#endif


void menuSpindle(void);
void spindleHandler(KEY_VALUES selectedItem);
extern ITEM spindleMenuItem;//= {ICON_SPINDLE, LABEL_SPINDLE,spindleHandler};

#ifdef __cplusplus
}
#endif

#endif
