// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputAction.h"
#include "Val_InputComponent.generated.h"



class AVal_PlayerController;
class UEnhancedInputComponent;
class UInputMappingContext;


USTRUCT(BlueprintType)
struct FPlayerInputActions
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input|Input Actions|Default Player Actions")
    TObjectPtr<UInputAction> Action_Move_W = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input|Input Actions|Default Player Actions")
    TObjectPtr<UInputAction> Action_Move_A = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input|Input Actions|Default Player Actions")
    TObjectPtr<UInputAction> Action_Move_S = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input|Input Actions|Default Player Actions")
    TObjectPtr<UInputAction> Action_Move_D = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input|Input Actions|Default Player Actions")
    TObjectPtr<UInputAction> Action_Look = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input|Input Actions|Default Player Actions")
    TObjectPtr<UInputAction> Action_Jump = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input|Input Actions|Default Player Actions")
    TObjectPtr<UInputAction> Action_Crouch = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input|Input Actions|Default Player Actions")
    TObjectPtr<UInputAction> Action_Walk = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input|Input Actions|Default Player Actions")
    TObjectPtr<UInputAction> Action_Use = nullptr;
};

USTRUCT(BlueprintType)
struct FWeaponInputActions
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input|Input Actions|Weapon Actions")
    TObjectPtr<UInputAction> Action_AltAttack = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input|Input Actions|Weapon Actions")
    TObjectPtr<UInputAction> Action_Attack = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input|Input Actions|Weapon Actions")
    TObjectPtr<UInputAction> Action_Drop = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input|Input Actions|Weapon Actions")
    TObjectPtr<UInputAction> Action_Equip_Melee = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input|Input Actions|Weapon Actions")
    TObjectPtr<UInputAction> Action_Equip_Secondary = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input|Input Actions|Weapon Actions")
    TObjectPtr<UInputAction> Action_Equip_Primary = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input|Input Actions|Weapon Actions")
    TObjectPtr<UInputAction> Action_Reload = nullptr;
};

USTRUCT(BlueprintType)
struct FAbilityInputActions
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input|Input |Ability Actions")
    TObjectPtr<UInputAction> Action_Ability1 = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input|Input Actions|Ability Actions")
    TObjectPtr<UInputAction> Action_Ability2 = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input|Input Actions|Ability Actions")
    TObjectPtr<UInputAction> Action_Ability3 = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input|Input Actions|Ability Actions")
    TObjectPtr<UInputAction> Action_Ability4 = nullptr;
};

USTRUCT(BlueprintType)
struct FMappingContexts
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input|Mapping Context")
    TObjectPtr<UInputMappingContext> Default_Mapping = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input|Mapping Context")
    TObjectPtr<UInputMappingContext> Weapon_Mapping = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input|Mapping Context")
    TObjectPtr<UInputMappingContext> Ability_Mapping = nullptr;
    
};


UCLASS(ClassGroup=(Input), meta=(BlueprintSpawnableComponent))
class VALORANTMECHANICS_API UVal_InputComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UVal_InputComponent();
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#pragma region DEINFE INPUT ACTIONS

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
    FPlayerInputActions playerActions;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
    FWeaponInputActions weaponActions;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
    FAbilityInputActions abilityActions;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
    FMappingContexts mappingContexts;

    
#pragma endregion


    void SetMappingContexts(AVal_PlayerController* classObject, TObjectPtr<UInputComponent> inputComponent);
    void SetInputActions(AVal_PlayerController* classObject);
    
    
    
protected:
    virtual void BeginPlay() override;
	UPROPERTY() TObjectPtr<UEnhancedInputComponent> enhancedInputComponent = nullptr;
	
    
};


// #undef DECLARE_INPUT_ACTION;
// #undef DECLARE_MAPPING_CONTEXT;