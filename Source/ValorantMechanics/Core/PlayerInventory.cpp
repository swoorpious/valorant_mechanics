// © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "PlayerInventory.h"
#include "ValorantMechanics/Weapons/CommonWeapon.h"
#include "Shared/WeaponData/WeaponAnimDataAsset.h"


UPlayerInventory::UPlayerInventory()
{
    inventoryMap.Add(EWeaponType::Empty, nullptr);
}


void UPlayerInventory::UpdateInventoryWeapon(const TObjectPtr<ACommonWeapon>& weapon)
{
    const EWeaponType weaponType = weapon->GetWeaponType();
    if (!weapon || weaponType == EWeaponType::Empty) return;
    this->HasWeapon(weaponType) ?
        inventoryMap[weaponType] = weapon :
        inventoryMap.Add(weaponType, weapon);

    // validate values to only hold correct weapon for corresponding weaponType
    for (const auto& pair : this->GetInventory())
    {
        const TObjectPtr<ACommonWeapon>& value = pair.Value;
        const EWeaponType& key = pair.Key;
        
        if (key != value->GetWeaponType()) this->DeleteWeaponByType(key);
    }
}


void UPlayerInventory::UpdateEquippedWeapon(EWeaponType weaponType)
{
    if (!this->HasWeapon(weaponType)) return;
    equippedWeaponType = weaponType;
}


TObjectPtr<ACommonWeapon> UPlayerInventory::GetWeaponByType(EWeaponType weaponType) const
{
    if (weaponType == EWeaponType::Empty || !this->HasWeapon(weaponType)) return nullptr;
    return inventoryMap[weaponType];
}


void UPlayerInventory::DropWeaponByType(EWeaponType weaponType)
{
    /*
     * TODO: implement and call weapon->Drop logic
     */

    this->DeleteWeaponByType(weaponType);
}


void UPlayerInventory::DeleteWeaponByType(EWeaponType weaponType)
{
    if (weaponType == EWeaponType::Empty || !this->HasWeapon(weaponType)) return;
    inventoryMap[weaponType] = nullptr;
}
