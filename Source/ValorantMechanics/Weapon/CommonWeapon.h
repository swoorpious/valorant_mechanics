// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#pragma once

#include "CoreMinimal.h"
#include "Val_WeaponFireConfig.h"

#include "GameFramework/Actor.h"
#include "ValorantMechanics/Core/Shared/SocketData.h"
#include "ValorantMechanics/Core/Shared/WeaponProperties.h"

#include "WeaponDelegates.h"

#include "CommonWeapon.generated.h"


class AVal_Character;
class UVal_LocalPlayerSubsystem;
class UVal_WeaponFireConfig;
class UVal_WeaponSFXConfig;
class UVal_WeaponAnimConfig;
class USoundCue;


UCLASS()
class VALORANTMECHANICS_API ACommonWeapon : public AActor
{
    GENERATED_BODY()

public:
    ACommonWeapon();

#pragma region UPROPERTY_DECLARATIONS
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Body")
    TObjectPtr<USkeletalMeshComponent> weaponMesh = nullptr;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Body")
    TObjectPtr<UStaticMeshComponent> magazineMesh = nullptr;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Body")
    TObjectPtr<UStaticMeshComponent> scopeMesh = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Body")
    TObjectPtr<class UBoxComponent> collisionBox = nullptr;


    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Particles")
    TObjectPtr<class UNiagaraSystem> bulletTracer = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Particles")
    TObjectPtr<UNiagaraSystem> muzzleParticle = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Particles")
    TObjectPtr<UNiagaraSystem> muzzleSmokeParticle = nullptr;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Particles")
    TObjectPtr<UNiagaraSystem> impactParticle = nullptr;


#pragma endregion UPROPERTY_DECLARATIONS
	
    /*
     * public getter functions
     */
    EWeaponType getWeaponType() const { return _weaponType; }
    EWeaponPickupType getWeaponPickupType() const { return _weaponPickupType; }
    UVal_WeaponAnimConfig* getAnimAsset() const { return _animConfig; }
    EFireMode getWeaponFireMode() const
    {
        return _weaponConfig ? _weaponConfig->fireMode : EFireMode::Manual;
    }
	
	OnWeaponStateChanged& getWeaponStateChangedDelegate() { return _stateChangeDelegate_; }
	
	
    bool canDrop() const { return _weaponPickupType != EWeaponPickupType::NonPickupable; }
	/*
	 * canAltFire: _weaponConfig->canAltFire
	 * canADS: _weaponConfig->canADS
	 */
	bool canAltFire() const { return _weaponConfig && _weaponConfig->canAltFire; }
	bool canADS() const { return _weaponConfig && _weaponConfig->canADS; }
	
	// fireStart() will only shoot one bullet for manual weapons
    virtual void fireStart();
    virtual void fireEnd();
	
	/*
	 * if the weapon has both canAltFire and canADS, the function will cause the weapon to switch to alt fire mode.
	 * if the weapons only has canAltFire, the function will just alt fire. 
	 */
    virtual void tryAltFire();
	
    virtual void tryWeaponReload();

    virtual bool tryWeaponPickUp(AVal_Character* ownerCharacter);
    virtual bool tryWeaponDrop();
	virtual bool trySwitchFireMode(EFireMode newMode);
    virtual void weaponEquip(EEquipType type);
    virtual void weaponUnequip();
    
protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
	
	/*
	 * can call multiple times in one fire shot
	 * for example a shot gun
	 */
	virtual void _shootBullet();
	
	bool _canFire() const;
	
	virtual void _onWeaponEquipped();

	/**
	 * _broadcastStateUpdate — the single choke-point for all weapon state changes.
	 *
	 * updates _weaponState, fires the weapon-level two-param delegate
	 * (old → new), and fires the character-level one-param delegate consumed
	 * by the AnimInstance. Always prefer this over setting _weaponState directly.
	 */
	void _broadcastStateUpdate(EWeaponState newState);

	/** Thin wrapper kept for backwards-compatibility with existing call-sites. */
	void _updateState(EWeaponState newState);

	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta=(DisplayName="Weapon Fire Config")) 
	TObjectPtr<UVal_WeaponFireConfig> _weaponConfig;
	
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (DisplayName = "SFX Data Asset"))
    TObjectPtr<UVal_WeaponSFXConfig> _sfxConfig;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (DisplayName = "Animation Data Asset"))
    TObjectPtr<UVal_WeaponAnimConfig> _animConfig;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Body|Sockets")
    FWeaponSocketData _socketData;

	
	/*
	 * properties meant to be set within the child classes 
	 * Melee, Secondary, and Primary weapon type only
	 */
	EWeaponType _weaponType; // variables initialized in derived class constructors
	EWeaponPickupType _weaponPickupType;
	EWeaponState _weaponState = EWeaponState::None;
	
	bool _isADS = false;
	bool _isOnCooldown = false;
	bool _isFireHeld = false;
	
	// exponential function accumulates heat per bullet fire
	int _heatAccumulated = 0;

private:
	FTimerHandle _timerHandle_handleRefire_;
	FTimerHandle _timerHandle_handleEquip_;
	
	UFUNCTION(CallInEditor, Category = "Weapon|Body")
	void _setupAttachments_() const;
	
    uint8 _currentMagAmmoCount_ = 0;
	uint8 _currentMagCount_ = 0;
    uint8 _totalAmmoCount_ = 0;
	float _timeBetweenConsecutiveShots_ = 0.f;
    
	OnWeaponStateChanged _stateChangeDelegate_;

    UPROPERTY() TObjectPtr<AVal_Character> _ownerCharacter_ = nullptr;
    // UPROPERTY() TObjectPtr<UVal_LocalPlayerSubsystem> _localSubsystem_ = nullptr;
	
};