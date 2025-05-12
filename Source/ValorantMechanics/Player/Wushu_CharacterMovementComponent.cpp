// Fill out your copyright notice in the Description page of Project Settings.


#include "Wushu_CharacterMovementComponent.h"
#include "Wushu_Character.h"


// Sets default values for this component's properties
UWushu_CharacterMovementComponent::UWushu_CharacterMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	PlayerCharacter = Cast<AWushu_Character>(GetOwner());

	// ...
}


// Called when the game starts
void UWushu_CharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

void UWushu_CharacterMovementComponent::HandleAirMovement(float DeltaTime)
{

	if (!IsFalling())
	{
		AirStrafeTime = 0.0f;
		CurrentAirStrafeMultiplier = 1.0f;
		LastAirMovementDirection = FAirMovementDirection::None;
		return;
	}

	FVector ViewVectorX = PlayerCharacter->GetActorForwardVector();
	FVector ViewVectorY = PlayerCharacter->GetActorRightVector();
	float InputY = PlayerCharacter->GetInputAxisValue("MoveForward");
	float InputX = PlayerCharacter->GetInputAxisValue("MoveRight");
	
	bool IsMovingLeft = InputX < -0.1f;
	bool IsMovingRight = InputX > 0.1f;
	bool IsMovingForward = InputY > 0.1f;
	bool IsMovingBackward = InputY < -0.1f;

	if (IsMovingLeft && ViewYawDelta < -0.1f)
		CurrentAirMovementDirection = FAirMovementDirection::Matching;
	else if (IsMovingRight && ViewYawDelta > 0.1f)
		CurrentAirMovementDirection = FAirMovementDirection::Matching;
	else if ((IsMovingLeft && ViewYawDelta > 0.1f) || (IsMovingRight && ViewYawDelta < -0.1f))
		CurrentAirMovementDirection = FAirMovementDirection::Opposing;
	else
		CurrentAirMovementDirection = FAirMovementDirection::Neutral;
}


// Called every frame
void UWushu_CharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);



	
}

