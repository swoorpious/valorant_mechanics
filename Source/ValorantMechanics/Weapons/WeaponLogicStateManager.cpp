// © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "WeaponLogicStateManager.h"
#include "CommonWeapon.h"



UWeaponLogicStateManager::UWeaponLogicStateManager(const FObjectInitializer&)
{
    PrimaryComponentTick.bCanEverTick = true;
}


void UWeaponLogicStateManager::InitializeWeaponStateManager(FDefaultWeaponProperties* defaultProperties, FAltWeaponProperties* altProperties)
{
    const auto* def = defaultProperties;
    const auto* alt = altProperties;
    
    SetDefaultState(states::Idle);

    UnallowStateTransition(states::Equip_Default, {
        states::None,
        states::Equip_Fast,
        states::Reloading, 
        states::Fired, 
        states::FireCooldown, 
        states::Inspecting
    });

    UnallowStateTransition(states::Equip_Fast, {
        states::None,
        states::Equip_Fast,
        states::Reloading, 
        states::Fired, 
        states::FireCooldown, 
        states::Inspecting
    });

    UnallowStateTransition(states::Fired, {
        states::Equip_Default,
        states::Equip_Fast,
        states::Reloading,
        states::Inspecting
    });

    

    SetStateTickingEnabled(states::Equip_Default, true);
    SetStateTickingEnabled(states::Equip_Fast, true);
    SetStateTickingEnabled(states::Reloading, true);
    SetStateTickingEnabled(states::FireCooldown, true);
    SetStateTickingEnabled(states::Inspecting, true);

    SetStateNotificationTime(states::Equip_Default, def->equipTimeDefault); // convert to milliseconds
    SetStateNotificationTime(states::Equip_Fast, def->equipTimeFast);
    SetStateNotificationTime(states::Reloading, def->reloadTime > 0.0f);
    SetStateNotificationTime(states::FireCooldown,
        (def->fireRate > 0.0f) ?
        (1.0f / def->fireRate) :
        0.0f);
    
}


void UWeaponLogicStateManager::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    TickState(DeltaTime);
}



void UWeaponLogicStateManager::OnUpdateState(EWeaponLogicState previousState, EWeaponLogicState enteredState)
{
    UE_LOG(LogTemp, Display, TEXT("OnStateUpdated on class - %s"), *GetOwner()->GetName());

    
    // OnUpdateState(previousState, enteredState);
}

void UWeaponLogicStateManager::OnStackState(EWeaponLogicState stackedState)
{
    // OnStackState(stackedState);
}

void UWeaponLogicStateManager::OnUnstackState(EWeaponLogicState unstackedState)
{
    // OnUnstackState(unstackedState);
}
