// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "PrimaryWeapon.h"

#include "MovieSceneTracksComponentTypes.h"
#include "Components/BoxComponent.h"
#include "ValorantMechanics/Weapons/WeaponDataAssets/WeaponAnimDataAsset.h"


// Sets default values
APrimaryWeapon::APrimaryWeapon()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    
}

// Called when the game starts or when spawned
void APrimaryWeapon::BeginPlay()
{
    Super::BeginPlay();

    CollisionBox = CreateDefaultSubobject<UBoxComponent>(FName("Collision Box"));
    CollisionBox->AttachToComponent(_WeaponSkeletalMesh, FAttachmentTransformRules::SnapToTargetIncludingScale);

    _MagazineSkeletalMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Magazine Skeletal Mesh"));
    _MagazineSkeletalMesh->AttachToComponent(_WeaponSkeletalMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Magazine_MainSocket"));
    
}

// Called every frame
void APrimaryWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

