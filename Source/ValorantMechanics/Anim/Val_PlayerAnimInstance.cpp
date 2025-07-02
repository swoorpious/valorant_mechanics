// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "Val_PlayerAnimInstance.h"
#include "AnimNotifier.h"
#include "ValorantMechanics/Weapons/CommonWeapon.h"
#include "ValorantMechanics/ValorantMechanics.h"
#include "ValorantMechanics/Player/Val_Character.h"


void UVal_PlayerAnimInstance::NativeBeginPlay()
{
    Super::NativeBeginPlay();

    // notifier = CreateDefaultSubobject<UAnimNotifier>(TEXT("Animation Notifier"));
    // notifier->onStateChange.AddUObject(this, &UUVal_PlayerAnimInstance::UpdateWeaponState);

    // if (AVal_Character* e = Cast<AVal_Character>(TryGetPawnOwner()))
    // {
    //     e->onWeaponSpawn.AddUObject(this, &UVal_AnimInstance::UpdateAnimDataAsset);
    //     e->onWeaponEquip.AddUObject(this, &UVal_AnimInstance::UpdateCurrentWeapon);
    //     e->onWeaponDrop.AddUObject(this, &UVal_AnimInstance::RemoveAnimDataAsset);
    // }
}


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
}


TObjectPtr<UWeaponAnimDataAsset> UVal_PlayerAnimInstance::GetAnimDataAsset(EWeaponType weaponType)
{
	if (weaponType == EWeaponType::Empty || !HasAnimDataForType(weaponType)) return nullptr;
    return animAssets.animDataMap[weaponType];
}


void UVal_PlayerAnimInstance::RemoveAnimDataAsset(EWeaponType weaponType)
{
	if (weaponType == EWeaponType::Empty || !HasAnimDataForType(weaponType)) return;
    animAssets.animDataMap.Remove(weaponType);
}

UWeaponAnimDataAsset* UVal_PlayerAnimInstance::GetCurrentAnimDataAsset()
{
    if (!HasAnimDataForType(animAssets.currentWeaponType)) return nullptr;
    
    return animAssets.animDataMap[animAssets.currentWeaponType];
}
