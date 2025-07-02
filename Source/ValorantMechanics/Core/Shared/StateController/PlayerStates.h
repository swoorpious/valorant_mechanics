// © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStates.generated.h"

/*
 * every state related to player's basic kinematic movement
 * 
 * UsingAbilityMovement indicates that the player character is now using movement altered by an ability
 */
UENUM()
enum class EMovementState : uint8
{
    None,
    Idle,
    Crouching,
    Walking,
    Running,
    Jumping,
    LightStunned,
    HeavyStunned,
    UsingAbilityMovement,
};


/*
 * these states are used by both the player and the weapon in AVal_Character and ACommonWeapon
 *
 * AVal_Character uses these states to apply corresponding character animations according to the state
 * ACommonWeapon uses these states to apply corresponding weapon animations/effects according to the state
 */
UENUM(BlueprintType)
enum class EWeaponState : uint8
{
    None, // when the weapon is dropped or not picked up yet
    Equip_Default,
    Equip_Fast,
    Blocked,
    Idle, // equipped -> idle
    Reloading,
    Fired, // state for single tick
    AltFired,
    FireCooldown, // firing -> fire cooldown -> idle/firing
    Inspecting,
    ScopedIn, // scope/ADS
};

UENUM(BlueprintType)
enum class EMagazineState : uint8
{
    Empty,
    HasLowAmmo,
    HasAmmo,
};