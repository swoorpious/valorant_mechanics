// © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"


template<typename TStateEnum>
class CommonStateManager
{
public:
    virtual ~CommonStateManager() = default;
    
    virtual void TryUpdateState(TStateEnum) = 0;
    virtual TStateEnum GetCurrentState() const = 0;
    virtual bool IsCurrentState(TStateEnum state) const = 0;
    virtual bool CanTransitionToState(TStateEnum state) const = 0;

    virtual void OnUpdateState(TStateEnum previousState, TStateEnum enteredState) = 0;
    
};
