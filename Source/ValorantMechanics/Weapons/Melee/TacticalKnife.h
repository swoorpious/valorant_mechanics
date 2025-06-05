// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "ValorantMechanics/Weapons/CommonWeapon.h"
#include "ValorantMechanics/Weapons/WeaponDataAssets/MeleeAnimDataAsset.h"



#include "TacticalKnife.generated.h"



UCLASS()
class VALORANTMECHANICS_API ATacticalKnife : public ACommonWeapon
{
    GENERATED_BODY()

public:
    ATacticalKnife();
    virtual void Tick(float DeltaTime) override;

    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
    TObjectPtr<UMeleeAnimDataAsset> AnimationData;


protected:
    virtual void BeginPlay() override;

    
    
};
