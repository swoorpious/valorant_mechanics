// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
// #include "AnimGraphNodes/AnimNode_CustomSequencePlayer.h"
#include "Delegates/DelegateCombinations.h"
#include "ValorantMechanics/Core/Shared/PlayerStates.h"
#include "ValorantMechanics/Core/Shared/WeaponProperties.h"
#include "Val_PlayerAnimInstance.generated.h"


class AVal_Character;
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
    
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Valorant Animations", meta=(DisplayName = "Character Is Falling"))
    bool isFalling = false;
    
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Valorant Animations|Animation Data Asset", meta=(DisplayName = "Current Animation Data Asset"))
    TObjectPtr<UVal_WeaponAnimConfig> _currAnimAsset = nullptr;
    
    UPROPERTY(BlueprintReadOnly, Category = "Valorant Animations|Animation Data Asset", meta=(DisplayName = "Left Hand IK Aplha"))
    float _leftHandIKAlpha = 0;
    
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Valorant Animations|Animation Data Asset", meta=(DisplayName = "Character Is Holding Gun"))
    bool isHoldingGun = false;
    
    UPROPERTY(EditAnywhere, Transient, BlueprintReadOnly, Category = "Valorant Input")
    FVector2D _lastLookVector = FVector2D::ZeroVector;

    UPROPERTY(EditAnywhere, Transient, BlueprintReadOnly, Category = "Valorant Input|Arm Sway", meta = (DisplayName = "Final Arm Rotation"))
    FRotator _finalArmRotation = FRotator::ZeroRotator;

    UPROPERTY(EditAnywhere, Transient, BlueprintReadOnly, Category = "Valorant Input|Arm Sway", meta = (DisplayName = "Final Arm Offset"))
    FVector _finalArmOffset = FVector::ZeroVector;

    // how fast duck/lag interpolate toward their target each tick (used as a Lerp alpha, not a spring)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Valorant Input|Arm Sway")
    float _armSwayLerpSpeed = 10.f;

    // clamped duck offset (cm) applied downward on the clavicle when the player is moving
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Valorant Input|Arm Sway")
    float _maxDuckOffset = 1.5f;

    // speed (uu/s) at which duck reaches its max — tune to your walk/sprint speed
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Valorant Input|Arm Sway")
    float _duckSpeedForMax = 600.f;

    // clamped lag roll (degrees) applied when turning left/right
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Valorant Input|Arm Sway")
    float _maxLagRoll = 3.f;

    // range of _lastLookVector.X that maps to full lag — tune to your mouse sensitivity
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Valorant Input|Arm Sway")
    float _lookXRangeForMaxLag = 20.f;
    
    UPROPERTY(EditAnywhere, Transient, BlueprintReadOnly, Category = "Valorant Input", meta = (DisplayName = "Player Velocity Vector"))
    FVector _playerVelocity = FVector::ZeroVector;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Animations|Player States", meta = (DisplayName = "Movement State"))
    EMovementState _movementState;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Animations|Player States", meta = (DisplayName = "Weapon State"))
    EWeaponState _weaponState;

    UPROPERTY(BlueprintReadOnly, meta=(DisplayName = "Valorant Player Character"))
    TObjectPtr<AVal_Character> valPlayer = nullptr;

private:
    
    void _updateWeaponAnimAsset(UVal_WeaponAnimConfig* newAnimConfig);
    void _updateWeaponStateChange(const EWeaponState newState);
    void _updateMovementStateChange(const EMovementState newState);
    void _playWeaponStateBasedMontage();
    void _updateArmSway(float DeltaSeconds);
    
    UPROPERTY() TObjectPtr<UVal_InputSystem> valInput = nullptr;
    UPROPERTY() TObjectPtr<UVal_CharacterMovementComponent> valMovement = nullptr;
    

    FName lastPlayerStateMachineStateName;

};