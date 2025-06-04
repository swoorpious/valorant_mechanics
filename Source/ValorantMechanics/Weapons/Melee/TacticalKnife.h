// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ValorantMechanics/Weapons/CommonWeapon.h"
#include "ValorantMechanics/Weapons/WeaponDataAssets/MeleeAnimDataAsset.h"



#include "TacticalKnife.generated.h"



class UAnimMontage;


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
