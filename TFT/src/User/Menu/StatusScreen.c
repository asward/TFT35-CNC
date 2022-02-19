#include "StatusScreen.h"
#include "includes.h"
#define UPDATE_TOOL_TIME 2000
static int8_t lastConnection_status = -1;
static bool msgNeedRefresh = false;

static char msgtitle[20];
static char msgbody[MAX_MSG_LENGTH];

// // // const char *const SpeedID[2] = SPEED_ID;
// // // text position rectangles for Live icons
// // // icon 0
const GUI_POINT ss_title_point = {SSICON_WIDTH - BYTE_WIDTH / 2, SSICON_NAME_Y0};
const GUI_POINT ss_val_point   = {SSICON_WIDTH / 2, SSICON_VAL_Y0};
// // #ifdef TFT70_V3_0
// //   const GUI_POINT ss_val2_point = {SSICON_WIDTH/2, SSICON_VAL2_Y0};
// // #endif

// // info box msg area
const  GUI_RECT msgRect = {START_X + 1 * ICON_WIDTH + 1 * SPACE_X + 2,   ICON_START_Y +  1 * ICON_HEIGHT + 1 * SPACE_Y + STATUS_MSG_BODY_YOFFSET,
                           START_X + 3 * ICON_WIDTH + 2 * SPACE_X - 2,   ICON_START_Y +  2 * ICON_HEIGHT + 1 * SPACE_Y - STATUS_MSG_BODY_BOTTOM};

const GUI_RECT RecGantry = {START_X,                                SSICON_HEIGHT + ICON_START_Y + STATUS_GANTRY_YOFFSET,
                            START_X + 4 * ICON_WIDTH + 3 * SPACE_X, ICON_HEIGHT + SPACE_Y + ICON_START_Y - STATUS_GANTRY_YOFFSET};

void drawStatus(void)
{
  // icons and their values are updated one by one to reduce flicker/clipping
  char tempstr[45];

  GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
  GUI_SetColor(GANTRYLBL_COLOR);
  GUI_SetBkColor(infoSettings.status_xyz_bg_color);
  sprintf(tempstr, "   X: %.2f   Y: %.2f   Z: %.2f   ", coordinateGetAxisActual(X_AXIS), coordinateGetAxisActual(Y_AXIS),
          coordinateGetAxisActual(Z_AXIS));
  GUI_DispStringInPrect(&RecGantry, (uint8_t *)tempstr);

  GUI_RestoreColorDefault();
}

void statusScreen_setMsg(const uint8_t *title, const uint8_t *msg)
{
  strncpy(msgtitle, (char *)title, sizeof(msgtitle));
  strncpy(msgbody, (char *)msg, sizeof(msgbody));
  msgNeedRefresh = true;
}

void statusScreen_setReady(void)
{
  strncpy(msgtitle, (char *)textSelect(LABEL_STATUS), sizeof(msgtitle));

  if (infoHost.connected == false)
  {
    strncpy(msgbody, (char *)textSelect(LABEL_UNCONNECTED), sizeof(msgbody));
  }
  else
  {
    strncpy(msgbody, (char *)machine_type, sizeof(msgbody));
    strcat(msgbody, " ");
    strcat(msgbody, (char *)textSelect(LABEL_READY));
  }

  msgNeedRefresh = true;
}

void drawStatusScreenMsg(void)
{
  GUI_SetTextMode(GUI_TEXTMODE_TRANS);

  IMAGE_ReadDisplay(rect_of_keySS[KEY_INFOBOX].x0, rect_of_keySS[KEY_INFOBOX].y0, INFOBOX_ADDR);
  GUI_SetColor(INFOMSG_BKCOLOR);
  GUI_DispString(rect_of_keySS[KEY_INFOBOX].x0 + STATUS_MSG_ICON_XOFFSET,
                 rect_of_keySS[KEY_INFOBOX].y0 + STATUS_MSG_ICON_YOFFSET,
                 IconCharSelect(CHARICON_INFO));

  GUI_DispString(rect_of_keySS[KEY_INFOBOX].x0 + BYTE_HEIGHT + STATUS_MSG_TITLE_XOFFSET,
                 rect_of_keySS[KEY_INFOBOX].y0 + STATUS_MSG_ICON_YOFFSET,
                 (uint8_t *)msgtitle);

  GUI_SetBkColor(INFOMSG_BKCOLOR);
  GUI_FillPrect(&msgRect);
  Scroll_CreatePara(&scrollLine, (uint8_t *)msgbody, &msgRect);
  GUI_RestoreColorDefault();

  msgNeedRefresh = false;
}

static inline void scrollMsg(void)
{
  GUI_SetBkColor(INFOMSG_BKCOLOR);
  GUI_SetColor(INFOMSG_COLOR);
  Scroll_DispString(&scrollLine, CENTER);
  GUI_RestoreColorDefault();
}

static inline void toggleTool(void)
{
  if (nextScreenUpdate(UPDATE_TOOL_TIME))
  {
    // // increment hotend index
    // if (infoSettings.hotend_count > 1)
    //   currentTool = (currentTool + 1) % infoSettings.hotend_count;

    // // switch bed/chamber index
    // if (infoSettings.chamber_en == 1)
    //   currentBCIndex = (currentBCIndex + 1) % 2;

    // // increment fan index
    // if ((infoSettings.fan_count + infoSettings.ctrl_fan_en) > 1)
    // {
    //   do
    //   {
    //     currentFan = (currentFan + 1) % MAX_FAN_COUNT;
    //   } while (!fanIsValid(currentFan));
    // }

    // // switch speed/flow
    // currentSpeedID = (currentSpeedID + 1) % 2;
    drawStatus();

    // gcode queries must be call after drawStatus
    coordinateQuery(UPDATE_TOOL_TIME / 1000);
    // speedQuery();
    // ctrlFanQuery();
  }
}

void menuBefore(MENUITEMS *menuItems){

  GUI_SetBkColor(infoSettings.bg_color);
  menuDrawPage(menuItems);
  GUI_SetColor(infoSettings.status_xyz_bg_color);
  GUI_FillPrect(&RecGantry);
  drawStatus();
  drawStatusScreenMsg();
}
void menuAfter(void){
  // disable position auto report
  coordinateQuery(0);
}
void menuStatus(void)
{
  
  MENUITEMS statusItems;

  initDefaultMenu(&statusItems, LABEL_STATUS, false);
  
  statusItems.items[KEY_ICON_0] = runMenuItem; //RUN
  statusItems.items[KEY_ICON_1] = moveMenuItem; //MOVE
  statusItems.items[KEY_ICON_2] = homeMenuItem; //HOME
  statusItems.items[KEY_ICON_3] = emerStopItem ; //EMER STOP

  statusItems.items[KEY_ICON_4] = terminalMenuItem ; //TERMINAL
  // statusItems.items[KEY_ICON_5] = //STATUS BLOCK
  // statusItems.items[KEY_ICON_6] = //STATUS BLOCK
  statusItems.items[KEY_ICON_7] = mainMenuItem; //Main Menu

  menuBefore(&statusItems);

  while (MENU_IS(menuStatus))
  {
    if (infoHost.connected != lastConnection_status)
    {
      statusScreen_setReady();
      lastConnection_status = infoHost.connected;
    }

    if (msgNeedRefresh)
      drawStatusScreenMsg();

    scrollMsg();

    menuRunItemDefaultHandler();

    toggleTool();
    loopProcess();
  }

  menuAfter();
}
