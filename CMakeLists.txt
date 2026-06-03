cmake_minimum_required(VERSION 3.10)
project(PVZ2ExpansionMod)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DSubstrateInternal)

# Force include pch.h trước tất cả các file (giống precompiled header của VS)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include ${CMAKE_SOURCE_DIR}/pch.h")

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/PvZ2
    ${CMAKE_SOURCE_DIR}/Reflection
    ${CMAKE_SOURCE_DIR}/Sexy
    ${CMAKE_SOURCE_DIR}/Substrate
    ${CMAKE_SOURCE_DIR}/And64Inline
)

add_library(PVZ2ExpansionMod SHARED
    PVZ2ExpansionMod.cpp
    memUtils.cpp
    SexyUtils.cpp
    Sexy/DelegateBase.cpp
    Sexy/RtClass.cpp
    PvZ2/Plant_PowerLily.cpp
    PvZ2/StageModule.cpp
    PvZ2/Zombie_Camel.cpp
    PvZ2/Zombie_EightiesArcade.cpp
    PvZ2/ZombieType.cpp
    Reflection/ReflectionBuilder.cpp
    Substrate/SubstrateHook.cpp
    Substrate/SubstratePosixMemory.cpp
    And64Inline/And64InlineHook.cpp
)

target_link_libraries(PVZ2ExpansionMod
    android
    log
)
