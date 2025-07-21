// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponProperties.h"

#include "WeaponAnimDataAsset.generated.h"




UCLASS()
class VALORANTMECHANICS_API UWeaponAnimDataAsset : public UDataAsset
{
public:
    GENERATED_BODY()


#pragma region ANIMATIONS

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations|Pose")
    TObjectPtr<UAnimSequence> idlePose = nullptr;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations|Pose", meta = (DisplayName= "Alternate Idle Pose"))
    TObjectPtr<UAnimSequence> altIdlePose = nullptr;



    /*
     * unlike attackArray (regular attack animations) alt attack is one single animation
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations|Alternate", meta = (DisplayName= "Alternate Attack", EditCondition = "hasAltAttack", EditConditionHides))
    TObjectPtr<UAnimSequence> altAttack = nullptr;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations|Alternate", meta = (DisplayName= "Alternate Idle Animation", EditCondition = "hasAltAttack", EditConditionHides))
    TObjectPtr<UAnimSequence> altIdleAnimation = nullptr;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations|Alternate", meta = (DisplayName= "Alternate Jump", EditCondition = "hasAltAttack", EditConditionHides))
    TObjectPtr<UAnimSequence> altJump = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations|Alternate", meta = (DisplayName= "Alternate Run", EditCondition = "hasAltAttack", EditConditionHides))
    TObjectPtr<UAnimSequence> altRun = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations|Alternate", meta = (DisplayName= "Alternate Walk", EditCondition = "hasAltAttack", EditConditionHides))
    TObjectPtr<UAnimSequence> altWalk = nullptr;



    /*
     * an attack animation will be chosen at random out of this array if there are multiple attack animations
     * weapon fire animation for guns is included in this array
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations", meta = (DisplayName = "Attack Animations"))
    TArray<TObjectPtr<UAnimSequence>> attackArray; // Attack1, Attack2, Attack3, Attack4
    
  
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations")
    TObjectPtr<UAnimSequence> equip = nullptr;
    

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations")
    TObjectPtr<UAnimSequence> equipFast = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations")
    TObjectPtr<UAnimSequence> idleAnimation = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations")
    TObjectPtr<UAnimSequence> inspectLoop = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations", meta = (EditCondition = "canReload", EditConditionHides))
    TObjectPtr<UAnimSequence> reload = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations")
    TObjectPtr<UAnimSequence> jump = nullptr;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations")
    TObjectPtr<UAnimSequence> run = nullptr;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations")
    TObjectPtr<UAnimSequence> walk = nullptr;



    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Configuration")
    bool hasAltAttack = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Configuration")
    bool hasWeaponAnimations = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Configuration")
    bool canReload = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Configuration")
    EWeaponType weaponType;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Animations", meta = (EditCondition="hasWeaponAnimations", EditConditionHides))
    TObjectPtr<UAnimSequence> weaponEquip = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Animations", meta = (EditCondition="hasWeaponAnimations", EditConditionHides))
    TObjectPtr<UAnimSequence> weaponFire = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Animations", meta = (EditCondition="hasWeaponAnimations && canReload",  EditConditionHides))
    TObjectPtr<UAnimSequence> weaponReload = nullptr;

#pragma endregion ANIMATIONS


    
#pragma region SOUNDS

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Sounds", meta = (DisplayName = "Attack Sound Effects"))
    TArray<TObjectPtr<USoundBase>> attackSFX;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Sounds", meta = (DisplayName = "Equip Sound Effect"))
    TObjectPtr<USoundBase> equipSFX = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Sounds", meta = (DisplayName = "Reload Sound Effect", EditCondition= "canReload", EditConditionHides))
    TObjectPtr<USoundBase> reloadSFX = nullptr;    
    
#pragma endregion SOUNDS
    
    

    UFUNCTION(BlueprintPure, Category = "Val Aniamtions Utils")
    UAnimSequence* GetRandomAttackAnimation(const bool usingAltFire = false) const
    {
        if (usingAltFire) return altAttack;

        if (attackArray.Num() == 0) return nullptr;
    
        const int32 RandomIndex = FMath::RandRange(0, attackArray.Num() - 1);
        return attackArray[RandomIndex];
    }

    UFUNCTION(BlueprintPure, Category = "Val SFX Utils")
    USoundBase* GetRandomAttackSFX() const
    {
        if (attackSFX.Num() == 0) return nullptr;

        const int32 RandomIndex = FMath::RandRange(0, attackSFX.Num() - 1);
        return attackSFX[RandomIndex];
    }
    
};
