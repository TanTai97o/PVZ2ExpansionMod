#include "PlantAnimRig_DoomShroom.h"
#include "../Logging.h"

DEFINE_STATIC_RT_CLASS_MEMBERS(PlantAnimRig_DoomShroom)

// --------------------------------------------------------
//  Helper: bind unkDelegateOnStop vào callback
//  Dùng địa chỉ 0x276FF8 (từ PlantAnimRig destructor comment)
//  là hàm set DelegateBase — tương tự cách PlantAnimRig gốc dùng
// --------------------------------------------------------
static void bindOnStop(PopAnimRig* rig, void* obj, void* func)
{
    typedef void (*SetDelegateFunc)(Sexy::DelegateBase*, void*, void*);
    SetDelegateFunc setDelegate = (SetDelegateFunc)getActualOffset(0x276FF8);
    setDelegate(&rig->unkDelegateOnStop, obj, func);
}

// --------------------------------------------------------
//  Function32 — PlayBorn
//  Gọi khi engine đặt cây xuống lần đầu
//  Chạy label "plant" 1 lần, khi xong gọi onPlantDone
// --------------------------------------------------------
void PlantAnimRig_DoomShroom::Function32()
{
    LOGI("[DoomShroom] Function32 — playing 'plant'");
    SexyString label = "plant";
    SetActiveAnim(&label, 0, 1, 0);  // 1 = ONE_SHOT
    bindOnStop(this, this, (void*)&PlantAnimRig_DoomShroom::onPlantDone);
}

// --------------------------------------------------------
//  onPlantDone — callback khi "plant" kết thúc
//  → chuyển sang Function33 để chạy "plant_idle"
// --------------------------------------------------------
void PlantAnimRig_DoomShroom::onPlantDone(PlantAnimRig_DoomShroom* self)
{
    LOGI("[DoomShroom] 'plant' done → playing 'plant_idle'");
    self->Function33();
}

// --------------------------------------------------------
//  Function33 — PlayIdle
//  Chạy label "plant_idle" 1 lần, khi xong gọi onPlantIdleDone
// --------------------------------------------------------
void PlantAnimRig_DoomShroom::Function33()
{
    LOGI("[DoomShroom] Function33 — playing 'plant_idle'");
    SexyString label = "plant_idle";
    SetActiveAnim(&label, 0, 1, 0);  // 1 = ONE_SHOT
    bindOnStop(this, this, (void*)&PlantAnimRig_DoomShroom::onPlantIdleDone);
}

// --------------------------------------------------------
//  onPlantIdleDone — callback khi "plant_idle" kết thúc
//  → chuyển sang attack
// --------------------------------------------------------
void PlantAnimRig_DoomShroom::onPlantIdleDone(PlantAnimRig_DoomShroom* self)
{
    LOGI("[DoomShroom] 'plant_idle' done → playing 'attack'");
    self->PlayAttackBasedOnTier();
}

// --------------------------------------------------------
//  PlayAttackBasedOnTier — chạy "attack" 1 lần (ONE_SHOT)
//  fscommand("use_action","") trong animation sẽ trigger damage
//  Khi attack xong engine tự gọi Die() qua PlantFramework
// --------------------------------------------------------
void PlantAnimRig_DoomShroom::PlayAttackBasedOnTier()
{
    LOGI("[DoomShroom] PlayAttackBasedOnTier — playing 'attack' ONE_SHOT");
    SexyString label = "attack";
    SetActiveAnim(&label, 0, 1, 0);  // 1 = ONE_SHOT, không loop
}

// --------------------------------------------------------
//  modInit — đăng ký RtClass
// --------------------------------------------------------
void PlantAnimRig_DoomShroom::modInit()
{
    LOGI("[DoomShroom] PlantAnimRig_DoomShroom::modInit");
    Sexy::RtClass* rtClass = PlantAnimRig_DoomShroom::getRtClass();
    if (rtClass)
        LOGI("[DoomShroom] AnimRig RtClass OK: %s", rtClass->TypeName);
    else
        LOGI("[DoomShroom] ERROR: AnimRig RtClass failed!");
}
