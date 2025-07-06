// Copyright 2025 Swaroop. Personal Unreal engine project inspired BY VALORANT.


#include "Val_LocalPlayerSubsystem.h"

void UVal_LocalPlayerSubsystem::UpdateMovementState(EMovementState oldState, EMovementState newState) const
{
    mStateChanged.Broadcast(oldState, newState);
}

void UVal_LocalPlayerSubsystem::UpdateWeaponLogicState(EWeaponLogicState oldState, EWeaponLogicState newState) const
{
    wLogicStateChanged.Broadcast(oldState, newState);
}

void UVal_LocalPlayerSubsystem::UpdateMovementAnimState(EMovementState oldState, EMovementState newState) const
{
    mAnimStateChanged.Broadcast(oldState, newState);
}

void UVal_LocalPlayerSubsystem::UpdateWeaponAnimState(EWeaponAnimState oldState, EWeaponAnimState newState) const
{
    wAnimStateChanged.Broadcast(oldState, newState);
}
