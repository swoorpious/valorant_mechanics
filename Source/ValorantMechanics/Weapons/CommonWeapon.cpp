// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "CommonWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"




// Sets default values
ACommonWeapon::ACommonWeapon()
{
    PrimaryActorTick.bCanEverTick = true;

    weaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
    weaponMesh->CastShadow = false;
    weaponMesh->bCastDynamicShadow = false;
    
    magazineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Magazine Mesh"));
    magazineMesh->SetupAttachment(weaponMesh, magazineMainSocketName);
    magazineMesh->CastShadow = false;
    magazineMesh->bCastDynamicShadow = false;

    scopeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Scope Mesh"));
    scopeMesh->SetupAttachment(weaponMesh, scopeMainSocketName);
    scopeMesh->CastShadow = false;
    scopeMesh->bCastDynamicShadow = false;

    collisionBox = CreateDefaultSubobject<UBoxComponent>(FName("Collision Box"));
    collisionBox->SetupAttachment(weaponMesh);

}


// Called when the game starts or when spawned
void ACommonWeapon::BeginPlay()
{
    Super::BeginPlay();
    
}

// Called every frame
void ACommonWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

