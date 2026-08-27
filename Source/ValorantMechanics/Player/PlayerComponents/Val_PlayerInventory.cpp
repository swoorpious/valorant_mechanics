// Copyright 2025 Swaroop. Personal Unreal engine project inspired BY VALORANT.

#include "Val_PlayerInventory.h"
#include "ValorantMechanics/Weapon/CommonWeapon.h"


UVal_PlayerInventory::UVal_PlayerInventory()
{
    // idk why this exists here
    inventory_map.Add(EWeaponType::Empty, nullptr);
}


bool UVal_PlayerInventory::addWeaponToInventory(ACommonWeapon* weapon)
{
    if (!weapon) return false;
    const EWeaponType weaponType = weapon->getWeaponType();
    
    /*
     * for EWeaponType::Empty: we do not want to update any weapon of this type
     * 
     * for EWeaponType::Melee: we cannot drop or swap the melee, 
     * so even if we get the function called with it, we ignore
     */
    if (weaponType == EWeaponType::Empty) return false;
    
    if (hasWeapon(weaponType))
    {
        if (weaponType == EWeaponType::Melee) return false;
        
        inventory_map[weaponType] = weapon;
        return true;
    }

    // if melee not already in the inventory, we add it
    inventory_map.Add(weaponType, weapon);
    validateInventory();
    return true;
}

bool UVal_PlayerInventory::removeWeaponFromInventory(EWeaponType weaponType)
{
    if (weaponType == EWeaponType::Empty || !hasWeapon(weaponType)) return false;
    
    inventory_map[weaponType] = nullptr;
    return true;
}


void UVal_PlayerInventory::switchEquippedWeapon(EWeaponType weaponType)
{
    if (!hasWeapon(weaponType)) return;
    equipped_weapon_type = weaponType;
}


ACommonWeapon* UVal_PlayerInventory::getWeaponByType(EWeaponType weaponType) const
{
    if (weaponType == EWeaponType::Empty || !hasWeapon(weaponType)) return nullptr;
    return inventory_map[weaponType];
}


bool UVal_PlayerInventory::hasWeapon(const EWeaponType weaponType) const
{
    return inventory_map.FindRef(weaponType) != nullptr;
}

ACommonWeapon* UVal_PlayerInventory::getEquippedWeapon() const
{
    return getWeaponByType(equipped_weapon_type);
}

const TMap<EWeaponType, ACommonWeapon*>& UVal_PlayerInventory::getInventory() const
{
    return inventory_map;
}


/*
 * iterates over the current inventory
 * removes the weapon if key and value->type do not match
 */
void UVal_PlayerInventory::validateInventory()
{
    for (const auto& pair : getInventory())
    {
        const TObjectPtr<ACommonWeapon>& value = pair.Value;
        const EWeaponType& key = pair.Key;

        if (key == EWeaponType::Empty) break;
        if (key != value->getWeaponType()) removeWeaponFromInventory(key);
    }
}
