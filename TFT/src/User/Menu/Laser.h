#ifndef _LASER_H_
#define _LASER_H_
#include "menu.h"

#ifdef __cplusplus
extern "C" {
#endif


void menuLaser(void);
void laserMenuHandler(KEY_VALUES selectedItem);
extern ITEM laserMenuItem ;

#ifdef __cplusplus
}
#endif

#endif
