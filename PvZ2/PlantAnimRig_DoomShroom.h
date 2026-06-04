#pragma once
#include "PlantAnimRig.h"
#include "../Reflection/ReflectionBuilder.h"
#include "../Logging.h"

// ============================================================
//  PlantAnimRig_DoomShroom
//
//  Flow:
//    Function32() → SetActiveAnim("plant")
//                   OnStop → Function33()
//    Function33() → SetActiveAnim("plant_idle")
//                   OnStop → PlayAttackBasedOnTier()
//    PlayAttackBasedOnTier() → SetActiveAnim("attack", ONE_SHOT)
//                              fscommand("use_action","") → damage
//                              OnStop → engine gọi Die()
// ============================================================

class PlantAnimRig_DoomShroom : public PlantAnimRig
{
public:
    DECLARE_STATIC_RT_CLASS_MEMBERS(PlantAnimRig_DoomShroom)

    DEFINE_RTOBJECT_OVERRIDE_GETTYPE(PlantAnimRig_DoomShroom)

    DEFINE_RT_CLASS_CONSTRUCT_FUNCTION(PlantAnimRig_DoomShroom, PLANTANIMRIG_CTOR)
    DEFINE_RT_CLASS_GET_CLASS_FUNCTION(PlantAnimRig_DoomShroom, PLANTANIMRIG_GETTYPE)
    DEFINE_RT_CLASS_REGISTER_CLASS_FUNCTION(PlantAnimRig_DoomShroom)
    DEFINE_RT_CLASS_BUILD_SYMBOLS_WITHOUT_CLASS_PROPERTIES(PlantAnimRig_DoomShroom, PlantAnimRig)

    // Đặt cây xuống → chạy "plant" 1 lần
    void Function32() override;

    // "plant" xong → chạy "plant_idle" 1 lần
    void Function33() override;

    // "plant_idle" xong → chạy "attack" 1 lần (ONE_SHOT)
    void PlayAttackBasedOnTier() override;

private:
    static void onPlantDone(PlantAnimRig_DoomShroom* self);
    static void onPlantIdleDone(PlantAnimRig_DoomShroom* self);
};
