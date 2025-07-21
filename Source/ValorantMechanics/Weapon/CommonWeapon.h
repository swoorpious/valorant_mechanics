// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "ValorantMechanics/Core/Shared/SocketData.h"
#include "ValorantMechanics/Core/Shared/WeaponProperties.h"
#include "ValorantMechanics/Core/Shared/WeaponAnimDataAsset.h"
#include "ValorantMechanics/Core/StateManager/StateManagerOwner.h"
#include "WeaponLogicStateManager.h"

#include "CommonWeapon.generated.h"


class AVal_Character;
class UVal_LocalPlayerSubsystem;



UCLASS()
class VALORANTMECHANICS_API ACommonWeapon : public AActor, public PrimaryStateManagerOwner<UWeaponLogicStateManager>
{
    GENERATED_BODY()

public:
    ACommonWeapon();


    
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
    

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon") FDefaultWeaponProperties defaultProperties;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon") FAltWeaponProperties altProperties;


    /*
     * public getter functions
     */
    virtual EWeaponType GetWeaponType();
    virtual EWeaponLogicState GetWeaponState();
    virtual EWeaponPickupType GetWeaponPickupType();
    virtual TObjectPtr<UWeaponAnimDataAsset>& GetAnimAsset();
    bool CanBeDropped() const;
    bool IsWeaponAutomatic() const;
    // virtual something GetWeaponAmmo();
    
    virtual void ExternFireStart();
    virtual void ExternFireEnd();
    virtual void ExternAltFireStart();
    virtual void ExternAltFireEnd();
    virtual void ExternReloadRequest();
    virtual void ExternFireTriggered();

    void ExternWeaponPickUp(AVal_Character* ownerCharacter);
    void ExternWeaponEquip();
    void ExternWeaponUnequip();
    void ExternWeaponDrop();
    
protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void StateUpdated(EWeaponLogicState oldState, EWeaponLogicState newState);


    virtual void FireShoot();
    virtual void InternalAltFireStart();
    virtual void InternalAltFireEnd();
    virtual void InternalReloadRequest();
    
    void InternalPickedUp(AVal_Character* ownerCharacter);
    void InternalEquipped();
    void InternalUnequipped();
    void InternalDropped();

    bool CanFire() const;
    

    EWeaponType weaponType; // variables initialized in derived class constructors
    EWeaponPickupType weaponPickupType;
    

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Animations", meta = (DisplayName = "Animation Data Asset"))
    TObjectPtr<UWeaponAnimDataAsset> animAsset;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Body|Sockets")
    FWeaponSocketData socketData;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|State Managers")
    TObjectPtr<UWeaponLogicStateManager> stateManager = nullptr;


private:

    bool isFireStarted = false;
    uint32 currentMagAmmoCount = 0;
    uint32 totalAmmoCount = 0;
    

    UPROPERTY() TObjectPtr<AVal_Character> pOwnerCharacter = nullptr;
    UPROPERTY() TObjectPtr<UVal_LocalPlayerSubsystem> pSubsystem = nullptr;
    
    friend UWeaponLogicStateManager;
    friend UWeaponAnimStateManager;
    
};
