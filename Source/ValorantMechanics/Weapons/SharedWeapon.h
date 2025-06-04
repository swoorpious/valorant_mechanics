
#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum class EEquipType : uint8
{
    EquipFull,
    EquipFast
};

UENUM(BlueprintType)
enum class EWeaponPickupType : uint8
{
    NonPickupable,  // non-droppable
    Pickupable, // droppable
};

UENUM(BlueprintType, DisplayName = "Weapon Type")
enum class EWeaponType : uint8
{
    Melee           UMETA(DisplayName = "Melee Weapon"), // tactical knife
    Secondary       UMETA(DisplayName = "Secondary Weapon"), // handguns 
    Primary         UMETA(DisplayName = "Primary Weapon"), // rifles
};

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
    None,
    Equipping,
    Idle, // equipped -> idle
    Reloading,
    Firing, // state for single tick
    FireCooldown // firing -> fire cooldown -> idle/firing
};