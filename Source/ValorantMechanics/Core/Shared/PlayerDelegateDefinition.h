// Copyright 2025 Swaroop. Personal Unreal engine project inspired BY VALORANT.


#pragma once

#include "CoreMinimal.h"
#include "WeaponProperties.h"
#include "PlayerStates.h"

class UVal_WeaponAnimConfig;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeaponChanged, UVal_WeaponAnimConfig*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeaponStateChanged, EWeaponState);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMovementStateChanged, EMovementState);
