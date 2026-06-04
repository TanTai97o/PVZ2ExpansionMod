#pragma once
#include "PlantFramework.h"
#include "../Reflection/ReflectionBuilder.h"
#include "../Logging.h"

// ============================================================
//  PlantDoomShroom — PlantFramework riêng cho DoomShroom
//
//  Mục đích duy nhất: đăng ký RtClass tên "PlantDoomShroom"
//  vào engine để JSON PlantType tìm được.
//
//  Toàn bộ logic game được xử lý bởi engine:
//    - Animation flow (plant → plant_idle → attack) : PlantAnimRig
//    - Gây sát thương : fscommand("use_action","") trong attack.xml
//    - Xóa cây sau khi attack xong : PlantFramework::Die() base
//
//  Không cần override bất kỳ virtual nào.
// ============================================================

class PlantDoomShroom : public PlantFramework
{
public:
    DECLARE_STATIC_RT_CLASS_MEMBERS(PlantDoomShroom)

    DEFINE_RTOBJECT_OVERRIDE_GETTYPE(PlantDoomShroom)

    DEFINE_RT_CLASS_CONSTRUCT_FUNCTION(PlantDoomShroom, PLANTFRAMEWORK_CTOR)
    DEFINE_RT_CLASS_GET_CLASS_FUNCTION(PlantDoomShroom, PLANTFRAMEWORK_GETTYPE)
    DEFINE_RT_CLASS_REGISTER_CLASS_FUNCTION(PlantDoomShroom)
    DEFINE_RT_CLASS_BUILD_SYMBOLS_WITHOUT_CLASS_PROPERTIES(PlantDoomShroom, PlantFramework)

};
