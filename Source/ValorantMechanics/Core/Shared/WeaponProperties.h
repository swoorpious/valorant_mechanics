// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#pragma once

#include "CoreMinimal.h"

#include "WeaponProperties.generated.h"


/*
 * these states are used by both the player and the weapon in AVal_Character and ACommonWeapon
 *
 * AVal_Character uses a pseudo implementation of these states to apply corresponding character animations according to the state
 * ACommonWeapon uses these states to apply corresponding weapon animations/effects according to the state
 */
UENUM(BlueprintType)
enum class EWeaponState : uint8
{
    None, // when the weapon is dropped or not picked up yet
    Equip_Default,
    Equip_Fast,
    Idle, // equipped -> idle
    Reloading,
    Firing, // state for single tick
    Heat_Cooldown, // firing -> gun heats -> cooldown (gun cannot fire at all) -> idle/firing
    Inspecting,
    Blocked,
};

UENUM(BlueprintType)
enum class EEquipType : uint8
{
    EquipDefault,
    EquipFast
};

UENUM(BlueprintType)
enum class EWeaponPickupType : uint8
{
    NonPickupable, // non-droppable
    Pickupable, // droppable
};


UENUM(BlueprintType, DisplayName = "Weapon Type")
enum class EWeaponType : uint8
{
    Empty           UMETA(DisplayName = "Empty Weapon - Placeholder for any weapon type."),
    Melee           UMETA(DisplayName = "Melee Weapon"), // tactical knife
    Secondary       UMETA(DisplayName = "Secondary Weapon"), // handguns 
    Primary         UMETA(DisplayName = "Primary Weapon"), // rifles
};



UENUM(BlueprintType)
enum class EMagazineState : uint8
{
    Empty,
    HasLowAmmo,
    HasAmmo,
};
