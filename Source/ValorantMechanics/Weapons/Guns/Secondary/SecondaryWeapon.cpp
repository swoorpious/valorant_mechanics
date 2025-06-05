// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "SecondaryWeapon.h"
#include "ValorantMechanics/Weapons/WeaponDataAssets/WeaponAnimDataAsset.h"


// Sets default values
ASecondaryWeapon::ASecondaryWeapon()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASecondaryWeapon::BeginPlay()
{
    Super::BeginPlay();
    
}

// Called every frame
void ASecondaryWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

