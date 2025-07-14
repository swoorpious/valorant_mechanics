// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
// #include "AnimGraphNodes/AnimNode_CustomSequencePlayer.h"
#include "Delegates/DelegateCombinations.h"
#include "ValorantMechanics/Core/Shared/PlayerStates.h"
#include "ValorantMechanics/Core/Shared/WeaponProperties.h"
#include "Val_PlayerAnimInstance.generated.h"


class UVal_CharacterMovementComponent;
class UVal_InputSystem;
class AVal_PlayerController;
class ACommonWeapon;
class UWeaponAnimDataAsset;
class UAnimNotifier;


USTRUCT(BlueprintType)
struct FWeaponAnimStates
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Animations|Player States", meta = (DisplayName = "Melee Weapon State"))
    EWeaponAnimState melee;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Animations|Player States", meta = (DisplayName = "Secondary Weapon State"))
    EWeaponAnimState secondary;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Animations|Player States", meta = (DisplayName = "Primary Weapon State"))
    EWeaponAnimState primary;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Valorant Animations|Player States")
    EWeaponType currentWeaponType;

};


USTRUCT(BlueprintType)
struct FAnimAssets
{
    GENERATED_BODY()
    
    TMap<EWeaponType, TObjectPtr<UWeaponAnimDataAsset>> animDataMap;

    /*
     * fallbackAnimDataAsset is used in case any key in animDataMap does not hold a valid animation data asset
     * this asset is also used when the current key value is EWeaponType::Empty
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Valorant Animations|Animation Data Assets", meta=(DisplayName = "Fallback Animation Data Asset"))
    TObjectPtr<UWeaponAnimDataAsset> fallbackAnimDataAsset;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Valorant Animations|Animation Data Assets", meta=(DisplayName = "Current Animation Data Asset"))
    TObjectPtr<UWeaponAnimDataAsset> currentAnimDataAsset;
    
};





/**
 * 
 */
UCLASS()
class VALORANTMECHANICS_API UVal_PlayerAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    virtual void NativeBeginPlay() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;


#pragma region ANIM DATA
    
    void UpdateAnimDataAsset(ACommonWeapon* equippedWeapon);
    void UpdateCurrentWeapon(EWeaponType weaponType);
    void RemoveAnimDataAsset(EWeaponType weaponType);
    const TObjectPtr<UWeaponAnimDataAsset> GetAnimDataAsset(EWeaponType weaponType);
	bool HasAnimDataForType(EWeaponType weaponType) const;

    
    UFUNCTION(BlueprintType, BlueprintPure, Category = "Valorant Animations|Animation Data assets")
    UWeaponAnimDataAsset* GetCurrentAnimDataAsset();

#pragma endregion ANIM DATA
    
       
    UFUNCTION(BlueprintType, BlueprintPure, Category = "Valorant Animations|Animation Data assets", meta = (BlueprintThreadSafe))
    bool CanTransitionToMovementAnimState(EMovementState state) const;
    
    UFUNCTION(BlueprintType, BlueprintPure, Category = "Valorant Animations|Animation Data assets", meta = (BlueprintThreadSafe))
    bool CanTransitionToWeaponAnimState(EWeaponAnimState state) const;
    

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Animations|Player States", meta = (DisplayName = "Player State Machine Name"))
    FName pStateMachineName;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Animations|Player States", meta = (DisplayName = "Weapon State Machine Name"))
    FName wStateMachineName;
    
    void UpdateWeaponAnimState(EWeaponType weaponType, EWeaponAnimState oldState, EWeaponAnimState newState);
    void UpdateMovementState(EMovementState oldState, EMovementState newState) { mState = newState; }
    
protected:
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Animations")
    FAnimAssets animAssets;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Animations|Player States", meta=(DisplayName = "Weapon States"))
    FWeaponAnimStates wAnimStates;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Animations|Player States", meta = (DisplayName = "Movement State"))
    EMovementState mState;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly , Category = "Valorant Input")
    FVector2D lastLookVector = FVector2D::ZeroVector;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly , Category = "Valorant Input", meta = (DisplayName = "Player Velocity Vector"))
    FVector pVelocity = FVector::ZeroVector;


private:
    UPROPERTY() TObjectPtr<UVal_InputSystem> valInput = nullptr;
    UPROPERTY() TObjectPtr<UVal_CharacterMovementComponent> pMovement= nullptr;

    FName lastPlayerStateMachineStateName;


    
};
