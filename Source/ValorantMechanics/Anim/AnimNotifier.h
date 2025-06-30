// © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Delegates/DelegateCombinations.h"
#include "ValorantMechanics/Core/Shared/WeaponData/WeaponProperties.h"
#include "AnimNotifier.generated.h"


DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCurrentAnimationDataChange, UWeaponAnimDataAsset*, UpdatedAnimAsset);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnWeaponStateChange, EWeaponState, weaponState);

DECLARE_MULTICAST_DELEGATE_OneParam(OnStateChangeByAnim, EWeaponState);


/**
 * 
 */
UCLASS()
class VALORANTMECHANICS_API UAnimNotifier : public UObject
{
    GENERATED_BODY()

public:

    OnStateChangeByAnim onStateChange;
    
    // UPROPERTY(BlueprintReadWrite, BlueprintReadWrite, BlueprintType, EditAnywhere, Category="Animations|Events", meta = (AllowPrivateAccess = true))
    // FOnCurrentAnimationDataChange onCurrentAnimDataChange;
    //
    // UPROPERTY(BlueprintReadWrite, BlueprintReadWrite, BlueprintType, EditAnywhere, Category="Animations|Events", meta = (AllowPrivateAccess = true))
    // FOnWeaponStateChange onWeaponStateChange;

    UFUNCTION(BlueprintCallable, Category = "Animation Notifier")
    void TryUpdateAnimState(const EWeaponState newState);
    
};
