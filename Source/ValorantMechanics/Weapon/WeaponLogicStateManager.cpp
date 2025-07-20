// © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "WeaponLogicStateManager.h"

#include "ValorantMechanics/Core/Val_LocalPlayerSubsystem.h"
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
        states::FireShot, 
        states::FireCooldown, 
        states::Inspecting
    });

    UnallowStateTransition(states::Equip_Fast, {
        states::None,
        states::Equip_Fast,
        states::Reloading, 
        states::FireShot, 
        states::FireCooldown, 
        states::Inspecting
    });

    UnallowStateTransition(states::FireShot, {
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

    /*  */
    SetStateNotificationTime(states::Equip_Default, def->equipTimeDefault); // convert to milliseconds
    SetStateNotificationTime(states::Equip_Fast, def->equipTimeFast);
    SetStateNotificationTime(states::Reloading, def->reloadTime > 0.0f ? def->reloadTime : 0.0f);
    SetStateNotificationTime(states::FireShot, 0.001f);
    SetStateNotificationTime(states::FireCooldown,
        (def->fireRate > 0.0f) ?
        (1.0f / def->fireRate) :
        0.0f);

    
    owner = Cast<ACommonWeapon>(GetOwner());

    // set initial state
    currentState = states::None;

    
    SetStateQueueWindow(EWeaponLogicState::Equip_Default, 0.1f);
    SetStateQueueWindow(EWeaponLogicState::Equip_Fast, 0.1f);
    SetStateQueueWindow(EWeaponLogicState::Blocked, 0.1f);
    SetStateQueueWindow(EWeaponLogicState::Reloading, 0.1f);
    SetStateQueueWindow(EWeaponLogicState::FireCooldown, 0.1f);
}


void UWeaponLogicStateManager::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    TickState(DeltaTime);
}



void UWeaponLogicStateManager::OnUpdateState(EWeaponLogicState previousState, EWeaponLogicState enteredState)
{
    StateManager::OnUpdateState(previousState, enteredState);
    
    UE_LOG(LogTemp, Display, TEXT("OnStateUpdated on class - %s"), *owner->GetName());

    const EWeaponAnimState prevAnimState = weaponLogicToAnimStatesMap[previousState];
    const EWeaponAnimState newAnimState = weaponLogicToAnimStatesMap[enteredState];
    owner->pSubsystem->GetWeaponAnimStateChangeRequestDelegate().Broadcast(owner->GetWeaponType(), prevAnimState, newAnimState);
    
}

void UWeaponLogicStateManager::OnStackState(EWeaponLogicState stackedState)
{
    // OnStackState(stackedState);
}

void UWeaponLogicStateManager::OnUnstackState(EWeaponLogicState unstackedState)
{
    // OnUnstackState(unstackedState);
}
