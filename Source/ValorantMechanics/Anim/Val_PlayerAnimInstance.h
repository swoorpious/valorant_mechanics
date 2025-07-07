// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Delegates/DelegateCombinations.h"
#include "ValorantMechanics/Core/Shared/PlayerStates.h"
#include "ValorantMechanics/Core/Shared/WeaponProperties.h"
#include "Val_PlayerAnimInstance.generated.h"


class ACommonWeapon;
class UWeaponAnimDataAsset;
class UAnimNotifier;



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
    
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Valorant Animations|Weapon")
    EWeaponType currentWeaponType;
    
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
    const TObjectPtr<UWeaponAnimDataAsset>& GetAnimDataAsset(EWeaponType weaponType);
	bool HasAnimDataForType(EWeaponType weaponType) const;

    
   
    UFUNCTION(BlueprintType, BlueprintPure, Category = "Valorant Animations|Animation Data assets")
    UWeaponAnimDataAsset* GetCurrentAnimDataAsset();

#pragma endregion ANIM DATA
    

    UFUNCTION(BlueprintType, BlueprintPure, Category = "Valorant Animations|Animation Data assets", meta = (BlueprintThreadSafe))
    bool CanTransitionToMovementAnimState(EMovementState state) const { return mState == state; }
    
    UFUNCTION(BlueprintType, BlueprintPure, Category = "Valorant Animations|Animation Data assets", meta = (BlueprintThreadSafe))
    bool CanTransitionToWeaponAnimState(EWeaponAnimState state) const { return false; }
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Valorant Animations|Notifier")
    TObjectPtr<UAnimNotifier> notifier;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Animations|Player States", meta = (DisplayName = "Player State Machine Name"))
    FName pStateMachineName;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Animations|Player States", meta = (DisplayName = "Weapon State Machine Name"))
    FName wStateMachineName;
    
    void UpdateWeaponLogicState(EWeaponLogicState oldState, EWeaponLogicState newState) { wState = newState; }
    void UpdateMovementState(EMovementState oldState, EMovementState newState) { mState = newState; }
    
protected:
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Animations")
    FAnimAssets animAssets;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Animations|Player States", meta = (DisplayName = "Weapon State"))
    EWeaponLogicState wState;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Animations|Player States", meta = (DisplayName = "Movement State"))
    EMovementState mState;


private:

    FName GetCurrentStateNameFromStateMachine(FName stateMachineName);
    FName lastPlayerStateMachineStateName;
};
