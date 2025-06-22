// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"

#include "WeaponProperties.generated.h"


#define MAGAZINE_PRIMARY_SOCKET_NAME TEXT("")
#define MAGAZINE_SECONDARY_SOCKET_NAME TEXT("")
#define SCOPE_PRIMARY_SOCKET_NAME TEXT("")



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
    Empty           UMETA(DisplayName = "Empty Weapon - Placeholder for any weapon type"),
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


// definition for alternate weapon actions 
USTRUCT(BlueprintType)
struct VALORANTMECHANICS_API FAltWeaponProperties
{
    GENERATED_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire Properties|Alternate Fire")
    bool canAltFire = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire Properties|Alternate Fire", meta = (EditCondition = "canAltFire", EditConditionHides))
    float altFireRate = 0.0f; // bullets/s
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire Properties|Alternate Fire", meta = (EditCondition = "canAltFire", EditConditionHides, DisplayName = "First Shot Spread when Alt Fire"))
    float altFirstShotSpread = 0.0f; // degrees
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire Properties|Alternate Fire", meta = (EditCondition = "canAltFire", EditConditionHides, DisplayName = "Can ADS"))
    bool canADS = false;
    
    // zoom FOV for the weapon when ADS. this is used for FOV scaling 
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire Properties|Alternate Fire", meta = (EditCondition = "canADS", EditConditionHides, DisplayName = "FOV when ADS"))
    float fovADS = 90.0f; // degrees
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire Properties|Alternate Fire", meta = (EditCondition = "canADS", EditConditionHides, DisplayName = "Run Speed when ADS"))
    float runSpeedADS = 0.0f; // cm/s

};


// definition for default/basic weapon actions 
USTRUCT(BlueprintType)
struct VALORANTMECHANICS_API FDefaultWeaponProperties
{
    GENERATED_BODY();

    // default FOV for the weapon, unused property
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Range")
    float fov = 90.0f; // degrees
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Range")
    float minRange = 0.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Range")
    float maxRange = 0.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire Properties",meta = (Unit = "cm/s"))
    float runSpeed = 0.0f; // cm/s
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire Properties")
    float equipTime = 0.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire Properties")
    bool hasCollisionBox = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire Properties")
    bool hasMagazine = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire Properties", meta = (EditCondition = "hasMagazine", EditConditionHides, Unit = "ms"))
    float reloadTime = 0.0f; // ms
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire Properties", meta = (EditCondition = "hasMagazine", EditConditionHides, Description = "Number of rounds per mag", ClampMin = 0, ClampMax = 128))
    int32 magazineSize = 0;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire Properties")
    float fireRate = 0.0f; // bullets/s
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire Properties")
    float firstShotSpread_HipFire = 0.0f; // degrees

};

