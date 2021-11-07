#ifndef _LASER_H_
#define _LASER_H_
#include "menu.h"

#ifdef __cplusplus
extern "C" {
#endif


void menuLaser(void);
void laserMenuHandler(KEY_VALUES selectedItem);
extern ITEM laserMenuItem ;//= {ICON_LASER, LABEL_BACKGROUND, laserMenuHandler};

#ifdef __cplusplus
}
#endif

#endif
