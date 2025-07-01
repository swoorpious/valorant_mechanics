// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "AnimNotifier.h"

void UAnimNotifier::TryUpdateAnimState(const EWeaponState newState)
{
    onStateChange.Broadcast(newState);
}
