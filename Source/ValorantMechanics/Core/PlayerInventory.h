// © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Shared/WeaponData/WeaponProperties.h"

#include "PlayerInventory.generated.h"

class ACommonWeapon;
class UWeaponAnimDataAsset;


/**
 * 
 */
UCLASS(EditInlineNew, DefaultToInstanced)
class VALORANTMECHANICS_API UPlayerInventory : public UObject
{
    GENERATED_BODY()

    UPROPERTY() EWeaponType equippedWeaponType = EWeaponType::Empty; // set to EWeaponType::Empty by default, updated to whichever the player spawns with
    UPROPERTY() TMap<EWeaponType, TObjectPtr<ACommonWeapon>> inventoryMap; // cannot have EWeaponType::Empty

public:
    UPlayerInventory();

#pragma region WEAPONS TO SPAWN
    
    UPROPERTY(EditDefaultsOnly, Category = "Spawn Properties|Equipped Weapons")
    TSubclassOf<ACommonWeapon> meleeToSpawn = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "Spawn Properties|Equipped Weapons")
    TSubclassOf<ACommonWeapon> secondaryToSpawn = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "Spawn Properties|Equipped Weapons")
    TSubclassOf<ACommonWeapon> primaryToSpawn = nullptr;
    
#pragma endregion WEAPONS TO SPAWN
    

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
    TObjectPtr<ACommonWeapon> GetWeaponByType(EWeaponType weaponType) const;

    /*
     * performs drop logic on the weapon then attempts to delete the weapon
     */
    void DropWeaponByType(EWeaponType weaponType);
    void DeleteWeaponByType(EWeaponType weaponType);
    
    bool HasWeapon(const EWeaponType weaponType) const { return inventoryMap.FindRef(weaponType) != nullptr; }
	
    FORCEINLINE TObjectPtr<ACommonWeapon> GetEquippedWeapon() const { return this->GetWeaponByType(equippedWeaponType); }
    FORCEINLINE TMap<EWeaponType, TObjectPtr<ACommonWeapon>> GetInventory() const { return inventoryMap; }
};


