// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Wushu_CharacterMovementComponent.generated.h"


/*
 * Things to do in Wushu_CharacterMovementComponent, Wushu_Character, and Wushu_PlayerController
 * TODO move all movement code from PlayerController to MovementComponent
 * TODO add movement smoothing
 * TODO redo jump/jump slowdown (stun) logic
 * TODO implement crouching
 * TODO refine air movement parameters/behavior
 * TODO refactor code and optimize logic
 */


class AWushu_PlayerController;
class AWushu_Character;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VALORANTMECHANICS_API UWushu_CharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UWushu_CharacterMovementComponent();
	virtual bool DoJump(bool bReplayingMoves, float DeltaTime) override;

	void SetMoveVector(FVector2d InMoveVector) { this->MoveVector = InMoveVector; }

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;
	
	void HandleAirMovement(float DeltaTime);


private:

#pragma region AirControl
	UPROPERTY(EditAnywhere, Category = "Air Movement", meta = (ClampMin = "0.0", ClampMax = "100.0")) // 10
	float _MaxAirControl = 0.7f;
    
	UPROPERTY(EditAnywhere, Category = "Air Movement", meta = (ClampMin = "0.0", ClampMax = "100.0")) // 10
	float _MidAirControl = 0.3f;
    
	UPROPERTY(EditAnywhere, Category = "Air Movement", meta = (ClampMin = "0.0", ClampMax = "100.0")) // 100
	float _MinAirControl = 0.1f;
    
	UPROPERTY(EditAnywhere, Category = "Air Movement", meta = (ClampMn = "1.0", ClampMax = "50.0")) // 5
	float _MaxAirStrafeMultiplier = 1.5f;
    
	UPROPERTY(EditAnywhere, Category = "Air Movement", meta = (ClampMin = "0.0", ClampMax = "50.0")) // 5
	float _AirStrafeAccelerationRate = 0.5f;
    
	UPROPERTY(EditAnywhere, Category = "Air Movement", meta = (ClampMin = "0.0", ClampMax = "100.0")) // 10
	float _AirStrafeDecayRate = 2.0f;
    
	UPROPERTY(EditAnywhere, Category = "Air Movement", meta = (ClampMin = "0.0", ClampMax = "10000.0"))
	float _AirStrafeForce = 250.0f;

	UPROPERTY(EditAnywhere, Category = "Air Movement", meta = (ClampMin = "0.0", ClampMax = "10.0"))
	float _ForceFactorX = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Air Movement", meta = (ClampMin = "0.0", ClampMax = "10.0"))
	float _ForceFactorY = 1.0f;
#pragma endregion


	UPROPERTY()
	TObjectPtr<AWushu_Character> PlayerCharacter = nullptr;
	UPROPERTY()
	TObjectPtr<AWushu_PlayerController> PlayerController = nullptr;

	enum class FAirMovementDirection
	{
		None,
		Matching,
		Opposing,
		Neutral
	};

	
	float airStrafeTime = 0.0f;
	float currentAirStrafeMultiplier = 1.0f;
	FAirMovementDirection lastAirMovementDirection;
	FVector2d MoveVector;
};
