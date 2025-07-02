// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Delegates/DelegateCombinations.h"
#include "ValorantMechanics/Core/Shared/WeaponData/WeaponProperties.h"
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
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animations|Animation Data Assets", meta=(DisplayName = "Fallback Animation Data Asset"))
    TObjectPtr<UWeaponAnimDataAsset> fallbackAnimDataAsset;
    
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapon")
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
    
    void UpdateAnimDataAsset(ACommonWeapon* equippedWeapon);
    void UpdateCurrentWeapon(EWeaponType weaponType);
    FORCEINLINE void UpdateWeaponState(EWeaponState newState) { weaponState = newState; }
    
    TObjectPtr<UWeaponAnimDataAsset> GetAnimDataAsset(EWeaponType weaponType);

   
    void RemoveAnimDataAsset(EWeaponType weaponType);
	FORCEINLINE bool HasAnimDataForType(EWeaponType weaponType) const { return animAssets.animDataMap.FindRef(weaponType) != nullptr; }


    UFUNCTION(BlueprintType, BlueprintPure, Category = "Animations|Animation Data assets")
    FORCEINLINE EWeaponState GetWeaponState() const { return weaponState; }
    
    UFUNCTION(BlueprintType, BlueprintPure, Category = "Animations|Animation Data assets")
    UWeaponAnimDataAsset* GetCurrentAnimDataAsset();

    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Animations|Notifier")
    TObjectPtr<UAnimNotifier> notifier;
    
protected:
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
    FAnimAssets animAssets;

    EWeaponState weaponState; // also player state

    
};
