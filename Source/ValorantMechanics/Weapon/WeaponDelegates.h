// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "ValorantMechanics/Core/Shared/WeaponProperties.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(OnWeaponStateChanged, EWeaponState /* old state */, EWeaponState /* new state */);