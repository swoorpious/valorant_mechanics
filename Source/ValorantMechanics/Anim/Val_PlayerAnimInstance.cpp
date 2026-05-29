// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "Val_PlayerAnimInstance.h"

#include "ValorantMechanics/Weapon/CommonWeapon.h"
#include "ValorantMechanics/Core/Val_LocalPlayerSubsystem.h"
#include "ValorantMechanics/Player/Controller/Val_PlayerController.h"
#include "ValorantMechanics/Player/PlayerComponents/Val_CharacterMovementComponent.h"
#include "ValorantMechanics/Player/Val_Character.h"


void UVal_PlayerAnimInstance::NativeBeginPlay()
{
    Super::NativeBeginPlay();
	
    if (AVal_Character* e = Cast<AVal_Character>(TryGetPawnOwner()))
    {
    	e->getOnWeaponChangedDelegate().AddUObject(this, &UVal_PlayerAnimInstance::_updateWeaponAnimAsset);
    	e->getOnWeaponStateChangedDelegate().AddUObject(this, &UVal_PlayerAnimInstance::_updateWeaponStateChange);

        valInput = e->GetValPlayerController()->GetInputSystem();
        pMovement = e->GetValMovementComponent();
    }

}

void UVal_PlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (valInput)
        _lastLookVector = valInput->GetLastLookVector();

    if (pMovement)
        _playerVelocity = pMovement->Velocity;
}

void UVal_PlayerAnimInstance::NativeUninitializeAnimation()
{
	Super::NativeUninitializeAnimation();
	
	if (AVal_Character* e = Cast<AVal_Character>(TryGetPawnOwner()))
	{
		e->getOnWeaponChangedDelegate().RemoveAll(this);
		e->getOnWeaponStateChangedDelegate().RemoveAll(this);

	}
	
	valInput = nullptr;
	pMovement = nullptr;
}


bool UVal_PlayerAnimInstance::_canTransitionToMovementAnimState(EMovementState state) const
{
    return _movementState == state;
}

bool UVal_PlayerAnimInstance::_canTransitionToWeaponAnimState(EWeaponState state) const
{
    return _weaponState == state;
}

void UVal_PlayerAnimInstance::_updateWeaponAnimAsset(UVal_WeaponAnimConfig* newAnimConfig)
{
	_currAnimAsset = newAnimConfig;
}

void UVal_PlayerAnimInstance::_updateWeaponStateChange(const EWeaponState newState)
{
	_weaponState = newState;
}


