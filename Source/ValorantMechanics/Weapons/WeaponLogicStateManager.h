// © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "ValorantMechanics/Core/StateManager/StateManager.h"
#include "ValorantMechanics/Core/Shared/PlayerStates.h"
#include "ValorantMechanics/Core/Shared/WeaponProperties.h"


#include "WeaponLogicStateManager.generated.h"



class ACommonWeapon;


/**
 * 
 */
UCLASS(ClassGroup=(StateManagerComponent), meta=(BlueprintSpawnableComponent))
class VALORANTMECHANICS_API UWeaponLogicStateManager : public UActorComponent, public StateManager<EWeaponLogicState>
{
    GENERATED_BODY()

public:
    explicit UWeaponLogicStateManager(const FObjectInitializer&);
    void InitializeWeaponStateManager(FDefaultWeaponProperties* defaultProperties, FAltWeaponProperties* altProperties);
    
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction) override;

protected:
    virtual void OnUpdateState(EWeaponLogicState previousState, EWeaponLogicState enteredState) override;
    virtual void OnStackState(EWeaponLogicState stackedState) override;
    virtual void OnUnstackState(EWeaponLogicState unstackedState) override;

private:
    using states = EWeaponLogicState;
    using fsm = StateManager<states>;
};
