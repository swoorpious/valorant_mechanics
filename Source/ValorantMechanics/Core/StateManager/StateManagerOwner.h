// © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"


template<typename FSMClass, typename TStateEnum>
class StateManagerOwner
{
public:
    // State Query Functions
    virtual TStateEnum GetCurrentState() const = 0;
    virtual TArray<TStateEnum> GetActiveStates() const = 0;
    virtual bool IsCurrentState(TStateEnum state) const = 0;
    virtual bool IsStateActive(TStateEnum state) const = 0;
    virtual bool CanTransitionToState(TStateEnum state) const = 0;

    // State Transition Functions
    virtual void TryTransitionToState(TStateEnum state) = 0;
    virtual void TryStackState(TStateEnum state) = 0;
    virtual void TryUnstackState(TStateEnum state) = 0;

    // State Queue Functions
    virtual bool TryQueueState(TStateEnum state) = 0;
    virtual void ClearQueuedState() = 0;
    virtual bool HasQueuedState() const = 0;
    virtual TStateEnum GetQueuedState() const = 0;

    // Delegate Access (for external binding)
    virtual typename FSMClass::OnStateUpdatedCallback& GetStateUpdateDelegate() = 0;
    virtual typename FSMClass::OnTimedStateCallback& GetTimedStateDelegate() = 0;
    virtual typename FSMClass::OnStateStackUpdatedCallback& GetStateStackUpdateDelegate() = 0;
};

/**
 * Concrete implementation mixin for classes that own a StateManager.
 * Inherit from this to automatically implement the interface.
 */
template<typename FSMClass, typename TStateEnum>
class TStateManagerOwnerMixin : public StateManagerOwner<FSMClass, TStateEnum>
{
public:

    // to call in deriving class's BeginPlay() after initializing the StateManager
    void InitializeStateManager()
    {
        localFSM = GetStateManager();
    }
    
    // State Query Functions
    virtual TStateEnum GetCurrentState() const override
    {
        return localFSM ? localFSM->GetCurrentState() : TStateEnum{};
    }

    virtual TArray<TStateEnum> GetActiveStates() const override
    {
        return localFSM ? localFSM->GetActiveStates() : TArray<TStateEnum>{};
    }

    virtual bool IsCurrentState(TStateEnum state) const override
    {
        return localFSM ? localFSM->IsCurrentState(state) : false;
    }

    virtual bool IsStateActive(TStateEnum state) const override
    {
        return localFSM ? localFSM->IsStateActive(state) : false;
    }

    virtual bool CanTransitionToState(TStateEnum state) const override
    {
        return localFSM ? localFSM->CanTransitionToState(state) : false;
    }

    // State Transition Functions
    virtual void TryTransitionToState(TStateEnum state) override
    {
        if (localFSM)
        {
            localFSM->TryTransitionToState(state);
        }
    }

    virtual void TryStackState(TStateEnum state) override
    {
        if (localFSM)
        {
            localFSM->TryStackState(state);
        }
    }

    virtual void TryUnstackState(TStateEnum state) override
    {
        if (localFSM)
        {
            localFSM->TryUnstackState(state);
        }
    }

    // state queue functions
    virtual bool TryQueueState(TStateEnum state) override
    {
        return localFSM ? localFSM->TryQueueState(state) : false;
    }

    virtual void ClearQueuedState() override
    {
        if (localFSM)
        {
            localFSM->ClearQueuedState();
        }
    }

    virtual bool HasQueuedState() const override
    {
        return localFSM ? localFSM->HasQueuedState() : false;
    }

    virtual TStateEnum GetQueuedState() const override
    {
        return localFSM ? localFSM->GetQueuedState() : TStateEnum{};
    }

    // delegate access
    virtual typename FSMClass::OnStateUpdatedCallback& GetStateUpdateDelegate() override
    {
        checkf(localFSM, TEXT("StateManager is null!"));
        return localFSM->GetStateUpdateCallbackDelegate();
    }

    virtual typename FSMClass::OnTimedStateCallback& GetTimedStateDelegate() override
    {
        checkf(localFSM, TEXT("StateManager is null!"));
        return localFSM->GetTimedStateCallbackDelegate();
    }

    virtual typename FSMClass::OnStateStackUpdatedCallback& GetStateStackUpdateDelegate() override
    {
        checkf(localFSM, TEXT("StateManager is null!"));
        return localFSM->GetStateStackUpdateCallbackDelegate();
    }

protected:
    // needs to be overridden
    virtual FSMClass* GetStateManager() const = 0;

private:
    FSMClass* localFSM = nullptr;
    
};