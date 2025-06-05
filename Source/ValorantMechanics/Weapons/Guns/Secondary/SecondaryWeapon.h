// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "ValorantMechanics/Weapons/CommonWeapon.h"
#include "SecondaryWeapon.generated.h"


class UWeaponAnimDataAsset;

UCLASS()
class VALORANTMECHANICS_API ASecondaryWeapon : public ACommonWeapon
{
    GENERATED_BODY()

public:
    ASecondaryWeapon();
    virtual void Tick(float DeltaTime) override;


    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Body")
    TObjectPtr<USkeletalMeshComponent> _MagazineSkeletalMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
    TObjectPtr<UWeaponAnimDataAsset> AnimationData;
    
protected:
    virtual void BeginPlay() override;


};
