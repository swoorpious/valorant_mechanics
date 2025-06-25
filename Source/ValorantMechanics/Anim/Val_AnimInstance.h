// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Delegates/DelegateCombinations.h"
#include "ValorantMechanics/Core/Shared/WeaponData/WeaponProperties.h"
#include "Val_AnimInstance.generated.h"


class ACommonWeapon;
class UWeaponAnimDataAsset;
class UAnimNotifier;



USTRUCT(BlueprintType)
struct FAnimAssets
{
    GENERATED_BODY()
    

    TMap<EWeaponType, TObjectPtr<UWeaponAnimDataAsset>> animDataMap;
    
    // using TObjectPtr<UWeaponAnimDataAsset> because melee and secondary and primary weapons types use subclasses of UWeaponAnimDataAsset
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animations|Animation Data Assets", meta=(DisplayName = "Melee Animation Data Asset"))
    TObjectPtr<UWeaponAnimDataAsset> meleeAnimAsset;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animations|Animation Data Assets", meta=(DisplayName = "Secondary Animation Data Asset"))
    TObjectPtr<UWeaponAnimDataAsset> secondaryAnimAsset;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animations|Animation Data Assets", meta=(DisplayName = "Primary Animation Data Asset"))
    TObjectPtr<UWeaponAnimDataAsset> primaryAnimAsset;


    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animations|Animation Data Assets", meta=(DisplayName = "TEST Animation Data Asset"))
    TObjectPtr<UWeaponAnimDataAsset> testAnimDataAsset;
    
    
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animations|Animation Data Assets", meta=(DisplayName = "Current Animation Data Asset"))
    TObjectPtr<UWeaponAnimDataAsset> currentAnimDataAsset;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapon")
    EWeaponType currentWeaponType;

    
    
};


/**
 * 
 */
UCLASS()
class VALORANTMECHANICS_API UVal_AnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    virtual void NativeBeginPlay() override;
    
    void UpdateAnimDataAsset(ACommonWeapon* equippedWeapon);
    void UpdateCurrentWeapon(EWeaponType weaponType);
    void UpdateWeaponState(EWeaponState weaponState);
    
    TObjectPtr<UWeaponAnimDataAsset> GetAnimDataAsset(EWeaponType weaponType);

   
    void RemoveAnimDataAsset(EWeaponType weaponType);
	FORCEINLINE bool HasAnimDataForType(EWeaponType weaponType) const { return animAssets.animDataMap.FindRef(weaponType) != nullptr; }


    UFUNCTION(BlueprintType, BlueprintPure, Category = "Animations|Animation Data assets")
    FORCEINLINE EWeaponState GetWeaponState() const { return weaponState; }
    
    UFUNCTION(BlueprintType, BlueprintPure, Category = "Animations|Animation Data assets")
    FORCEINLINE UWeaponAnimDataAsset* GetCurrentAnimDataAsset() { return animAssets.currentAnimDataAsset; }

#if WITH_EDITOR
    
    UFUNCTION(BlueprintType, CallInEditor, Category = "Animations|Animation Data assets")
    FORCEINLINE void SetTestAnimDataAsset() { animAssets.currentAnimDataAsset = animAssets.testAnimDataAsset; }

#endif
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations|Notifier")
    TObjectPtr<UAnimNotifier> notifier;
    
protected:
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
    FAnimAssets animAssets;

    EWeaponState weaponState; // also player state

    
};
