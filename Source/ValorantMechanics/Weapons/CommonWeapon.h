// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SharedWeapon.h"

#include "CommonWeapon.generated.h"


class USkeletalMeshComponent;
class UStaticMeshComponent;
class UBoxComponent;



USTRUCT(BlueprintType)
struct FDefaultWeaponProperties
{
    GENERATED_BODY()

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
    int32 magazineSize = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire Properties")
    float fireRate = 0.0f; // bullets/s
        
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire Properties")
    float firstShotSpread_HipFire = 0.0f; // degrees
    
};


USTRUCT(BlueprintType)
struct FAltWeaponProperties
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
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire Properties|Alternate Fire", meta = (EditCondition = "canADS", EditConditionHides, DisplayName = "FOV when ADS"))
    float fovADS = 90.0f; // degrees

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire Properties|Alternate Fire", meta = (EditCondition = "canADS", EditConditionHides, DisplayName = "Run Speed when ADS"))
    float runSpeedADS = 0.0f; // cm/s
    
};




UCLASS()
class VALORANTMECHANICS_API ACommonWeapon : public AActor
{
    GENERATED_BODY()

public:
    ACommonWeapon();

    virtual void Fire() {}
    virtual void Reload() {}
    virtual void Equip(EEquipType EquipType) {}
    virtual EWeaponType GetWeaponType() { return weaponType; }
    virtual EWeaponState GetWeaponState() { return weaponState; }
    virtual EWeaponPickupType GetWeaponPickupType() { return weaponPickupType; }
    virtual TObjectPtr<UDataAsset> GetAnimAsset() { return animAsset; }
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Body")
    TObjectPtr<USkeletalMeshComponent> weaponMesh;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Body", meta=(EditCondition = "defaultProperties.hasMagazine", EditConditionHides))
    TObjectPtr<UStaticMeshComponent> magazineMesh;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Body", meta=(EditCondition = "altProperties.canADS", EditConditionHides))
    TObjectPtr<UStaticMeshComponent> scopeMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Body", meta=(EditCondition = "defaultProperties.hasCollisionBox", EditConditionHides))
    TObjectPtr<UBoxComponent> collisionBox;


    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Socket Names") FName magazineMainSocketName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Socket Names") FName scopeMainSocketName;
    

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon") FDefaultWeaponProperties defaultProperties;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon") FAltWeaponProperties altProperties;



protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    EWeaponType weaponType; // variables initialized in derived class constructors 
    EWeaponState weaponState;
    EWeaponPickupType weaponPickupType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Animations", meta = (DisplayName = "Animation Data Asset"))
    TObjectPtr<UDataAsset> animAsset;
};
