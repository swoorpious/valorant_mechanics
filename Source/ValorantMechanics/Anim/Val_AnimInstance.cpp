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
                meleeAnimDataAsset = AnimDataAsset;
                break;
            case EWeaponType::Primary:
                secondaryAnimDataAsset = AnimDataAsset;
                break;
            case EWeaponType::Secondary:
                primaryAnimDataAsset = AnimDataAsset;
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
            currentAnimDataAsset = meleeAnimDataAsset;
            break;
        case EWeaponType::Primary:
            currentWeaponType = EWeaponType::Primary;
            currentAnimDataAsset = primaryAnimDataAsset;
            break;
        case EWeaponType::Secondary:
            currentWeaponType = EWeaponType::Secondary;
            currentAnimDataAsset = secondaryAnimDataAsset;
            break;
        default: break;
    }
}
