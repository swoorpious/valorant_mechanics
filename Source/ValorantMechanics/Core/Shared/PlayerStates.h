// © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStates.generated.h"

/*
 * every state related to player's basic kinematic movement
 *
 * EMovementState is meant to be used by the Val_CharacterMovementComponent
 * UsingAbilityMovement indicates that the player character is now using movement altered by an ability
 */
UENUM(BlueprintType)
enum class EMovementState : uint8
{
    None, 
    Idle,
    Walk,
    Run,
    Jump_Up,
	Jump_Fall,

    // stackable states
    Crouched,
    LightStunned,
    MediumStunned,
    HeavyStunned,

    UsingAbilityMovement,

    /*
     * TODO: implement a jumping system to keep track of players jump height
     * allow queuing bunnyhop jumps certain distance above the ground before landing
     *
     * jump system shall also keep track of players jumping z-coordinate and landing z-coordinate to determine light/medium/heavy stuns
     */
    
    // crouching is detected by bIsCrouched, it will reduce the current MaxWalkSpeed by 33%
};





/*
 * redundant
 */
UENUM(BlueprintType)
enum class EWeaponAnimState : uint8
{
    None, // when the weapon is dropped or not picked up yet
    Equip_Default,
    Equip_Fast,
    Idle, // equipped -> idle
    Reloading,
    Firing,
    Inspecting,

    // stackable states
    Blocked,
    ScopedIn, // scope/ADS
};



// const TMap<EWeaponLogicState, EWeaponAnimState> weaponLogicToAnimStatesMap = {
//     
//     { EWeaponLogicState::None, EWeaponAnimState::None },
//     { EWeaponLogicState::Equip_Default, EWeaponAnimState::Equip_Default },
//     { EWeaponLogicState::Equip_Fast, EWeaponAnimState::Equip_Fast },
//     { EWeaponLogicState::Idle, EWeaponAnimState::Idle },
//     { EWeaponLogicState::Reloading, EWeaponAnimState::Reloading },
//     { EWeaponLogicState::FireShot, EWeaponAnimState::Firing },
//     { EWeaponLogicState::FireCooldown, EWeaponAnimState::Firing },
//     { EWeaponLogicState::Inspecting, EWeaponAnimState::Inspecting },
//
//     // stacked states
//     { EWeaponLogicState::Blocked, EWeaponAnimState::Blocked },
//     { EWeaponLogicState::ScopedIn, EWeaponAnimState::ScopedIn },
// };

