// © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Val_PlayerInterface.generated.h"

// forward declarations
class AVal_PlayerController;
class AVal_Character;
class UVal_CharacterMovementComponent;
class UVal_AnimInstance;
class UVal_InputComponent;


UINTERFACE() class UVal_PlayerInterface : public UInterface { GENERATED_BODY() };



/**
 * this class, so far is only used to fix the architecture/structure of the code and make referencing components easier
 */
class VALORANTMECHANICS_API IVal_PlayerInterface
{
    GENERATED_BODY()

    
public:

    template <typename T, typename Func>
    T* GetReference(Func getter) { return Cast<T>(getter()); }
    
    virtual AVal_PlayerController* GetValPlayerController() = 0;
    virtual AVal_Character* GetValCharacter() = 0;
    virtual UVal_CharacterMovementComponent* GetValMovementComponent() = 0;
    virtual UVal_AnimInstance* GetValAnimInstance () = 0;
	virtual UVal_InputComponent* GetValInputComponent() = 0;
    
};
