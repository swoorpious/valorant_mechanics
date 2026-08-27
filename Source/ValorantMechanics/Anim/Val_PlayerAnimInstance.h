// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
// #include "AnimGraphNodes/AnimNode_CustomSequencePlayer.h"
#include "Delegates/DelegateCombinations.h"
#include "ValorantMechanics/Core/Shared/PlayerStates.h"
#include "ValorantMechanics/Core/Shared/WeaponProperties.h"
#include "Val_PlayerAnimInstance.generated.h"


class UVal_WeaponAnimConfig;
class UVal_CharacterMovementComponent;
class UVal_InputSystem;
class AVal_PlayerController;
class ACommonWeapon;
class UWeaponAnimDataAsset;
class UAnimNotifier;



/**
 * 
 */
UCLASS()
class VALORANTMECHANICS_API UVal_PlayerAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    virtual void NativeInitializeAnimation() override;
    virtual void NativeBeginPlay() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
    virtual void NativeUninitializeAnimation() override;

#pragma region ANIM DATA
    
    /*
     * using this function to use the current animation asset in implementing the BP
     * to test a custom anim asset in the BP we can set it on the _currAnimAsset
     */
    UFUNCTION(BlueprintType, BlueprintPure, Category = "Valorant Animations|Animation Data Asset", meta = (BlueprintThreadSafe))
    UVal_WeaponAnimConfig* getCurrentAnimAsset() { return _currAnimAsset.Get(); }


#pragma endregion ANIM DATA

    
protected:    
    UFUNCTION(BlueprintType, BlueprintPure, Category = "Valorant Animations|Animation Data Asset", meta = (BlueprintThreadSafe))
    bool _canTransitionToMovementAnimState(EMovementState state) const;
    
    UFUNCTION(BlueprintType, BlueprintPure, Category = "Valorant Animations|Animation Data Asset", meta = (BlueprintThreadSafe))
    bool _canTransitionToWeaponAnimState(EWeaponState state) const;
    
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Valorant Animations|Animation Data Asset", meta=(DisplayName = "Current Animation Data Asset"))
    TObjectPtr<UVal_WeaponAnimConfig> _currAnimAsset = nullptr;
    
    UPROPERTY(EditAnywhere, Transient, BlueprintReadOnly, Category = "Valorant Input")
    FVector2D _lastLookVector = FVector2D::ZeroVector;

    UPROPERTY(EditAnywhere, Transient, BlueprintReadOnly, Category = "Valorant Input", meta = (DisplayName = "Player Velocity Vector"))
    FVector _playerVelocity = FVector::ZeroVector;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Animations|Player States", meta = (DisplayName = "Movement State"))
    EMovementState _movementState;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Animations|Player States", meta = (DisplayName = "Weapon State"))
    EWeaponState _weaponState;

private:
    
    void _updateWeaponAnimAsset(UVal_WeaponAnimConfig* newAnimConfig);
    void _updateWeaponStateChange(const EWeaponState newState);
    void _playWeaponStateBasedMontage();
    
    UPROPERTY() TObjectPtr<UVal_InputSystem> valInput = nullptr;

    FName lastPlayerStateMachineStateName;

};