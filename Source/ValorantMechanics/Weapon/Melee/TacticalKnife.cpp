// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.



#include "TacticalKnife.h"


// Sets default values
ATacticalKnife::ATacticalKnife()
{
    PrimaryActorTick.bCanEverTick = true;

    _weaponType = EWeaponType::Melee;
    _weaponPickupType = EWeaponPickupType::NonPickupable;
}

