// © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "WeaponProperties.h"

class ACommonWeapon;
class UDataAsset;



/*
 * delegates called when the player presses corresponding key, or attempts to do certain action
 * only used to notify to try and "equip"
 */

// called when player tries to pick up said weapon
DECLARE_MULTICAST_DELEGATE_OneParam(DRequestWeaponSpawn, TSubclassOf<ACommonWeapon>)

DECLARE_MULTICAST_DELEGATE_OneParam(DRequestWeaponEquip, EWeaponType)
DECLARE_MULTICAST_DELEGATE_OneParam(DRequestWeaponDrop, EWeaponType)

/*
 * spawning a TSubclassOf<ACommonWeapon> updates the following:
 *  - adds/updates entry for corresponding EWeaponType in FPlayerInventory
 *  - anim data asset for the player in UVal_PlayerAnimInstance (TODO: rename UVal_AnimInstance to UVal_PlayerAnimInstance)
 *  - anim data asset for the weapon in UVal_WeaponAnimInstance (TODO: create UVal_WeaponAnimInstance)
 */
DECLARE_MULTICAST_DELEGATE_OneParam(DSuccessWeaponSpawn, ACommonWeapon*)


/*
 * called after equipping is successful
 * 
 * equipping a TSubclassOf<ACommonWeapon> updates the following things:
 *  - current weapon type in FPlayerInventory and UVal_AnimInstance
 *  - update EWeaponState in the corresponding weapon class to EWeaponState::Equipping 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(DSuccessWeaponEquip, EWeaponType)


/*
 * dropping a TSubclassOf<ACommonWeapon> updates the following:
 *  - removes entry for corresponding EWeaponType in FPlayerInventory
 *  - empties the anim data asset for the player in UVal_PlayerAnimInstance
 *  - empties the anim data asset for the weapon in UVal_WeaponAnimInstance
 */
DECLARE_MULTICAST_DELEGATE_OneParam(DSuccessWeaponDrop, EWeaponType)
