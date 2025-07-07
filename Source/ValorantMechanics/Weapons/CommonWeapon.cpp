// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.



#include "CommonWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"




// Sets default values
ACommonWeapon::ACommonWeapon()
{
    PrimaryActorTick.bCanEverTick = true;
    
    socketData = CreateDefaultSubobject<UWeaponSocketData>(TEXT("Player Socket Names"));    
    

    weaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
    weaponMesh->CastShadow = false;
    weaponMesh->bCastDynamicShadow = false;
    weaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    weaponMesh->SetGenerateOverlapEvents(false);
    weaponMesh->SetSimulatePhysics(false);
    
    magazineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Magazine Mesh"));
    magazineMesh->SetupAttachment(weaponMesh, socketData->magazineMainSocket);
    magazineMesh->CastShadow = false;
    magazineMesh->bCastDynamicShadow = false;
    magazineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    magazineMesh->SetGenerateOverlapEvents(false);
    magazineMesh->SetSimulatePhysics(false);

    scopeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Scope Mesh"));
    scopeMesh->SetupAttachment(weaponMesh, socketData->reflexSocket);
    scopeMesh->CastShadow = false;
    scopeMesh->bCastDynamicShadow = false;
    scopeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    scopeMesh->SetGenerateOverlapEvents(false);
    scopeMesh->SetSimulatePhysics(false);


    collisionBox = CreateDefaultSubobject<UBoxComponent>(FName("Collision Box"));
    collisionBox->SetupAttachment(weaponMesh);
    collisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);


    weaponLogicSM = CreateDefaultSubobject<UWeaponLogicStateManager>(TEXT("Weapon Logic State Manager"));
    InitializePrimaryStateManager(weaponLogicSM);

    
    weaponAnimSM = CreateDefaultSubobject<UWeaponAnimStateManager>(TEXT("Weapon Anim State Manager"));
    InitializeSecondaryStateManager(weaponAnimSM);
}



EWeaponType ACommonWeapon::GetWeaponType()
{
    return weaponType;
}

EWeaponLogicState ACommonWeapon::GetWeaponState()
{
    return weaponLogicSM->GetCurrentState();
}

EWeaponPickupType ACommonWeapon::GetWeaponPickupType()
{
    return weaponPickupType;
}

TObjectPtr<UWeaponAnimDataAsset>& ACommonWeapon::GetAnimAsset()
{
    return animAsset;
}


// Called when the game starts or when spawned
void ACommonWeapon::BeginPlay()
{
    Super::BeginPlay();
    
    weaponLogicSM->InitializeWeaponStateManager(&defaultProperties, &altProperties);
    
}


// Called every frame
void ACommonWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

