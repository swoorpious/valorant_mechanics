// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "AnimNotifier.h"
#include "ValorantMechanics/Core/Val_LocalPlayerSubsystem.h"
#include "ValorantMechanics/Core/Shared/PlayerStates.h"


void UAnimNotifier::Init(UVal_LocalPlayerSubsystem* localPlayerSubsystem)
{
    pSubsystem = localPlayerSubsystem;
}

void UAnimNotifier::NotifyWeaponStateMachineStateChange(FName lastState, FName newState)
{
    EWeaponLogicState prevState = EWeaponLogicState::None;
    EWeaponLogicState currState = EWeaponLogicState::None;

    // switch (lastState.ToString())
    // {
    //     case "Equip_Default": prevState = EWeaponLogicState::Equip_Default; break;
    //     case "Equip_Fast": prevState = EWeaponLogicState::Equip_Fast; break;
    //     case "Blocked": prevState = EWeaponLogicState::Blocked; break;
    //     case "Idle": prevState = EWeaponLogicState::Idle; break;
    //     case "Reloading": prevState = EWeaponLogicState::Reloading; break;
    //     case "Fired": prevState = EWeaponLogicState::Fired; break;
    //     case "AltFired": prevState = EWeaponLogicState::AltFired; break;
    //     case "FireCooldown": prevState = EWeaponLogicState::FireCooldown; break;
    //     case "Inspecting": prevState = EWeaponLogicState::Inspecting; break;
    //     case "ScopedIn": prevState = EWeaponLogicState::ScopedIn; break;
    //
    //     case "None":
    //     case "Empty":
    //     default: break;
    // }
    //
    // switch (newState.ToString())
    // {
    //     case "Equip_Default": currState = EWeaponLogicState::Equip_Default; break;
    //     case "Equip_Fast": currState = EWeaponLogicState::Equip_Fast; break;
    //     case "Blocked": currState = EWeaponLogicState::Blocked; break;
    //     case "Idle": currState = EWeaponLogicState::Idle; break;
    //     case "Reloading": currState = EWeaponLogicState::Reloading; break;
    //     case "Fired": currState = EWeaponLogicState::Fired; break;
    //     case "AltFired": currState = EWeaponLogicState::AltFired; break;
    //     case "FireCooldown": currState = EWeaponLogicState::FireCooldown; break;
    //     case "Inspecting": currState = EWeaponLogicState::Inspecting; break;
    //     case "ScopedIn": currState = EWeaponLogicState::ScopedIn; break;
    //
    //     case "None":
    //     case "Empty":
    //     default: break;
    // }
    
    // pSubsystem->UpdateWeaponAnimState(prevState, currState);
}
