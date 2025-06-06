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

	
// #pragma region InputActions
// 	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
// 	// TObjectPtr<UInputAction> Action_Move = nullptr;
// 	
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
// 	TObjectPtr<UInputAction> Action_Move_W = nullptr;
// 	
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
// 	TObjectPtr<UInputAction> Action_Move_A = nullptr;
// 	
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
// 	TObjectPtr<UInputAction> Action_Move_S = nullptr;
// 	
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
// 	TObjectPtr<UInputAction> Action_Move_D = nullptr;
// 	
// 	
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
// 	TObjectPtr<UInputAction> Action_Look = nullptr;
//
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
// 	TObjectPtr<UInputAction> Action_Jump = nullptr;
//
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
// 	TObjectPtr<UInputAction> Action_Crouch = nullptr;
//
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
// 	TObjectPtr<UInputAction> Action_Walk = nullptr;
//
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
// 	TObjectPtr<UInputAction> Action_Use = nullptr;
//
//
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
// 	TObjectPtr<UInputMappingContext> InputMappingContext = nullptr;
// #pragma endregion


	virtual void PlayerLook(const FInputActionInstance& InputActionInstance);
	virtual void PlayerMove(const FInputActionInstance& InputActionInstance);
	virtual void PlayerJump(const FInputActionInstance& InputActionInstance);
	virtual void PlayerCrouch(const FInputActionInstance& InputActionInstance);
	virtual void PlayerWalk(const FInputActionInstance& InputActionInstance);
	virtual void PlayerUse(const FInputActionInstance& InputActionInstance);
	virtual void Tick(float DeltaSeconds) override;





	
protected:
	void AddLookInput(FVector2D Look) const;

	bool HasPlayerMovementInX() const { return isAction_Move_A ^ isAction_Move_D; }
	bool HasPlayerMovementInY() const { return isAction_Move_W ^ isAction_Move_S; }

	
	UPROPERTY() TObjectPtr<AVal_Character> playerCharacter = nullptr;
	UPROPERTY() TObjectPtr<UVal_InputComponent> valInputComponent = nullptr;
	// UPROPERTY() TObjectPtr<UCharacterMovementComponent> PlayerCharacterMovementComponent = nullptr;


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

	

	// FVector2D InputReductionScaling(const FVector2D& CurrentMove);
	
	
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

private:


};

