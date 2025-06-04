// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ValorantMechanics/Weapons/SharedWeapon.h"


#include "WushuAnimInstance.generated.h"



/**
 * 
 */
UCLASS()
class VALORANTMECHANICS_API UWushuAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

    UFUNCTION(BlueprintCallable, Category = "Wushu")
    void UpdateAnimDataAsset(EWeaponType WeaponType, UDataAsset* AnimDataAsset);
    
    
protected:
    // using TObjectPtr<UDataAsset> because melee and secondary and primary weapons types use subclasses of UDataAsset
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animations|Animation Data Assets", meta=(DisplayName = "Melee Animation Data Asset"))
    TObjectPtr<UDataAsset> MeleeAnimDataAsset;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animations|Animation Data Assets", meta=(DisplayName = "Secondary Animation Data Asset"))
    TObjectPtr<UDataAsset> SecondaryAnimDataAsset;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animations|Animation Data Assets", meta=(DisplayName = "Primary Animation Data Asset"))
    TObjectPtr<UDataAsset> PrimaryAnimDataAsset;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapon")
    EWeaponType CurrentWeaponType;

    

    
};
