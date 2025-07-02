// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "ValorantMechanics/Core/Shared/SocketData.h"
#include "ValorantMechanics/Core/Shared/WeaponData/WeaponProperties.h"
#include "ValorantMechanics/Core/Shared/WeaponData/WeaponAnimDataAsset.h"


#include "CommonWeapon.generated.h"


class USkeletalMeshComponent;
class UStaticMeshComponent;
class UBoxComponent;



UCLASS()
class VALORANTMECHANICS_API ACommonWeapon : public AActor
{
    GENERATED_BODY()

public:
    ACommonWeapon();

    virtual void Fire() {}
    virtual void Reload() {}
    virtual void Equip(EEquipType EquipType) {}
    FORCEINLINE virtual EWeaponType GetWeaponType() { return weaponType; }
    // FORCEINLINE virtual EWeaponState GetWeaponState() { return weaponState; }
    FORCEINLINE virtual EWeaponPickupType GetWeaponPickupType() { return weaponPickupType; }
    virtual TObjectPtr<UWeaponAnimDataAsset> GetAnimAsset() { return animAsset; }
    
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
    // EWeaponState weaponState;
    EWeaponPickupType weaponPickupType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Animations", meta = (DisplayName = "Animation Data Asset"))
    TObjectPtr<UWeaponAnimDataAsset> animAsset;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WAnimDataAsseteapon|Mesh|Sockets")
    TObjectPtr<UWeaponSocketData> socketData;
};
