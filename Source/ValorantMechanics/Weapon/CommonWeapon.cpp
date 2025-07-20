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

    
    // muzzleParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Muzzle Sprite"));
    // muzzleParticle->SetupAttachment(weaponMesh, socketData.muzzleSocket);
    //
    // bulletTracer = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Bullet Tracer"));
    // impactParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Impact Sprite"));

    
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
    currentMagAmmoCount = defaultProperties.magazineCapacity;
    totalAmmoCount = defaultProperties.magazineCapacity * defaultProperties.magazineCount;

    weaponLogicSM->GetStateUpdateCallbackDelegate()->AddUObject(this, &ACommonWeapon::StateUpdated);
}


// Called every frame
void ACommonWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACommonWeapon::StateUpdated(EWeaponLogicState oldState, EWeaponLogicState newState)
{
        UE_LOG(LogTemp, Display, TEXT("State update recieved on the weapon class"))
    if (newState == EWeaponLogicState::FireShot)
    {
        UE_LOG(LogTemp, Display, TEXT("Transitioned to EWeaponLogicState::FireShot"))
        FireShoot();
    }
}


#pragma region GETTER FUNCS
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
#pragma endregion GETTER FUNCS


void ACommonWeapon::ExternFireStart()
{
    if (CanFire())
    {
        weaponLogicSM->ForceTransitionToState(EWeaponLogicState::FireShot);
    }
}

void ACommonWeapon::ExternFireEnd()
{
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

    if (CanFire())
    {
        if (queueableStates.Contains(weaponLogicSM->GetCurrentState()) || queueableStates.Contains(weaponLogicSM->GetLastState()))
        {
            UE_LOG(LogTemp, Display, TEXT("trying to queue shoot"));
            weaponLogicSM->TryQueueState(EWeaponLogicState::FireShot);
        }
        else
            weaponLogicSM->TryTransitionToState(EWeaponLogicState::FireShot);
        
    }

}

void ACommonWeapon::FireShoot()
{
    UE_LOG(LogTemp, Display, TEXT("FireShoot()"))

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

//         UNiagaraComponent* tracerComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
//     GetWorld(),
//     bulletTracerSystem,                      // Your NiagaraSystem*
//     startLocation,                           // Bullet origin (e.g., muzzle)
//     FRotator::ZeroRotator,
//     FVector(1.0f),                           // Scale
//     true,                                    // Auto destroy
//     true,                                    // Auto activate
//     ENCPoolMethod::AutoRelease,
//     true
// );
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
    isFireHeld = false;

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


bool ACommonWeapon::CanFire() const
{
    const bool baseCondition = weaponLogicSM->CanTransitionToState(EWeaponLogicState::FireShot) && currentMagAmmoCount > 0;
    
    if (weaponLogicSM->GetLastState() == EWeaponLogicState::FireShot)
    {
        const float fireRate = (defaultProperties.fireRate > 0) ? (1.0f / defaultProperties.fireRate) - 0.1f : 1.0f;
        return weaponLogicSM->GetTimeSinceLastState() >= fireRate && baseCondition;
    }
    
    return baseCondition;
}

