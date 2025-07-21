// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.



#include "CommonWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"



#include "ValorantMechanics/Player/Val_Character.h"
#include "ValorantMechanics/Core/Val_LocalPlayerSubsystem.h"
#include "ValorantMechanics/Player/Controller/Val_PlayerController.h"



// Sets default values
ACommonWeapon::ACommonWeapon()
{
    PrimaryActorTick.bCanEverTick = true;
    

    weaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
    weaponMesh->CastShadow = false;
    weaponMesh->bCastDynamicShadow = false;
    weaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    weaponMesh->SetGenerateOverlapEvents(false);
    weaponMesh->SetSimulatePhysics(false);
    
    magazineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Magazine Mesh"));
    magazineMesh->SetupAttachment(weaponMesh, socketData.magazineMainSocket);
    magazineMesh->CastShadow = false;
    magazineMesh->bCastDynamicShadow = false;
    magazineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    magazineMesh->SetGenerateOverlapEvents(false);
    magazineMesh->SetSimulatePhysics(false);

    scopeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Scope Mesh"));
    scopeMesh->SetupAttachment(weaponMesh, socketData.reflexSocket);
    scopeMesh->CastShadow = false;
    scopeMesh->bCastDynamicShadow = false;
    scopeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    scopeMesh->SetGenerateOverlapEvents(false);
    scopeMesh->SetSimulatePhysics(false);


    collisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
    collisionBox->SetupAttachment(weaponMesh);
    collisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    
    stateManager = CreateDefaultSubobject<UWeaponLogicStateManager>(TEXT("Weapon Logic State Manager"));
    InitializePrimaryStateManager(stateManager);
    
}


// Called when the game starts or when spawned
void ACommonWeapon::BeginPlay()
{
    Super::BeginPlay();
    
    stateManager->InitializeWeaponStateManager(&defaultProperties, &altProperties);
    currentMagAmmoCount = defaultProperties.magazineCapacity;
    totalAmmoCount = defaultProperties.magazineCapacity * defaultProperties.magazineCount;

    stateManager->GetStateUpdateCallbackDelegate()->AddUObject(this, &ACommonWeapon::StateUpdated);
}


// Called every frame
void ACommonWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACommonWeapon::StateUpdated(EWeaponLogicState oldState, EWeaponLogicState newState)
{
    UE_LOG(LogTemp, Display, TEXT("previous state: %d; new state: %d"), (uint8)oldState, (uint8)newState)

    if (newState == EWeaponLogicState::FireShot) FireShoot();
}


#pragma region GETTER FUNCS
EWeaponType ACommonWeapon::GetWeaponType()
{
    return weaponType;
}

EWeaponLogicState ACommonWeapon::GetWeaponState()
{
    return stateManager->GetCurrentState();
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

bool ACommonWeapon::IsWeaponAutomatic() const
{
    return defaultProperties.weaponIsAutomatic;
}
#pragma endregion GETTER FUNCS


void ACommonWeapon::ExternFireStart()
{
    if (CanFire())
    {
        isFireStarted = true;

        if (!defaultProperties.weaponIsAutomatic && !semiAutoFireBlocked)
        {
            stateManager->TryTransitionToState(EWeaponLogicState::FireShot);
            semiAutoFireBlocked = true; // block shots until button is released 
        }
    }
}

void ACommonWeapon::ExternFireEnd()
{
    isFireStarted = false;
    if (!defaultProperties.weaponIsAutomatic)
    {
        semiAutoFireBlocked = false;
    }
}

void ACommonWeapon::ExternAltFireStart()
{
}

void ACommonWeapon::ExternAltFireEnd()
{
}


void ACommonWeapon::ExternReloadRequest()
{
}


void ACommonWeapon::ExternFireTriggered()
{
    const TSet<EWeaponLogicState> queueableStates = {
        EWeaponLogicState::FireCooldown,
        EWeaponLogicState::Blocked,
        EWeaponLogicState::Equip_Default,
        EWeaponLogicState::Equip_Fast
    };

    if (CanFire() && defaultProperties.weaponIsAutomatic && isFireStarted)
    {
        if (queueableStates.Contains(stateManager->GetCurrentState()))
        {
            UE_LOG(LogTemp, Display, TEXT("trying to queue shoot"));
            stateManager->TryQueueState(EWeaponLogicState::FireShot);
        }
        else stateManager->TryTransitionToState(EWeaponLogicState::FireShot);
        
    }

}

void ACommonWeapon::FireShoot()
{
    stateManager->ForceTransitionToState(EWeaponLogicState::FireCooldown);

    FHitResult hit;
    FCollisionQueryParams queryParams = FCollisionQueryParams(SCENE_QUERY_STAT(WeaponTrace), false, this);

    // TODO: update to gameplay camera once implemented
    const auto& e = pOwnerCharacter->characterMeshCamera;
    const FVector startPoint = e->GetComponentLocation();
    const FVector endPoint = e->GetForwardVector() * defaultProperties.maxRange + startPoint;

    if (GetWorld()->LineTraceSingleByChannel(hit, startPoint, endPoint, ECC_Visibility, queryParams))
    {
        if (impactParticle)
        {
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
                impactParticle,
                hit.ImpactPoint,
                hit.ImpactNormal.Rotation(),
                FVector(1,1,1),
                true,
                true,
                ENCPoolMethod::AutoRelease,
                true);
        }
    }

    if (muzzleParticle)
    {
        UNiagaraComponent* spawnedSystem = UNiagaraFunctionLibrary::SpawnSystemAttached(
            muzzleParticle,
            weaponMesh,
            socketData.muzzleSocket,
            FVector::ZeroVector,
            FRotator::ZeroRotator,
            FVector(100.0f, 100.0f, 100.0f),
            EAttachLocation::SnapToTarget,
            true,
            ENCPoolMethod::AutoRelease,
            true,
            true
        );
        spawnedSystem->Activate();
    }


    pOwnerCharacter->PlayLocalSound(animAsset->GetRandomAttackSFX());
    
    UE_LOG(LogTemp, Warning, TEXT("weapon trace start: %s"), *startPoint.ToString())
    UE_LOG(LogTemp, Warning, TEXT("weapon trace start: %s"), *endPoint.ToString())
}

void ACommonWeapon::InternalAltFireStart()
{
}

void ACommonWeapon::InternalAltFireEnd()
{
}

void ACommonWeapon::InternalReloadRequest()
{
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
    stateManager->PrimaryComponentTick.bCanEverTick = true;
    
    pOwnerCharacter = Cast<AVal_Character>(ownerCharacter);
    if (const auto* e = pOwnerCharacter->GetValPlayerController()->GetLocalPlayer(); pOwnerCharacter && e)
    {
        pSubsystem = e->GetSubsystem<UVal_LocalPlayerSubsystem>();
    }

    // the player could just pick up the weapon but might still have another weapon equipped
    SetActorHiddenInGame(true);
    
}

void ACommonWeapon::InternalEquipped()
{
    PrimaryActorTick.bCanEverTick = true;
    stateManager->PrimaryComponentTick.bCanEverTick = true;
    stateManager->TryTransitionToState(EWeaponLogicState::Equip_Default);

    SetActorHiddenInGame(false);
    pOwnerCharacter->PlayLocalSound(animAsset->equipSFX);
}

void ACommonWeapon::InternalUnequipped()
{
    stateManager->ForceTransitionToState(EWeaponLogicState::None);
    PrimaryActorTick.bCanEverTick = false;
    stateManager->PrimaryComponentTick.bCanEverTick = false;
    isFireStarted = false;

    SetActorHiddenInGame(true);
}

void ACommonWeapon::InternalDropped()
{
    PrimaryActorTick.bCanEverTick = false;
    stateManager->PrimaryComponentTick.bCanEverTick = false;
    SetActorHiddenInGame(false);
    
    pOwnerCharacter = nullptr;
    pSubsystem = nullptr;
}


bool ACommonWeapon::CanFire() const
{
    if (currentMagAmmoCount <= 0)
    {
        UE_LOG(LogTemp, Display, TEXT("cannot fire: no ammo"));
        return false;
    }


    if (!stateManager->CanTransitionToState(EWeaponLogicState::FireShot))
    {
        UE_LOG(LogTemp, Display, TEXT("cannot fire: state transition denied"));
        return false;
    }

    
    if (defaultProperties.weaponIsAutomatic)
    {
        const float fireRateSecs = defaultProperties.fireRate > 0.0f ?
            1.0f / defaultProperties.fireRate - 0.05f :
            0.1f;
        const float timeSinceLastFire = stateManager->GetTimeSinceLastState();

        if (timeSinceLastFire >= fireRateSecs) return true;        

        UE_LOG(LogTemp, Display, TEXT("cannot fire: still in fire cooldown (%.3f seconds left)"), fireRateSecs - timeSinceLastFire);
        return false;
    }

    
    // for semi-automatic weapons
    const EWeaponLogicState lastState = stateManager->GetLastState();
    if (lastState != EWeaponLogicState::FireShot) return true;

    UE_LOG(LogTemp, Display, TEXT("cannot fire: semi-auto fire already triggered and waiting for release"));
    return false;
}


