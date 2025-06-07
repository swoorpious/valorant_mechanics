// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ValorantMechanics/Weapons/SharedWeapon.h"


#include "Val_AnimInstance.generated.h"



/**
 * 
 */
UCLASS()
class VALORANTMECHANICS_API UVal_AnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:

    
    
    UFUNCTION(BlueprintCallable, Category = "Animations|Animation Data Assets")
    void UpdateAnimDataAsset(EWeaponType WeaponType, UDataAsset* AnimDataAsset);
    
    UFUNCTION(BlueprintCallable, Category = "Animations|Animation Data Assets")
    void UpdateCurrentWeapon(EWeaponType WeaponType);
    
    
protected:
    
    // using TObjectPtr<UDataAsset> because melee and secondary and primary weapons types use subclasses of UDataAsset
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animations|Animation Data Assets", meta=(DisplayName = "Melee Animation Data Asset"))
    TObjectPtr<UDataAsset> meleeAnimDataAsset;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animations|Animation Data Assets", meta=(DisplayName = "Secondary Animation Data Asset"))
    TObjectPtr<UDataAsset> secondaryAnimDataAsset;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animations|Animation Data Assets", meta=(DisplayName = "Primary Animation Data Asset"))
    TObjectPtr<UDataAsset> primaryAnimDataAsset;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapon")
    EWeaponType currentWeaponType;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animations", meta=(DisplayName = "Current Animation Data Asset"))
    TObjectPtr<UDataAsset> currentAnimDataAsset;

    

    
};
