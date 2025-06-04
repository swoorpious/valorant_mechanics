// Fill out your copyright notice in the Description page of Project Settings.


#include "WushuAnimInstance.h"
// #include "ValorantMechanics/Weapons/WeaponDataAssets/MeleeAnimDataAsset.h"
// #include "ValorantMechanics/Weapons/WeaponDataAssets/WeaponAnimDataAsset.h"


void UWushuAnimInstance::UpdateAnimDataAsset(EWeaponType WeaponType, UDataAsset* AnimDataAsset)
{
    if (AnimDataAsset)
    {
        switch (WeaponType)
        {
        case EWeaponType::Melee:
            MeleeAnimDataAsset = AnimDataAsset;
            break;
        case EWeaponType::Primary:
            PrimaryAnimDataAsset = AnimDataAsset;
            break;
        case EWeaponType::Secondary:
            SecondaryAnimDataAsset = AnimDataAsset;
            break;
        default: break;
        }
    }

}
