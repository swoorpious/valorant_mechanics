// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "CommonWeapon.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
// #include "NiagaraSystem.h"
#include "NiagaraConstants.h"
#include "Val_WeaponFireConfig.h"

#include "DrawDebugHelpers.h"

#include "ValorantMechanics/Player/Val_Character.h"
#include "ValorantMechanics/Core/Val_LocalPlayerSubsystem.h"
// #include "ValorantMechanics/Player/Controller/Val_PlayerController.h"


// Sets default values
ACommonWeapon::ACommonWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    weaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
    weaponMesh->CastShadow = false;
    weaponMesh->bCastDynamicShadow = false;
    weaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    weaponMesh->SetGenerateOverlapEvents(false);
    weaponMesh->SetSimulatePhysics(false);

    magazineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Magazine Mesh"));
    magazineMesh->CastShadow = false;
    magazineMesh->bCastDynamicShadow = false;
    magazineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    magazineMesh->SetGenerateOverlapEvents(false);
    magazineMesh->SetSimulatePhysics(false);

    scopeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Scope Mesh"));
    scopeMesh->CastShadow = false;
    scopeMesh->bCastDynamicShadow = false;
    scopeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    scopeMesh->SetGenerateOverlapEvents(false);
    scopeMesh->SetSimulatePhysics(false);

    collisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
    collisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);


    if (weaponMesh) _applyRenderOnTopParams_(true);
    if (magazineMesh) _applyRenderOnTopParams_(true);
    if (scopeMesh) _applyRenderOnTopParams_(true);


    this->_setupAttachments_();
}


void ACommonWeapon::fireStart()
{
    if (_isFireHeld || _currentMagAmmoCount_ == 0) return;
    _isFireHeld = true;

    /*
     * override this function and implement firing logic 
     */
}

void ACommonWeapon::fireEnd()
{
    _isFireHeld = false;
}


void ACommonWeapon::tryWeaponReload()
{
    // only reload if the mag is not already full and we have spare mags
    if (!_weaponConfig) return;
    if (_currentMagCount_ == 0) return;
    if (_currentMagAmmoCount_ >= _weaponConfig->magSize) return;

    const int32 magCapacity = _weaponConfig->magSize;
    const int32 ammoNeeded = magCapacity - _currentMagAmmoCount_;
    const int32 ammoAvailable = _currentMagCount_ * magCapacity; // total reserve rounds

    const int32 ammoToAdd = FMath::Min(ammoNeeded, ammoAvailable);
    _currentMagAmmoCount_ += ammoToAdd;

    const int32 magsUsed = FMath::DivideAndRoundUp(ammoToAdd, magCapacity);
    _currentMagCount_ = FMath::Max(0, _currentMagCount_ - magsUsed);

    _totalAmmoCount_ = _currentMagAmmoCount_ + _currentMagCount_ * magCapacity;
}

bool ACommonWeapon::tryWeaponPickUp(AVal_Character* ownerCharacter)
{
    /*
     * doing this on pickup method because once dropped, the material properties will create issues with rendering
     * we remove these values on drop
     */
    _applyRenderOnTopParams_(true);

    _ownerCharacter_ = ownerCharacter;
    return true;
}

bool ACommonWeapon::tryWeaponDrop()
{
    if (!canDrop()) return false;

    _applyRenderOnTopParams_(false);

    _ownerCharacter_ = nullptr;
    return true;
}

// TODO implement
bool ACommonWeapon::trySwitchFireMode(EFireMode newMode)
{
    return false;
}

// assuming that the player can equip this weapon,
// we just proceed to equip this, because the player calls this function
void ACommonWeapon::weaponEquip(EEquipType type)
{
    PrimaryActorTick.bCanEverTick = true;
    SetActorHiddenInGame(false);

    // broadcast weapon change
    if (_ownerCharacter_ && _animConfig)
        _ownerCharacter_->getOnWeaponChangedDelegate()->Broadcast(_animConfig);

    // set correct equip state BEFORE starting the timer, and don't call
    // _onWeaponEquipped() immediately — let the timer do it.
    if (type == EEquipType::EquipFast)
    {
        _updateState(EWeaponState::Equip_Fast);
        // add timer for length Equip_Fast
        GetWorldTimerManager().SetTimer(
            _timerHandle_handleEquip_,
            this,
            &ACommonWeapon::_onWeaponEquipped,
            _weaponConfig ? _weaponConfig->equipTimeFast : 0.1f,
            false
        );
    }
    else
    {
        _updateState(EWeaponState::Equip_Default);
        // add timer for length Equip_Default
        GetWorldTimerManager().SetTimer(
            _timerHandle_handleEquip_,
            this,
            &ACommonWeapon::_onWeaponEquipped,
            _weaponConfig ? _weaponConfig->equipTimeDefault : 0.1f,
            false
        );
    }
}

void ACommonWeapon::weaponUnequip()
{
    PrimaryActorTick.bCanEverTick = false;
    SetActorHiddenInGame(true);
    _isFireHeld = false;
    GetWorldTimerManager().ClearTimer(_timerHandle_handleEquip_);
    GetWorldTimerManager().ClearTimer(_timerHandle_handleRefire_);
    _updateState(EWeaponState::None);
}

// Called when the game starts or when spawned
void ACommonWeapon::BeginPlay()
{
    Super::BeginPlay();

    if (_weaponConfig)
    {
        _currentMagAmmoCount_ = static_cast<uint8>(FMath::Clamp(_weaponConfig->magSize, 0, 255));
        _currentMagCount_ = static_cast<uint8>(FMath::Clamp(_weaponConfig->magCount, 0, 255));
        _totalAmmoCount_ = static_cast<uint8>(FMath::Clamp(_weaponConfig->magSize * _weaponConfig->magCount, 0, 255));
    }
}


// Called every frame
void ACommonWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}


void ACommonWeapon::_onWeaponEquipped()
{
    _updateState(EWeaponState::Idle);
    // whatever to do further    
}


void ACommonWeapon::_updateState(EWeaponState newState)
{
    // const EWeaponState oldState = _weaponState;
    _weaponState = newState;

    // fire the character-level delegate (one-param consumed by AnimInstance)
    if (_ownerCharacter_)
    {
        _ownerCharacter_->getOnWeaponStateChangedDelegate()->Broadcast(newState);
        UE_LOGFMT(LogActor, Display, "broadcasting state change to owner characters delegate: {0}",
                  static_cast<uint8>(newState));
        UE_LOGFMT(
            LogActor,
            Display,
            "broadcasting on character: {0}, bound: {1}",
            reinterpret_cast<uintptr_t>(_ownerCharacter_.Get()),
            _ownerCharacter_->getOnWeaponStateChangedDelegate()->IsBound()
        );
    }
}


/*
 * this function does not play bullet sound effects
 * it handled by the function that calls this function
 */
void ACommonWeapon::_shootBullet(
    /*
     * TODO parameters for adjusting the error in fire, in degrees
     */
)
{
    // FIX: guard against null owner or config before dereferencing
    if (!_ownerCharacter_ || !_weaponConfig) return;

    FHitResult hit;
    FCollisionQueryParams queryParams = FCollisionQueryParams(SCENE_QUERY_STAT(WeaponTrace), false, _ownerCharacter_);

    // TODO: update to gameplay camera once implemented
    const auto& e = _ownerCharacter_->characterMeshCamera;
    if (!e) return; // guard against null camera

    const FVector startPoint = e->GetComponentLocation();
    const FVector endPoint = e->GetForwardVector() * _weaponConfig->maxRange + startPoint;

    if (GetWorld()->LineTraceSingleByChannel(hit, startPoint, endPoint, ECC_Visibility, queryParams))
    {
        if (impactParticle)
        {
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                GetWorld(),
                impactParticle,
                hit.ImpactPoint,
                hit.ImpactNormal.Rotation(),
                FVector(1, 1, 1),
                true,
                true,
                ENCPoolMethod::AutoRelease,
                true);
        }

#if WITH_EDITOR
        DrawDebugLine(
            GetWorld(),
            startPoint,
            endPoint,
            FColor::Red,
            false,
            5.0f,
            0,
            5.0f
        );
#endif
    }

    if (muzzleParticle)
    {
        UNiagaraComponent* spawnedSystem = UNiagaraFunctionLibrary::SpawnSystemAttached(
            muzzleParticle,
            weaponMesh,
            _socketData.muzzleSocket,
            FVector::ZeroVector,
            FRotator::ZeroRotator,
            FVector(100.0f, 100.0f, 100.0f),
            EAttachLocation::SnapToTarget,
            true,
            ENCPoolMethod::AutoRelease,
            true,
            true
        );
        // SpawnSystemAttached can return null if the pool is exhausted
        if (spawnedSystem) spawnedSystem->Activate();
    }

    // pOwnerCharacter->PlayLocalSound(animAsset->GetRandomAttackSFX());

    UE_LOG(LogTemp, Warning, TEXT("weapon trace start: %s"), *startPoint.ToString())
    UE_LOG(LogTemp, Warning, TEXT("weapon trace end:   %s"), *endPoint.ToString())
}


bool ACommonWeapon::_canFire() const
{
    if (!_weaponConfig) return false;

    // check current mag ammo instead of total ammo available
    // since total ammo can be non-zero but currently available is still zero
    if (_currentMagAmmoCount_ <= 0)
    {
        UE_LOG(LogTemp, Display, TEXT("cannot fire: no ammo"));
        return false;
    }

    return !_isOnCooldown;
}


void ACommonWeapon::_setupAttachments_() const
{
    magazineMesh->SetupAttachment(weaponMesh, _socketData.magazineMainSocket);
    scopeMesh->SetupAttachment(weaponMesh, _socketData.reflexSocket);
    collisionBox->SetupAttachment(weaponMesh);
}

void ACommonWeapon::_applyRenderOnTopParams_(bool isPickup)
{
    auto createAndApply = [&, isPickup](UMeshComponent* mesh, UMaterialInstanceDynamic*& mid)
    {
        if (!mesh) return;

        if (!mid)
        {
            UMaterialInterface* base_mat = mesh->GetMaterial(0);
            if (!base_mat) return;
            mid = UMaterialInstanceDynamic::Create(base_mat, this);
            mesh->SetMaterial(0, mid);
        }

        mid->SetScalarParameterValue(TEXT("Target FOV"), isPickup ? targetFOV : 90.f);
        mid->SetScalarParameterValue(TEXT("Scale in Depth"), isPickup ? targetRenderScaleInDepth : 1.f);
    };

    createAndApply(weaponMesh, _midBody_);
    createAndApply(magazineMesh, _midMag_);
    createAndApply(scopeMesh, _midScope_);
}
