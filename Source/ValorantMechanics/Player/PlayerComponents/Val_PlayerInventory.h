// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ValorantMechanics/Core/Shared/WeaponProperties.h"

#include "Val_PlayerInventory.generated.h"



class ACommonWeapon;
class UWeaponAnimDataAsset;


/**
 * 
 */
UCLASS()
class VALORANTMECHANICS_API UVal_PlayerInventory : public UActorComponent
{
    GENERATED_BODY()

    UPROPERTY() EWeaponType equippedWeaponType = EWeaponType::Empty; // set to EWeaponType::Empty by default, updated to whichever the player spawns with
    UPROPERTY() TMap<EWeaponType, TObjectPtr<ACommonWeapon>> inventoryMap; // cannot have EWeaponType::Empty

public:
    UVal_PlayerInventory();

    
    /*
     * does not update equippedWeaponType
     * updates if weapon of EWeaponType exists
     * otherwise adds weapon of EWeaponType
     */
    void UpdateInventoryWeapon(const TObjectPtr<ACommonWeapon>& weapon);

    /*
     * only updates equippedWeaponType if weapon type exists in inventoryMap
     * use UpdateInventoryWeapon to add/update inventory slots
     */
    void UpdateEquippedWeapon(EWeaponType weaponType);
    const TObjectPtr<ACommonWeapon> GetWeaponByType(EWeaponType weaponType) const;

    /*
     * performs drop logic on the weapon then attempts to delete the weapon
     */
    void DropWeaponByType(EWeaponType weaponType);
    void DeleteWeaponByType(EWeaponType weaponType);
    
    bool HasWeapon(const EWeaponType weaponType) const;
    
    const TObjectPtr<ACommonWeapon> GetEquippedWeapon() const;
    const TMap<EWeaponType, TObjectPtr<ACommonWeapon>>& GetInventory() const;
};


