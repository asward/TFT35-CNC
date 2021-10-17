#ifndef _EMER_STOP_H
#define _EMER_STOP_H

#ifdef __cplusplus
extern "C" {
#endif

#define EMERSTOP_ITEM  {ICON_STOP, LABEL_EMERGENCYSTOP,emerStopHandler}

void emerStopHandler(void);

#ifdef __cplusplus
}
#endif

#endif
