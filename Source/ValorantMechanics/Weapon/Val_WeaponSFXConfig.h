// © 2026 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Val_WeaponSFXConfig.generated.h"

/**
 * 
 */
UCLASS()
class VALORANTMECHANICS_API UVal_WeaponSFXConfig : public UDataAsset
{
    GENERATED_BODY()
    
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Sounds", meta = (DisplayName = "Attack"))
    TArray<TObjectPtr<USoundBase>> attack;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Sounds", meta = (DisplayName = "Equip Default"))
    TObjectPtr<USoundBase> equip_default = nullptr;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Sounds", meta = (DisplayName = "Equip Fast"))
    TObjectPtr<USoundBase> equip_fast = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Sounds", meta = (DisplayName = "Reload", EditCondition= "canReload", EditConditionHides))
    TObjectPtr<USoundBase> reload = nullptr;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Sounds", meta = (DisplayName = "Magazine In", EditCondition= "canReload", EditConditionHides))
    TObjectPtr<USoundBase> mag_in = nullptr;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Sounds", meta = (DisplayName = "Magazine Out", EditCondition= "canReload", EditConditionHides))
    TObjectPtr<USoundBase> mag_out = nullptr;    

};
