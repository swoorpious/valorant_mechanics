// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "Val_AnimInstance.h"
// #include "ValorantMechanics/Weapons/WeaponDataAssets/MeleeAnimDataAsset.h"
// #include "ValorantMechanics/Weapons/WeaponDataAssets/WeaponAnimDataAsset.h"


void UVal_AnimInstance::UpdateAnimDataAsset(EWeaponType WeaponType, UDataAsset* AnimDataAsset)
{
    if (AnimDataAsset)
    {
        switch (WeaponType)
        {
            case EWeaponType::Melee:
                animAssets.meleeAnimAsset = AnimDataAsset;
                break;
            case EWeaponType::Primary:
                animAssets.secondaryAnimAsset = AnimDataAsset;
                break;
            case EWeaponType::Secondary:
                animAssets.primaryAnimAsset = AnimDataAsset;
                break;
            default: break;
        }
    }

}

void UVal_AnimInstance::UpdateCurrentWeapon(EWeaponType WeaponType)
{
    switch (WeaponType)
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
