// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

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
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations|Configuration")
    bool hasAltAttack = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations|Configuration")
    bool hasWeaponAnimations = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations|Configuration")
    bool canReload = false;

    // what weapon type is this data asset for
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations|Configuration")
    EWeaponType weaponType;
    
    

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations|Pose")
    TObjectPtr<UAnimSequence> idlePose;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations|Pose", meta = (DisplayName= "Alternate Idle Pose"))
    TObjectPtr<UAnimSequence> altIdlePose;



    /*
     * unlike attackArray (regular attack animations) alt attack is one single animation
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations|Alternate", meta = (DisplayName= "Alternate Attack", EditCondition = "hasAltAttack", EditConditionHides))
    TObjectPtr<UAnimSequence> altAttack;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations|Alternate", meta = (DisplayName= "Alternate Idle Animation", EditCondition = "hasAltAttack", EditConditionHides))
    TObjectPtr<UAnimSequence> altIdleAnimation;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations|Alternate", meta = (DisplayName= "Alternate Jump", EditCondition = "hasAltAttack", EditConditionHides))
    TObjectPtr<UAnimSequence> altJump;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations|Alternate", meta = (DisplayName= "Alternate Run", EditCondition = "hasAltAttack", EditConditionHides))
    TObjectPtr<UAnimSequence> altRun;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations|Alternate", meta = (DisplayName= "Alternate Walk", EditCondition = "hasAltAttack", EditConditionHides))
    TObjectPtr<UAnimSequence> altWalk;



    /*
     * an attack animation will be chosen at random out of this array if there are multiple attack animations
     * weapon fire animation for guns is included in this array
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations", meta = (DisplayName = "Attack Animations"))
    TArray<TObjectPtr<UAnimSequence>> attackArray; // Attack1, Attack2, Attack3, Attack4
    
  
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations")
    TObjectPtr<UAnimSequence> equip;
    

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations")
    TObjectPtr<UAnimSequence> equipFast;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations")
    TObjectPtr<UAnimSequence> idleAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations")
    TObjectPtr<UAnimSequence> inspectLoop;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations", meta = (EditCondition = "canReload", EditConditionHides))
    TObjectPtr<UAnimSequence> reload;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations")
    TObjectPtr<UAnimSequence> jump;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations")
    TObjectPtr<UAnimSequence> run;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations")
    TObjectPtr<UAnimSequence> walk;


    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Animations", meta = (EditCondition="hasWeaponAnimations", EditConditionHides))
    TObjectPtr<UAnimSequence> weaponEquip;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Animations", meta = (EditCondition="hasWeaponAnimations", EditConditionHides))
    TObjectPtr<UAnimSequence> weaponFire;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Animations", meta = (EditCondition="hasWeaponAnimations", EditConditionHides))
    TObjectPtr<UAnimSequence> weaponReload;

#pragma endregion ANIMATIONS

    UFUNCTION(BlueprintPure, Category = "Aniamtions Utils")
    FORCEINLINE UAnimSequence* GetRandomAttackAnimation(const bool usingAltFire = false) const
    {
        if (usingAltFire) return altAttack;
    
        if (attackArray.Num() == 0) return nullptr;
        if (attackArray.Num() == 1) return attackArray[0];
    
    
        const int32 RandomIndex = FMath::RandRange(0, attackArray.Num() - 1);
        return attackArray[RandomIndex];
    }
    
};
