// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "PrimaryWeapon.h"

#include "ValorantMechanics/Weapons/WeaponDataAssets/WeaponAnimDataAsset.h"


APrimaryWeapon::APrimaryWeapon()
{
    PrimaryActorTick.bCanEverTick = true;

    weaponType = EWeaponType::Primary;
    weaponPickupType = EWeaponPickupType::Pickupable;
}

// Called when the game starts or when spawned
void APrimaryWeapon::BeginPlay()
{
    Super::BeginPlay();
    
}


void APrimaryWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

