// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "ValorantMechanics/Core/Shared/WeaponProperties.h"


#include "Val_AnimInstance.generated.h"


class ACommonWeapon;


USTRUCT(BlueprintType)
struct FAnimAssets
{
    GENERATED_BODY()

// private:


    TMap<EWeaponType, TObjectPtr<UDataAsset>> animDataAssets;
    
    // using TObjectPtr<UDataAsset> because melee and secondary and primary weapons types use subclasses of UDataAsset
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animations|Animation Data Assets", meta=(DisplayName = "Melee Animation Data Asset"))
    TObjectPtr<UDataAsset> meleeAnimAsset;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animations|Animation Data Assets", meta=(DisplayName = "Secondary Animation Data Asset"))
    TObjectPtr<UDataAsset> secondaryAnimAsset;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animations|Animation Data Assets", meta=(DisplayName = "Primary Animation Data Asset"))
    TObjectPtr<UDataAsset> primaryAnimAsset;

    
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animations", meta=(DisplayName = "Current Animation Data Asset"))
    TObjectPtr<UDataAsset> currentAnimDataAsset;

	bool HasAnimDataForType(EWeaponType weaponType) const { return animDataAssets.FindRef(weaponType) != nullptr; }
    
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

    // UFUNCTION(BlueprintCallable, Category = "Animations|Animation Data Assets")
    void UpdateAnimDataAsset(ACommonWeapon* equippedWeapon);
    
    // UFUNCTION(BlueprintCallable, Category = "Animations|Animation Data Assets")
    void UpdateCurrentWeapon(EWeaponType weaponType);
    
    // UFUNCTION(BlueprintCallable, Category = "Animations|Animation Data Assets")
    TObjectPtr<UDataAsset> GetAnimDataAsset(EWeaponType weaponType);
    //
    // UFUNCTION(BlueprintCallable, Category = "Animations|Animation Data Assets")
    // void RemoveAnimDataAsset(EWeaponType weaponType);
protected:
    

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapon")
    EWeaponType currentWeaponType;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations" )
    FAnimAssets animAssets;

    EWeaponState weaponState; // also player state

    
};
