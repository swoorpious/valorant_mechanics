// © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Delegates/DelegateCombinations.h"
#include "AnimNotifier.generated.h"


DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCurrentAnimationDataChange, UWeaponAnimDataAsset*, UpdatedAnimAsset);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnWeaponStateChange, EWeaponState, wWaponState);

DECLARE_MULTICAST_DELEGATE_OneParam(OnAnimBegin, FName);


/**
 * 
 */
UCLASS()
class VALORANTMECHANICS_API UAnimNotifier : public UObject
{
    GENERATED_BODY()

public:
    
    UPROPERTY(BlueprintReadWrite, BlueprintReadWrite, BlueprintType, EditAnywhere, Category="Animations|Events", meta = (AllowPrivateAccess = true))
    FOnCurrentAnimationDataChange onCurrentAnimDataChange;

    UPROPERTY(BlueprintReadWrite, BlueprintReadWrite, BlueprintType, EditAnywhere, Category="Animations|Events", meta = (AllowPrivateAccess = true))
    FOnWeaponStateChange onWeaponStateChange;

    
    
};
