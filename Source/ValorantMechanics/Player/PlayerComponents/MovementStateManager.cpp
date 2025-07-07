// © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "MovementStateManager.h"
#include "ValorantMechanics/Core/Val_LocalPlayerSubsystem.h"
#include "ValorantMechanics/Player/Val_Character.h"
#include "Val_CharacterMovementComponent.h"




UMovementStateManager::UMovementStateManager()
{
    parent = Cast<UVal_CharacterMovementComponent>(GetOuter());
}

void UMovementStateManager::OnUpdateState(EMovementState previousState, EMovementState enteredState)
{
    parent->pSubsystem->UpdateMovementState(previousState, enteredState);

    /*
     * movement speeds are reduced by 33% for any condition if bIsCrouched
     */
    switch (enteredState)
    {
    case EMovementState::Idle:
    case EMovementState::Running:
        parent->MaxWalkSpeed = parent->pCharacter->bIsCrouched ?
            AdjustSpeedForCrouch(parent->movementProperties.walkSpeed):
        AdjustSpeedForCrouch(parent->movementProperties.runSpeed);
        break;
        
    case EMovementState::Walking:
        parent->MaxWalkSpeed = AdjustSpeedForCrouch(parent->movementProperties.walkSpeed);
        break;

        /*
         * TODO complete OnStun logic
         */
    case EMovementState::LightStunned:
        parent->MaxWalkSpeed = AdjustSpeedForCrouch(parent->movementProperties.lightStunSpeed);
        break;
    case EMovementState::HeavyStunned:
        parent->MaxWalkSpeed = AdjustSpeedForCrouch(parent->movementProperties.heavyStunSpeed);
        break;
        
    default:
    case EMovementState::UsingAbilityMovement:
    case EMovementState::Jumping:
    case EMovementState::None:
        break;
    }
    
}

void UMovementStateManager::OnStackState(EMovementState stackedState)
{
}

void UMovementStateManager::OnUnstackState(EMovementState unstackedState)
{
}

float UMovementStateManager::AdjustSpeedForCrouch(const float value) const
{
    // reduces speed by 33% when crouched
    return parent->pCharacter && parent->pCharacter->bIsCrouched ? value * 0.67f : value;
}
