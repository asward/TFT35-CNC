
#include "includes.h"
// On select of laser item
void emerStopHandler(KEY_VALUES selectedKey){
    Serial_Puts(SERIAL_PORT, "M112\n");
}
ITEM emerStopItem = {ICON_STOP, LABEL_EMERGENCYSTOP,emerStopHandler};
