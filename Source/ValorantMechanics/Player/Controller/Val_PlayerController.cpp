// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.



#include "Val_PlayerController.h"

#include "../Val_Character.h"
#include "Val_InputSystem.h"
#include "ValorantMechanics/ValorantMechanics.h"
#include "ValorantMechanics/Player/PlayerComponents/Val_CharacterMovementComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/InputSettings.h"


AVal_PlayerController::AVal_PlayerController()
{
	inputSystem = CreateDefaultSubobject<UVal_InputSystem>(TEXT("Val Input System"));
	
}

void AVal_PlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	
	pCharacter = Cast<AVal_Character>(aPawn);
	if (!pCharacter) LOG(Val_Player, Error, "This controller and its descendants should only possess AMySpecificCharacterClass derived pawns!");
	pMovement = pCharacter->GetValMovementComponent();

	
	inputSystem->Init(this, InputComponent);
	
	InputComponent->bBlockInput = false;

	UInputSettings* inputSettings = UInputSettings::GetInputSettings();
	inputSettings->bUseMouseForTouch = false;
	bEnableMouseOverEvents = true;
	
}

void AVal_PlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}




void AVal_PlayerController::AddLookInput(FVector2D Look) const
{
	if (!pCharacter || !pCharacter->sceneComponent) return;

	// TODO: implement FOV scaling for ads weapons -> scaleFOV = currentFOV / baseFOV
	// would use scaleFOV instead later
	FVector2D const viewportSize = GetWorld()->GetGameViewport()->Viewport->GetSizeXY();
	float const cameraFOV = pCharacter->characterMeshCamera->FieldOfView;
	
	// pCharacter handles yaw
	FRotator const yaw = pCharacter->GetActorRotation() + FRotator(
		0,
		Look.X * (cameraFOV / viewportSize.X),
		0);
	pCharacter->SetActorRotation(yaw); // update yaw (left/right)


	// SceneComponent handles pitch
	FRotator pitch = pCharacter->sceneComponent->GetRelativeRotation() + FRotator(
		Look.Y * (cameraFOV / viewportSize.X),
		0, 0);
	pitch.Pitch = FMath::Clamp(pitch.Pitch, -89.9f, 89.9f);
	pCharacter->sceneComponent->SetRelativeRotation(pitch); // update pitch (up/down)

}


void AVal_PlayerController::PlayerMove() const
{
	if (!inputSystem->HasMovementInput()) return;
	
	FVector2d moveVector = inputSystem->GetAdditiveMovementInput();
	constexpr float minThreshold = 0.05f;
	constexpr float maxScale = 10.0f;


	/*
	 * strafe movement reduction
	 * this code reduces value of A/D input as the camera moves in the direction of the strafe
	 * this softens the player movement making it feel natural and not overly responsive, and
	 * reduces the feeling of "player sliding"
	 */
	if (FMath::Abs(inputSystem->GetLastLookVector().X) > minThreshold)
	{
		float scaleFactor = FMath::Clamp(
			1.0f / FMath::Pow(FMath::Abs(inputSystem->GetLastLookVector().X), 0.5f), 
			1.0f / maxScale,
			maxScale
		);
    
		moveVector.X *= scaleFactor;
	}
	
	
	const FVector playerRight = pCharacter->GetActorRightVector();
	const FVector playerForward = pCharacter->GetActorForwardVector();
	const FVector worldVector = playerRight * moveVector.X +
		playerForward * moveVector.Y;
	
		
	pCharacter->AddMovementInput(worldVector.GetSafeNormal(), 1.0f);
	
}



/*
 * TODO: implement short time period after landing to slow down the player, with an even shorter window before it activates
 */
void AVal_PlayerController::PlayerJump(const FInputActionInstance& inputInstance)
{
	// TODO: implement started, ongoing, cancelled/completed
	
	if (pCharacter)
    {
    	pCharacter->UnCrouch();
    	pCharacter->Jump();
    }
}

void AVal_PlayerController::PlayerCrouch(const FInputActionInstance& inputInstance)
{
}


void AVal_PlayerController::PlayerWalk(const FInputActionInstance& inputInstance)
{
	switch (inputInstance.GetTriggerEvent()) {
		case ETriggerEvent::Started:
		case ETriggerEvent::Triggered:
		case ETriggerEvent::Ongoing:
			pMovement->TryUpdateState(EMovementState::Walking);
			break;
		
		case ETriggerEvent::Canceled:
		case ETriggerEvent::Completed:
			pCharacter->bIsCrouched ?
				pMovement->TryUpdateState(EMovementState::Crouching) :
				pMovement->TryUpdateState(EMovementState::Running);
			break;

		default: break;
	}
}

void AVal_PlayerController::PlayerUse(const FInputActionInstance& inputInstance)
{
}







