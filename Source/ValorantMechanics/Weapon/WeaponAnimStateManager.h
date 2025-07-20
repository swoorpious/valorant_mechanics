// © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ValorantMechanics/Core/StateManager/StateManager.h"
#include "ValorantMechanics/Core/Shared/PlayerStates.h"
#include "ValorantMechanics/Core/Shared/WeaponProperties.h"
#include "WeaponAnimStateManager.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VALORANTMECHANICS_API UWeaponAnimStateManager : public UActorComponent, public StateManager<EWeaponAnimState>
{
    GENERATED_BODY()

public:
    UWeaponAnimStateManager();
    void InitializeWeaponStateManager(FDefaultWeaponProperties* defaultProperties, FAltWeaponProperties* altProperties);
    
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction) override;

protected:
    virtual void OnUpdateState(EWeaponAnimState previousState, EWeaponAnimState enteredState) override;
    virtual void OnStackState(EWeaponAnimState stackedState) override;
    virtual void OnUnstackState(EWeaponAnimState unstackedState) override;

private:
    using states = EWeaponAnimState;
    using fsm = StateManager<states>;
};
