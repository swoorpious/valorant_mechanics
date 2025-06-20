// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "Val_PlayerController.h"
#include "Val_Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/InputSettings.h"
#include "ValorantMechanics/Input/Val_InputComponent.h"




void AVal_PlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}




void AVal_PlayerController::AddLookInput(FVector2D Look) const
{
	if (!playerCharacter || !playerCharacter->sceneComponent) return;

	// TODO: implement FOV scaling for ads weapons -> scaleFOV = currentFOV / baseFOV
	// would use scaleFOV instead later
	FVector2D viewportSize = GetWorld()->GetGameViewport()->Viewport->GetSizeXY();
	float cameraFOV = playerCharacter->characterMeshCamera->FieldOfView;
	
	// playerCharacter handles yaw
	FRotator yaw = playerCharacter->GetActorRotation() + FRotator(
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
	if (!valInputComponent->HasMovementInput()) return;
	

	FVector2d moveVector = valInputComponent->GetAdditiveMovementInput();
	constexpr float minThreshold = 0.05f;
	constexpr float maxScale = 10.0f;


	/*
	 * strafe movement reduction
	 * this code reduces value of A/D input as the camera moves in the direction of the strafe
	 * this softens the player movement making it feel natural and not overly responsive, and
	 * reduces the feeling of "player sliding"
	 */
	if (FMath::Abs(valInputComponent->GetLastLookVector().X) > minThreshold)
	{
		float scaleFactor = FMath::Clamp(
			1.0f / FMath::Pow(FMath::Abs(valInputComponent->GetLastLookVector().X), 0.5f), 
			1.0f / maxScale,
			maxScale
		);
    
		moveVector.X *= scaleFactor;
	}

	/*
	 * forward movement reduction
	 * perhaps unnecessary chunk of code
	 * does the same thing as strafe movement reduction but for W/S
	 */
	if (FMath::Abs(valInputComponent->GetLastLookVector().Y) > minThreshold)
	{
		float scaleFactor = FMath::Clamp(
			1.0f / FMath::Pow(FMath::Abs(valInputComponent->GetLastLookVector().Y), 0.5f), 
			1.0f / maxScale,
			maxScale
		);
    
		moveVector.Y *= scaleFactor;
	}
		
	const FVector WorldVector = playerCharacter->GetActorRightVector() * moveVector.X +
		playerCharacter->GetActorForwardVector() * moveVector.Y;
		
	playerCharacter->AddMovementInput(WorldVector.GetSafeNormal(), 1.0f);
	
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
	checkf(playerCharacter, TEXT("playerCharacter must be AVal_Character."));

	valInputComponent = playerCharacter->GetValInputComponent();

	valInputComponent->SetMappingContexts(this, InputComponent);
	valInputComponent->SetInputActions(this);

	InputComponent->bBlockInput = false;

	UInputSettings* inputSettings = UInputSettings::GetInputSettings();
	inputSettings->bUseMouseForTouch = false;
	bEnableMouseOverEvents = true;
	
}

void AVal_PlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}






