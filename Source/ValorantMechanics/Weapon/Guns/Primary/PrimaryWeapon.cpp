// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.



#include "PrimaryWeapon.h"


APrimaryWeapon::APrimaryWeapon()
{
    PrimaryActorTick.bCanEverTick = true;

    _weaponType = EWeaponType::Primary;
    _weaponPickupType = EWeaponPickupType::Pickupable;
}
