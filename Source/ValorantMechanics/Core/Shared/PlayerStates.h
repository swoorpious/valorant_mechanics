// © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStates.generated.h"

/*
 * every state related to player's basic kinematic movement
 *
 * EMovementState is meant to be used by the Val_CharacterMovementComponent
 * UsingAbilityMovement indicates that the player character is now using movement altered by an ability
 */
UENUM(BlueprintType)
enum class EMovementState : uint8
{
    None, 
    Idle,
    Walk,
    Run,
    Jump_Up,
    Jump_Fall,

    // stackable states
    Crouched,
    LightStunned,
    MediumStunned,
    HeavyStunned,

    UsingAbilityMovement,

    /*
     * TODO: implement a jumping system to keep track of players jump height
     * allow queuing bunnyhop jumps certain distance above the ground before landing
     *
     * jump system shall also keep track of players jumping z-coordinate and landing z-coordinate to determine light/medium/heavy stuns
     */
    
    // crouching is detected by bIsCrouched, it will reduce the current MaxWalkSpeed by 33%
};


