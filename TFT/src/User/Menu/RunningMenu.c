/* ==================================================================== */
/* ========================== include files =========================== */
/* ==================================================================== */
#include "Printing.h"
#include "includes.h"

/* ==================================================================== */
/* ============================ constants ============================= */
/* ==================================================================== */
#define TOGGLE_TIME  2000  // 1 seconds is 1000

/* ==================================================================== */
/* ======================== global variables ========================== */
/* ==================================================================== */

/* ==================================================================== */
/* ========================== public data ============================= */
/* ==================================================================== */

/* ==================================================================== */
/* ============================== data ================================ */
/* ==================================================================== */
static PROGRESS_DISPLAY progDisplayType;
static LAYER_TYPE layerDisplayType;
static bool lastPause = false;
static bool  lastPrinting = false;
static uint8_t oldProgress = 0;
static uint16_t curspeed[2] = {0};
static uint32_t time = 0;
static MENUITEMS runningGcodeItems;
enum
{
  ICON_POS_EXT = 0,
  ICON_POS_BED,
  ICON_POS_FAN,
  ICON_POS_TIM,
  ICON_POS_Z,
  ICON_POS_SPD,
};


/* ==================================================================== */
/* =========================== functions ============================== */
/* ==================================================================== */

static void setLayerHeightText(char * layer_height_txt)
{
  float layer_height;
  layer_height = (infoFile.source >= BOARD_SD) ? coordinateGetAxisActual(Z_AXIS) : coordinateGetAxisTarget(Z_AXIS);
  if (layer_height > 0)
  {
    sprintf(layer_height_txt, "%6.2fmm", layer_height);
  }
  else
  {
    strcpy(layer_height_txt, " --- mm ");  // leading and trailing space char so the text is centered on both rows
  }
}
static void setLayerNumberTxt(char * layer_number_txt)
{
  uint16_t layerNumber = getPrintLayerNumber();
  uint16_t layerCount = getPrintLayerCount();
  if (layerNumber > 0)
  {
    if (layerCount > 0 && layerCount < 1000)
    { // there's no space to display layer number & count if the layer count is above 999
      sprintf(layer_number_txt, " %u/%u ", layerNumber, layerCount);
    }
    else
    {
      sprintf(layer_number_txt, "%s%u%s", "  ", layerNumber, "  ");
    }
  }
  else
  {
    strcpy(layer_number_txt, "---");
  }
}
static inline void reDrawPrintingValue(uint8_t icon_pos, uint8_t draw_type)
{
  uint8_t icon = printingIcon[icon_pos];
  char tempstr[14];

  switch (icon_pos)
  {
    case ICON_POS_BED:
      if (currentBCIndex != 0)
        icon = printingIcon2nd[0];
      break;

    case ICON_POS_SPD:
      if (currentSpeedID != 0)
        icon = printingIcon2nd[1];
      break;

    default:
      break;
  }

  if (draw_type & PRINT_ICON)
  {
    ICON_ReadDisplay(rect_of_keyPS[icon_pos].x0, rect_of_keyPS[icon_pos].y0, icon);
    GUI_SetTextMode(GUI_TEXTMODE_TRANS);
  }
  else
  {
    ICON_PrepareRead(rect_of_keyPS[icon_pos].x0, rect_of_keyPS[icon_pos].y0, icon);
  }

  #ifdef UNIFORM_LIVE_TEXT_BG_COLOR  // it set the sampled background color as the background color to be used
    GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
    GUI_SetBkColor(textBgColor);
  #endif

  if (draw_type & PRINT_TOP_ROW)
  {
    uint8_t* textString = NULL;  // always initialize to NULL pointer as default value

    switch (icon_pos)
    {
      case ICON_POS_EXT:
        textString = (uint8_t *)heatDisplayID[currentTool];
        break;

      case ICON_POS_BED:
        textString = (uint8_t *)heatDisplayID[BED + currentBCIndex];
        break;

      case ICON_POS_FAN:
        textString = (uint8_t *)fanID[currentFan];
        break;

      case ICON_POS_TIM:
      {
        if ((getPrintRemainingTime() == 0) || (progDisplayType != ELAPSED_REMAINING))
        {
          uint8_t printProgress = getPrintProgress();
          snprintf(tempstr, 9, "%d%%      ", printProgress);
        }
        else
        {
          uint8_t hour, min, sec;

          getPrintTimeDetail(&hour, &min, &sec);
          sprintf(tempstr, "%02u:%02u:%02u", hour, min, sec);
        }
        textString = (uint8_t *)tempstr;
        break;
      }

      case ICON_POS_Z:
        if (layerDisplayType == SHOW_LAYER_BOTH)
        {
          setLayerHeightText(tempstr);
        }
        else if (layerDisplayType == CLEAN_LAYER_NUMBER || layerDisplayType == CLEAN_LAYER_BOTH)
        {
          strcpy(tempstr, "        ");
        }
        else
        {
        //   strcpy(tempstr, LAYER_TITLE);
        }
        textString = (uint8_t *)tempstr;
        break;

      case ICON_POS_SPD:
        textString = (uint8_t *)speedId[currentSpeedID];
        break;

      default:
        break;
    }

    GUI_DispString(rect_of_keyPS[icon_pos].x0 + PICON_TITLE_X, rect_of_keyPS[icon_pos].y0 + PICON_TITLE_Y, textString);
  }

  if (draw_type & PRINT_BOTTOM_ROW)
  {
    tempstr[0] = 0;  // always initialize to empty string as default value

    switch (icon_pos)
    {
      case ICON_POS_EXT:
        sprintf(tempstr, "%3d/%-3d", heatGetCurrentTemp(currentTool), heatGetTargetTemp(currentTool));
        break;

      case ICON_POS_BED:
        sprintf(tempstr, "%3d/%-3d", heatGetCurrentTemp(BED + currentBCIndex), heatGetTargetTemp(BED + currentBCIndex));
        break;

      case ICON_POS_FAN:
        if (infoSettings.fan_percentage == 1)
          sprintf(tempstr, "%3d%%", fanGetCurPercent(currentFan));  // 4 chars
        else
          sprintf(tempstr, "%3d ", fanGetCurSpeed(currentFan));  // 4 chars
        break;

      case ICON_POS_TIM:
        {
          uint8_t hour, min, sec;

          if ((getPrintRemainingTime() == 0) || (progDisplayType == PERCENTAGE_ELAPSED))
            getPrintTimeDetail(&hour, &min, &sec);
          else
            getPrintRemainingTimeDetail(&hour, &min, &sec);

          sprintf(tempstr, "%02u:%02u:%02u", hour, min, sec);
        }
        break;

      case ICON_POS_Z:
        if (layerDisplayType == SHOW_LAYER_HEIGHT)  // layer height
        {
          setLayerHeightText(tempstr);
        }
        else if (layerDisplayType == SHOW_LAYER_NUMBER || layerDisplayType == SHOW_LAYER_BOTH)  // layer number or height & number (both)
        {
          setLayerNumberTxt(tempstr);
        }
        else
        {
          strcpy(tempstr, "        ");
        }
        break;

      case ICON_POS_SPD:
        sprintf(tempstr, "%3d%%", speedGetCurPercent(currentSpeedID));
        break;

      default:
        break;
    }

    GUI_DispStringInPrect(&printinfo_val_rect[icon_pos], (uint8_t *)tempstr);
  }

  if (!(draw_type & PRINT_ICON))
    ICON_PrepareReadEnd();
}  // reDrawPrintingValue
static inline void reDrawProgressBar(uint8_t prevProgress, uint8_t nextProgress, uint16_t barColor, uint16_t sliceColor)
{
  uint16_t start = (PROGRESS_BAR_FULL_WIDTH * prevProgress) / 100;
  uint16_t end = (PROGRESS_BAR_FULL_WIDTH * nextProgress) / 100;

  GUI_FillRectColor(progressBar.x0 + start, progressBar.y0, progressBar.x0 + end, progressBar.y1, barColor);

  #ifdef MARKED_PROGRESS_BAR
    GUI_SetColor(sliceColor);

    start = prevProgress / 10 + 1;  // number of 10% markers + 1 (to skip redraw of 0% and already drawn marker)
    end = nextProgress / 10;        // number of 10% markers

    if (end == 10)  // avoid to draw the marker for 100% progress
      end--;

    for (int i = start; i <= end; i++)
      GUI_VLine(progressBar.x0 + PROGRESS_BAR_SLICE_WIDTH * i - 1, progressBar.y0 + 1, progressBar.y1 - 1);

    GUI_RestoreColorDefault();
  #endif
}
static inline void reDrawProgress(uint8_t prevProgress)
{
  uint8_t nextProgress = getPrintProgress();

  if (nextProgress != prevProgress)
  { // we need speed, do not draw anything if progress isn't changed
    if (nextProgress > prevProgress)
      reDrawProgressBar(prevProgress, nextProgress, MAT_ORANGE, BLACK);
    else  // if regress, swap indexes and colors
      reDrawProgressBar(nextProgress, prevProgress, DARKGRAY, MAT_ORANGE);
    if (progDisplayType != ELAPSED_REMAINING)
    {
      reDrawPrintingValue(ICON_POS_TIM, PRINT_TOP_ROW);
    }
  }
}
static inline void runningDrawPage(void)
{
  #ifdef UNIFORM_LIVE_TEXT_BG_COLOR  // it samples the background color from an icon
    ICON_PrepareRead(rect_of_keyPS[ICON_POS_EXT].x0, rect_of_keyPS[ICON_POS_EXT].y0, ICON_PRINTING_NOZZLE);
    textBgColor = ICON_ReadPixel(rect_of_keyPS[ICON_POS_EXT].x0 + PICON_TITLE_X, rect_of_keyPS[ICON_POS_EXT].y0 + PICON_TITLE_Y);
    ICON_PrepareReadEnd();
  #endif

  for (uint8_t i = 0; i < 6; i++)
    reDrawPrintingValue(i, PRINT_ICON | PRINT_TOP_ROW | PRINT_BOTTOM_ROW);

  // progress
  GUI_SetColor(ORANGE);
  GUI_DrawRect(progressBar.x0 - 1, progressBar.y0 - 1, progressBar.x1 + 1, progressBar.y1 + 1);  // draw progress bar border
  GUI_RestoreColorDefault();
  reDrawProgressBar(0, 100, DARKGRAY, MAT_ORANGE);  // draw progress bar
  reDrawProgress(0);  // draw progress
}
static void drawInfo(void)
{
  GUI_SetTextMode(GUI_TEXTMODE_TRANS);

  IMAGE_ReadDisplay(rect_of_keySS[KEY_INFOBOX].x0, rect_of_keySS[KEY_INFOBOX].y0, INFOBOX_ADDR);

  GUI_SetColor(INFOMSG_BKCOLOR);
  GUI_DispString(rect_of_keySS[KEY_INFOBOX].x0 + STATUS_MSG_ICON_XOFFSET, rect_of_keySS[KEY_INFOBOX].y0 + STATUS_MSG_ICON_YOFFSET,
                 IconCharSelect(CHARICON_INFO));
  GUI_DispStringInRectEOL(rect_of_keySS[KEY_INFOBOX].x0 + BYTE_HEIGHT + STATUS_MSG_TITLE_XOFFSET,
                          rect_of_keySS[KEY_INFOBOX].y0 + STATUS_MSG_ICON_YOFFSET,
                          rect_of_keySS[KEY_INFOBOX].x1 - STATUS_MSG_TITLE_XOFFSET,
                          rect_of_keySS[KEY_INFOBOX].y1 - STATUS_MSG_ICON_YOFFSET,
                          (uint8_t *)textSelect(LABEL_PRINT_FINISHED));

  GUI_SetColor(INFOMSG_COLOR);
  GUI_SetBkColor(INFOMSG_BKCOLOR);
  GUI_DispStringInPrect(&msgRect, LABEL_CLICK_FOR_MORE);
  GUI_RestoreColorDefault();
}
static void stopConfirm(void)
{
    printAbort();
    CLOSE_MENU();
}
static void pauseHandler(KEY_VALUES selectedItem){
    if (getHostDialog())
        addToast(DIALOG_TYPE_ERROR, (char *)textSelect(LABEL_BUSY));
    else if (getPrintRunout())
        addToast(DIALOG_TYPE_ERROR, (char *)textSelect(LABEL_FILAMENT_RUNOUT));
    else
        printPause(!isPaused(), PAUSE_NORMAL);
}
static void resumeHandler(KEY_VALUES selectedItem){
          clearInfoPrint();
          clearInfoFile();
          infoMenu.cur = 0;
}
static void moreHandler(KEY_VALUES selectedItem){
        OPEN_MENU(menuMore);

}
static void stopHandler(KEY_VALUES selectedItem){
    if (isPrinting())
    {
        setDialogText(LABEL_WARNING, LABEL_STOP_PRINT, LABEL_CONFIRM, LABEL_CANCEL);
        showDialog(DIALOG_TYPE_ALERT, stopConfirm, NULL, NULL);
    }
    else
    {
        clearInfoPrint();
        CLOSE_MENU();
    }
}
static void printInfoHandler(KEY_VALUES selectedItem){
  uint8_t hour = infoPrintSummary.time / 3600;
  uint8_t min = infoPrintSummary.time % 3600 / 60;
  uint8_t sec = infoPrintSummary.time % 60;
  char showInfo[150];
  char tempstr[30];

  sprintf(showInfo, (char*)textSelect(LABEL_PRINT_TIME), hour, min, sec);

  if (infoPrintSummary.length + infoPrintSummary.weight + infoPrintSummary.cost == 0)  // all  equals 0
  {
    strcat(showInfo, (char *)textSelect(LABEL_NO_FILAMENT_STATS));
  }
  else
  {
    if (infoPrintSummary.length > 0)
    {
      sprintf(tempstr, (char *)textSelect(LABEL_FILAMENT_LENGTH), infoPrintSummary.length);
      strcat(showInfo, tempstr);
    }
    if (infoPrintSummary.weight > 0)
    {
      sprintf(tempstr, (char *)textSelect(LABEL_FILAMENT_WEIGHT), infoPrintSummary.weight);
      strcat(showInfo, tempstr);
    }
    if (infoPrintSummary.cost > 0)
    {
      sprintf(tempstr, (char *)textSelect(LABEL_FILAMENT_COST), infoPrintSummary.cost);
      strcat(showInfo, tempstr);
    }
  }
  popupReminder(DIALOG_TYPE_INFO, (uint8_t *)infoPrintSummary.name, (uint8_t *)showInfo);
}

static void initStatus(void){
    // initialize things before the print starts
    progDisplayType = infoSettings.prog_disp_type;
    layerDisplayType = infoSettings.layer_disp_type * 2;
    lastPause = isPaused();
    lastPrinting = isPrinting();
    oldProgress = 0;
    curspeed[0] = 0;
    curspeed[1] = 0;
    time = 0;

    if (lastPrinting == true)
    {
        if (infoMachineSettings.longFilename == ENABLED && infoFile.source == BOARD_SD)
            runningGcodeItems.title.address = (uint8_t *) infoFile.Longfile[infoFile.fileIndex];
        else
            runningGcodeItems.title.address = getPrintName(infoFile.title);
        // printingItems.items[KEY_ICON_5].icon = (infoFile.source < BOARD_SD && isPrintModelIcon()) ? ICON_PREVIEW : ICON_BABYSTEP;
    }
    else  // returned to this menu after a print was done (ex: after a popup)
    {
        runningGcodeItems.title.address = (uint8_t *)infoPrintSummary.name;
    }
}
static void initMenu(){ 
    if (lastPrinting == false)
    {
        drawInfo();    
    }
}
static void updateProgress(void){

    // check printing progress
    if (getPrintSize() != 0)
    {
        // check print time change
        if (time != getPrintTime())
        {
            time = getPrintTime();
            RAPID_SERIAL_LOOP();  // perform backend printing loop before drawing to avoid printer idling
            if (progDisplayType == ELAPSED_REMAINING)
            {
              reDrawPrintingValue(ICON_POS_TIM, PRINT_TOP_ROW | PRINT_BOTTOM_ROW);
            }
            else
            {
              reDrawPrintingValue(ICON_POS_TIM, PRINT_BOTTOM_ROW);
            }
        }

        // check print progress percentage change
        if (updatePrintProgress())
        {
            RAPID_SERIAL_LOOP();  // perform backend printing loop before drawing to avoid printer idling
            reDrawProgress(oldProgress);
            oldProgress = getPrintProgress();
        }
    }
    else
    {
        if (getPrintProgress() != 100)
        {
            reDrawPrintingValue(ICON_POS_TIM, PRINT_BOTTOM_ROW);
            updatePrintProgress();
            reDrawProgress(oldProgress);
            oldProgress = getPrintProgress();
        }   
    }
}

/* ==================================================================== */
/* ======================= public functions =========================== */
/* ==================================================================== */
void menuRunningGcode(void)
{
    initStatus();

    ITEM pauseItem = {ICON_PAUSE, LABEL_PAUSE, pauseHandler};
    ITEM resumeItem = {ICON_RESUME, LABEL_RESUME, resumeHandler};
    ITEM moreItem = {ICON_MORE, LABEL_MORE, moreHandler};
    ITEM stopItem = {ICON_STOP, LABEL_STOP, stopHandler};
    ITEM infoItem = {ICON_SCREEN_INFO, LABEL_INFO, printInfoHandler};

    initDefaultMenu(&runningGcodeItems, LABEL_BACKGROUND, true);
    if(isPaused()){        
        runningGcodeItems.items[KEY_ICON_4] = resumeItem;
    }else{
        runningGcodeItems.items[KEY_ICON_4] = pauseItem;
    }
    
    initMenu();

    menuDrawPage(&runningGcodeItems);

    runningDrawPage();

    while (MENU_IS(menuRunningGcode))
    {
        updateProgress();

        // check if print is paused
        if (lastPause != isPaused()) //Toggled to/from pause
        {
            lastPause = isPaused(); //toggle our status - lastPause is now our current pause state
            if(lastPause){ //we're paused
                runningGcodeItems.items[KEY_ICON_4] = resumeItem;
            }else{ //we're resumed
                runningGcodeItems.items[KEY_ICON_4] = pauseItem;
            }
            menuDrawItem(&runningGcodeItems.items[KEY_ICON_4], KEY_ICON_4);
        }

        // check if print just started or just finished
        if (lastPrinting != isPrinting()) //Toggle to/from printing
        {
            return;  // It will restart this interface if directly return this function without modify the value of infoMenu
        }

        KEY_VALUES key_num = menuRunItemDefaultHandler();

        loopProcess();
    }
}
