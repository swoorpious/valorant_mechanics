// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonWeapon.h"
#include "Components/SkeletalMeshComponent.h"


// Sets default values
ACommonWeapon::ACommonWeapon()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    _WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Skeletal Mesh"));
    _WeaponSkeletalMesh->CastShadow = false;
    _WeaponSkeletalMesh->bCastDynamicShadow = false;
}

void ACommonWeapon::Fire()
{
}

void ACommonWeapon::Reload()
{
}

void ACommonWeapon::Equip(FEquipType EquipType)
{
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

