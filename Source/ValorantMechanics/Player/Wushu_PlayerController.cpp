// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "Wushu_PlayerController.h"
#include "Wushu_Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
// #include "GameFramework/CharacterMovementComponent.h"
// #include "MeshPaintVisualize.h"
// #include "GameFramework/CharacterMovementComponent.h"
// #include "rapidjson/document.h"


void AWushu_PlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	
	if (HasMovementInput()) {
		FVector2d MoveVector = GetAdditiveMovementInput();
		constexpr float MinThreshold = 0.05f;
		constexpr float MaxScale = 10.0f;
		constexpr float MaxInput = 1.0f;

		
		
		if (FMath::Abs(lastLookVector.X) > MinThreshold)
		{
			float ScaleFactor = FMath::Clamp(
				1.0f / FMath::Pow(FMath::Abs(lastLookVector.X), 0.5f), 
				1.0f / MaxScale,
				MaxScale  
			);
    
			MoveVector.X *= ScaleFactor;
		}

		if (FMath::Abs(lastLookVector.Y) > MinThreshold)
		{
			float ScaleFactor = FMath::Clamp(
				1.0f / FMath::Pow(FMath::Abs(lastLookVector.Y), 0.5f), 
				1.0f / MaxScale,
				MaxScale
			);
    
			MoveVector.Y *= ScaleFactor;
		}
		
		const FVector WorldVector = PlayerCharacter->GetActorRightVector() * MoveVector.X + PlayerCharacter->GetActorForwardVector() * MoveVector.Y;
		PlayerCharacter->AddMovementInput(WorldVector.GetSafeNormal(), 1.0f);
	}
	

}

/*
FVector2D AWushu_PlayerController::InputReductionScaling(const FVector2D& CurrentMove)
{
	constexpr float MinThreshold = 0.05f;
	constexpr float MaxScale = 10.0f;

	FVector2D ScaledMove = CurrentMove;

	// X-axis scaling
	if (FMath::Abs(lastLookVector.X) > MinThreshold)
	{
		// Exponential inverse scaling
		float ScaleFactor = FMath::Clamp(
			1.0f / FMath::Pow(FMath::Abs(lastLookVector.X), 0.5f), 
			1.0f / MaxScale,  // Lower bound
			MaxScale          // Upper bound
		);
    
		ScaledMove.X *= ScaleFactor;
	}

	// Y-axis scaling (similar logic)
	if (FMath::Abs(lastLookVector.Y) > MinThreshold)
	{
		float ScaleFactor = FMath::Clamp(
			1.0f / FMath::Pow(FMath::Abs(lastLookVector.Y), 0.5f), 
			1.0f / MaxScale,  // Lower bound
			MaxScale          // Upper bound
		);
    
		ScaledMove.Y *= ScaleFactor;
	}

	return ScaledMove;
}
*/


void AWushu_PlayerController::AddLookInput(FVector2D Look) const
{
	if (!PlayerCharacter || !PlayerCharacter->SceneComponent) return;

	// PlayerCharacter handles yaw
	FRotator yaw = PlayerCharacter->GetActorRotation() + FRotator(0, Look.X, 0);
	PlayerCharacter->SetActorRotation(yaw); // update yaw (left/right)


	// SceneComponent handles pitch
	FRotator pitch = PlayerCharacter->SceneComponent->GetRelativeRotation() + FRotator(Look.Y, 0, 0);
	pitch.Pitch = FMath::Clamp(pitch.Pitch, -89.9f, 89.9f);
	PlayerCharacter->SceneComponent->SetRelativeRotation(pitch); // update pitch (up/down)

	
	UE_LOG(LogPlayerInput, Log, TEXT("Look Input - X: %.2f, Y: %.2f | Actor Yaw: %.2f, Scene Pitch: %.2f"), Look.X, Look.Y, yaw.Yaw, pitch.Pitch);
}


void AWushu_PlayerController::PlayerLook(const FInputActionInstance& InputActionInstance)
{
	const FInputActionValue& InputActionValue = InputActionInstance.GetValue();
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
	lastLookVector = LookAxisVector;

	/*
	 * x -> yaw
	 * y -> pitch
	 */
	AddLookInput(LookAxisVector * Sensitivity);
}


void AWushu_PlayerController::PlayerMove(const FInputActionInstance& InputActionInstance)
{
	const UInputAction* ActionSource = InputActionInstance.GetSourceAction();
	const ETriggerEvent ActionTrigger = InputActionInstance.GetTriggerEvent();

	const FString ActionName = ActionSource->GetName();

	
	if (ActionTrigger == ETriggerEvent::Triggered)
	{
		     if (ActionName == "VIA_Move_W") isAction_Move_W = true;
		else if (ActionName == "VIA_Move_A") isAction_Move_A = true;
		else if (ActionName == "VIA_Move_D") isAction_Move_D = true;
		else if (ActionName == "VIA_Move_S") isAction_Move_S = true;
	}
	else if (ActionTrigger == ETriggerEvent::Canceled || ActionTrigger == ETriggerEvent::Completed)
	{
		     if (ActionName == "VIA_Move_W") isAction_Move_W = false;
		else if (ActionName == "VIA_Move_A") isAction_Move_A = false;
		else if (ActionName == "VIA_Move_D") isAction_Move_D = false;
		else if (ActionName == "VIA_Move_S") isAction_Move_S = false;
	}

	
	// canMove = isAction_Move_W ^ isAction_Move_S || isAction_Move_A ^ isAction_Move_D;
}



/*
 * TODO: implement short time period after landing to slow down the player, with an even shorter window before it activates
 */
void AWushu_PlayerController::PlayerJump(const FInputActionInstance& InputActionInstance)
{
	TArray<UInputTrigger*> Triggers = InputActionInstance.GetTriggers();
	
	if (PlayerCharacter)
    {
    	PlayerCharacter->UnCrouch();
    	PlayerCharacter->Jump();
    }
}

void AWushu_PlayerController::PlayerCrouch(const FInputActionInstance& InputActionInstance)
{
}

void AWushu_PlayerController::PlayerWalk(const FInputActionInstance& InputActionInstance)
{
	const ETriggerEvent ActionTrigger = InputActionInstance.GetTriggerEvent();

	UE_LOG(LogTemp, Warning, TEXT("PlayerWalk()"));
	if (ActionTrigger == ETriggerEvent::Started) PlayerCharacter->Walk();
	if (ActionTrigger == ETriggerEvent::Canceled || ActionTrigger == ETriggerEvent::Completed) PlayerCharacter->Unwalk();
}

void AWushu_PlayerController::PlayerUse(const FInputActionInstance& InputActionInstance)
{
}



void AWushu_PlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	
	PlayerCharacter = Cast<AWushu_Character>(aPawn);
	checkf(PlayerCharacter, TEXT("PlayerCharacter must be AWushu_Character."));

	PlayerCharacterMovementComponent = PlayerCharacter->GetCharacterMovement();

	EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	checkf(EnhancedInputComponent, TEXT("Unable to get a reference to EnhancedInputComponent."));

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	checkf(InputSubsystem, TEXT("Unable to get a reference to UEnhancedInputLocalPlayerSubsystem."));

	checkf(InputMappingContext, TEXT("InputMappingContext was not specified."));
	InputSubsystem->AddMappingContext(InputMappingContext, 0);


#pragma region EIC Bindings
	/*
	 * TODO: the system for started, canceled, and completed events for movement keys can be improved
	 * doing it this way to add inputs instead of "whichever is pressed next"
	 */
	
	EnhancedInputComponent->BindAction(Action_Move_W, ETriggerEvent::Triggered, this, &AWushu_PlayerController::PlayerMove); 
	EnhancedInputComponent->BindAction(Action_Move_A, ETriggerEvent::Triggered, this, &AWushu_PlayerController::PlayerMove); 
	EnhancedInputComponent->BindAction(Action_Move_D, ETriggerEvent::Triggered, this, &AWushu_PlayerController::PlayerMove); 
	EnhancedInputComponent->BindAction(Action_Move_S, ETriggerEvent::Triggered, this, &AWushu_PlayerController::PlayerMove);
	
	EnhancedInputComponent->BindAction(Action_Move_W, ETriggerEvent::Canceled, this, &AWushu_PlayerController::PlayerMove); 
	EnhancedInputComponent->BindAction(Action_Move_A, ETriggerEvent::Canceled, this, &AWushu_PlayerController::PlayerMove); 
	EnhancedInputComponent->BindAction(Action_Move_D, ETriggerEvent::Canceled, this, &AWushu_PlayerController::PlayerMove); 
	EnhancedInputComponent->BindAction(Action_Move_S, ETriggerEvent::Canceled, this, &AWushu_PlayerController::PlayerMove);
	
	EnhancedInputComponent->BindAction(Action_Move_W, ETriggerEvent::Completed, this, &AWushu_PlayerController::PlayerMove); 
	EnhancedInputComponent->BindAction(Action_Move_A, ETriggerEvent::Completed, this, &AWushu_PlayerController::PlayerMove); 
	EnhancedInputComponent->BindAction(Action_Move_D, ETriggerEvent::Completed, this, &AWushu_PlayerController::PlayerMove); 
	EnhancedInputComponent->BindAction(Action_Move_S, ETriggerEvent::Completed, this, &AWushu_PlayerController::PlayerMove); 


	
	EnhancedInputComponent->BindAction(Action_Look, ETriggerEvent::Triggered, this, &AWushu_PlayerController::PlayerLook); 
	EnhancedInputComponent->BindAction(Action_Look, ETriggerEvent::Canceled, this, &AWushu_PlayerController::PlayerLook); 
	EnhancedInputComponent->BindAction(Action_Look, ETriggerEvent::Completed, this, &AWushu_PlayerController::PlayerLook); 

	EnhancedInputComponent->BindAction(Action_Jump, ETriggerEvent::Started, this, &AWushu_PlayerController::PlayerJump); 
	EnhancedInputComponent->BindAction(Action_Jump, ETriggerEvent::Ongoing, this, &AWushu_PlayerController::PlayerJump); 
	EnhancedInputComponent->BindAction(Action_Jump, ETriggerEvent::Completed, this, &AWushu_PlayerController::PlayerJump); 

	EnhancedInputComponent->BindAction(Action_Crouch, ETriggerEvent::Started, this, &AWushu_PlayerController::PlayerCrouch); 
	EnhancedInputComponent->BindAction(Action_Crouch, ETriggerEvent::Completed, this, &AWushu_PlayerController::PlayerCrouch); 

	EnhancedInputComponent->BindAction(Action_Walk, ETriggerEvent::Started, this, &AWushu_PlayerController::PlayerWalk); 
	EnhancedInputComponent->BindAction(Action_Walk, ETriggerEvent::Canceled, this, &AWushu_PlayerController::PlayerWalk); 
	EnhancedInputComponent->BindAction(Action_Walk, ETriggerEvent::Completed, this, &AWushu_PlayerController::PlayerWalk); 

	EnhancedInputComponent->BindAction(Action_Use, ETriggerEvent::Started, this, &AWushu_PlayerController::PlayerUse);
#pragma endregion

	
}

void AWushu_PlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}






