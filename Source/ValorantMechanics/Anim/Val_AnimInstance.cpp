// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "Val_AnimInstance.h"
#include "ValorantMechanics/ValorantMechanics.h"
#include "ValorantMechanics/Weapons/CommonWeapon.h"

#include "ValorantMechanics/Core/Val_Character.h"
// #include "ValorantMechanics/Weapons/WeaponDataAssets/MeleeAnimDataAsset.h"
// #include "ValorantMechanics/Weapons/WeaponDataAssets/WeaponAnimDataAsset.h"


void UVal_AnimInstance::NativeBeginPlay()
{
    Super::NativeBeginPlay();

    if (AVal_Character* e = Cast<AVal_Character>(TryGetPawnOwner()))
    {
        e->onWeaponSpawn.AddUObject(this, &UVal_AnimInstance::UpdateAnimDataAsset);
        e->onWeaponEquip.AddUObject(this, &UVal_AnimInstance::UpdateCurrentWeapon);
        e->onWeaponDrop.AddUObject(this, &UVal_AnimInstance::RemoveAnimDataAsset);
    }
}


void UVal_AnimInstance::UpdateAnimDataAsset(ACommonWeapon* equippedWeapon)
{
    const TObjectPtr<UDataAsset> animDataAsset = equippedWeapon->GetAnimAsset();
    EWeaponType weaponType = equippedWeapon->GetWeaponType();
    animAssets.HasAnimDataForType(weaponType) ?
        animAssets.animDataAssets[weaponType] = animDataAsset :
        animAssets.animDataAssets.Add(weaponType, animDataAsset);

}

void UVal_AnimInstance::UpdateCurrentWeapon(EWeaponType weaponType)
{

    switch (weaponType)
    {
        case EWeaponType::Melee:
            currentWeaponType = EWeaponType::Melee;
            animAssets.currentAnimDataAsset = animAssets.meleeAnimAsset;
            break;
        case EWeaponType::Primary:
            currentWeaponType = EWeaponType::Primary;
            animAssets.currentAnimDataAsset = animAssets.primaryAnimAsset;
            break;
        case EWeaponType::Secondary:
            currentWeaponType = EWeaponType::Secondary;
            animAssets.currentAnimDataAsset = animAssets.secondaryAnimAsset;
            break;
        default: break;
    }
}


TObjectPtr<UDataAsset> UVal_AnimInstance::GetAnimDataAsset(EWeaponType weaponType)
{
	if (weaponType == EWeaponType::Empty || !animAssets.HasAnimDataForType(weaponType)) return nullptr;
    return animAssets.animDataAssets[weaponType];
}


void UVal_AnimInstance::RemoveAnimDataAsset(EWeaponType weaponType)
{
	if (weaponType == EWeaponType::Empty || !animAssets.HasAnimDataForType(weaponType)) return;
    animAssets.animDataAssets.Remove(weaponType);
}
