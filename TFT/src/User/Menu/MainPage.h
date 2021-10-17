#ifndef _MAINPAGE_H_
#define _MAINPAGE_H_

#ifdef __cplusplus
extern "C" {
#endif
#define MAINMENU_ITEM {ICON_MAINMENU, LABEL_MAINMENU,mainHandler}
void mainHandler(void);
void menuMain(void);

#ifdef __cplusplus
}
#endif

#endif
