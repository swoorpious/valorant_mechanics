// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "ValorantMechanics/Core/Shared/SocketData.h"
#include "ValorantMechanics/Core/Shared/WeaponProperties.h"
#include "ValorantMechanics/Core/Shared/WeaponAnimDataAsset.h"
#include "ValorantMechanics/Core/StateManager/StateManagerOwner.h"
#include "WeaponLogicStateManager.h"
#include "WeaponAnimStateManager.h"

#include "CommonWeapon.generated.h"


class USkeletalMeshComponent;
class UStaticMeshComponent;
class UBoxComponent;



UCLASS()
class VALORANTMECHANICS_API ACommonWeapon : public AActor, public PrimaryStateManagerOwner<UWeaponLogicStateManager>, public SecondaryStateManagerOwner<UWeaponAnimStateManager>
{
    GENERATED_BODY()

public:
    ACommonWeapon();

    virtual void Fire() {}
    virtual void Reload() {}
    virtual void Equip(EEquipType EquipType) {}
    virtual EWeaponType GetWeaponType();
    virtual EWeaponLogicState GetWeaponState();
    virtual EWeaponPickupType GetWeaponPickupType();
    virtual TObjectPtr<UWeaponAnimDataAsset>& GetAnimAsset();
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Body")
    TObjectPtr<USkeletalMeshComponent> weaponMesh;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Body")
    TObjectPtr<UStaticMeshComponent> magazineMesh;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Body")
    TObjectPtr<UStaticMeshComponent> scopeMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Body")
    TObjectPtr<UBoxComponent> collisionBox;
    

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon") FDefaultWeaponProperties defaultProperties;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon") FAltWeaponProperties altProperties;



protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    EWeaponType weaponType; // variables initialized in derived class constructors
    // EWeaponLogicState weaponState;
    EWeaponPickupType weaponPickupType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Animations", meta = (DisplayName = "Animation Data Asset"))
    TObjectPtr<UWeaponAnimDataAsset> animAsset;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WAnimDataAsseteapon|Mesh|Sockets")
    TObjectPtr<UWeaponSocketData> socketData;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|State Managers")
    TObjectPtr<UWeaponLogicStateManager> weaponLogicSM = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|State Managers")
    TObjectPtr<UWeaponAnimStateManager> weaponAnimSM = nullptr;

    
    
    
};
