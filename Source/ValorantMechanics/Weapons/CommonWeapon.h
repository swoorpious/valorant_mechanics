// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SharedWeapon.h"

#include "CommonWeapon.generated.h"



class USkeletalMeshComponent;



UCLASS()
class VALORANTMECHANICS_API ACommonWeapon : public AActor
{
    GENERATED_BODY()

public:
    ACommonWeapon();

    virtual void Fire();
    virtual void Reload();
    virtual void Equip(EEquipType EquipType);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Body")
    TObjectPtr<USkeletalMeshComponent> _WeaponSkeletalMesh;


#pragma region Weapon Fire Properties
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Range")
    float _MinRange = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Range")
    float _MaxRange = 0.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Usage Properties",meta = (Unit = "cm/s"))
    float _RunSpeed = 0.0f; // cm/s
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Usage Properties", meta = (Unit = "ms"))
    float _ReloadTime = 0.0f; // ms
        
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Usage Properties")
    bool _CanReload = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Usage Properties", meta = (EditCondition = "_CanReload", Description = "Number of rounds per mag", ClampMin = 0, ClampMax = 128))
    int32 _MagazineSize = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Usage Properties")
    float _EquipTime = 0.0f;
        
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Usage Properties")
    float _FireRate = 0.0f; // bullets/s
        
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Usage Properties")
    float _FirstShotSpread_HipFire = 0.0f; // degrees
        
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Usage Properties|Alternate Fire")
    bool _CanAltFire = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Usage Properties|Alternate Fire", meta = (EditCondition = "_CanAltFire"))
    float _FireRate_Alt = 0.0f; // bullets/s

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Usage Properties|Alternate Fire", meta = (EditCondition = "_CanAltFire"))
    bool _IsAltFireMode = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Usage Properties|Alternate Fire", meta = (EditCondition = "_IsAltFireMode"))
    float _RunSpeed_Alt = 0.0f; // cm/s
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Usage Properties|Alternate Fire", meta = (EditCondition = "_CanAltFire"))
    float _FirstShotSpread_Alt = 0.0f; // degrees
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Usage Properties|Alternate Fire", meta = (EditCondition = "_CanAltFire"))
    bool _CanZoom = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Usage Properties|Alternate Fire", meta = (EditCondition = "_CanZoom"))
    float _Zoom_Alt = 0.0f;

  
#pragma endregion


protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    EWeaponType WeaponType;
    EWeaponState CurrentWeaponState;
    EWeaponPickupType WeaponPickupType;
};
