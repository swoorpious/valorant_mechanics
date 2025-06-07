// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "ValorantMechanics/Weapons/CommonWeapon.h"
#include "PrimaryWeapon.generated.h"


class UWeaponAnimDataAsset;


UCLASS()
class VALORANTMECHANICS_API APrimaryWeapon : public ACommonWeapon
{
    GENERATED_BODY()

public:
    APrimaryWeapon();
    virtual void Tick(float DeltaTime) override;




    
protected:
    virtual void BeginPlay() override;

};
