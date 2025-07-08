// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.



#include "CommonWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

#include "ValorantMechanics/Player/Val_Character.h"
#include "ValorantMechanics/Core/Val_LocalPlayerSubsystem.h"
#include "ValorantMechanics/Player/Controller/Val_PlayerController.h"



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

bool ACommonWeapon::CanBeDropped() const
{
    if (weaponPickupType == EWeaponPickupType::NonPickupable) return false;
    return true;
}


void ACommonWeapon::ExternWeaponPickUp(AVal_Character* ownerCharacter)
{
    if (weaponType == EWeaponType::Empty) return;

    InternalPickedUp(ownerCharacter);
}

void ACommonWeapon::ExternWeaponEquip()
{
    if (!pOwnerCharacter) return;
    InternalEquipped();
}

void ACommonWeapon::ExternWeaponUnequip()
{
    if (!pOwnerCharacter) return;
    InternalUnequipped();
}

void ACommonWeapon::ExternWeaponDrop()
{
    if (!pOwnerCharacter || CanBeDropped()) return;
    InternalDropped();
}



void ACommonWeapon::InternalPickedUp(AVal_Character* ownerCharacter)
{
    if (!ownerCharacter) return;
    
    PrimaryActorTick.bCanEverTick = true;
    weaponLogicSM->PrimaryComponentTick.bCanEverTick = true;
    
    pOwnerCharacter = Cast<AVal_Character>(ownerCharacter);
    if (pOwnerCharacter; const auto* e = pOwnerCharacter->GetValPlayerController()->GetLocalPlayer())
    {
        pSubsystem = e->GetSubsystem<UVal_LocalPlayerSubsystem>();
    }

    // the player could just pick up the weapon but might still have another weapon equipped
    SetActorHiddenInGame(true);
    
}

void ACommonWeapon::InternalEquipped()
{
    PrimaryActorTick.bCanEverTick = true;
    weaponLogicSM->PrimaryComponentTick.bCanEverTick = true;
    weaponLogicSM->TryTransitionToState(EWeaponLogicState::Equip_Default);

    this->SetActorHiddenInGame(false);
}

void ACommonWeapon::InternalUnequipped()
{
    weaponLogicSM->ForceTransitionToState(EWeaponLogicState::None);
    PrimaryActorTick.bCanEverTick = false;
    weaponLogicSM->PrimaryComponentTick.bCanEverTick = false;

    SetActorHiddenInGame(true);
}

void ACommonWeapon::InternalDropped()
{
    PrimaryActorTick.bCanEverTick = false;
    weaponLogicSM->PrimaryComponentTick.bCanEverTick = false;
    SetActorHiddenInGame(false);
    
    pOwnerCharacter = nullptr;
    pSubsystem = nullptr;
}

