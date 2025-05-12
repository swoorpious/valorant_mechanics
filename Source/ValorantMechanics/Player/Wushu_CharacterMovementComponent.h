// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Wushu_CharacterMovementComponent.generated.h"


// TODO 


class AWushu_Character;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VALORANTMECHANICS_API UWushu_CharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UWushu_CharacterMovementComponent();

protected:
	virtual void BeginPlay() override;

	void HandleAirMovement(float DeltaTime);

public:
	TObjectPtr<AWushu_Character> PlayerCharacter = nullptr;
	

	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	enum class FAirMovementDirection
	{
		None,
		Matching,
		Opposing,
		Neutral
	};

	float AirStrafeTime = 0.0f;
	float CurrentAirStrafeMultiplier = 1.0f;
	float PreviousYaw = 0.0f;
	FAirMovementDirection LastAirMovementDirection;
};
