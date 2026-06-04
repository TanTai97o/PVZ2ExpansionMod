#pragma once
#include "ObjectTypeDescriptor.h"
#include "Plant.h"

#include <Sexy/RtWeakPtr.h>

// Forward declare
class PlantDoomShroom;

class PlantType : public ObjectTypeDescriptor
{
public:
    enum PlantMasteryFocusType {
        health,
        piercing
    };

    SexyString PlantFramework;
    SexyString AnimRigClass;
    SexyString PopAnim;
    int unk1;
    Sexy::RtWeakPtr<PlantPropertySheet> Properties;
    bool Enabled = true;
    bool Premium;
    std::vector<SexyString> Families;
    PlantMasteryFocusType MasteryType = piercing;
    SexyString HomeWorld;
    SexyString AlmanacBackdropName;
    float SeedChooserVerticalOffset;
    float SeedChooserBigVerticalOffset;
    float SeedChooserBigHorizontalOffset;
    float SeedChooserBigScale;
    int unk2;
    int BundleVerticalOffset;
    bool HideInPlantViewers;
    bool DenyPlantfoodCooldownReset;
    bool CannotBeImitated;
    float SeasonsVerticalOffset = 1.4f;
    std::vector<SexyString> PlantResourceGroups;
    std::vector<SexyString> PlantPreviewResourceGroups;

    // extended properties
    int IntegerID = 0;
    bool HasShadow = true;
    bool HasStoreShadow = true;
    float ShadowScale = 1.0f;
    SexyVector2 ShadowOffset;

    static Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc oPlantTypeBuildSymbols;
    static Reflection::CRefManualSymbolBuilder::ConstructFunc    oPlantTypeConstruct;

    // --------------------------------------------------------
    //  construct — hook duy nhat tai 0x8D3150
    //
    //  Gop chung:
    //    1. Goi original constructor (oPlantTypeConstruct)
    //    2. Dang ky typename vao g_modPlantTypenames
    //       (truoc day la hkPlantTypeCtor rieng → double-hook)
    //    3. Init extended fields
    //    4. Per-plant overrides (DoomShroom PopAnim, v.v.)
    //
    //  Dinh nghia nam trong PVZ2ExpansionMod.cpp.
    // --------------------------------------------------------
    static void* construct(PlantType* self);

    // --------------------------------------------------------
    //  buildSymbols — hook tai 0x8D1FE8
    //  Dinh nghia nam trong PVZ2ExpansionMod.cpp.
    // --------------------------------------------------------
    static void buildSymbols(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rclass);
};

static_assert(sizeof(PlantType) == 0xB8);
static_assert(offsetof(PlantType, PlantFramework) == 0x10);
static_assert(offsetof(PlantType, HomeWorld) == 0x54);
static_assert(offsetof(PlantType, AlmanacBackdropName) == 0x60);
static_assert(offsetof(PlantType, HideInPlantViewers) == 0x84);
static_assert(offsetof(PlantType, PlantResourceGroups) == 0x8C);
static_assert(offsetof(PlantType, IntegerID) == 0xA4);
static_assert(offsetof(PlantType, HasShadow) == 0xA8);
static_assert(offsetof(PlantType, HasStoreShadow) == 0xA9);
static_assert(offsetof(PlantType, ShadowScale) == 0xAC);
static_assert(offsetof(PlantType, ShadowOffset) == 0xB0);

// Khai bao extern sau class PlantType de compiler biet kieu
extern std::vector<PlantType*> g_modPlantTypenames;
