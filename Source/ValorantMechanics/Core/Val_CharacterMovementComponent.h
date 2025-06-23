// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
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


UENUM(BlueprintType)
enum class EAirMovementInputDirection : uint8
{
	None			UMETA(Description = "Default for when on ground, or when not falling."),
	Matching		UMETA(Description = "Mouse movement with matching strafe key, e.g. mouse moves right while holding D on the keyboard."),
	Opposing		UMETA(Description = "Mouse movement with opposing strafe key, e.g. mouse moves right while holding A on the keyboard."),
	Neutral			UMETA(Description = "No mouse movement while strafe key is held, or vice versa.")
};

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	// ability specific movements are not defined here
	Still,
	Walking,
	Running,
	Crouching,
	Falling,
	MovementStunned
};


USTRUCT(BlueprintType)
struct FPlayerMovementProperties
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Character Properties|Movement")
    float runSpeed = 750.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Character Properties|Movement")
    float walkSpeed = 400.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Character Properties|Movement")
    float regularAcceleration = 3072.0f;

    // jump
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Character Properties|Movement")
    float bunnyHopTimeThreshold = 0.08f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Character Properties|Movement")
    float walkSpeedAfterJump = 300.0f; // could be 150.0f
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Character Properties|Movement")
    float jumpStunDuration = 0.2f;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Character Properties|Movement")
    float movementAccelerationWhileJumping = 1024.0f;

	// jump uses last ground velocity
    // UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Character Properties|Movement", meta = (META))
    // float walkSpeedWhileJumping = 0.0f;
	
};


USTRUCT(BlueprintType)
struct FPlayerAirMovementProperties
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Character Properties|Air Movement", meta = (ClampMin = "0.0", ClampMax = "100.0")) // 10
	float maxAirControl = 0.7f;
    
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Character Properties|Air Movement", meta = (ClampMin = "0.0", ClampMax = "100.0")) // 10
	float midAirControl = 0.3f;
    
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Character Properties|Air Movement", meta = (ClampMin = "0.0", ClampMax = "100.0")) // 100
	float minAirControl = 0.1f;
    
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Character Properties|Air Movement", meta = (ClampMn = "1.0", ClampMax = "50.0")) // 5
	float maxAirStrafeMultiplier = 1.5f;
    
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Character Properties|Air Movement", meta = (ClampMin = "0.0", ClampMax = "50.0")) // 5
	float airStrafeAccelerationRate = 0.5f;
    
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Character Properties|Air Movement", meta = (ClampMin = "0.0", ClampMax = "100.0")) // 10
	float airStrafeDecayRate = 2.0f;
    
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Character Properties|Air Movement", meta = (ClampMin = "0.0", ClampMax = "10000.0"))
	float airStrafeForce = 250.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Character Properties|Air Movement", meta = (ClampMin = "0.0", ClampMax = "10.0"))
	float forceFactorX = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Character Properties|Air Movement", meta = (ClampMin = "0.0", ClampMax = "10.0"))
	float forceFactorY = 1.0f;
};



UCLASS(ClassGroup=(Valorant), meta=(BlueprintSpawnableComponent))
class VALORANTMECHANICS_API UVal_CharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UVal_CharacterMovementComponent();
	virtual bool DoJump(bool bReplayingMoves, float DeltaTime) override;
	// virtual void OnLanded(/* add hit target @params in future */); 


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
	
	UPROPERTY()	TObjectPtr<AVal_Character> playerCharacter = nullptr;
	UPROPERTY()	TObjectPtr<UVal_InputSystem> valInputSystem = nullptr;
	
	
	float airStrafeTime = 0.0f;
	float currentAirStrafeMultiplier = 1.0f;
	
	EAirMovementInputDirection lastAirInputDirection = EAirMovementInputDirection::None;
	FVector2d moveVector = FVector2d::ZeroVector;


	
};
