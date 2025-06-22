// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "Val_PlayerController.generated.h"

class UVal_InputComponent;
class UEnhancedInputComponent;
class UInputMappingContext;
class AVal_Character;
class UPawnMovementComponent;


/**
 * wushu
 */
UCLASS()
class VALORANTMECHANICS_API AVal_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;
		

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="General")
	float Sensitivity = 1.0f;


	void PlayerJump(const FInputActionInstance& InputActionInstance);
	void PlayerCrouch(const FInputActionInstance& InputActionInstance);
	void PlayerWalk(const FInputActionInstance& InputActionInstance);
	void PlayerUse(const FInputActionInstance& InputActionInstance);

	void PlayerEquipMelee();

	// functions called from Val_InputComponent.h
	void PlayerMove() const;
	FORCEINLINE void PlayerLook(const FVector2D lookVector) const { AddLookInput(lookVector * Sensitivity); }

	
protected:
	void AddLookInput(FVector2D Look) const;

	
	UPROPERTY() TObjectPtr<AVal_Character> playerCharacter = nullptr;
	UPROPERTY() TObjectPtr<UVal_InputComponent> valInputComponent = nullptr;
	
	
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

};

