// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "ValorantMechanics/Weapons/CommonWeapon.h"
#include "PrimaryWeapon.generated.h"


class UBoxComponent;
class UWeaponAnimDataAsset;


UCLASS()
class VALORANTMECHANICS_API APrimaryWeapon : public ACommonWeapon
{
    GENERATED_BODY()

public:
    APrimaryWeapon();
    virtual void Tick(float DeltaTime) override;


    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Body")
    TObjectPtr<UBoxComponent> CollisionBox;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Body")
    TObjectPtr<UStaticMeshComponent> _MagazineSkeletalMesh;
    
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
    TObjectPtr<UWeaponAnimDataAsset> AnimationData;

    
protected:
    virtual void BeginPlay() override;

};
