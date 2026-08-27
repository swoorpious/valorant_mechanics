// © 2026 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Val_WeaponAnimConfig.generated.h"


UENUM()
enum EWeaponAnimType
{
    /* 
     * which type of anim it is 
     * used for when the anim bp requests animation to play
     * conditionally used inside logic
     */
};

/**
 * 
 */
UCLASS()
class VALORANTMECHANICS_API UVal_WeaponAnimConfig : public UDataAsset
{
    GENERATED_BODY()
    
public:
    
#pragma region WEAPON_ANIMATIONS
    /*
     * boolean properties only for data-only blueprint
     * these booleans are not used in code for logic.
     * we use values from UVal_WeaponFireConfig for logic instead
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Configuration")
    bool hasAltAttack = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Configuration")
    bool hasWeaponAnimations = false;
        
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Configuration")
    bool canReload = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Animations", meta = (EditCondition="hasWeaponAnimations", EditConditionHides))
    TObjectPtr<UAnimMontage> weaponEquip = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Animations", meta = (EditCondition="hasWeaponAnimations", EditConditionHides))
    TObjectPtr<UAnimMontage> weaponFire = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Animations", meta = (EditCondition="hasWeaponAnimations && canReload",  EditConditionHides))
    TObjectPtr<UAnimMontage> weaponReload = nullptr;
    
#pragma endregion WEAPON_ANIMATIONS
    
    
/*
 * weapon related character animations since each weapon has its own character specific set of animations/poses
 */
#pragma region CHARACTER_ANIMATIONS
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations|Pose")
    TObjectPtr<UAnimSequence> idlePose = nullptr;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations|Pose", meta = (DisplayName= "Alternate Idle Pose"))
    TObjectPtr<UAnimSequence> altIdlePose = nullptr;
    

    /*
     * unlike attackArray (regular attack animations) alt attack is one single animation
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations|Alternate", meta = (DisplayName= "Alternate Attack", EditCondition = "hasAltAttack", EditConditionHides))
    TObjectPtr<UAnimMontage> altAttack = nullptr;
    
    // following alternate animations states are only used when the equippable can ADS or scope in
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations|Alternate", meta = (DisplayName= "Alternate Idle Animation", EditCondition = "hasAltAttack", EditConditionHides))
    TObjectPtr<UAnimSequence> altIdleLoop = nullptr;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations|Alternate", meta = (DisplayName= "Alternate Jump", EditCondition = "hasAltAttack", EditConditionHides))
    TObjectPtr<UAnimSequence> altJump = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations|Alternate", meta = (DisplayName= "Alternate Jump Land", EditCondition = "hasAltAttack", EditConditionHides))
    TObjectPtr<UAnimSequence> altJumpLand = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations|Alternate", meta = (DisplayName= "Alternate Run", EditCondition = "hasAltAttack", EditConditionHides))
    TObjectPtr<UAnimSequence> altRun = nullptr;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations|Alternate", meta = (DisplayName= "Alternate Walk", EditCondition = "hasAltAttack", EditConditionHides))
    TObjectPtr<UAnimSequence> altWalk = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations|Alternate", meta = (DisplayName= "Alternate Crouch In", EditCondition = "hasAltAttack", EditConditionHides))
    TObjectPtr<UAnimSequence> altCrouchIn = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations|Alternate", meta = (DisplayName= "Alternate Crouch Out", EditCondition = "hasAltAttack", EditConditionHides))
    TObjectPtr<UAnimSequence> altCrouchOut = nullptr;



    /*
     * an attack animation will be chosen at random out of this array if there are multiple attack animations
     * weapon fire animation for guns is included in this array
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations", meta = (DisplayName = "Attack Animations"))
    TArray<TObjectPtr<UAnimMontage>> attackAnims; // Attack1, Attack2, Attack3, Attack4
    
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations")
    TObjectPtr<UAnimMontage> equipDefault = nullptr;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations")
    TObjectPtr<UAnimMontage> equipFast = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations")
    TObjectPtr<UAnimSequence> idleLoop = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations")
    TObjectPtr<UAnimMontage> inspectLoop = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations", meta = (EditCondition = "canReload", EditConditionHides))
    TObjectPtr<UAnimMontage> reload = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations")
    TObjectPtr<UAnimSequence> jump = nullptr;
 
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations")
    TObjectPtr<UAnimSequence> jumpLand = nullptr;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations")
    TObjectPtr<UAnimSequence> run = nullptr;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations")
    TObjectPtr<UAnimSequence> walk = nullptr;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations")
    TObjectPtr<UAnimSequence> crouchIn = nullptr;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Animations")
    TObjectPtr<UAnimSequence> crouchOut = nullptr;


#pragma endregion CHARACTER_ANIMATIONS

};
