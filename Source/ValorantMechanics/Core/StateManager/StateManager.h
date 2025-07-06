// © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "Templates/IsEnum.h"


#pragma region STATE MANAGER

template<typename TStateEnum>
class StateManager
{

    static_assert(TIsEnum<TStateEnum>::Value, "TStateEnum must be an enum.");
    
public:
    virtual ~StateManager() = default;

    DECLARE_MULTICAST_DELEGATE_TwoParams(OnStateUpdatedCallback, TStateEnum /* old state */, TStateEnum /* new state */);
    DECLARE_MULTICAST_DELEGATE_OneParam(OnTimedStateCallback, TStateEnum /* state that timed out */);
    DECLARE_MULTICAST_DELEGATE_TwoParams(OnStateStackUpdatedCallback, TStateEnum /* state */, bool /* bIsStacked - true for stacked, false for unstacked */);

    virtual void TickState(float deltaTime);
    
    virtual void UnallowStateTransition(TStateEnum fromState, TSet<TStateEnum> unallowedStates);
    virtual TStateEnum GetCurrentState() const;

    virtual TArray<TStateEnum> GetActiveStates() const;
    virtual bool IsCurrentState(TStateEnum state) const;
    virtual bool IsStateActive(TStateEnum state) const;
    virtual bool CanTransitionToState(TStateEnum state) const;
    virtual void TryTransitionToState(TStateEnum updateToState);
    virtual void TryStackState(TStateEnum stateToStack);
    virtual void TryUnstackState(TStateEnum stateToUnstack);

    /*
     * declare which states should tick and their notification times
     */
    virtual void SetStateTickingEnabled(TStateEnum state, bool bEnabled);
    virtual void SetStateNotificationTime(TStateEnum state, float notificationTime);

    /*
     * default state
     */
    virtual void SetDefaultState(TStateEnum state);
    virtual TStateEnum GetDefaultState() const;

    /*
     * this allows queuing the next state during the last few milliseconds of a timed state
     * queueWindowMs: time in milliseconds before state completion when queuing is allowed
     */
    virtual void SetStateQueueWindow(TStateEnum state, float queueWindowMs = 50.0f);
    virtual bool TryQueueState(TStateEnum stateToQueue);
    virtual void ClearQueuedState();
    virtual bool HasQueuedState() const;
    virtual TStateEnum GetQueuedState() const;

    /*
     * for external functions to bind
     */
    OnStateUpdatedCallback& GetStateUpdateCallbackDelegate() { return onStateUpdatedDelegate; }
    OnTimedStateCallback& GetTimedStateCallbackDelegate() { return onTimedStateDelegate; }
    OnStateStackUpdatedCallback& GetStateStackUpdateCallbackDelegate() { return onStateStackUpdatedDelegate; }
    
    
protected:
    virtual void InternalTickState(float deltaTime);
    virtual void InternalUpdateState(TStateEnum stateToExit, TStateEnum stateToEnter);
    virtual void InternalStackState(TStateEnum stateToStack);
    virtual void InternalUnstackState(TStateEnum stateToUnstack);
    virtual void OnUpdateState(TStateEnum previousState, TStateEnum enteredState) = 0;
    virtual void OnStackState(TStateEnum stackedState) = 0;
    virtual void OnUnstackState(TStateEnum unstackedState) = 0;

    /*
     * manage automatic state transitions when timed states complete
     */
    virtual void HandleTimedStateCompletion(TStateEnum completedState);

    /*
     * TStateEnum -> { ...set of TStateEnum states the key can transition to }
     * key references to the current state or state to transition from
     * value references to all the state that cannot be transitioned to
     *
     * values to this must be added at the time of construction
     */
    TMap<TStateEnum, TSet<TStateEnum>> unallowedStateTransitions;

    /*
     * states that are allowed to tick (have their time tracked)
     */
    TSet<TStateEnum> tickingStates;

    /*
     * time based notifications for specific states
     * TStateEnum -> notification time in milliseconds
     */
    TMap<TStateEnum, float> stateNotificationTimes;

    /*
     * queue window times for states in milliseconds
     * TStateEnum -> time before automatic state transition when queuing next state is allowed
     */
    TMap<TStateEnum, float> stateQueueWindows;

    /*
     * timing for states that have notification times
     * TStateEnum -> time since state became active
     */
    TMap<TStateEnum, float> stateTimers;

    /*
     * states that have already fired their notifications to prevent unnecessary ticking
     */
    TSet<TStateEnum> notifiedStates;

    /*
     * this contains stacked modifier states only; states like ScopedIn could be stacked with main states like Idle/Fired/FireCooldown or currenState etc.
     * currentState is separate and managed independently
     */
    TArray<TStateEnum> stateStack;

    /*
     * default state to transition to when timed states complete (if no queued state)
     */
    TStateEnum defaultState;
    bool bHasDefaultState = false;

    /*
     * state queue
     */
    TStateEnum queuedState;
    bool hasQueuedState = false;

    // can be used for time based changes depending on the state
    float timeSinceLastStateTransition = 0.0f;
    
    TStateEnum lastState;
    TStateEnum currentState; // the main state, stateStack contains additional stacked states

    OnStateUpdatedCallback onStateUpdatedDelegate;
    OnTimedStateCallback onTimedStateDelegate;
    OnStateStackUpdatedCallback onStateStackUpdatedDelegate;

};



template<typename TStateEnum>
void StateManager<TStateEnum>::TickState(float deltaTime)
{
    InternalTickState(deltaTime);
}


template<typename TStateEnum>
void StateManager<TStateEnum>::UnallowStateTransition(TStateEnum fromState, TSet<TStateEnum> unallowedStates)
{
    TSet<TStateEnum>& currentSet = unallowedStateTransitions.FindOrAdd(fromState);
    currentSet.Append(unallowedStates);
}


template<typename TStateEnum>
TStateEnum StateManager<TStateEnum>::GetCurrentState() const
{
    return currentState;
}





template<typename TStateEnum>
TArray<TStateEnum> StateManager<TStateEnum>::GetActiveStates() const
{
    TArray<TStateEnum> activeStates;
    activeStates.Add(currentState);
    activeStates.Append(stateStack);
    return activeStates;
}


template<typename TStateEnum>
bool StateManager<TStateEnum>::IsCurrentState(TStateEnum state) const
{
    return state == GetCurrentState();
}


template<typename TStateEnum>
bool StateManager<TStateEnum>::IsStateActive(TStateEnum state) const
{
    return state == currentState || stateStack.Contains(state);
}


template<typename TStateEnum>
bool StateManager<TStateEnum>::CanTransitionToState(TStateEnum state) const
{        
    if (const TSet<TStateEnum>* e = unallowedStateTransitions.Find(GetCurrentState()))
        return !e->Contains(state);

    return true;
}


template<typename TStateEnum>
void StateManager<TStateEnum>::TryTransitionToState(TStateEnum updateToState)
{
    if (!CanTransitionToState(updateToState) || IsCurrentState(updateToState)) return;

    InternalUpdateState(GetCurrentState(), updateToState);
}


template<typename TStateEnum>
void StateManager<TStateEnum>::TryStackState(TStateEnum stateToStack)
{
    if (IsStateActive(stateToStack)) return;

    InternalStackState(stateToStack);
}


template<typename TStateEnum>
void StateManager<TStateEnum>::TryUnstackState(TStateEnum stateToUnstack)
{
    if (!IsStateActive(stateToUnstack)) return;

    InternalUnstackState(stateToUnstack);
}


template<typename TStateEnum>
void StateManager<TStateEnum>::SetStateTickingEnabled(TStateEnum state, bool bEnabled)
{
    if (bEnabled)
        tickingStates.Add(state);
    else
        tickingStates.Remove(state);

}


template<typename TStateEnum>
void StateManager<TStateEnum>::SetStateNotificationTime(TStateEnum state, float notificationTime)
{
    stateNotificationTimes.Add(state, notificationTime);
}


template<typename TStateEnum>
void StateManager<TStateEnum>::SetDefaultState(TStateEnum state)
{
    defaultState = state;
    bHasDefaultState = true;
}


template<typename TStateEnum>
TStateEnum StateManager<TStateEnum>::GetDefaultState() const
{
    return defaultState;
}


template<typename TStateEnum>
void StateManager<TStateEnum>::SetStateQueueWindow(TStateEnum state, float queueWindowMs)
{
    stateQueueWindows.Add(state, queueWindowMs);
}


template<typename TStateEnum>
bool StateManager<TStateEnum>::TryQueueState(TStateEnum stateToQueue)
{
    
    // only allow queuing if the current state has a notification time or queue window
    const float* notificationTime = stateNotificationTimes.Find(currentState);
    const float* queueWindow = stateQueueWindows.Find(currentState);
    
    if (!notificationTime || !queueWindow)
        return false; // currentState doesn't allow state queuing

    
    // check if in the queue window
    const float* currentTimer = stateTimers.Find(currentState);
    if (!currentTimer)
        return false; // state not actively timed
    
    float timeRemaining = *notificationTime - *currentTimer;
    if (timeRemaining > *queueWindow)
        return false; // not in queue window

    
    if (!CanTransitionToState(stateToQueue))
        return false;

    
    queuedState = stateToQueue;
    hasQueuedState = true;
    return true;
}


template<typename TStateEnum>
void StateManager<TStateEnum>::ClearQueuedState()
{
    hasQueuedState = false;
}


template<typename TStateEnum>
bool StateManager<TStateEnum>::HasQueuedState() const
{
    return hasQueuedState;
}


template<typename TStateEnum>
TStateEnum StateManager<TStateEnum>::GetQueuedState() const
{
    return queuedState;
}


template<typename TStateEnum>
void StateManager<TStateEnum>::InternalTickState(float deltaTime)
{
    timeSinceLastStateTransition += deltaTime;

    // tick timing for current state if time tracked
    if (tickingStates.Contains(currentState))
    {
        float& stateTimer = stateTimers.FindOrAdd(currentState);
        stateTimer += deltaTime;

        
        if (const float* notificationTime = stateNotificationTimes.Find(currentState))
        {
            if (stateTimer >= *notificationTime && !notifiedStates.Contains(currentState))
            {
                notifiedStates.Add(currentState);
                onTimedStateDelegate.Broadcast(currentState);
                
                
                HandleTimedStateCompletion(currentState);
            }
        }
    }

    // tick timing for all stacked states that are configured to tick
    for (const TStateEnum& stackedState : stateStack)
    {
        if (tickingStates.Contains(stackedState))
        {
            float& stateTimer = stateTimers.FindOrAdd(stackedState);
            stateTimer += deltaTime;


            if (const float* notificationTime = stateNotificationTimes.Find(stackedState))
            {
                if (stateTimer >= *notificationTime && !notifiedStates.Contains(stackedState))
                {
                    notifiedStates.Add(stackedState);
                    onTimedStateDelegate.Broadcast(stackedState);

                    
                    // stacked states can't trigger automatic transitions
                }
            }
        }
    }
}


template<typename TStateEnum>
void StateManager<TStateEnum>::InternalUpdateState(TStateEnum stateToExit, TStateEnum stateToEnter)
{
    lastState = currentState;
    currentState = stateToEnter;
    timeSinceLastStateTransition = 0.0f;
    
    if (tickingStates.Contains(stateToEnter))
    {
        stateTimers.FindOrAdd(stateToEnter) = 0.0f;
        notifiedStates.Remove(stateToEnter); // allow re-notification for this state
    }
    

    ClearQueuedState();
    
    OnUpdateState(stateToExit, stateToEnter);
}


template<typename TStateEnum>
void StateManager<TStateEnum>::InternalStackState(TStateEnum stateToStack)
{
    stateStack.Add(stateToStack);
    
    if (tickingStates.Contains(stateToStack))
    {
        stateTimers.Add(stateToStack, 0.0f);
        notifiedStates.Remove(stateToStack); // allow notification for this state
    }
    
    onStateStackUpdatedDelegate.Broadcast(stateToStack, true);
    
    OnStackState(stateToStack);
}


template<typename TStateEnum>
void StateManager<TStateEnum>::InternalUnstackState(TStateEnum stateToUnstack)
{
    stateStack.Remove(stateToUnstack);
    
    stateTimers.Remove(stateToUnstack);
    notifiedStates.Remove(stateToUnstack);
    
    onStateStackUpdatedDelegate.Broadcast(stateToUnstack, false);
    
    OnUnstackState(stateToUnstack);
}


template<typename TStateEnum>
void StateManager<TStateEnum>::HandleTimedStateCompletion(TStateEnum completedState)
{
    if (completedState != currentState) return;
    
    TStateEnum nextState = static_cast<TStateEnum>(0);
    bool shouldTransition = false;

    
    if (hasQueuedState)
    {
        nextState = queuedState;
        shouldTransition = true;
        ClearQueuedState();
    }
    else if (bHasDefaultState)
    {
        nextState = defaultState;
        shouldTransition = true;
    }

    
    if (shouldTransition && CanTransitionToState(nextState))
        TryTransitionToState(nextState);
}

#pragma endregion STATE MANAGER

