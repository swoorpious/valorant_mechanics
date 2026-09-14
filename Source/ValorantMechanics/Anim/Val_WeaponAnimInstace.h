// © 2026 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ValorantMechanics/Core/Shared/WeaponProperties.h"
#include "Val_WeaponAnimInstace.generated.h"


class UVal_WeaponAnimConfig;

/**
 * 
 */
UCLASS()
class VALORANTMECHANICS_API UVal_WeaponAnimInstace : public UAnimInstance
{
    GENERATED_BODY()
    
public:
    virtual void NativeInitializeAnimation() override;
    virtual void NativeBeginPlay() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
    virtual void NativeUninitializeAnimation() override;

    void updateWeaponStateChange(const EWeaponState newState);

#pragma region ANIM DATA
    
    /*
     * we won't need to use this function in weapon ABP for most part since the weapon only plays montages
     */
    UFUNCTION(BlueprintType, BlueprintPure, Category = "Valorant Animations|Animation Data Asset", meta = (BlueprintThreadSafe))
    UVal_WeaponAnimConfig* getCurrentAnimAsset() { return _animAsset.Get(); }

#pragma endregion ANIM DATA

protected:

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Valorant Animations|Animation Data Asset", meta=(DisplayName = "Current Animation Data Asset"))
    TObjectPtr<UVal_WeaponAnimConfig> _animAsset = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Animations|Player States", meta = (DisplayName = "Weapon State"))
    EWeaponState _weaponState;

    void _playWeaponStateBasedMontage();
};
