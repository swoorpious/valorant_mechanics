// // Copyright 2025 Swaroop. Personal Unreal engine project inspired BY VALORANT.
//
//
// #pragma once
//
// #include "CoreMinimal.h"
// #include "Subsystems/LocalPlayerSubsystem.h"
//
// #include "Shared/PlayerDelegateDefinition.h"
//
// #include "Val_LocalPlayerSubsystem.generated.h"
//
// /**
//  * 
//  */
// UCLASS()
// class VALORANTMECHANICS_API UVal_LocalPlayerSubsystem : public ULocalPlayerSubsystem
// {
//     GENERATED_BODY()
//
// public:
//
// #pragma region STATE MACHINE
//
//     /*
//      * prefixes
//      * m -> movement
//      * w -> weapon
//      */
//
//     OnRequestedWeaponAnimStateChange& GetWeaponAnimStateChangeRequestDelegate();
//     OnMovementStateChanged& GetMovementStateChangeDelegate();
//
//
//     void UpdateMovementState(EMovementState oldState, EMovementState newState) const;
//     void UpdateWeaponLogicState(EWeaponLogicState oldState, EWeaponLogicState newState) const;
//
//     // update for when the anim sequence finishes playing
//     // can be used to check for time duration for actions, like equipping.
//     void UpdateMovementAnimState(EMovementState oldState, EMovementState newState) const;
//     void UpdateWeaponAnimState(EWeaponAnimState oldState, EWeaponAnimState newState) const;
//
//     
// #pragma endregion STATE MACHINE
//
// protected:
//     // may or may not be implemented
//     OnWeaponLogicStateChanged wLogicStateChanged;
//     OnMovementAnimStateChanged mAnimStateChanged;
//     OnWeaponAnimStateChanged wAnimStateChanged;
//
//     // currently implemented
//     OnMovementStateChanged mStateChanged;
//     OnRequestedWeaponAnimStateChange reqWeaponAnimStateChange;
// };
