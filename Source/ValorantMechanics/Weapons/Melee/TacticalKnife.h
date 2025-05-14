// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ValorantMechanics/Weapons/CommonWeapon.h"
#include "TacticalKnife.generated.h"

UCLASS()
class VALORANTMECHANICS_API ATacticalKnife : public ACommonWeapon
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ATacticalKnife();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
