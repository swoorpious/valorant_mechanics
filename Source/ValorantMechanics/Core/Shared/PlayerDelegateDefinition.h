// Copyright 2025 Swaroop. Personal Unreal engine project inspired BY VALORANT.


#pragma once

#include "CoreMinimal.h"
#include "WeaponProperties.h"
#include "PlayerStates.h"

class ACommonWeapon;
class UDataAsset;



/*
 * delegates called when the player presses corresponding key, or attempts to do certain action
 * only used to notify to try and "equip"
 */

// called when player tries to pick up said weapon
DECLARE_MULTICAST_DELEGATE_OneParam(RequestWeaponSpawn, TSubclassOf<ACommonWeapon>)

DECLARE_MULTICAST_DELEGATE_OneParam(RequestWeaponEquip, EWeaponType)
DECLARE_MULTICAST_DELEGATE_OneParam(RequestWeaponDrop, EWeaponType)

/*
 * spawning a TSubclassOf<ACommonWeapon> updates the following:
 *  - adds/updates entry for corresponding EWeaponType in FPlayerInventory
 *  - anim data asset for the weapon in UVal_WeaponAnimInstance (TODO: create UVal_WeaponAnimInstance)
 */
DECLARE_MULTICAST_DELEGATE_OneParam(OnWeaponSpawned, ACommonWeapon*)


/*
 * called after equipping is successful
 * 
 * equipping a TSubclassOf<ACommonWeapon> updates the following things:
 *  - current weapon type in FPlayerInventory and UVal_AnimInstance
 *  - update EWeaponLogicState in the corresponding weapon class to EWeaponLogicState::Equipping 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(OnWeaponEquipped, EWeaponType)


/*
 * dropping a TSubclassOf<ACommonWeapon> updates the following:
 *  - removes entry for corresponding EWeaponType in FPlayerInventory
 *  - empties the anim data asset for the player in UVal_PlayerAnimInstance
 *  - empties the anim data asset for the weapon in UVal_WeaponAnimInstance
 */
DECLARE_MULTICAST_DELEGATE_OneParam(OnWeaponDropped, EWeaponType)




DECLARE_MULTICAST_DELEGATE_TwoParams(OnMovementStateChanged, EMovementState /* old state */, EMovementState /* new state */);
DECLARE_MULTICAST_DELEGATE_TwoParams(OnWeaponLogicStateChanged, EWeaponLogicState /* old state */, EWeaponLogicState /* new state */);

DECLARE_MULTICAST_DELEGATE_TwoParams(OnMovementAnimStateChanged, EMovementState /* old state */, EMovementState /* new state */);
DECLARE_MULTICAST_DELEGATE_TwoParams(OnWeaponAnimStateChanged, EWeaponAnimState /* old state */, EWeaponAnimState /* new state */);


















