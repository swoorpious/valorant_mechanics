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
UENUM()
enum class EMovementState : uint8
{
    None, 
    Idle,
    Walking,
    Running,
    Jumping,

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
 * these states are used by both the player and the weapon in AVal_Character and ACommonWeapon
 *
 * AVal_Character uses a pseudo implementation of these states to apply corresponding character animations according to the state
 * ACommonWeapon uses these states to apply corresponding weapon animations/effects according to the state
 */
UENUM(BlueprintType)
enum class EWeaponLogicState : uint8
{
    None, // when the weapon is dropped or not picked up yet
    Equip_Default,
    Equip_Fast,
    Idle, // equipped -> idle
    Reloading,
    Fired, // state for single tick
    FireCooldown, // firing -> fire cooldown -> idle/firing
    Inspecting,

        
    // stackable states
    Blocked,
    ScopedIn, // scope/ADS
};

UENUM(BlueprintType)
enum class EWeaponAnimState : uint8
{
    None, // when the weapon is dropped or not picked up yet
    Equip_Default,
    Equip_Fast,
    Blocked,
    Idle, // equipped -> idle
    Reloading,
    Firing,
    Inspecting,

    // stackable states
    ScopedIn, // scope/ADS
};


UENUM(BlueprintType)
enum class EMagazineState : uint8
{
    Empty,
    HasLowAmmo,
    HasAmmo,
};