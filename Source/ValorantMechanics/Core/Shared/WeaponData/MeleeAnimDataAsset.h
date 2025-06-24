// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MeleeAnimDataAsset.generated.h"

#define CategoryName

/**
 * list of melee animations (ripped from VALORANT)
 * - Aim2E
 * - Aim2N
 * - Aim2S
 * - Aim2W
 * - AimE
 * - AimN
 * - AimS
 * - AimW
 * - Attack1
 * - Attack2
 * - Attack3
 * - Attack4
 * - Attack1_Modes
 * - Attack2_Modes
 * - Attack3_Modes
 * - Attack4_Modes
 * - AltAttack
 * - CrouchIn
 * - CrouchOut
 * - Equip_Modes
 * - EquipFast
 * - EquipFast_Modes
 * - IdleAdd
 * - IdleAdd_Modes
 * - IdlePose
 * - IdlePose_Modes
 * - Inspect
 * - Inspect_Modes
 * - JumpAdd
 * - JumpLandAdd
 * - RunAdd
 * - WalkAdd
 */
UCLASS()
class VALORANTMECHANICS_API UMeleeAnimDataAsset : public UDataAsset
{
public:
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations|Attack", meta = (DisplayName= "Attack Variations Array"))
    TArray<TObjectPtr<UAnimSequence>> Attack; // Attack1, Attack2, Attack3, Attack4
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations|Attack", meta = (DisplayName= "Alternate Attack"))
    TObjectPtr<UAnimSequence> AltAttack;
    

    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations")
    TObjectPtr<UAnimSequence> Equip;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations")
    TObjectPtr<UAnimSequence> EquipFast;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations")
    TObjectPtr<UAnimSequence> IdleAnimation;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations|Poses")
    TObjectPtr<UAnimSequence> IdlePose;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations")
    TObjectPtr<UAnimSequence> InspectLoop;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations")
    TObjectPtr<UAnimSequence> Jump;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations")
    TObjectPtr<UAnimSequence> Run;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animations")
    TObjectPtr<UAnimSequence> Walk;

};
