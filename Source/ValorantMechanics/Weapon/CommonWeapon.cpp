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

void ACommonWeapon::tryAltFire()
{
}

void ACommonWeapon::tryWeaponReload()
{
	// only reload if the mag is not already full and we have spare mags
	if (!_weaponConfig) return;
	if (_currentMagCount_ == 0) return;
	if (_currentMagAmmoCount_ >= _weaponConfig->magSize) return;

	const int32 magCapacity   = _weaponConfig->magSize;
	const int32 ammoNeeded    = magCapacity - _currentMagAmmoCount_;
	const int32 ammoAvailable = _currentMagCount_ * magCapacity; // total reserve rounds

	// pull only what we need (or what's available) from the reserve
	const int32 ammoToAdd = FMath::Min(ammoNeeded, ammoAvailable);
	_currentMagAmmoCount_ += ammoToAdd;

	// deduct full mags consumed (ceiling division so a partial mag is still charged)
	const int32 magsUsed = FMath::DivideAndRoundUp(ammoToAdd, magCapacity);
	_currentMagCount_ = FMath::Max(0, _currentMagCount_ - magsUsed);

	// keep total in sync
	_totalAmmoCount_ = _currentMagAmmoCount_ + _currentMagCount_ * magCapacity;
}

bool ACommonWeapon::tryWeaponPickUp(AVal_Character* ownerCharacter)
{
	// if cannot drop, cannot pick up
	if (!canDrop()) return false;
	
	_ownerCharacter_ = ownerCharacter;
	return true;
}

bool ACommonWeapon::tryWeaponDrop()
{
	if (!canDrop()) return false;

	_ownerCharacter_ = nullptr;
	return true;
}

// TODO implement
bool ACommonWeapon::trySwitchFireMode(EFireMode newMode)
{
	return false;
}

void ACommonWeapon::weaponEquip(EEquipType type)
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorHiddenInGame(false);
	
	// set correct equip state BEFORE starting the timer, and don't call
	// _onWeaponEquipped() immediately — let the timer do it.
	if (type == EEquipType::EquipFast)
	{
		_broadcastStateUpdate(EWeaponState::Equip_Fast);
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
		_broadcastStateUpdate(EWeaponState::Equip_Default);
		GetWorldTimerManager().SetTimer(
			_timerHandle_handleEquip_,
			this, 
			&ACommonWeapon::_onWeaponEquipped,
			_weaponConfig ? _weaponConfig->equipTimeDefault : 0.1f,
			false
		);
	}
	
	// guard against null _ownerCharacter_ and null _animConfig before broadcasting
	if (_ownerCharacter_ && _animConfig)
	{
		_ownerCharacter_->getOnWeaponChangedDelegate().Broadcast(_animConfig);
	}
}

void ACommonWeapon::weaponUnequip()
{
	PrimaryActorTick.bCanEverTick = false;
	SetActorHiddenInGame(true);
	_isFireHeld = false;
	GetWorldTimerManager().ClearTimer(_timerHandle_handleEquip_);
	GetWorldTimerManager().ClearTimer(_timerHandle_handleRefire_);
	_broadcastStateUpdate(EWeaponState::None);
}

// Called when the game starts or when spawned
void ACommonWeapon::BeginPlay()
{
    Super::BeginPlay();
    
	if (_weaponConfig)
	{
	    _currentMagAmmoCount_ = static_cast<uint8>(FMath::Clamp(_weaponConfig->magSize,   0, 255));
		_currentMagCount_ = static_cast<uint8>(FMath::Clamp(_weaponConfig->magCount,  0, 255));
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
	_broadcastStateUpdate(EWeaponState::Idle);
	// whatever to do further	
}


void ACommonWeapon::_broadcastStateUpdate(EWeaponState newState)
{
	const EWeaponState oldState = _weaponState;
	_weaponState = newState;

	// fire the weapon-level delegate (two-param: old → new)
	_stateChangeDelegate_.Broadcast(oldState, newState);

	// fire the character-level delegate (one-param consumed by AnimInstance)
	if (_ownerCharacter_)
	{
		_ownerCharacter_->getOnWeaponStateChangedDelegate().Broadcast(newState);
	}
}

// Keep _updateState as a thin wrapper so existing call-sites compile.
void ACommonWeapon::_updateState(EWeaponState newState)
{
	_broadcastStateUpdate(newState);
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