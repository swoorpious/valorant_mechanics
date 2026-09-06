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
#include "ValorantMechanics/Player/PlayerComponents/Val_CharacterMovementComponent.h"
#include "ValorantMechanics/Core/Val_LocalPlayerSubsystem.h"
#include "ValorantMechanics/Core/Log.h"
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

    leftHandIK = CreateDefaultSubobject<USceneComponent>(TEXT("Left Hand IK Scene"));

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


#pragma region PUBLIC_GETTER_FUNCTIONS

USceneComponent* ACommonWeapon::getLeftHandIKComponent() const { return leftHandIK.Get(); }
UVal_WeaponFireConfig* ACommonWeapon::getWeaponFireConfig() const { return _weaponConfig.Get(); } 
EWeaponType ACommonWeapon::getWeaponType() const { return _weaponType; }
EWeaponPickupType ACommonWeapon::getWeaponPickupType() const { return _weaponPickupType; }
UVal_WeaponAnimConfig* ACommonWeapon::getAnimAsset() const { return _animConfig; }
EFireMode ACommonWeapon::getWeaponFireMode() const
{
    return _weaponConfig ? _weaponConfig->fireMode : EFireMode::Manual;
}
float ACommonWeapon::getWeaponRunSpeed()
{
    // a weapon with no fire config (e.g. melee), or one whose runSpeed hasn't
    // been authored yet (still the 0.0f default), has no usable speed of its
    // own - fall back to the character's base movement speed rather than
    // freezing movement by writing a 0 into MaxWalkSpeed.
    if (_weaponConfig && _weaponConfig->runSpeed > 0.f) return _weaponConfig->runSpeed;
    const auto* movement = _ownerCharacter_ ? _ownerCharacter_->GetValMovementComponent() : nullptr;
    return movement ? movement->movementProperties.runSpeed : 0.f;
}

float ACommonWeapon::getWeaponWalkSpeed()
{
    if (_weaponConfig && _weaponConfig->runSpeed > 0.f) return _weaponConfig->runSpeed * .67f;
    const auto* movement = _ownerCharacter_ ? _ownerCharacter_->GetValMovementComponent() : nullptr;
    return movement ? movement->movementProperties.walkSpeed : 0.f;
}

#pragma endregion //PUBLIC_GETTER_FUNCTIONS



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

    _isEquipActive_ = true;
    _broadcastAssetChanged();

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
    _isEquipActive_ = false;

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
    
    if (GEngine && _isEquipActive_)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            DeltaTime,
            FColor::White,
            FString::Printf(
                TEXT("current weapon: %s"),
                *StaticClass()->GetName()
            ));
        GEngine->AddOnScreenDebugMessage(
            -1,
            DeltaTime,
            FColor::White,
            FString::Printf(
                TEXT("state: %s"),
                *StaticEnum<EWeaponState>()->GetDisplayNameTextByValue(static_cast<int8>(_weaponState)).ToString()
            ));
        
    }
    
}


void ACommonWeapon::_onWeaponEquipped()
{
    _updateState(EWeaponState::Idle);
    // whatever to do further    
}


void ACommonWeapon::_updateState(EWeaponState newState)
{
    if (newState != EWeaponState::None && !_isEquipActive_) return;

    // const EWeaponState oldState = _weaponState;
    _weaponState = newState;

    // fire the character-level delegate (one-param consumed by AnimInstance)
    if (_ownerCharacter_)
    {
        _ownerCharacter_->getOnWeaponStateChangedDelegate()->Broadcast(newState);
        LOGObjName(this, LogActor, Display, "broadcasting state change to owner characters delegate: %d",
                  static_cast<uint8>(newState));
        LOGObjName(
            this,
            LogActor,
            Display,
            "broadcasting on character: %p, bound: %d",
            _ownerCharacter_.Get(),
            _ownerCharacter_->getOnWeaponStateChangedDelegate()->IsBound()
        );
    }
}


void ACommonWeapon::_broadcastAssetChanged()
{
    if (!_ownerCharacter_) return;
    _ownerCharacter_->getOnWeaponChangedDelegate()->Broadcast(_animConfig);
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

    const bool bHitSomething = GetWorld()->LineTraceSingleByChannel(hit, startPoint, endPoint, ECC_Visibility, queryParams);

    if (bHitSomething && impactParticle)
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
    // always draw the weapon's fire trace out to its range, hit or not,
    // so range/spread can be sanity checked in the editor: red up to the
    // hit point when something was hit, green out to maxRange otherwise.
    DrawDebugLine(
        GetWorld(),
        startPoint,
        bHitSomething ? hit.ImpactPoint : endPoint,
        bHitSomething ? FColor::Red : FColor::Green,
        false,
        5.0f,
        0,
        5.0f
    );
#endif

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

    LOGObjName(this, LogTemp, Warning, "weapon trace start: %s", *startPoint.ToString());
    LOGObjName(this, LogTemp, Warning, "weapon trace end:   %s", *endPoint.ToString());
}


bool ACommonWeapon::_canFire() const
{
    if (!_weaponConfig) return false;

    // check current mag ammo instead of total ammo available
    // since total ammo can be non-zero but currently available is still zero
    if (_currentMagAmmoCount_ <= 0)
    {
        LOGObjName(this, LogTemp, Display, "cannot fire: no ammo");
        return false;
    }

    return !_isOnCooldown;
}


void ACommonWeapon::_setupAttachments_() const
{
    magazineMesh->SetupAttachment(weaponMesh, _socketData.magazineMainSocket);
    scopeMesh->SetupAttachment(weaponMesh, _socketData.reflexSocket);
    collisionBox->SetupAttachment(weaponMesh);
    leftHandIK->SetupAttachment(weaponMesh, _socketData.leftHandTargetSocket);
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

