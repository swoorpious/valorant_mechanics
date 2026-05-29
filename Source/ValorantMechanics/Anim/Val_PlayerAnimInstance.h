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


USTRUCT(BlueprintType)
struct FWeaponAnimStates
{
    GENERATED_BODY()

    


};


USTRUCT(BlueprintType)
struct FAnimAssets
{
    GENERATED_BODY()
    
    TMap<EWeaponType, TObjectPtr<UVal_WeaponAnimConfig>> anim_map;

    /*
     * fallbackAnimDataAsset is used in case any key in animDataMap does not hold a valid animation data asset
     * this asset is also used when the current key value is EWeaponType::Empty
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Valorant Animations|Animation Data Assets", meta=(DisplayName = "Fallback Animation Data Asset"))
    TObjectPtr<UVal_WeaponAnimConfig> fallback_anim_asset;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Valorant Animations|Animation Data Assets", meta=(DisplayName = "Current Animation Data Asset"))
    TObjectPtr<UVal_WeaponAnimConfig> curr_anim_asset;
    
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
    virtual void NativeUninitializeAnimation() override;

#pragma region ANIM DATA
	
    void setAnimDataAsset(EWeaponType weapon_type, UVal_WeaponAnimConfig* anim_asset);
    void setCurrentWeapon(EWeaponType weapon_type);
    void removeAnimDataAsset(EWeaponType weapon_type);
    UVal_WeaponAnimConfig* getAnimAsset(EWeaponType weapon_type);



#pragma endregion ANIM DATA
	
	
    void updateWeaponAnimState(EWeaponType weaponType, EWeaponAnimState oldState, EWeaponAnimState newState);
    void updateMovementState(EMovementState oldState, EMovementState newState) { _movementState = newState; }
    
protected:
	bool _hasAnimDataForType(EWeaponType weapon_type) const;
	
	UFUNCTION(BlueprintType, BlueprintPure, Category = "Valorant Animations|Animation Data assets", meta = (BlueprintThreadSafe))
	bool _canTransitionToMovementAnimState(EMovementState state) const;
    
	UFUNCTION(BlueprintType, BlueprintPure, Category = "Valorant Animations|Animation Data assets", meta = (BlueprintThreadSafe))
	bool _canTransitionToWeaponAnimState(EWeaponState state) const;
    
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Valorant Animations|Animation Data Assets", meta=(DisplayName = "Current Animation Data Asset"))
	TObjectPtr<UVal_WeaponAnimConfig> _currAnimAsset;
    
    UPROPERTY(EditAnywhere, Transient, BlueprintReadOnly, Category = "Valorant Input")
    FVector2D _lastLookVector = FVector2D::ZeroVector;

    UPROPERTY(EditAnywhere, Transient, BlueprintReadOnly, Category = "Valorant Input", meta = (DisplayName = "Player Velocity Vector"))
    FVector _playerVelocity = FVector::ZeroVector;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Animations|Player States", meta = (DisplayName = "Movement State"))
    EMovementState _movementState;

	UPROPERTY(BlueprintReadOnly, Category = "Valorant Animations|Player States", meta = (DisplayName = "Weapon State"))
	EWeaponState _weaponState;

private:
	
	void _updateWeaponAnimAsset(UVal_WeaponAnimConfig* newAnimConfig);
	void _updateWeaponStateChange(const EWeaponState newState);
	
    UPROPERTY() TObjectPtr<UVal_InputSystem> valInput = nullptr;
    UPROPERTY() TObjectPtr<UVal_CharacterMovementComponent> pMovement= nullptr;

    FName lastPlayerStateMachineStateName;

};
