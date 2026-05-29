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

    UPROPERTY() EWeaponType equipped_weapon_type = EWeaponType::Empty; // set to EWeaponType::Empty by default, updated to whichever the player spawns with
    UPROPERTY() TMap<EWeaponType, ACommonWeapon*> inventory_map; // cannot have EWeaponType::Empty
	void validateInventory();

public:
    UVal_PlayerInventory();
	
    /*
     * adds the weapon to inventory
     *
     * does not update equippedWeaponType
     * updates if weapon of EWeaponType exists
     * otherwise adds weapon of EWeaponType
     */
    bool addWeaponToInventory(ACommonWeapon* weapon);
    bool removeWeaponFromInventory(EWeaponType weaponType);

    /*
     * only updates equippedWeaponType if weapon type exists in inventoryMap
     */
    void switchEquippedWeapon(EWeaponType weaponType);
	
    bool hasWeapon(const EWeaponType weaponType) const;
    const TMap<EWeaponType, ACommonWeapon*>& getInventory() const;
    ACommonWeapon* getWeaponByType(EWeaponType weaponType) const;
    ACommonWeapon* getEquippedWeapon() const;
	
};


