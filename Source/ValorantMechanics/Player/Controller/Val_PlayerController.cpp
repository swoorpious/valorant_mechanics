// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.



#include "Val_PlayerController.h"
#include "../Val_Character.h"
#include "Val_InputSystem.h"
#include "ValorantMechanics/ValorantMechanics.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/InputSettings.h"


AVal_PlayerController::AVal_PlayerController()
{
	valInputSystem = CreateDefaultSubobject<UVal_InputSystem>(TEXT("Val Input System"));
	
}





void AVal_PlayerController::AddLookInput(FVector2D Look) const
{
	if (!playerCharacter || !playerCharacter->sceneComponent) return;

	// TODO: implement FOV scaling for ads weapons -> scaleFOV = currentFOV / baseFOV
	// would use scaleFOV instead later
	FVector2D const viewportSize = GetWorld()->GetGameViewport()->Viewport->GetSizeXY();
	float const cameraFOV = playerCharacter->characterMeshCamera->FieldOfView;
	
	// playerCharacter handles yaw
	FRotator const yaw = playerCharacter->GetActorRotation() + FRotator(
		0,
		Look.X * (cameraFOV / viewportSize.X),
		0);
	playerCharacter->SetActorRotation(yaw); // update yaw (left/right)


	// SceneComponent handles pitch
	FRotator pitch = playerCharacter->sceneComponent->GetRelativeRotation() + FRotator(
		Look.Y * (cameraFOV / viewportSize.X),
		0, 0);
	pitch.Pitch = FMath::Clamp(pitch.Pitch, -89.9f, 89.9f);
	playerCharacter->sceneComponent->SetRelativeRotation(pitch); // update pitch (up/down)

}


void AVal_PlayerController::PlayerMove() const
{
	if (!valInputSystem->HasMovementInput()) return;
	
	FVector2d moveVector = valInputSystem->GetAdditiveMovementInput();
	constexpr float minThreshold = 0.05f;
	constexpr float maxScale = 10.0f;


	/*
	 * strafe movement reduction
	 * this code reduces value of A/D input as the camera moves in the direction of the strafe
	 * this softens the player movement making it feel natural and not overly responsive, and
	 * reduces the feeling of "player sliding"
	 */
	if (FMath::Abs(valInputSystem->GetLastLookVector().X) > minThreshold)
	{
		float scaleFactor = FMath::Clamp(
			1.0f / FMath::Pow(FMath::Abs(valInputSystem->GetLastLookVector().X), 0.5f), 
			1.0f / maxScale,
			maxScale
		);
    
		moveVector.X *= scaleFactor;
	}
	
	
	const FVector playerRight = playerCharacter->GetActorRightVector();
	const FVector playerForward = playerCharacter->GetActorForwardVector();
	const FVector worldVector = playerRight * moveVector.X +
		playerForward * moveVector.Y;
	
		
	playerCharacter->AddMovementInput(worldVector.GetSafeNormal(), 1.0f);
	
}



/*
 * TODO: implement short time period after landing to slow down the player, with an even shorter window before it activates
 */
void AVal_PlayerController::PlayerJump(const FInputActionInstance& InputActionInstance)
{
	// TArray<UInputTrigger*> triggers = InputActionInstance.GetTriggers();

	// TODO: implement started, ongoing, cancelled/completed
	/* switch (InputActionInstance.GetTriggerEvent()) {...} */
	
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
	if (playerCharacter->bIsCrouched) return;
	
	switch (InputActionInstance.GetTriggerEvent()) {
		case ETriggerEvent::Started:
		case ETriggerEvent::Triggered:
		case ETriggerEvent::Ongoing:
			playerCharacter->Walk();
			break;
		
		case ETriggerEvent::Canceled:
		case ETriggerEvent::Completed:
			playerCharacter->Unwalk();
			break;

		default: break;
	}
}

void AVal_PlayerController::PlayerUse(const FInputActionInstance& InputActionInstance)
{
}


void AVal_PlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	
	playerCharacter = Cast<AVal_Character>(aPawn);
	if (!playerCharacter) LOG(Val_Player, Error, "This controller and its descendants should only possess AMySpecificCharacterClassderived pawns!");
	
	valInputSystem->Init(this, InputComponent);
	
	
	InputComponent->bBlockInput = false;

	UInputSettings* inputSettings = UInputSettings::GetInputSettings();
	inputSettings->bUseMouseForTouch = false;
	bEnableMouseOverEvents = true;
	
}

void AVal_PlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}






