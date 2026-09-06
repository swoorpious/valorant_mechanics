// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "Val_PlayerAnimInstance.h"

#include "ValorantMechanics/Weapon/CommonWeapon.h"
#include "ValorantMechanics/Core/Val_LocalPlayerSubsystem.h"
#include "ValorantMechanics/Player/Controller/Val_PlayerController.h"
#include "ValorantMechanics/Player/PlayerComponents/Val_CharacterMovementComponent.h"
#include "ValorantMechanics/Player/Val_Character.h"
#include "ValorantMechanics/Weapon/Val_WeaponAnimConfig.h"
#include "ValorantMechanics/Core/Log.h"



void UVal_PlayerAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();
    LOGObjName(this, LogAnimation, Warning, "NativeInitializeAnimation on %s", *GetNameSafe(this));
}

void UVal_PlayerAnimInstance::NativeBeginPlay()
{
    Super::NativeBeginPlay();
    
    AVal_Character* e = Cast<AVal_Character>(GetOwningActor());
    if (!e)
    {
        LOGObjName(this, LogAnimation, Error, "NativeBeginPlay: GetOwningActor() failed to resolve to AVal_Character (owner: %s)", *GetNameSafe(GetOwningActor()));
        return;
    }
    
    e->getOnWeaponChangedDelegate()->AddUObject(this, &UVal_PlayerAnimInstance::_updateWeaponAnimAsset);
    e->getOnWeaponStateChangedDelegate()->AddUObject(this, &UVal_PlayerAnimInstance::_updateWeaponStateChange);
    e->getOnMovementStateChangedDelegate()->AddUObject(this, &UVal_PlayerAnimInstance::_updateMovementStateChange);
    valPlayer = e;
    valMovement = e->GetValMovementComponent();
    LOGObjName(this, LogAnimation, Warning, "Subscribed. this=%p character=%p", this, e);

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

    if (valPlayer)
    {
        isHoldingGun = valPlayer->isHoldingGun();
        if (isHoldingGun) _leftHandIKAlpha = 1.f;
        else _leftHandIKAlpha = 0.f;
    }
    if (valMovement) isFalling = valMovement->IsFalling();

    _updateArmSway(DeltaSeconds);

}

void UVal_PlayerAnimInstance::NativeUninitializeAnimation()
{
    Super::NativeUninitializeAnimation();
    
    if (valPlayer)
    {
        valPlayer->getOnWeaponChangedDelegate()->RemoveAll(this);
        valPlayer->getOnWeaponStateChangedDelegate()->RemoveAll(this);
        valPlayer->getOnMovementStateChangedDelegate()->RemoveAll(this);
    }

    valMovement = nullptr;
    valPlayer = nullptr;
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
    LOGObjName(this, LogAnimation, Display, "updating weapon anim instance");
    _currAnimAsset = newAnimConfig;
}

void UVal_PlayerAnimInstance::_updateWeaponStateChange(const EWeaponState newState)
{
    _weaponState = newState;
    LOGObjName(this, LogAnimation, Log, "updating weapon state: %d", static_cast<uint8>(newState));
    _playWeaponStateBasedMontage();
}

void UVal_PlayerAnimInstance::_updateMovementStateChange(const EMovementState newState)
{
    _movementState = newState;
    LOGObjName(this, LogAnimation, Log, "updating movement state: %d", static_cast<uint8>(newState));
}

void UVal_PlayerAnimInstance::_playWeaponStateBasedMontage()
{
    if (!_currAnimAsset) return;

    auto play = [&](UAnimMontage* montage)
    {
        if (!montage) return;
        static const FName begin = FName("Begin");

        if (Montage_IsPlaying(montage))
            Montage_JumpToSection(begin, montage);
        else
        {
            FAlphaBlendArgs BlendIn;
            BlendIn.BlendTime = 0.f;
            Montage_PlayWithBlendIn(montage, BlendIn, 1.f);
        }
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
    // if a montage played earlier, 
    // it needs to explicitly stop to go back to the state machine anims
    default:
        Montage_Stop(.2f, nullptr);
        break;
    }
}

void UVal_PlayerAnimInstance::_updateArmSway(float DeltaSeconds)
{
    const float armSwayAlpha  = FMath::Clamp(DeltaSeconds * _armSwayLerpSpeed,   0.f, 1.f);

    // --- Duck: any movement (planar or vertical) ducks the arms slightly, regardless of direction ---
    const float speed       = _lastLookVector.Size();
    const float duckAlpha   = FMath::Clamp(speed / FMath::Max(_duckSpeedForMax, KINDA_SMALL_NUMBER), 0.f, 1.f);
    const float targetDuckZ = -duckAlpha * _maxDuckOffset; // negative Z = down in component space

    // --- Lag: only horizontal look movement matters. Mouse left -> arms lag right (opposite sign) ---
    const float lagAlpha      = FMath::Clamp(_lastLookVector.X / FMath::Max(_lookXRangeForMaxLag, KINDA_SMALL_NUMBER), -1.f, 1.f);
    const float targetLagRoll = -lagAlpha * _maxLagRoll;

    _finalArmOffset.X = 0.f;
    _finalArmOffset.Y = 0.f;
    _finalArmOffset.Z = FMath::Lerp(_finalArmOffset.Z, targetDuckZ, armSwayAlpha);

    // Pitch/Yaw stay at 0 here on purpose: look-driven pitch/yaw is already handled
    // separately via _handSwayYaw/_handSwayPitch above (consumed in the ABP). Only
    // the movement-driven duck and look-driven lag roll feed the clavicle rotator.
    _finalArmRotation.Pitch = 0.f;
    _finalArmRotation.Yaw   = 0.f;
    _finalArmRotation.Roll  = FMath::Lerp(_finalArmRotation.Roll, targetLagRoll, armSwayAlpha);
}