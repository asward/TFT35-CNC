#include "includes.h"
#define NUM_PER_PAGE 5
bool update = true;
uint8_t pageCount = 1;
static const int16_t labelVolumeError[3] = {LABEL_READ_TFTSD_ERROR, LABEL_READ_U_DISK_ERROR, LABEL_READ_ONBOARDSD_ERROR};
const uint8_t selectedKeyToFileIndex[8] = {8,0,1,8,2,3,4,8}; //* SelectedVal -> page Index for file ops */
const KEY_VALUES iconMenuDyanmicItems[NUM_PER_PAGE] = {KEY_ICON_1,KEY_ICON_2,KEY_ICON_4,KEY_ICON_5,KEY_ICON_6}; 
const GUI_RECT myTitleRect = {10, (TITLE_END_Y - BYTE_HEIGHT) / 2, LCD_WIDTH - 10, (TITLE_END_Y - BYTE_HEIGHT) / 2 + BYTE_HEIGHT};

MENUITEMS loadGCodeItems;

static bool initGCode(void)
{
  // load stat/end/cancel gcodes from spi flash

    switch (infoFile.source)
    {
        case BOARD_SD:  // GCode from file on ONBOARD SD
        {
            uint32_t size;

            size = request_M23_M36(infoFile.title + (infoMachineSettings.firmwareType == FW_REPRAPFW ? 0 : 5));
            //if (powerFailedCreate(infoFile.title) == false)
            //{
            //
            //}  // FIXME: Powerfail resume is not yet supported for ONBOARD_SD. Need more work.

            if (size == 0)
            {
              return false;
            }

            //if (powerFailedExist())
            //{
            request_M24(0);
            //}
            //else
            //{
            //request_M24(infoBreakPoint.offset);
            //}

            printStart(NULL, size);
            break;
        }

        case TFT_UDISK:
        case TFT_SD:  // GCode from file on TFT SD
        {
            FIL file;

            if (f_open(&file, infoFile.title, FA_OPEN_EXISTING | FA_READ) != FR_OK)
            {
            return false;
            }

            if (powerFailedCreate(infoFile.title) == false)
            {}
            powerFailedlSeek(&file);

            printStart(&file, f_size(&file));
            break;
        }

        default:
        {
          return false;
        }
    }
    coordinateSetAxisActual(Z_AXIS, 0);
    coordinateSetAxisTarget(Z_AXIS, 0);
    setM73_presence(false);

    return true;

}
void startGCode(void)
{
    if(initGCode()){ //Init succeded
        OPEN_MENU(menuRunningGcode);
    }else{
        ExitDir();
        // Todo, errors?
    }
}
void selectedItemHandler(uint8_t fileIndex){
    int folderFileIndex = infoFile.cur_page * NUM_PER_PAGE + fileIndex;
    if (folderFileIndex < infoFile.folderCount)  // folder
    {
        if (EnterDir(infoFile.folder[folderFileIndex]) == false) //Failed to enter dir
        {
            update = false;
        }
        else //Entered dir
        {
            scanPrintFiles();
            infoFile.cur_page = 0;
            update = true;
        }
    }
    else if (folderFileIndex < infoFile.folderCount + infoFile.fileCount)  // file
    {
        infoFile.fileIndex = folderFileIndex - infoFile.folderCount;

        if (infoHost.connected != true || EnterDir(infoFile.file[infoFile.fileIndex]) == false) //* No host or can't open file *//
        {
            update = false;
        }
        else //* Host and file OK*//
        {
            // load model preview in flash if icon exists
            setPrintModelIcon(infoFile.source < BOARD_SD && model_DecodeToFlash(infoFile.title));

            char temp_info[FILE_NUM + 50];
            sprintf(temp_info, (char *)textSelect(LABEL_START_PRINT),
                    (uint8_t *)((infoMachineSettings.longFilename == ENABLED && infoFile.source == BOARD_SD) ?
                                    infoFile.Longfile[infoFile.fileIndex] :
                                    infoFile.file[infoFile.fileIndex]));
            // confirm file selction
            setDialogText(LABEL_PRINT, (uint8_t *)temp_info, LABEL_CONFIRM, LABEL_CANCEL);
            showDialog(DIALOG_TYPE_QUESTION, startGCode, ExitDir, NULL);

            update = false;
        }
    }
}
void selectedItemListMenuHandler(LISTITEM * item, uint16_t index, uint8_t itemPos){
    selectedItemHandler(index);
}
void selectedItemIconMenuHandler(KEY_VALUES selectedSetting){
    selectedItemHandler(selectedKeyToFileIndex[selectedSetting]);
}

void pageUpHandler(KEY_VALUES selectedSetting){
    if (infoFile.cur_page + 1 < pageCount)
    {
        infoFile.cur_page++;
        update = true;
    }
}

void pageDownHandler(KEY_VALUES selectedSetting){
    if (infoFile.cur_page > 0)
    {
        infoFile.cur_page--;
        update = true;
    }
}

void backGenericHandler(void){
    infoFile.cur_page = 0;

    if (IsRootDir() == true)
    {
        clearInfoFile();
        CLOSE_MENU();
    }
    else
    {
        ExitDir();
        scanPrintFiles();
        update = true;
    } 
}
void backItemIconMenuHander(KEY_VALUES selectedSetting){
    backGenericHandler();
}

ITEM pageUpItem = {ICON_PAGE_UP, LABEL_PAGE_UP, pageUpHandler};
ITEM pageDownItem = {ICON_PAGE_DOWN, LABEL_PAGE_DOWN, pageDownHandler};
ITEM pageBackItem = {ICON_BACK, LABEL_BACK, backItemIconMenuHander};

// update items in list mode
void prepareListItemHandler(LISTITEM * item, uint16_t index, uint8_t itemPos)
{
  if (index < infoFile.folderCount)
  {
    // folder
    item->icon = CHARICON_FOLDER;
    item->titlelabel.index = LABEL_DYNAMIC;
    item->itemType = LIST_LABEL;
    setDynamicLabel(itemPos, infoFile.folder[index]);
  }
  else if (index < (infoFile.fileCount + infoFile.folderCount))
  {
    // file
    item->icon = CHARICON_FILE;
    item->itemType = LIST_LABEL;
    item->titlelabel.index = LABEL_DYNAMIC;
    if (infoMachineSettings.longFilename == ENABLED && infoFile.source == BOARD_SD)
      setDynamicLabel(itemPos, infoFile.Longfile[index - infoFile.folderCount]);
    else
      setDynamicLabel(itemPos, infoFile.file[index - infoFile.folderCount]);
  }
}

void updateIconMenu(){
    // update files menu in icon mode
    // Set menu items 0-NUM_PER_PAGE based on currenty selected page
    // List folders before files
    uint8_t item_in_page_index = 0;
    uint8_t iconMenuPosition = 0;
    ITEM curItem = {ICON_BACKGROUND, LABEL_BACKGROUND}; 
    
    // draw folders
    for (item_in_page_index = 0; (item_in_page_index + infoFile.cur_page * NUM_PER_PAGE < infoFile.folderCount) && (item_in_page_index < NUM_PER_PAGE); item_in_page_index++)
    {
        iconMenuPosition = iconMenuDyanmicItems[item_in_page_index];
        curItem.icon = ICON_FOLDER;
        menuDrawItem(&curItem, iconMenuPosition);
        menuDrawIconTextStr((uint8_t*)infoFile.folder[item_in_page_index + infoFile.cur_page * NUM_PER_PAGE],iconMenuPosition);
    }

    // draw files
    for (; (item_in_page_index + infoFile.cur_page * NUM_PER_PAGE < infoFile.fileCount + infoFile.folderCount) && (item_in_page_index < NUM_PER_PAGE); item_in_page_index++)
    {        
        iconMenuPosition = iconMenuDyanmicItems[item_in_page_index];
        curItem.icon = ICON_FILE;
        menuDrawItem(&curItem, iconMenuPosition);
        menuDrawIconTextStr((uint8_t*)infoFile.file[item_in_page_index + infoFile.cur_page * NUM_PER_PAGE - infoFile.folderCount],iconMenuPosition);
    }

  // clear blank icons
  for (; (item_in_page_index < NUM_PER_PAGE); item_in_page_index++)
  {        
    iconMenuPosition = iconMenuDyanmicItems[item_in_page_index];
    curItem.icon = ICON_BACKGROUND;
    menuDrawItem(&curItem, iconMenuPosition);
  }
}
void refreshMenu(){
    // refresh file menu
    if (update)
    {
        update = false;

        if (infoSettings.files_list_mode == true)
        {
            listViewCreateWithHandlers((LABEL){.address = (uint8_t *)infoFile.title}, 
                            NULL, 
                            infoFile.folderCount + infoFile.fileCount,
                            &infoFile.cur_page,
                            backGenericHandler,
                            selectedItemListMenuHandler, 
                            NULL,
                            prepareListItemHandler);
        }
        else
        {
            loadGCodeItems.title.address = (uint8_t *)infoFile.title;
            updateIconMenu();
        }

        Scroll_CreatePara(&scrollLine, (uint8_t *)infoFile.title, &myTitleRect);
        GUI_SetBkColor(infoSettings.title_bg_color);
        GUI_ClearRect(0, 0, LCD_WIDTH, TITLE_END_Y);
        GUI_SetBkColor(infoSettings.bg_color);
    }
    GUI_SetBkColor(infoSettings.title_bg_color);
    Scroll_DispString(&scrollLine, LEFT);
    GUI_SetBkColor(infoSettings.bg_color);
}
void checkSDStatus(){
    #ifdef SD_CD_PIN
      if (isVolumeExist(infoFile.source) != true)
      {
        resetInfoFile();
        CLOSE_MENU();
      }
    #endif
}
void loadFiles(void){
    GUI_Clear(infoSettings.bg_color);
    GUI_DispStringInRect(0, 0, LCD_WIDTH, LCD_HEIGHT, LABEL_LOADING);
    if (mountFS() != true || scanPrintFiles() != true)
    {
        if (infoFile.source == BOARD_SD)  // error when the filesystem selected from TFT not available
        {
            GUI_DispStringInRect(0, 0, LCD_WIDTH, LCD_HEIGHT, (uint8_t*)requestCommandInfo.cmd_rev_buf);
        }    
        else{            
            GUI_DispStringInRect(0, 0, LCD_WIDTH, LCD_HEIGHT, labelVolumeError[infoFile.source]);
        }
        Delay_ms(1000);
        CLOSE_MENU();
    }

}
static void initMenu(void){
    if(!infoSettings.files_list_mode){
        initDefaultMenu(&loadGCodeItems, LABEL_BACKGROUND, true);

        loadGCodeItems.items[KEY_ICON_0] = pageBackItem;
        loadGCodeItems.items[KEY_ICON_3] = pageUpItem;

        loadGCodeItems.items[KEY_ICON_6] = pageDownItem;

        menuDrawPage(&loadGCodeItems);
    }else{
        // List menu generated dynamically on refreshMenu();
    }
}
void menuLoadGCodeFromSource(void)
{
    initMenu();

    update = true;
    loadFiles();

    while (MENU_IS(menuLoadGCodeFromSource))
    {
        refreshMenu();
        if(infoSettings.files_list_mode){     
            listViewGetSelectedIndex();
        } else{
            menuRunItemDefaultHandler();
        }
        loopProcess();
    }
}
void loadGCodeFromSourceHandler(KEY_VALUES selectedSetting){
    OPEN_MENU(menuLoadGCodeFromSource);
}
ITEM loadGCodeFromSourceItem = {ICON_LOAD,LABEL_LOAD,loadGCodeFromSourceHandler};