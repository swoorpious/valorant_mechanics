// © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "WeaponProperties.h"

class ACommonWeapon;

using namespace Weapon;



/*
 * delegates called when the player presses corresponding key, or attempts to do certain action
 * only used to notify to try and "equip"
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(TryWeaponSpawn, TSubclassOf<ACommonWeapon>, EquippedWeapon)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(TryWeaponEquip, EWeaponType, WeaponType)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(TryWeaponDrop, EWeaponType, WeaponType)



/*
 * called after equipping is successful
 * 
 * equipping a TSubclassOf<ACommonWeapon> updates the following things:
 *  - current weapon type in FPlayerInventory and UVal_AnimInstance
 *  - update EWeaponState in the corresponding weapon class to EWeaponState::Equipping 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(OnWeaponEquip, EWeaponType, WeaponType)


/*
 * spawning a TSubclassOf<ACommonWeapon> updates the following:
 *  - adds/updates entry for corresponding EWeaponType in FPlayerInventory
 *  - anim data asset for the player in UVal_PlayerAnimInstance (TODO: rename UVal_AnimInstance to UVal_PlayerAnimInstance)
 *  - anim data asset for the weapon in UVal_WeaponAnimInstance (TODO: create UVal_WeaponAnimInstance)
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(OnWeaponSpawn, TSubclassOf<ACommonWeapon>, EquippedWeapon)


/*
 * dropping a TSubclassOf<ACommonWeapon> updates the following:
 *  - removes entry for corresponding EWeaponType in FPlayerInventory
 *  - empties the anim data asset for the player in UVal_PlayerAnimInstance
 *  - empties the anim data asset for the weapon in UVal_WeaponAnimInstance
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(OnWeaponDrop, EWeaponType, WeaponType)
