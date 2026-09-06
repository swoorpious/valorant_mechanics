// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Val_WeaponFireConfig.h"
#include "ValorantMechanics/Core/Shared/SocketData.h"
#include "ValorantMechanics/Core/Shared/WeaponProperties.h"
#include "WeaponDelegates.h"

#include "UObject/Interface.h"

#include "CommonWeapon.generated.h"


class AVal_Character;
class UVal_LocalPlayerSubsystem;
class UVal_WeaponFireConfig;
class UVal_WeaponSFXConfig;
class UVal_WeaponAnimConfig;
class USoundCue;



UINTERFACE(MinimalAPI, Blueprintable)
class UWeaponInputReceiver : public UInterface
{
    GENERATED_BODY()
};


class IWeaponInputReceiver
{
    GENERATED_BODY()
    
public:
    
    virtual void fireStart() = 0; // primary action key down
    virtual void fireEnd() = 0; // primary action key up
    
    virtual void altFireStart() = 0; // secondary action key down
    virtual void altFireEnd() = 0; // secondary action key up
    
    virtual void tryWeaponReload() = 0 ;

    virtual bool tryWeaponPickUp(AVal_Character* ownerCharacter) = 0;
    virtual bool tryWeaponDrop() = 0;
    virtual bool trySwitchFireMode(EFireMode newMode) = 0;
};




UCLASS()
class VALORANTMECHANICS_API ACommonWeapon : public AActor, public IWeaponInputReceiver
{
    GENERATED_BODY()

public:
    ACommonWeapon();

#pragma region UPROPERTY_DECLARATIONS
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Body")
    TObjectPtr<USkeletalMeshComponent> weaponMesh = nullptr;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Body")
    TObjectPtr<USceneComponent> leftHandIK;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Body")
    TObjectPtr<UStaticMeshComponent> magazineMesh = nullptr;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Body")
    TObjectPtr<UStaticMeshComponent> scopeMesh = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon|Body", meta = (AllowPrivateAccess = "true"))
    float targetFOV = 60.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon|Body", meta = (AllowPrivateAccess = "true"))
    float targetRenderScaleInDepth = 0.1f;
    
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

#pragma region PUBLIC_GETTER_FUNCTIONS
    UFUNCTION(BlueprintType, BlueprintPure, Category="Weapon")
    USceneComponent* getLeftHandIKComponent() const;

    UFUNCTION(BlueprintType, BlueprintPure, Category="Weapon")
    UVal_WeaponFireConfig* getWeaponFireConfig() const;

    EWeaponType getWeaponType() const;
    EWeaponPickupType getWeaponPickupType() const;
    UVal_WeaponAnimConfig* getAnimAsset() const;
    EFireMode getWeaponFireMode() const;
    float getWeaponRunSpeed();
    float getWeaponWalkSpeed();

    bool canDrop() const { return _weaponPickupType != EWeaponPickupType::NonPickupable; }

    /*
     * canAltFire: _weaponConfig->canAltFire
     * canADS: _weaponConfig->canADS
     */
    bool canAltFire() const { return _weaponConfig && _weaponConfig->canAltFire; }
    bool canADS() const { return _weaponConfig && _weaponConfig->canADS; }
#pragma endregion //PUBLIC_GETTER_FUNCTIONS

    // fireStart() will only shoot one bullet for manual weapons
    virtual void fireStart() override;
    virtual void fireEnd() override;

    /*
     * if the weapon has both canAltFire and canADS, the function will cause the weapon to switch to alt fire mode.
     * if the weapons only has canAltFire, the function will just alt fire. 
     */
    virtual void altFireStart() override { }
    virtual void altFireEnd() override { }

    virtual void tryWeaponReload() override;

    virtual bool tryWeaponPickUp(AVal_Character* ownerCharacter) override;
    virtual bool tryWeaponDrop() override;
    virtual bool trySwitchFireMode(EFireMode newMode) override;
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
    void _updateState(EWeaponState newState);

    /**
     * _broadcastAssetChanged — always fires OnWeaponChanged for this weapon's
     * _animConfig (even if null), unconditionally, so the AnimInstance's
     * cached anim asset is NEVER left stale/pointing at a previous weapon.
     * Always called first, immediately before _updateState(Equip_*), from
     * weaponEquip() so both delegates fire as one paired, ordered unit.
     */
    void _broadcastAssetChanged();

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

    /*
     * true from the moment weaponEquip() is called until weaponUnequip() is called.
     * _updateState() uses this to refuse broadcasting non-None states for a weapon
     * that is no longer the one being equipped — this is what stops a stale/late
     * broadcast (e.g. a delayed timer callback) from one weapon overlapping with
     * the next weapon's own equip sequence.
     */
    bool _isEquipActive_ = false;
    
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
    
    UPROPERTY() TObjectPtr<AVal_Character> _ownerCharacter_ = nullptr;
    
    /*
     * methods/properties for rendering on top
     *
     * @param isPickup
     * if true, will apply targetFOV and targetRenderScaleInDepth
     * if false, will reset to 90.f, and 1.f
     */
    UPROPERTY()
    UMaterialInstanceDynamic* _midBody_ = nullptr;

    UPROPERTY()
    UMaterialInstanceDynamic* _midMag_ = nullptr;

    UPROPERTY()
    UMaterialInstanceDynamic* _midScope_ = nullptr;
    
    void _applyRenderOnTopParams_(bool isPickup);
    
    // UPROPERTY() TObjectPtr<UVal_LocalPlayerSubsystem> _localSubsystem_ = nullptr;
    
};

