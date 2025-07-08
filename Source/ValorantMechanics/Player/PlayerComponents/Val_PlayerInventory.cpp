// Copyright 2025 Swaroop. Personal Unreal engine project inspired BY VALORANT.

#include "Val_PlayerInventory.h"
#include "ValorantMechanics/Weapons/CommonWeapon.h"


UVal_PlayerInventory::UVal_PlayerInventory()
{
    inventoryMap.Add(EWeaponType::Empty, nullptr);
}


void UVal_PlayerInventory::UpdateInventoryWeapon(const TObjectPtr<ACommonWeapon>& weapon)
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

        if (key == EWeaponType::Empty) break;
        if (key != value->GetWeaponType()) this->DeleteWeaponByType(key);
    }
}


void UVal_PlayerInventory::UpdateEquippedWeapon(EWeaponType weaponType)
{
    if (!this->HasWeapon(weaponType)) return;
    equippedWeaponType = weaponType;
}


const TObjectPtr<ACommonWeapon> UVal_PlayerInventory::GetWeaponByType(EWeaponType weaponType) const
{
    if (weaponType == EWeaponType::Empty || !this->HasWeapon(weaponType)) return nullptr;
    return inventoryMap[weaponType];
}


void UVal_PlayerInventory::DropWeaponByType(EWeaponType weaponType)
{
    /*
     * TODO: implement and call weapon->Drop logic
     */

    this->DeleteWeaponByType(weaponType);
}


void UVal_PlayerInventory::DeleteWeaponByType(EWeaponType weaponType)
{
    if (weaponType == EWeaponType::Empty || !this->HasWeapon(weaponType)) return;
    inventoryMap[weaponType] = nullptr;
}

bool UVal_PlayerInventory::HasWeapon(const EWeaponType weaponType) const
{
    return inventoryMap.FindRef(weaponType) != nullptr;
}

const TObjectPtr<ACommonWeapon> UVal_PlayerInventory::GetEquippedWeapon() const
{
    return this->GetWeaponByType(equippedWeaponType);
}

const TMap<EWeaponType, TObjectPtr<ACommonWeapon>>& UVal_PlayerInventory::GetInventory() const
{
    return inventoryMap;
}
