// © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"


template<typename FSMClass>
class StateManagerOwner
{
public:
    void InitializeStateManager(FSMClass* inClass)
    {
        localFSM = inClass;
    }

    
    virtual FSMClass* GetStateManager() const
    {
        return localFSM;
    };
    

    // delegate access
    virtual typename FSMClass::OnStateUpdatedCallback& GetStateUpdateCallbackDelegate()
    {
        return localFSM->GetStateUpdateCallbackDelegate();
    }
    
    virtual typename FSMClass::OnTimedStateCallback& GetTimedStateCallbackDelegate()
    {
        return localFSM->GetTimedStateCallbackDelegate();
    }
    
    virtual typename FSMClass::OnStateStackUpdatedCallback& GetStateStackUpdateCallbackDelegate()
    {
        return localFSM->GetStateStackUpdateCallbackDelegate();
    }

    
protected:
    FSMClass* localFSM = nullptr;
    
};



template<typename FSMClass>
class PrimaryStateManagerOwner : public StateManagerOwner<FSMClass>
{
public:
    void InitializePrimaryStateManager(FSMClass* inClass)
    {
        this->InitializeStateManager(inClass);
    }

    FSMClass* GetPrimaryStateManager()
    {
        return this->GetStateManager();
    }

    virtual typename FSMClass::OnStateUpdatedCallback& GetPrimaryStateUpdateCallbackDelegate()
    {
        checkf(this->localFSM, TEXT("Primary State Manager is null."));
        return this->GetStateUpdateCallbackDelegate();
    }
    
    virtual typename FSMClass::OnTimedStateCallback& GetPrimaryTimedStateCallbackDelegate()
    {
        checkf(this->localFSM, TEXT("Primary State Manager is null."));
        return this->GetTimedStateCallbackDelegate();
    }
    
    virtual typename FSMClass::OnStateStackUpdatedCallback& GetPrimaryStateStackUpdateCallbackDelegate()
    {
        checkf(this->localFSM, TEXT("Primary State Manager is null."));
        return this->GetStateStackUpdateCallbackDelegate();
    }

};


template<typename FSMClass>
class SecondaryStateManagerOwner : public StateManagerOwner<FSMClass>
{
public:
    void InitializeSecondaryStateManager(FSMClass* inClass)
    {
        this->InitializeStateManager(inClass);
    }

    FSMClass* GetSecondaryStateManager()
    {
        return this->GetStateManager();
    }

    virtual typename FSMClass::OnStateUpdatedCallback& GetSecondaryStateUpdateCallbackDelegate()
    {
        checkf(this->localFSM, TEXT("Secondary State Manager is null."));
        return this->GetStateUpdateCallbackDelegate();
    }
    
    virtual typename FSMClass::OnTimedStateCallback& GetSecondaryTimedStateCallbackDelegate()
    {
        checkf(this->localFSM, TEXT("Secondary State Manager is null."));
        return this->GetTimedStateCallbackDelegate();
    }
    
    virtual typename FSMClass::OnStateStackUpdatedCallback& GetSecondaryStateStackUpdateCallbackDelegate()
    {
        checkf(this->localFSM, TEXT("Secondary State Manager is null."));
        return this->GetStateStackUpdateCallbackDelegate();
    }

};