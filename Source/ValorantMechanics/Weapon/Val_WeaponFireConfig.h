// © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Val_WeaponFireConfig.generated.h"

UENUM(BlueprintType)
enum class EFireMode : uint8
{
    Manual,
    Semi_Automatic,
    Automatic,
};

/**
 * one data asset instance for each gun
 * 
 * all guns can share the same editor instance in blueprints
 * since each gun will use the same config across skins 
 */
UCLASS()
class VALORANTMECHANICS_API UVal_WeaponFireConfig : public UDataAsset
{
    GENERATED_BODY()
    
public:
    
#pragma region REGULAR_FIRE_PROPERTIES
    // default FOV for the weapon, unused property
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Range")
    float fov = 90.0f; // degrees
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Range")
    float minRange = 0.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Range")
    float maxRange = 0.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire Properties", meta = (Unit = "cm/s"))
    EFireMode fireMode = EFireMode::Manual;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire Properties", meta = (Unit = "cm/s"))
    bool canSwitchFireMode = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire Properties", meta = (Unit = "cm/s"))
    float runSpeed = 0.0f; // cm/s
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire Properties", meta = (ToolTip = "Time to equip the weapon normally. Time in seconds"))
    float equipTimeDefault = 0.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire Properties", meta = (ToolTip = "Time to equip the weapon quickly. Time in seconds"))
    float equipTimeFast = 0.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire Properties")
    bool hasMag = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire Properties", meta = (EditCondition = "hasMag", EditConditionHides, Description = "Number of rounds per mag", ClampMin = 0, ClampMax = 128))
    int32 magSize = 0;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire Properties", meta = (EditCondition = "hasMag", EditConditionHides, Description = "Number of mags", ClampMin = 0, ClampMax = 128))
    int32 magCount = 0;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire Properties", meta = (EditCondition = "hasMag", EditConditionHides, Unit = "ms"))
    float reloadTime = 0.0f; // ms
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire Properties", meta=(DisplayName="Fire Rate (bullets/s)"))
    float fireRate = 0.0f; // bullets/s
    
    // TODO: add vectors that define bullet spread
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire Properties")
    float firstShotSpread_HipFire = 0.0f; // degrees
    
#pragma endregion REGULAR_FIRE_PROPERTIES
    
#pragma region ALTERNATE_FIRE_PROPERTIES
    /*
     * alternate fire and ADS/scope-in are treated differently
     * for example, a knife may alternative fire but cannot ADS/scope-in, 
     * whereas a rifle can alternate fire and ADS
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alternate Fire Properties")
    bool canAltFire = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alternate Fire Properties", meta = (EditCondition = "canAltFire", EditConditionHides))
    float alrFireRate = 0.0f; // bullets/s
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alternate Fire Properties", meta = (EditCondition = "canAltFire", EditConditionHides, DisplayName = "First Shot Spread when Alt Fire"))
    float altFirstShotSpread = 0.0f; // degrees
    
    /*
     * ADS can be both ADS and scope-in
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alternate Fire Properties", meta = (EditCondition = "canAltFire", EditConditionHides, DisplayName = "Can ADS"))
    bool canADS = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alternate Fire Properties", meta = (EditCondition = "canADS", EditConditionHides, DisplayName = "Time to ADS (ms)"))
    float timeToADS = 0.0f; // ms
    
    // zoom FOV for the weapon when ADS. this is used for FOV scaling 
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alternate Fire Properties", meta = (EditCondition = "canADS", EditConditionHides, DisplayName = "FOV when ADS (degrees)"))
    float fovADS = 90.0f; // degrees
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alternate Fire Properties", meta = (EditCondition = "canADS", EditConditionHides, DisplayName = "Run Speed when ADS (cm/s)"))
    float runSpeedADS = 0.0f; // cm/s

#pragma endregion ALTERNATE_FIRE_PROPERTIES
};
