// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "SecondaryWeapon.h"
#include "ValorantMechanics/Weapons/WeaponDataAssets/WeaponAnimDataAsset.h"


// Sets default values
ASecondaryWeapon::ASecondaryWeapon()
{
    PrimaryActorTick.bCanEverTick = true;

    weaponType = EWeaponType::Secondary;
    weaponPickupType = EWeaponPickupType::Pickupable;
}

void ASecondaryWeapon::BeginPlay()
{
    Super::BeginPlay();
    
}

// Called every frame
void ASecondaryWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

