// © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once


#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "ValorantMechanics/Core/StateManager/StateManager.h"
#include "ValorantMechanics/Core/Shared/PlayerStates.h"

#include "MovementSM.generated.h"


class UVal_CharacterMovementComponent;


/**
 * 
 */
UCLASS()
class VALORANTMECHANICS_API UMovementStateManager : public UObject, public StateManager<EMovementState>
{
    GENERATED_BODY()

public:
    UMovementStateManager();

protected:
    virtual void OnUpdateState(EMovementState previousState, EMovementState enteredState) override;
    virtual void OnStackState(EMovementState stackedState) override;
    virtual void OnUnstackState(EMovementState unstackedState) override;

private:
    TObjectPtr<UVal_CharacterMovementComponent> parent = nullptr;

    float AdjustSpeedForCrouch(const float value) const;
};
