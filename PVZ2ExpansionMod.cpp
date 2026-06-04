#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <unistd.h>

#include "PVZ2ExpansionMod.h"

#include <Sexy/SexyApp.h>

#include "PvZ2/Board.h"
#include "PvZ2/PlantType.h"
#include "PvZ2/WorldMap.h"
#include "PvZ2/ZombieType.h"

#include "PvZ2/Plant_PowerLily.h"
#include "PvZ2/Zombie_EightiesArcade.h"
#include "PvZ2/Zombie_Camel.h"
#include "PvZ2/Plant_DoomShroom.h"


#pragma region Alias to ID

class ZombieAlmanac
{
public:
    void* vftable;
    std::map<SexyString, int> m_aliasToId;
};

class PlantNameMapper
{
public:
    void* vftable;
    std::map<SexyString, int> m_aliasToId;
};

std::vector<PlantType*> g_modPlantTypenames;
std::vector<ZombieType*> g_modZombieTypenames;

typedef PlantNameMapper* (*PlantNameMapperCtor)(PlantNameMapper*);
PlantNameMapperCtor oPlantNameMapperCtor = nullptr;

void* hkCreatePlantNameMapper(PlantNameMapper* self)
{
    oPlantNameMapperCtor(self);
    self->m_aliasToId.clear();

    for (size_t iter = 0; iter < g_modPlantTypenames.size(); iter++)
    {
        PlantType* type = g_modPlantTypenames[iter];
        self->m_aliasToId[type->TypeName] = type->IntegerID;
    }

    return self;
}

typedef ZombieAlmanac* (*ZombieAlmanacCtor)(ZombieAlmanac*);
ZombieAlmanacCtor oZombieAlmanacCtor = nullptr;

void* hkCreateZombieTypenameMap(ZombieAlmanac* self)
{
    oZombieAlmanacCtor(self);
    self->m_aliasToId.clear();

    for (size_t iter = 0; iter < g_modZombieTypenames.size(); iter++)
    {
        auto* type = g_modZombieTypenames[iter];
        self->m_aliasToId[type->TypeName] = type->IntegerID;
    }

    return self;
}

#pragma endregion

#pragma region Mummy Memory Fix

#ifdef A32
#define CAMEL_MINIGAME_MODULE_FUNC 0x78CFA0
#else
#define CAMEL_MINIGAME_MODULE_FUNC 0xB1BE04
#endif

typedef void(*camelMinigameModuleFunc)(int, int, int);
camelMinigameModuleFunc cmmFunc = (camelMinigameModuleFunc)getActualOffset(CAMEL_MINIGAME_MODULE_FUNC);

void hkCamelZombieFunc(int a1, int a2, int a3)
{
    cmmFunc(a1, a2, a3);
}

#pragma endregion

#pragma region Board Zoom + GetBoard

typedef int64_t (*mGetBoard)();
mGetBoard oGetBoard = nullptr;

Board* hkGetBoard() {
    return (Board*)oGetBoard();
}

Board* getBoard() {
    return hkGetBoard();
}

typedef void(*boardTest)(Board*, int*, int, int);
boardTest oBoardTest = nullptr;

void hkBoardTest(Board* self, int* a2, int a3, int a4)
{
    LOGI("hai: %d, %d", a3, a4);
    oBoardTest(self, a2, a3, a4);
}

#pragma endregion

#pragma region Build Symbol Funcs

Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc PlantType::oPlantTypeBuildSymbols = nullptr;
Reflection::CRefManualSymbolBuilder::ConstructFunc    PlantType::oPlantTypeConstruct    = nullptr;
Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc ZombieType::oZombieTypeBuildSymbols = nullptr;
Reflection::CRefManualSymbolBuilder::ConstructFunc    ZombieType::oZombieTypeConstruct    = nullptr;

// --------------------------------------------------------
//  PlantType::construct — hook duy nhat tai 0x8D3150
//
//  Gop chung logic cua hkPlantTypeCtor (cu) vao day:
//    - Goi original
//    - Dang ky typename vao g_modPlantTypenames
//    - Init extended fields
//    - Per-plant PopAnim overrides
// --------------------------------------------------------
void* PlantType::construct(PlantType* self)
{
    oPlantTypeConstruct(self);

    // Dang ky typename (truoc day la hook rieng hkPlantTypeCtor)
    g_modPlantTypenames.push_back(self);

    // Init extended fields
    self->IntegerID      = 0;
    self->HasShadow      = true;
    self->HasStoreShadow = true;
    self->ShadowScale    = 1.0f;
    self->ShadowOffset   = SexyVector2();

    return self;
}

void PlantType::buildSymbols(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rclass)
{
    oPlantTypeBuildSymbols(builder, rclass);
    REGISTER_STANDARD_PROPERTY(builder, rclass, PlantType, IntegerID);
    REGISTER_STANDARD_PROPERTY(builder, rclass, PlantType, HasShadow);
    REGISTER_STANDARD_PROPERTY(builder, rclass, PlantType, HasStoreShadow);
    REGISTER_STANDARD_PROPERTY(builder, rclass, PlantType, ShadowScale);
    REGISTER_CLASS_PROPERTY(builder, rclass, PlantType, SexyVector2, ShadowOffset);
}

#pragma endregion

__attribute__((constructor))
void libPVZ2ExpansionMod_main()
{
    LOGI("Initializing %s", LIB_TAG);

    // --------------------------------------------------------
    //  PlantType hooks
    //  0x8D3150 chi hook 1 lan — PlantType::construct da gop
    //  ca logic dang ky typename lan per-plant overrides.
    // --------------------------------------------------------
    PVZ2HookFunction(0x8D3150, (void*)PlantType::construct,    (void**)&PlantType::oPlantTypeConstruct);
    PVZ2HookFunction(0x8D1FE8, (void*)PlantType::buildSymbols, (void**)&PlantType::oPlantTypeBuildSymbols);
    PVZ2HookFunction(0xDA5C58, (void*)hkCreatePlantNameMapper, (void**)&oPlantNameMapperCtor);

    // --------------------------------------------------------
    //  ZombieType hooks
    // --------------------------------------------------------
    PVZ2HookFunction(0xCA5768, (void*)ZombieType::construct,      (void**)&ZombieType::oZombieTypeConstruct);
    PVZ2HookFunction(0xCA5894, (void*)ZombieType::buildSymbols,   (void**)&ZombieType::oZombieTypeBuildSymbols);
    PVZ2HookFunction(0x10643E0,(void*)hkCreateZombieTypenameMap,  (void**)&oZombieAlmanacCtor);

    // --------------------------------------------------------
    //  Misc hooks
    // --------------------------------------------------------
    PVZ2HookFunction(0x789DC8, (void*)hkCamelZombieFunc, nullptr);
    PVZ2HookFunction(0x949EFC, (void*)hkGetBoard,        (void**)&oGetBoard);
    PVZ2HookFunction(0x724520, (void*)hkBoardTest,       (void**)&oBoardTest);

    // --------------------------------------------------------
    //  Per-module init
    //  PlantDoomShroom::modInit() dang ky RtClass "PlantDoomShroom"
    //  vao engine — phai goi truoc khi bat ky PlantType nao
    //  co PlantFramework = "PlantDoomShroom" duoc load.
    // --------------------------------------------------------
    PowerLilyProps::modInit();
    ZombieEightiesArcadeProps::modInit();
    PlantDoomShroom::modInit();

    //ZombieCamel::modInit();

    LOGI("Finished initializing");
}
