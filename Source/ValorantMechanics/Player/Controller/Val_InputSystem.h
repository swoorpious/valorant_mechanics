// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputDefinition.h"

#include "Val_InputSystem.generated.h"


class AVal_PlayerController;
class UEnhancedInputComponent;




UCLASS(BlueprintType, Blueprintable, ClassGroup=(Input), meta=(BlueprintSpawnableComponent))
class VALORANTMECHANICS_API UVal_InputSystem : public UObject
{
    GENERATED_BODY()

public:
    void Init(AVal_PlayerController* classObject, const TObjectPtr<UInputComponent>& inputComponent);
    void SetMappingContexts(const TObjectPtr<UInputComponent>& inputComponent);
    void SetInputActions();
    
    
#pragma region INPUT ACTIONS

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
    FPlayerInputActions playerActions;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
    FWeaponInputActions weaponActions;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
    FAbilityInputActions abilityActions;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
    FMappingContexts mappingContexts;

    
#pragma endregion INPUT ACTIONS
   

    // returns look vector (mouse movement) in previous tick
    virtual FVector2D GetLastLookVector() const;

    // returns true if there is any movement that is not cancelled by the opposite direction
    bool HasMovementInput() const { return inputMap.W ^ inputMap.S || inputMap.A ^ inputMap.D; }
    
    // adds input in both axes separately
    // ActionA and ActionD cancel out each other, similarly ActionW and ActionS
    FVector2d GetAdditiveMovementInput() const
    {
        const double x = inputMap.A * -1 + inputMap.D * 1;
        const double y = inputMap.W * 1 + inputMap.S * -1;
        return FVector2d(x, y);
    }
    

    virtual void HandleLookInput(const FInputActionInstance& InputActionInstance);
    virtual void HandleMoveInput(const FInputActionInstance& InputActionInstance);
    virtual void HandleEquipInput(const FInputActionInstance& InputActionInstance);
    // virtual void HandleDropInput(const FInputActionInstance& InputActionInstance);
    
    
protected:
    UPROPERTY() TObjectPtr<UEnhancedInputComponent> enhancedInputComponent = nullptr;
    UPROPERTY() AVal_PlayerController* pController = nullptr;

    FVector2D lastLookVector;
    InputMap inputMap;


    friend AVal_PlayerController;
};


// #undef DECLARE_INPUT_ACTION;
// #undef DECLARE_MAPPING_CONTEXT;