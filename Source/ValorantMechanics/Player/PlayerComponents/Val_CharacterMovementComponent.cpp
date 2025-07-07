// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.



#include "Val_CharacterMovementComponent.h"

#include "../Val_Character.h"
#include "../Controller/Val_PlayerController.h"
#include "../Controller/Val_InputSystem.h"
#include "ValorantMechanics/Core/Val_LocalPlayerSubsystem.h"


#include "ValorantMechanics/ValorantMechanics.h"



// Sets default values for this component's properties
UVal_CharacterMovementComponent::UVal_CharacterMovementComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    bOrientRotationToMovement = false;
    
    movementSM = CreateDefaultSubobject<UMovementStateManager>(TEXT("Movement State Manager"));
    InitializePrimaryStateManager(movementSM);

}


void UVal_CharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    movementSM->TickState(DeltaTime);

    HandleAirMovement(DeltaTime);
}




void UVal_CharacterMovementComponent::BeginPlay()
{
    Super::BeginPlay();

    pCharacter = Cast<AVal_Character>(GetOwner());
    if (pCharacter)
    {
        valInputSystem = pCharacter->GetValPlayerController()->GetInputSystem();
        pSubsystem = pCharacter->GetValPlayerController()->GetLocalPlayer()->GetSubsystem<UVal_LocalPlayerSubsystem>();
        
    }

    // movementManager = NewObject<UMovementStateManager>(this);
    
}


bool UVal_CharacterMovementComponent::DoJump(bool bReplayingMoves, float DeltaTime)
{
    return Super::DoJump(bReplayingMoves, DeltaTime);
}


/*
 * TODO: Function needs tweaking
 * * air strafing does not feel responsive
 * * the character still drifts
 */
void UVal_CharacterMovementComponent::HandleAirMovement(float DeltaTime)
{
    if (!IsFalling())
    {
        airStrafeTime = 0.0f;
        currentAirStrafeMultiplier = 1.0f;
        lastAirInputDirection = EAirMovementInputDirection::None;
        return;
    }
    

    FVector const viewVectorX = pCharacter->GetActorForwardVector();
    FVector const viewVectorY = pCharacter->GetActorRightVector();
    FVector2d const inputVector = valInputSystem->GetAdditiveMovementInput();

    float const viewDeltaX = valInputSystem->GetLastLookVector().X;

    FVector const currentDirection = Velocity.GetSafeNormal();
    
    bool const isMovingLeft = inputVector.X < -0.1f;
    bool const isMovingRight = inputVector.X > 0.1f;


    
    EAirMovementInputDirection currentAirMovementDirection;
    if (isMovingLeft && viewDeltaX < -0.1f)
        currentAirMovementDirection = EAirMovementInputDirection::Matching;
    
    else if (isMovingRight && viewDeltaX > 0.1f)
        currentAirMovementDirection = EAirMovementInputDirection::Matching;
    
    else if ((isMovingLeft && viewDeltaX > 0.1f) || (isMovingRight && viewDeltaX < -0.1f))
        currentAirMovementDirection = EAirMovementInputDirection::Opposing;
    
    else currentAirMovementDirection = EAirMovementInputDirection::Neutral;


    float airControlFactor;
    
    switch (currentAirMovementDirection)
    {
        // gain momentum when strafing with the view direction
        case EAirMovementInputDirection::Matching: 

            LOG(Val_Player, Warning, "Matching Direction");
        
            airStrafeTime += DeltaTime;
            currentAirStrafeMultiplier = FMath::Min(
                airMovementProperties.maxAirStrafeMultiplier,
                1.0f + airMovementProperties.airStrafeAccelerationRate * airStrafeTime
            );
            airControlFactor = airMovementProperties.maxAirControl;
            break;

        // reduce speed when strafing against view direction
        case EAirMovementInputDirection::Opposing: 

            LOG(Val_Player, Warning, "Opposing Direction");
                    
            airStrafeTime = FMath::Max(0.0f, airStrafeTime - DeltaTime * 2.0f);
            currentAirStrafeMultiplier = FMath::Max(
                1.0f,
                currentAirStrafeMultiplier * 0.95f
            );
            airControlFactor = airMovementProperties.minAirControl;
            break;

        // to maintain momentum when not strafing
        case EAirMovementInputDirection::Neutral: 
        
            LOG(Val_Player, Warning, "Neutral Direction");
            
            airStrafeTime = FMath::Max(0.0f, airStrafeTime - DeltaTime * 0.5f);
            currentAirStrafeMultiplier = FMath::Lerp(
                currentAirStrafeMultiplier,
                1.0f,
                DeltaTime * airMovementProperties.airStrafeDecayRate
            );
            airControlFactor = airMovementProperties.midAirControl;
            break;
                
        default:
            airControlFactor = 0.0f;
            break;
    }

    bool const hasMovementInput = valInputSystem->HasMovementInput();

    AirControl = hasMovementInput ? airControlFactor : 0.0f;

    if (currentAirStrafeMultiplier > 1.0f && hasMovementInput && Velocity.Size() > 0.0f)
    {
        FVector const desiredDirection = (viewVectorY * inputVector.Y * airMovementProperties.forceFactorY +
            viewVectorX * (lastAirInputDirection != EAirMovementInputDirection::Opposing ? inputVector.X : 0) * airMovementProperties.forceFactorX)
            .GetSafeNormal();

        
        if (float const directionAlignment = FVector::DotProduct(desiredDirection, currentDirection) > 0.0f)
        {
            float const extraForce = currentAirStrafeMultiplier * airMovementProperties.airStrafeForce * directionAlignment;
            
            if (Velocity.Size() + extraForce <= 0.0f) AddForce(desiredDirection * extraForce);
        }
    }

    lastAirInputDirection = currentAirMovementDirection;
    
}

UMovementStateManager* UVal_CharacterMovementComponent::GetStateManager() const
{
    return movementSM;
}




