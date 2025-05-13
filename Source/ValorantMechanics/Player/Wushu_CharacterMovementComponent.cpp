// Fill out your copyright notice in the Description page of Project Settings.


#include "Wushu_CharacterMovementComponent.h"
#include "Wushu_Character.h"

#include "Wushu_PlayerController.h"


// Sets default values for this component's properties
UWushu_CharacterMovementComponent::UWushu_CharacterMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UWushu_CharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<AWushu_Character>(GetOwner());	
	PlayerController = Cast<AWushu_PlayerController>(GetController());

}

void UWushu_CharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HandleAirMovement(DeltaTime);
	
}


void UWushu_CharacterMovementComponent::HandleAirMovement(float DeltaTime)
{
	if (!IsFalling())
	{
		airStrafeTime = 0.0f;
		currentAirStrafeMultiplier = 1.0f;
		lastAirMovementDirection = FAirMovementDirection::None;
		return;
	}

	FVector viewVectorX = PlayerCharacter->GetActorForwardVector();
	FVector viewVectorY = PlayerCharacter->GetActorRightVector();
	FVector2d inputVector = PlayerController->GetAdditiveMovementInput();

	float viewDeltaX = PlayerController->GetLastLookActionVector().X;

	FVector currentDirection = Velocity.GetSafeNormal();
	
	bool IsMovingLeft = inputVector.X < -0.1f;
	bool IsMovingRight = inputVector.X > 0.1f;
	// bool IsMovingForward = inputY > 0.1f;
	// bool IsMovingBackward = inputY < -0.1f;


	
	FAirMovementDirection currentAirMovementDirection;
	if (IsMovingLeft && viewDeltaX < -0.1f)
		currentAirMovementDirection = FAirMovementDirection::Matching;
	else if (IsMovingRight && viewDeltaX > 0.1f)
		currentAirMovementDirection = FAirMovementDirection::Matching;
	else if ((IsMovingLeft && viewDeltaX > 0.1f) || (IsMovingRight && viewDeltaX < -0.1f))
		currentAirMovementDirection = FAirMovementDirection::Opposing;
	else
		currentAirMovementDirection = FAirMovementDirection::Neutral;


	float airControlFactor;
	
	switch (currentAirMovementDirection)
	{
	case FAirMovementDirection::Matching: // gain momentum when strafing with the view direction 
		airStrafeTime += DeltaTime;
		currentAirStrafeMultiplier = FMath::Min(
			_MaxAirStrafeMultiplier,
			1.0f + (_AirStrafeAccelerationRate * airStrafeTime)
		);
		airControlFactor = _MaxAirControl;
		break;
            
	case FAirMovementDirection::Opposing: // reduce speed when strafing against view direction
		airStrafeTime = FMath::Max(0.0f, airStrafeTime - DeltaTime * 2.0f);
		currentAirStrafeMultiplier = FMath::Max(
			1.0f,
			currentAirStrafeMultiplier * 0.95f
		);
		airControlFactor = _MinAirControl;
		break;
            
	case FAirMovementDirection::Neutral: // to maintain momentum when not strafing
		airStrafeTime = FMath::Max(0.0f, airStrafeTime - DeltaTime * 0.5f);
		currentAirStrafeMultiplier = FMath::Lerp(
			currentAirStrafeMultiplier,
			1.0f,
			DeltaTime * _AirStrafeDecayRate
		);
		airControlFactor = _MidAirControl;
		break;
            
	default:
		airControlFactor = 0.0f;
		break;
	}

	bool hasMovementInput = PlayerController->HasMovementInput();

	AirControl = hasMovementInput ? airControlFactor : 0.0f;

	if (currentAirStrafeMultiplier > 1.0f && hasMovementInput && Velocity.Size() > 0.0f)
	{
		FVector desiredDirection = (viewVectorY * inputVector.Y * _ForceFactorY + viewVectorX * (lastAirMovementDirection != FAirMovementDirection::Opposing ? inputVector.X : 0) * _ForceFactorX).GetSafeNormal();
		float directionAlignment = FVector::DotProduct(desiredDirection, currentDirection);

		if (directionAlignment > 0.0f)
		{
			float extraForce = currentAirStrafeMultiplier * _AirStrafeForce * directionAlignment;
			FVector forceToApply = desiredDirection * extraForce;

			if (Velocity.Size() + extraForce <= 0.0f)
			{
				AddForce(forceToApply);
			}
		}
	}

	lastAirMovementDirection = currentAirMovementDirection;
	
}



