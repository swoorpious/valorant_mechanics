// © 2026 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "Val_WeaponAnimInstace.h"
#include "ValorantMechanics/Core/Log.h"
#include "ValorantMechanics/Weapon/CommonWeapon.h"
#include "ValorantMechanics/Weapon/Val_WeaponAnimConfig.h"


void UVal_WeaponAnimInstace::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();
}

void UVal_WeaponAnimInstace::NativeBeginPlay()
{
    Super::NativeBeginPlay();

    ACommonWeapon* e = Cast<ACommonWeapon>(GetOwningActor());
    if (!e)
    {
        LOGObjName(this, LogAnimation, Error,
            "NativeBeginPlay: GetOwningActor() failed to resolve to ACommonWeapon (owner: %s)",
            *GetNameSafe(GetOwningActor()));
        return;
    }

    _animAsset = e->getAnimAsset();
}

void UVal_WeaponAnimInstace::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);
}

void UVal_WeaponAnimInstace::NativeUninitializeAnimation()
{
    Super::NativeUninitializeAnimation();
}

void UVal_WeaponAnimInstace::updateWeaponStateChange(const EWeaponState newState)
{
    _weaponState = newState;
    LOGObjName(this, LogAnimation, Log, "updating weapon state: %d", static_cast<uint8>(newState));
    _playWeaponStateBasedMontage();
}

void UVal_WeaponAnimInstace::_playWeaponStateBasedMontage()
{
    if (!_animAsset) return;

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
        play(_animAsset->weaponEquip);
        break;
    case EWeaponState::Firing:
        play(_animAsset->weaponFire);
        break;
    case EWeaponState::Reloading:
        play(_animAsset->weaponReload);
        break;
        // if a montage played earlier, 
        // it needs to explicitly stop to go back to the state machine anims
    default:
        Montage_Stop(.2f, nullptr);
        break;
    }
}