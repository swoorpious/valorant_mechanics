// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "Val_PlayerAnimInstance.h"
#include "AnimNotifier.h"
#include "ValorantMechanics/Weapons/CommonWeapon.h"
#include "ValorantMechanics/Core/Val_LocalPlayerSubsystem.h"
#include "ValorantMechanics/Player/Controller/Val_PlayerController.h"
#include "ValorantMechanics/ValorantMechanics.h"
#include "ValorantMechanics/Player/Val_Character.h"


void UVal_PlayerAnimInstance::NativeBeginPlay()
{
    Super::NativeBeginPlay();

    // notifier = CreateDefaultSubobject<UAnimNotifier>(TEXT("Animation Notifier"));

    if (AVal_Character* e = Cast<AVal_Character>(TryGetPawnOwner()))
    {
        // e->onWeaponSpawn.AddUObject(this, &UVal_PlayerAnimInstance::UpdateAnimDataAsset);
        // e->onWeaponEquip.AddUObject(this, &UVal_PlayerAnimInstance::UpdateCurrentWeapon);
        // e->onWeaponDrop.AddUObject(this, &UVal_PlayerAnimInstance::RemoveAnimDataAsset);
		UVal_LocalPlayerSubsystem* j = e->GetValPlayerController()->GetLocalPlayer()->GetSubsystem<UVal_LocalPlayerSubsystem>();
        j->mStateChanged.AddUObject(this, &UVal_PlayerAnimInstance::UpdateMovementState);
        j->wLogicStateChanged.AddUObject(this, &UVal_PlayerAnimInstance::UpdateWeaponLogicState);

        // notifier->Init(j);
    }

}

void UVal_PlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (const FName currentState = GetCurrentStateNameFromStateMachine(wStateMachineName); currentState != lastPlayerStateMachineStateName)
    {
        notifier->NotifyWeaponStateMachineStateChange(lastPlayerStateMachineStateName, currentState);
        lastPlayerStateMachineStateName = currentState;
    }
}


FName UVal_PlayerAnimInstance::GetCurrentStateNameFromStateMachine(FName stateMachineName)
{
    const int8 machineIndex = GetStateMachineIndex(stateMachineName);
    return GetCurrentStateName(machineIndex);
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
    animAssets.currentWeaponType = weaponType;
    animAssets.currentAnimDataAsset = GetAnimDataAsset(weaponType);
}


void UVal_PlayerAnimInstance::RemoveAnimDataAsset(EWeaponType weaponType)
{
    if (weaponType == EWeaponType::Empty || !HasAnimDataForType(weaponType)) return;
    animAssets.animDataMap.Remove(weaponType);
}


const TObjectPtr<UWeaponAnimDataAsset>& UVal_PlayerAnimInstance::GetAnimDataAsset(EWeaponType weaponType)
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
    if (!HasAnimDataForType(animAssets.currentWeaponType)) return nullptr;
    
    return animAssets.animDataMap[animAssets.currentWeaponType];
}


#pragma endregion ANIM DATA
