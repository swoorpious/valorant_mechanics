// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.



#include "SecondaryWeapon.h"


// Sets default values
ASecondaryWeapon::ASecondaryWeapon()
{
    PrimaryActorTick.bCanEverTick = true;

    _weaponType = EWeaponType::Secondary;
    _weaponPickupType = EWeaponPickupType::Pickupable;
}

