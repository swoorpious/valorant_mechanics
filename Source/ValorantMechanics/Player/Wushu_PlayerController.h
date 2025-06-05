// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "Wushu_PlayerController.generated.h"

class UEnhancedInputComponent;
class UInputMappingContext;
class AWushu_Character;
class UPawnMovementComponent;
class UCharacterMovementComponent;


/**
 * wushu
 */
UCLASS()
class VALORANTMECHANICS_API AWushu_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	FVector2d GetLastLookActionVector() const { return lastLookVector; }
	bool HasMovementInput() const { return isAction_Move_W ^ isAction_Move_S || isAction_Move_A ^ isAction_Move_D; }


#pragma region MovementAdditives
	FVector2d GetAdditiveMovementInput() const	{ return FVector2d(CalculateMovementAdditiveX(), CalculateMovementAdditiveY());	}

	int8 CalculateMovementAdditiveX() const
	{
		const double MovementInX = isAction_Move_A * -1 + isAction_Move_D * 1;
		return canBunny_Hop_A || canBunny_Hop_D ? 0 : MovementInX;
	}

	int8 CalculateMovementAdditiveY() const
	{
		const double MovementInY = isAction_Move_W * 1 + isAction_Move_S * -1;
		return MovementInY;
	}
#pragma endregion
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="General")
	float Sensitivity = 1.0f;

	
#pragma region InputActions
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	// TObjectPtr<UInputAction> Action_Move = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> Action_Move_W = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> Action_Move_A = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> Action_Move_S = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> Action_Move_D = nullptr;
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> Action_Look = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> Action_Jump = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> Action_Crouch = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> Action_Walk = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> Action_Use = nullptr;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputMappingContext> InputMappingContext = nullptr;
#pragma endregion
	
protected:
	void AddLookInput(FVector2D Look) const;
	void PlayerLook(const FInputActionInstance& InputActionInstance);
	void PlayerMove(const FInputActionInstance& InputActionInstance);
	void PlayerJump(const FInputActionInstance& InputActionInstance);
	void PlayerCrouch(const FInputActionInstance& InputActionInstance);
	void PlayerWalk(const FInputActionInstance& InputActionInstance);
	void PlayerUse(const FInputActionInstance& InputActionInstance);
	virtual void Tick(float DeltaSeconds) override;
	

	// FVector2D InputReductionScaling(const FVector2D& CurrentMove);
	
	
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

private:
	bool HasPlayerMovementInX() const { return isAction_Move_A ^ isAction_Move_D; }
	bool HasPlayerMovementInY() const { return isAction_Move_W ^ isAction_Move_S; }

	
	/*
	 * TODO: make additive for separate axes
	 * i don't remember why
	 */
	
	UPROPERTY() TObjectPtr<AWushu_Character> PlayerCharacter = nullptr;
	UPROPERTY() TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = nullptr;
	UPROPERTY() TObjectPtr<UCharacterMovementComponent> PlayerCharacterMovementComponent = nullptr;


	FVector2d lastLookVector;
	
	
	// is movement key pressed
    bool isAction_Move_W = false;
    bool isAction_Move_A = false;
    bool isAction_Move_S = false;
    bool isAction_Move_D = false;

    // bunny hop
    bool canBunny_Hop_A = false;
    bool canBunny_Hop_D = false;
	

	FVector2D currentVelocity = FVector2D::ZeroVector;
	float AccelerationRate = 10.0f;
	float MaxSpeed = 1.0f;

};

