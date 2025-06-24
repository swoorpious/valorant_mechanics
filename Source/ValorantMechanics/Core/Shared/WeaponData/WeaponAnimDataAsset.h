// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "WeaponAnimDataAsset.generated.h"




UCLASS()
class VALORANTMECHANICS_API UWeaponAnimDataAsset : public UDataAsset
{
public:
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations|Checks")
    bool hasAltAttack = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations|Checks")
    bool hasMultipleAttacks = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations|Checks")
    bool hasSingleAttack = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations|Checks")
    bool hasWeaponAnimations = false;
    
    

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations|Pose")
    TObjectPtr<UAnimSequence> IdlePose;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations|Pose", meta = (DisplayName= "Alternate Idle Pose"))
    TObjectPtr<UAnimSequence> AltIdlePose;


    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations|Alternate", meta = (DisplayName= "Alternate Attack", EditCondition = "hasAltAttack", EditConditionHides))
    TObjectPtr<UAnimSequence> AltAttack;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations|Alternate", meta = (DisplayName= "Alternate Idle Animation", EditCondition = "hasAltAttack", EditConditionHides))
    TObjectPtr<UAnimSequence> AltIdleAnimation;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations|Alternate", meta = (DisplayName= "Alternate Jump", EditCondition = "hasAltAttack", EditConditionHides))
    TObjectPtr<UAnimSequence> AltJump;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations|Alternate", meta = (DisplayName= "Alternate Run", EditCondition = "hasAltAttack", EditConditionHides))
    TObjectPtr<UAnimSequence> AltRun;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations|Alternate", meta = (DisplayName= "Alternate Walk", EditCondition = "hasAltAttack", EditConditionHides))
    TObjectPtr<UAnimSequence> AltWalk;



    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations", meta = (EditCondition = "hasSingleAttack", EditConditionHides))
    TObjectPtr<UAnimSequence> Attack;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations", meta = (EditCondition = "hasMultipleAttacks", EditConditionHides))
    TArray<TObjectPtr<UAnimSequence>> AttackArray; // Attack1, Attack2, Attack3, Attack4
    
  
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations")
    TObjectPtr<UAnimSequence> Equip;
    

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations")
    TObjectPtr<UAnimSequence> EquipFast;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations")
    TObjectPtr<UAnimSequence> Fire;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations")
    TObjectPtr<UAnimSequence> IdleAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations")
    TObjectPtr<UAnimSequence> InspectLoop;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations")
    TObjectPtr<UAnimSequence> Reload;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations")
    TObjectPtr<UAnimSequence> Jump;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations")
    TObjectPtr<UAnimSequence> Run;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations")
    TObjectPtr<UAnimSequence> Walk;


    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Animations", meta = (EditCondition="hasWeaponAnimations", EditConditionHides))
    TObjectPtr<UAnimSequence> WeaponEquip;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Animations", meta = (EditCondition="hasWeaponAnimations", EditConditionHides))
    TObjectPtr<UAnimSequence> WeaponFire;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Animations", meta = (EditCondition="hasWeaponAnimations", EditConditionHides))
    TObjectPtr<UAnimSequence> WeaponReload;

    
};
