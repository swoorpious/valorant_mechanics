// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "Val_PlayerController.h"
#include "Val_Character.h"
#include "ValorantMechanics/Input/Val_InputComponent.h"




void AVal_PlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	
	if (HasMovementInput()) {
		FVector2d moveVector = GetAdditiveMovementInput();
		constexpr float minThreshold = 0.05f;
		constexpr float maxScale = 10.0f;

		
		// strafe reduction	
		if (FMath::Abs(lastLookVector.X) > minThreshold)
		{
			float scaleFactor = FMath::Clamp(
				1.0f / FMath::Pow(FMath::Abs(lastLookVector.X), 0.5f), 
				1.0f / maxScale,
				maxScale
			);
    
			moveVector.X *= scaleFactor;
		}

		if (FMath::Abs(lastLookVector.Y) > minThreshold)
		{
			float scaleFactor = FMath::Clamp(
				1.0f / FMath::Pow(FMath::Abs(lastLookVector.Y), 0.5f), 
				1.0f / maxScale,
				maxScale
			);
    
			moveVector.Y *= scaleFactor;
		}
		
		const FVector WorldVector = playerCharacter->GetActorRightVector() * moveVector.X + playerCharacter->GetActorForwardVector() * moveVector.Y;
		playerCharacter->AddMovementInput(WorldVector.GetSafeNormal(), 1.0f);
	}
	

}




void AVal_PlayerController::AddLookInput(FVector2D Look) const
{
	if (!playerCharacter || !playerCharacter->SceneComponent) return;

	// playerCharacter handles yaw
	FRotator yaw = playerCharacter->GetActorRotation() + FRotator(0, Look.X, 0);
	playerCharacter->SetActorRotation(yaw); // update yaw (left/right)


	// SceneComponent handles pitch
	FRotator pitch = playerCharacter->SceneComponent->GetRelativeRotation() + FRotator(Look.Y, 0, 0);
	pitch.Pitch = FMath::Clamp(pitch.Pitch, -89.9f, 89.9f);
	playerCharacter->SceneComponent->SetRelativeRotation(pitch); // update pitch (up/down)

}


void AVal_PlayerController::PlayerLook(const FInputActionInstance& InputActionInstance)
{
	const FInputActionValue& InputActionValue = InputActionInstance.GetValue();
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
	lastLookVector = LookAxisVector;
	AddLookInput(LookAxisVector * Sensitivity);

	/*
	 * x -> yaw
	 * y -> pitch
	 */
}


void AVal_PlayerController::PlayerMove(const FInputActionInstance& InputActionInstance)
{
	const UInputAction* actionSource = InputActionInstance.GetSourceAction();
	const ETriggerEvent actionTrigger = InputActionInstance.GetTriggerEvent();

	const FString actionName = actionSource->GetName();

	
	if (actionTrigger == ETriggerEvent::Triggered)
	{
		     if (actionName == "VIA_Move_W") isAction_Move_W = true;
		else if (actionName == "VIA_Move_A") isAction_Move_A = true;
		else if (actionName == "VIA_Move_D") isAction_Move_D = true;
		else if (actionName == "VIA_Move_S") isAction_Move_S = true;
	}
	else if (actionTrigger == ETriggerEvent::Canceled || actionTrigger == ETriggerEvent::Completed)
	{
		     if (actionName == "VIA_Move_W") isAction_Move_W = false;
		else if (actionName == "VIA_Move_A") isAction_Move_A = false;
		else if (actionName == "VIA_Move_D") isAction_Move_D = false;
		else if (actionName == "VIA_Move_S") isAction_Move_S = false;
	}
	
}



/*
 * TODO: implement short time period after landing to slow down the player, with an even shorter window before it activates
 */
void AVal_PlayerController::PlayerJump(const FInputActionInstance& InputActionInstance)
{
	// TArray<UInputTrigger*> triggers = InputActionInstance.GetTriggers();
	
	if (playerCharacter)
    {
    	playerCharacter->UnCrouch();
    	playerCharacter->Jump();
    }
}

void AVal_PlayerController::PlayerCrouch(const FInputActionInstance& InputActionInstance)
{
}

void AVal_PlayerController::PlayerWalk(const FInputActionInstance& InputActionInstance)
{
	const ETriggerEvent actionTrigger = InputActionInstance.GetTriggerEvent();

	if (actionTrigger == ETriggerEvent::Started) playerCharacter->Walk();
	if (actionTrigger == ETriggerEvent::Canceled || actionTrigger == ETriggerEvent::Completed) playerCharacter->Unwalk();
}

void AVal_PlayerController::PlayerUse(const FInputActionInstance& InputActionInstance)
{
}



void AVal_PlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	
	playerCharacter = Cast<AVal_Character>(aPawn);
	checkf(playerCharacter, TEXT("playerCharacter must be AVal_Character."));

	valInputComponent = playerCharacter->valInputComponent;

	valInputComponent->SetMappingContexts(this, InputComponent);
	valInputComponent->SetInputActions(this);
	
}

void AVal_PlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}






