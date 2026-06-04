#include "Plant_DoomShroom.h"
#include "../Logging.h"

DEFINE_STATIC_RT_CLASS_MEMBERS(PlantDoomShroom)

void PlantDoomShroom::modInit()
{
    LOGI("[DoomShroom] modInit — registering RtClass");

    Sexy::RtClass* rtClass = PlantDoomShroom::getRtClass();
    if (rtClass)
        LOGI("[DoomShroom] RtClass OK: %s", rtClass->TypeName);
    else
        LOGI("[DoomShroom] ERROR: RtClass registration failed!");
}
