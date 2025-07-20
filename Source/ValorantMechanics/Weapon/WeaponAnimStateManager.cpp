// © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "WeaponAnimStateManager.h"


// Sets default values for this component's properties
UWeaponAnimStateManager::UWeaponAnimStateManager()
{
    PrimaryComponentTick.bCanEverTick = true;

}

void UWeaponAnimStateManager::InitializeWeaponStateManager(FDefaultWeaponProperties* defaultProperties,
    FAltWeaponProperties* altProperties)
{
}



void UWeaponAnimStateManager::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    
}

void UWeaponAnimStateManager::OnUpdateState(EWeaponAnimState previousState, EWeaponAnimState enteredState)
{
}

void UWeaponAnimStateManager::OnStackState(EWeaponAnimState stackedState)
{
}

void UWeaponAnimStateManager::OnUnstackState(EWeaponAnimState unstackedState)
{
}

