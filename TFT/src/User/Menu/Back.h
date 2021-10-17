#ifndef _BACK_H
#define _BACK_H

#ifdef __cplusplus
extern "C" {
#endif

#define BACK_ITEM  {ICON_BACK, LABEL_BACK, backHandler}

void backHandler(void);

#ifdef __cplusplus
}
#endif

#endif
