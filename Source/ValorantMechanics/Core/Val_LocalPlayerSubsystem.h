// Copyright 2025 Swaroop. Personal Unreal engine project inspired BY VALORANT.


#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"

#include "Shared/PlayerDelegateDefinition.h"

#include "Val_LocalPlayerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class VALORANTMECHANICS_API UVal_LocalPlayerSubsystem : public ULocalPlayerSubsystem
{
    GENERATED_BODY()

public:

#pragma region STATE MACHINE

    OnMovementStateChanged mStateChanged;
    OnWeaponStateChanged wStateChanged;
    OnPlayerAnimStateChanged pAnimStateChanged;
    OnWeaponAnimStateChanged wAnimStateChanged;


    void UpdateMovementState();
    void UpdateWeaponState();
    void UpdateMovementAnimState();
    void UpdateWeaponAnimState();

#pragma endregion STATE MACHINE
};
