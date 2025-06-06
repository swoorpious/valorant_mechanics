// © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "Val_InputComponent.h"
#include "ValorantMechanics/Player/Val_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


UVal_InputComponent::UVal_InputComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}


void UVal_InputComponent::SetMappingContexts(AVal_PlayerController* classObject, TObjectPtr<UInputComponent> inputComponent)
{
	
    enhancedInputComponent = Cast<UEnhancedInputComponent>(inputComponent);
    checkf(enhancedInputComponent, TEXT("Unable to get a reference to EnhancedInputComponent."));

    UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(classObject->GetLocalPlayer());
    checkf(InputSubsystem, TEXT("Unable to get a reference to UEnhancedInputLocalPlayerSubsystem."));

    checkf(mappingContexts.Default_Mapping, TEXT("Default_Mapping context was not specified."));
    InputSubsystem->AddMappingContext(mappingContexts.Default_Mapping, 0);

    checkf(mappingContexts.Weapon_Mapping, TEXT("Weapon_Mapping context was not specified."));
    InputSubsystem->AddMappingContext(mappingContexts.Weapon_Mapping, 0);

    checkf(mappingContexts.Ability_Mapping, TEXT("Ability_Mapping context was not specified."));
    InputSubsystem->AddMappingContext(mappingContexts.Ability_Mapping, 0);
    
}

void UVal_InputComponent::SetInputActions(AVal_PlayerController* classObject)
{
	const TArray<ETriggerEvent> events1 = {
		ETriggerEvent::Triggered, 
		ETriggerEvent::Canceled, 
		ETriggerEvent::Completed,
	};
	
	const TArray<ETriggerEvent> events2 = {
		ETriggerEvent::Started,
		ETriggerEvent::Ongoing,
		ETriggerEvent::Canceled,
		ETriggerEvent::Completed,
	};

	for (ETriggerEvent e : events1)
	{
		enhancedInputComponent->BindAction(playerActions.Action_Move_W, e, classObject, &AVal_PlayerController::PlayerMove); 
		enhancedInputComponent->BindAction(playerActions.Action_Move_A, e, classObject, &AVal_PlayerController::PlayerMove); 
		enhancedInputComponent->BindAction(playerActions.Action_Move_D, e, classObject, &AVal_PlayerController::PlayerMove); 
		enhancedInputComponent->BindAction(playerActions.Action_Move_S, e, classObject, &AVal_PlayerController::PlayerMove);
		
		enhancedInputComponent->BindAction(playerActions.Action_Look, e, classObject, &AVal_PlayerController::PlayerLook); 
	}

	for (ETriggerEvent e : events2)
	{
		enhancedInputComponent->BindAction(playerActions.Action_Jump, e, classObject, &AVal_PlayerController::PlayerJump); 
		enhancedInputComponent->BindAction(playerActions.Action_Crouch, e, classObject, &AVal_PlayerController::PlayerCrouch); 
		enhancedInputComponent->BindAction(playerActions.Action_Walk, e, classObject, &AVal_PlayerController::PlayerWalk); 
	}


	enhancedInputComponent->BindAction(playerActions.Action_Use, ETriggerEvent::Started, classObject, &AVal_PlayerController::PlayerUse);
    
}


void UVal_InputComponent::BeginPlay()
{
    Super::BeginPlay();
    
}



void UVal_InputComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

