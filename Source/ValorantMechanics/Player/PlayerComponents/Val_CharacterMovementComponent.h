// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#pragma once

#include "CoreMinimal.h"
#include "PlayerMovementProperties.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ValorantMechanics/Core/Shared/StateController/StateManager.h"
#include "PlayerMovementProperties.h"
#include "ValorantMechanics/Core/Shared/StateController/PlayerStates.h"
#include "Val_CharacterMovementComponent.generated.h"


/*
 * Things to do in Wushu_CharacterMovementComponent, Wushu_Character, and Wushu_PlayerController
 * TODO move all movement code from PlayerController to MovementComponent
 * TODO add movement smoothing
 * TODO redo jump/jump slowdown (stun) logic
 * TODO implement crouching
 * TODO refine air movement parameters/behavior
 * TODO refactor code and optimize logic
 */


class AVal_Character;
class UVal_InputSystem;
class UVal_LocalPlayerSubsystem;



UCLASS(ClassGroup=(Valorant), meta=(BlueprintSpawnableComponent))
class VALORANTMECHANICS_API UVal_CharacterMovementComponent : public UCharacterMovementComponent, public CommonStateManager<EMovementState>
{
	GENERATED_BODY()

public:
	UVal_CharacterMovementComponent();
	virtual bool DoJump(bool bReplayingMoves, float DeltaTime) override;
	// virtual void OnLanded(/* add hit target @params in future */);
	

	virtual void TryUpdateState(EMovementState) override;
	virtual EMovementState GetCurrentState() const override;
	virtual bool IsCurrentState(EMovementState state) const override;
	virtual bool CanTransitionToState(EMovementState state) const override;

	virtual void OnUpdateState(EMovementState previousState, EMovementState enteredState) override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Character Properties")
	FPlayerMovementProperties movementProperties;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Character Properties")
	FPlayerAirMovementProperties airMovementProperties;
	

	FORCEINLINE void SetMoveVector(FVector2d const inMoveVector) { this->moveVector = inMoveVector; }
	
protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;
	
	void HandleAirMovement(float DeltaTime);


private:
	
	UPROPERTY()	TObjectPtr<AVal_Character> pCharacter = nullptr;
	UPROPERTY() TObjectPtr<UVal_LocalPlayerSubsystem> pSubsystem = nullptr;
	UPROPERTY()	TObjectPtr<UVal_InputSystem> valInputSystem = nullptr;
	
	EMovementState pState = EMovementState::Idle;
	float airStrafeTime = 0.0f;
	float currentAirStrafeMultiplier = 1.0f;
	
	EAirMovementInputDirection lastAirInputDirection = EAirMovementInputDirection::None;
	FVector2d moveVector = FVector2d::ZeroVector;


	
};
