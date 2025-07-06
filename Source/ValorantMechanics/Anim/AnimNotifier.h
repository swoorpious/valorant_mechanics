// Copyright 2025 Swaroop. Personal Unreal engine project inspired BY VALORANT.


#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Delegates/DelegateCombinations.h"
#include "AnimNotifier.generated.h"


class UVal_LocalPlayerSubsystem;

/**
 * 
 */
UCLASS()
class VALORANTMECHANICS_API UAnimNotifier : public UObject
{
    GENERATED_BODY()

public:

    void Init(UVal_LocalPlayerSubsystem* localPlayerSubsystem);

    void NotifyWeaponStateMachineStateChange(FName lastState, FName newState);

private:
    UPROPERTY() TObjectPtr<UVal_LocalPlayerSubsystem> pSubsystem = nullptr;
};
