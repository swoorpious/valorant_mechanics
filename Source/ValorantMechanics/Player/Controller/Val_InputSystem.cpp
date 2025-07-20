// Copyright 2025 Swaroop. Personal Unreal engine project inspired BY VALORANT.


#include "Val_InputSystem.h"
#include "../Val_Character.h"
#include "Val_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


void UVal_InputSystem::Init(AVal_PlayerController* classObject, const TObjectPtr<UInputComponent>& inputComponent)
{
    pController = classObject;
    this->SetMappingContexts(inputComponent);
    this->SetInputActions();
}

void UVal_InputSystem::SetMappingContexts(const TObjectPtr<UInputComponent>& inputComponent)
{    
    enhancedInputComponent = Cast<UEnhancedInputComponent>(inputComponent);
    checkf(enhancedInputComponent, TEXT("Unable to get a reference to EnhancedInputComponent."));

    UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pController->GetLocalPlayer());
    checkf(InputSubsystem, TEXT("Unable to get a reference to UEnhancedInputLocalPlayerSubsystem."));

    checkf(mappingContexts.Default_Mapping, TEXT("Default_Mapping context was not specified."));
    InputSubsystem->AddMappingContext(mappingContexts.Default_Mapping, 0);

    checkf(mappingContexts.Weapon_Mapping, TEXT("Weapon_Mapping context was not specified."));
    InputSubsystem->AddMappingContext(mappingContexts.Weapon_Mapping, 1);

    checkf(mappingContexts.Ability_Mapping, TEXT("Ability_Mapping context was not specified."));
    InputSubsystem->AddMappingContext(mappingContexts.Ability_Mapping, 3);
    
}

void UVal_InputSystem::SetInputActions()
{
    const TArray<ETriggerEvent> events1 = {
        ETriggerEvent::Started,
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

    for (ETriggerEvent const e : events1)
    {
        enhancedInputComponent->BindAction(playerActions.Action_Move_W, e, this, &UVal_InputSystem::HandleMoveInput); 
        enhancedInputComponent->BindAction(playerActions.Action_Move_A, e, this, &UVal_InputSystem::HandleMoveInput); 
        enhancedInputComponent->BindAction(playerActions.Action_Move_D, e, this, &UVal_InputSystem::HandleMoveInput); 
        enhancedInputComponent->BindAction(playerActions.Action_Move_S, e, this, &UVal_InputSystem::HandleMoveInput);
        
        enhancedInputComponent->BindAction(playerActions.Action_Look, e, this, &UVal_InputSystem::HandleLookInput);
        
        enhancedInputComponent->BindAction(weaponActions.Action_Attack, e, pController, &AVal_PlayerController::WeaponFire);
        enhancedInputComponent->BindAction(weaponActions.Action_AltAttack, e, pController, &AVal_PlayerController::WeaponFire);
    }

    for (ETriggerEvent const e : events2)
    {
        enhancedInputComponent->BindAction(playerActions.Action_Jump, e, pController, &AVal_PlayerController::PlayerJump); 
        enhancedInputComponent->BindAction(playerActions.Action_Crouch, e, pController, &AVal_PlayerController::PlayerCrouch); 
        enhancedInputComponent->BindAction(playerActions.Action_Walk, e, pController, &AVal_PlayerController::PlayerWalk); 
    }


    enhancedInputComponent->BindAction(playerActions.Action_Use, ETriggerEvent::Started, pController, &AVal_PlayerController::PlayerUse);

    
    // enhancedInputComponent->BindAction(weaponActions.Action_Drop, ETriggerEvent::Started, this, &UVal_InputSystem::HandleDropInput);
    enhancedInputComponent->BindAction(weaponActions.Action_Equip_Melee, ETriggerEvent::Started, this, &UVal_InputSystem::HandleEquipInput);
    enhancedInputComponent->BindAction(weaponActions.Action_Equip_Secondary, ETriggerEvent::Started, this, &UVal_InputSystem::HandleEquipInput);
    enhancedInputComponent->BindAction(weaponActions.Action_Equip_Primary, ETriggerEvent::Started, this, &UVal_InputSystem::HandleEquipInput);
    // enhancedInputComponent->BindAction(weaponActions.Action_Reload, ETriggerEvent::Started, this, &AVal_PlayerController::TryReload);
}


FVector2D UVal_InputSystem::GetLastLookVector() const
{
    if (lastLookVector.Size() > 0) return lastLookVector;
    return FVector2D::ZeroVector;
}

void UVal_InputSystem::HandleLookInput(const FInputActionInstance& InputActionInstance)
{
    if (const ETriggerEvent actionTrigger = InputActionInstance.GetTriggerEvent(); actionTrigger == ETriggerEvent::Triggered)
    {
        const FVector2D LookAxisVector = InputActionInstance.GetValue().Get<FVector2D>();
        lastLookVector = LookAxisVector;
        pController->PlayerLook(LookAxisVector);
    }
    else if (actionTrigger == ETriggerEvent::Canceled || actionTrigger == ETriggerEvent::Completed)
        lastLookVector = FVector2d::ZeroVector;
        
}


void UVal_InputSystem::HandleMoveInput(const FInputActionInstance& InputActionInstance)
{
    const FString actionName = InputActionInstance.GetSourceAction()->GetName();
    
    if (const ETriggerEvent actionTrigger = InputActionInstance.GetTriggerEvent(); actionTrigger == ETriggerEvent::Triggered)
    {
        // cannot use switch/case cuz unreal is bullshit
             if (actionName == "VIA_Move_W") inputMap.W = true;
        else if (actionName == "VIA_Move_A") inputMap.A = true;
        else if (actionName == "VIA_Move_D") inputMap.D = true;
        else if (actionName == "VIA_Move_S") inputMap.S = true;

        pController->PlayerMove();
    }
    else if (actionTrigger == ETriggerEvent::Canceled || actionTrigger == ETriggerEvent::Completed)
    {
             if (actionName == "VIA_Move_W") inputMap.W = false;
        else if (actionName == "VIA_Move_A") inputMap.A = false;
        else if (actionName == "VIA_Move_D") inputMap.D = false;
        else if (actionName == "VIA_Move_S") inputMap.S = false;
    }
}

void UVal_InputSystem::HandleEquipInput(const FInputActionInstance& InputActionInstance)
{
    const FString actionName = InputActionInstance.GetSourceAction()->GetName();

    if (const ETriggerEvent actionTrigger = InputActionInstance.GetTriggerEvent(); actionTrigger == ETriggerEvent::Started)
    {
             if (actionName == "VIA_Weapon_Melee") pController->TryWeaponEquip(EWeaponType::Melee);
        else if (actionName == "VIA_Weapon_Secondary") pController->TryWeaponEquip(EWeaponType::Secondary);
        else if (actionName == "VIA_Weapon_Primary") pController->TryWeaponEquip(EWeaponType::Primary);
        
    }

}




