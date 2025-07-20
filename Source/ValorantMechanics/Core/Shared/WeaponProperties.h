// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#pragma once

#include "CoreMinimal.h"

#include "WeaponProperties.generated.h"



UENUM(BlueprintType)
enum class EEquipType : uint8
{
    EquipDefault,
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
    Empty           UMETA(DisplayName = "Empty Weapon - Placeholder for any weapon type. Can be used as a \"test animation data asset\" in ABP."),
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


// definition for alternate weapon actions 
USTRUCT(BlueprintType)
struct VALORANTMECHANICS_API FAltWeaponProperties
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire Properties|Alternate Fire")
    bool canAltFire = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire Properties|Alternate Fire", meta = (EditCondition = "canAltFire", EditConditionHides))
    float altFireRate = 0.0f; // bullets/s
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire Properties|Alternate Fire", meta = (EditCondition = "canAltFire", EditConditionHides, DisplayName = "First Shot Spread when Alt Fire"))
    float altFirstShotSpread = 0.0f; // degrees
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire Properties|Alternate Fire", meta = (EditCondition = "canAltFire", EditConditionHides, DisplayName = "Can ADS"))
    bool canADS = false;

    // TODO: add time to ADS/scopein
    
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
    GENERATED_BODY()

    // default FOV for the weapon, unused property
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Range")
    float fov = 90.0f; // degrees
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Range")
    float minRange = 0.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Range")
    float maxRange = 0.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire Properties", meta = (Unit = "cm/s"))
    float runSpeed = 0.0f; // cm/s
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire Properties", meta = (ToolTip = "Time to equip the weapon normally. Time in seconds"))
    float equipTimeDefault = 0.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire Properties", meta = (ToolTip = "Time to equip the weapon quickly. Time in seconds"))
    float equipTimeFast = 0.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire Properties")
    bool hasCollisionBox = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire Properties")
    bool hasMagazine = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire Properties", meta = (EditCondition = "hasMagazine", EditConditionHides, Unit = "ms"))
    float reloadTime = 0.0f; // ms
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire Properties", meta = (EditCondition = "hasMagazine", EditConditionHides, Description = "Number of rounds per mag", ClampMin = 0, ClampMax = 128))
    int32 magazineCapacity = 0;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire Properties", meta = (EditCondition = "hasMagazine", EditConditionHides, Description = "Number of mags", ClampMin = 0, ClampMax = 128))
    int32 magazineCount = 0;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire Properties")
    float fireRate = 0.0f; // bullets/s
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire Properties")
    float firstShotSpread_HipFire = 0.0f; // degrees

};

