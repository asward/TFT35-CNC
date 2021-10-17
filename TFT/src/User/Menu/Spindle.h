#ifndef _SPINDLE_H_
#define _SPINDLE_H_

#ifdef __cplusplus
extern "C" {
#endif
#define SPINDLE_ITEM  {ICON_SPINDLE, LABEL_SPINDLE,spindleHandler}
void spindleHandler(void);

void menuSpindle(void);

#ifdef __cplusplus
}
#endif

#endif
