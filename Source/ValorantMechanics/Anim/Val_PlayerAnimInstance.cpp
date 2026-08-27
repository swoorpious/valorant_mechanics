// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "Val_PlayerAnimInstance.h"

#include "ValorantMechanics/Weapon/CommonWeapon.h"
#include "ValorantMechanics/Core/Val_LocalPlayerSubsystem.h"
#include "ValorantMechanics/Player/Controller/Val_PlayerController.h"
#include "ValorantMechanics/Player/PlayerComponents/Val_CharacterMovementComponent.h"
#include "ValorantMechanics/Player/Val_Character.h"
#include "ValorantMechanics/Weapon/Val_WeaponAnimConfig.h"



void UVal_PlayerAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();
    UE_LOG(LogAnimation, Warning, TEXT("[AnimInstance] NativeInitializeAnimation on %s"), *GetNameSafe(this));

}

void UVal_PlayerAnimInstance::NativeBeginPlay()
{
    Super::NativeBeginPlay();
    
    AVal_Character* e = Cast<AVal_Character>(GetOwningActor());
    if (!e)
    {
        UE_LOGFMT(LogAnimation, Error, "[Val_PlayerAnimInstance] NativeBeginPlay: GetOwningActor() failed to resolve to AVal_Character (owner: {s})", *GetNameSafe(GetOwningActor()));
        return;
    }
    
    e->getOnWeaponChangedDelegate()->AddUObject(this, &UVal_PlayerAnimInstance::_updateWeaponAnimAsset);
    e->getOnWeaponStateChangedDelegate()->AddUObject(this, &UVal_PlayerAnimInstance::_updateWeaponStateChange);
    UE_LOG(LogAnimation, Warning, TEXT("[AnimInstance] Subscribed. this=%p character=%p"), this, e);

    valInput = e->GetValPlayerController()->GetInputSystem();

    if (UVal_WeaponAnimConfig* config = e->getCurrentAnimAsset())
        _updateWeaponAnimAsset(config);
    
    
}

void UVal_PlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (valInput)
        _lastLookVector = valInput->GetLastLookVector();

       _playerVelocity = GetOwningActor()->GetVelocity();
}

void UVal_PlayerAnimInstance::NativeUninitializeAnimation()
{
    Super::NativeUninitializeAnimation();
    
    if (AVal_Character* e = Cast<AVal_Character>(TryGetPawnOwner()))
    {
        e->getOnWeaponChangedDelegate()->RemoveAll(this);
        e->getOnWeaponStateChangedDelegate()->RemoveAll(this);

    }
    
    valInput = nullptr;
}


// nothing updates the movement state atm
bool UVal_PlayerAnimInstance::_canTransitionToMovementAnimState(EMovementState state) const
{
    return _movementState == state;
}

bool UVal_PlayerAnimInstance::_canTransitionToWeaponAnimState(EWeaponState state) const
{
    return _weaponState == state;
}


// when this function is called, it basicallly means the weapon has changed
void UVal_PlayerAnimInstance::_updateWeaponAnimAsset(UVal_WeaponAnimConfig* newAnimConfig)
{
    // stop all montages
    Montage_Stop(0.f, nullptr);
    UE_LOG(LogAnimation, Display, TEXT("[AnimInstance] updating weapon anim instance"));
    _currAnimAsset = newAnimConfig;
}

void UVal_PlayerAnimInstance::_updateWeaponStateChange(const EWeaponState newState)
{
    _weaponState = newState;
    UE_LOGFMT(LogAnimation, Log, "[AnimInstance] updating state: {0}", static_cast<uint8>(newState));
    _playWeaponStateBasedMontage();
}

void UVal_PlayerAnimInstance::_playWeaponStateBasedMontage()
{
    auto play = [&](UAnimMontage* montage)
    {
        if (!montage) return;
        static FName begin = FName("Begin");
        static FName end = FName("End");
        
        if (Montage_IsPlaying(montage))
            Montage_JumpToSection(begin);
        else
            Montage_Play(montage, 1.f);
    };

    // only states where it is a montage
    // other states go to the state machine
    switch (_weaponState)
    {
    case EWeaponState::Equip_Default:
        play(_currAnimAsset->equipDefault);
        break;
    case EWeaponState::Equip_Fast:
        play(_currAnimAsset->equipFast);
        break;
    case EWeaponState::Firing:
        play(_currAnimAsset->weaponFire);
        break;
    case EWeaponState::Inspecting:
        play(_currAnimAsset->inspectLoop);
        break;
    case EWeaponState::Reloading:
        play(_currAnimAsset->reload);
        break;
    }
}