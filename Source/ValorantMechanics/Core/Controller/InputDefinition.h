// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputDefinition.generated.h"



class UInputMappingContext;


/*
 * every action related to basic player movement
 */
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


/*
 * every action related to weapon actions
 */
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


/*
 * every action related to abilities for the player
 */
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



/*
 * input mapping contexts
 * player inputs -> Default_Mapping
 * weapon inputs -> Weapon_Mapping
 * player ability inputs -> Ability_Mapping
 */
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


/*
 * 
 */
struct InputMap
{
    bool W = false; 
    bool A = false; 
    bool S = false; 
    bool D = false; 
};