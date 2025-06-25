// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "Val_AnimInstance.h"
#include "AnimNotifier.h"
#include "ValorantMechanics/Weapons/CommonWeapon.h"
#include "ValorantMechanics/Core/Shared/WeaponData/WeaponAnimDataAsset.h"
#include "ValorantMechanics/ValorantMechanics.h"
#include "ValorantMechanics/Core/Val_Character.h"


void UVal_AnimInstance::NativeBeginPlay()
{
    Super::NativeBeginPlay();

    notifier = CreateDefaultSubobject<UAnimNotifier>(TEXT("Animation Notifier"));

    // if (AVal_Character* e = Cast<AVal_Character>(TryGetPawnOwner()))
    // {
    //     e->onWeaponSpawn.AddUObject(this, &UVal_AnimInstance::UpdateAnimDataAsset);
    //     e->onWeaponEquip.AddUObject(this, &UVal_AnimInstance::UpdateCurrentWeapon);
    //     e->onWeaponDrop.AddUObject(this, &UVal_AnimInstance::RemoveAnimDataAsset);
    // }
}


void UVal_AnimInstance::UpdateAnimDataAsset(ACommonWeapon* equippedWeapon)
{
    TObjectPtr<UWeaponAnimDataAsset> const animDataAsset = equippedWeapon->GetAnimAsset();
    EWeaponType const weaponType = equippedWeapon->GetWeaponType();
    HasAnimDataForType(weaponType) ?
        animAssets.animDataMap[weaponType] = animDataAsset :
        animAssets.animDataMap.Add(weaponType, animDataAsset);

}


void UVal_AnimInstance::UpdateCurrentWeapon(EWeaponType weaponType)
{
    bool shouldBroadcast = false;
    
    switch (weaponType)
    {
        case EWeaponType::Melee:
            animAssets.currentWeaponType = EWeaponType::Melee;
            animAssets.currentAnimDataAsset = animAssets.meleeAnimAsset;
            shouldBroadcast = true;
            break;
        case EWeaponType::Primary:
            animAssets.currentWeaponType = EWeaponType::Primary;
            animAssets.currentAnimDataAsset = animAssets.primaryAnimAsset;
            shouldBroadcast = true;
            break;
        case EWeaponType::Secondary:
            animAssets.currentWeaponType = EWeaponType::Secondary;
            animAssets.currentAnimDataAsset = animAssets.secondaryAnimAsset;
            shouldBroadcast = true;
            break;
        default: break;
    }

    if (shouldBroadcast) notifier->onCurrentAnimDataChange.ExecuteIfBound(animAssets.currentAnimDataAsset);
}


TObjectPtr<UWeaponAnimDataAsset> UVal_AnimInstance::GetAnimDataAsset(EWeaponType weaponType)
{
	if (weaponType == EWeaponType::Empty || !HasAnimDataForType(weaponType)) return nullptr;
    return animAssets.animDataMap[weaponType];
}


void UVal_AnimInstance::RemoveAnimDataAsset(EWeaponType weaponType)
{
	if (weaponType == EWeaponType::Empty || !HasAnimDataForType(weaponType)) return;
    animAssets.animDataMap.Remove(weaponType);
}

void UVal_AnimInstance::UpdateWeaponState(EWeaponState newState)
{
    weaponState = newState;
    notifier->onWeaponStateChange.ExecuteIfBound(newState);
}
