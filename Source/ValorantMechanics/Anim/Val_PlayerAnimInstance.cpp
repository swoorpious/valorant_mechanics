// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "Val_PlayerAnimInstance.h"

#include "ValorantMechanics/Weapons/CommonWeapon.h"
#include "ValorantMechanics/Core/Val_LocalPlayerSubsystem.h"
#include "ValorantMechanics/Player/Controller/Val_PlayerController.h"
#include "ValorantMechanics/Player/PlayerComponents/Val_CharacterMovementComponent.h"
#include "ValorantMechanics/ValorantMechanics.h"
#include "ValorantMechanics/Player/Val_Character.h"


void UVal_PlayerAnimInstance::NativeBeginPlay()
{
    Super::NativeBeginPlay();


    if (AVal_Character* e = Cast<AVal_Character>(TryGetPawnOwner()))
    {
		UVal_LocalPlayerSubsystem* j = e->GetValPlayerController()->GetLocalPlayer()->GetSubsystem<UVal_LocalPlayerSubsystem>();
        j->GetMovementStateChangeDelegate().AddUObject(this, &UVal_PlayerAnimInstance::UpdateMovementState);
        j->GetWeaponAnimStateChangeRequestDelegate().AddUObject(this, &UVal_PlayerAnimInstance::UpdateWeaponAnimState);
        
        valInput = e->GetValPlayerController()->GetInputSystem();
        pMovement = e->GetValMovementComponent();
    }

}

void UVal_PlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (valInput)
        lastLookVector = valInput->GetLastLookVector();

    if (pMovement)
        pVelocity = pMovement->Velocity;
}


void UVal_PlayerAnimInstance::UpdateWeaponAnimState(EWeaponType weaponType, EWeaponAnimState oldState, EWeaponAnimState newState)
{
    if (weaponType == EWeaponType::Empty) return;
    if (weaponType == EWeaponType::Melee) wAnimStates.melee = newState;
    if (weaponType == EWeaponType::Secondary) wAnimStates.secondary = newState;
    if (weaponType == EWeaponType::Primary) wAnimStates.primary = newState;

    
    if (newState == EWeaponAnimState::Equip_Default || newState == EWeaponAnimState::Equip_Fast)
        wAnimStates.currentWeaponType = weaponType;
   
}


#pragma region ANIM DATA
void UVal_PlayerAnimInstance::UpdateAnimDataAsset(ACommonWeapon* equippedWeapon)
{
    TObjectPtr<UWeaponAnimDataAsset> const animDataAsset = equippedWeapon->GetAnimAsset();
    EWeaponType const weaponType = equippedWeapon->GetWeaponType();
    HasAnimDataForType(weaponType) ?
        animAssets.animDataMap[weaponType] = animDataAsset :
        animAssets.animDataMap.Add(weaponType, animDataAsset);

}


void UVal_PlayerAnimInstance::UpdateCurrentWeapon(EWeaponType weaponType)
{
    if (!HasAnimDataForType(weaponType)) return;
    wAnimStates.currentWeaponType = weaponType;
    animAssets.currentAnimDataAsset = GetAnimDataAsset(weaponType);
}


void UVal_PlayerAnimInstance::RemoveAnimDataAsset(EWeaponType weaponType)
{
    if (weaponType == EWeaponType::Empty || !HasAnimDataForType(weaponType)) return;
    animAssets.animDataMap.Remove(weaponType);
}


const TObjectPtr<UWeaponAnimDataAsset> UVal_PlayerAnimInstance::GetAnimDataAsset(EWeaponType weaponType)
{
	if (weaponType == EWeaponType::Empty || !HasAnimDataForType(weaponType)) return nullptr;
    return animAssets.animDataMap[weaponType];
}

bool UVal_PlayerAnimInstance::HasAnimDataForType(EWeaponType weaponType) const
{
    return animAssets.animDataMap.FindRef(weaponType) != nullptr;
}


UWeaponAnimDataAsset* UVal_PlayerAnimInstance::GetCurrentAnimDataAsset()
{
    if (!HasAnimDataForType(wAnimStates.currentWeaponType)) return nullptr;
    
    return animAssets.animDataMap[wAnimStates.currentWeaponType];
}

bool UVal_PlayerAnimInstance::CanTransitionToMovementAnimState(EMovementState state) const
{
    return mState == state;
}

bool UVal_PlayerAnimInstance::CanTransitionToWeaponAnimState(EWeaponAnimState state) const
{
    const EWeaponType e = wAnimStates.currentWeaponType;

    if (e == EWeaponType::Melee) return wAnimStates.melee == state;
    if (e == EWeaponType::Secondary) return wAnimStates.secondary == state;
    if (e == EWeaponType::Primary) return wAnimStates.primary == state;


    return false;
}


#pragma endregion ANIM DATA
