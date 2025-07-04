// © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once


template<typename TStateEnum>
class StateManager
{
public:
    virtual ~StateManager() = default;

    
    virtual void UnallowStateTransition(TStateEnum fromState, TSet<TStateEnum> unallowedStates);
    
    virtual TStateEnum GetCurrentState() const;
    
    virtual bool IsCurrentState(TStateEnum state) const;
    
    virtual bool CanTransitionToState(TStateEnum state) const;

    virtual void TryTransitionToState(TStateEnum updateToState);

    
protected:
    virtual void TickState(float deltaTime);
    
    virtual void InternalUpdateState(TStateEnum stateToExit, TStateEnum stateToEnter);

    virtual void OnUpdateState(TStateEnum previousState, TStateEnum enteredState) = 0;
    

    /*
     * TStateEnum -> { ...set of TStateEnum states the key can transition to }
     * key references to the current state or state to transition from
     * value references to all the state that cannot be transitioned to
     *
     * values to this must be added at the time of construction
     */
    TMap<TStateEnum, TSet<TStateEnum>> unallowedStateTransitions;

    // can be used for time based changes depending on the state
    float timeSinceLastStateTransition = 0.0f;
    
    TStateEnum lastState;
    TStateEnum currentState;
};



template <typename TStateEnum>
void StateManager<TStateEnum>::TickState(float deltaTime)
{
    timeSinceLastStateTransition += deltaTime;
}


template <typename TStateEnum>
void StateManager<TStateEnum>::UnallowStateTransition(TStateEnum fromState, TSet<TStateEnum> unallowedStates)
{
    TSet<TStateEnum>& currentSet = unallowedStateTransitions.FindOrAdd(fromState);
    currentSet.Append(unallowedStates);
}


template <typename TStateEnum>
TStateEnum StateManager<TStateEnum>::GetCurrentState() const
{
    return currentState;
}


template <typename TStateEnum>
bool StateManager<TStateEnum>::IsCurrentState(TStateEnum state) const
{
    return state == GetCurrentState();
}


template <typename TStateEnum>
bool StateManager<TStateEnum>::CanTransitionToState(TStateEnum state) const
{        
    if (const TSet<TStateEnum>* e = unallowedStateTransitions.Find(GetCurrentState()))
        return !e->Contains(state);

    return true;
}


template <typename TStateEnum>
void StateManager<TStateEnum>::TryTransitionToState(TStateEnum updateToState)
{
    if (!CanTransitionToState(updateToState) || IsCurrentState(updateToState)) return;

    InternalUpdateState(GetCurrentState(), updateToState);
}



template <typename TStateEnum>
void StateManager<TStateEnum>::InternalUpdateState(TStateEnum stateToExit, TStateEnum stateToEnter)
{
    lastState = currentState;
    currentState = stateToEnter;
    timeSinceLastStateTransition = 0.0f;
    
    OnUpdateState(stateToExit, stateToEnter);
}