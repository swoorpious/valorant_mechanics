// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "Val_Character.h"

#include "ValorantMechanics/Anim/Val_PlayerAnimInstance.h"
#include "ValorantMechanics/Weapon/CommonWeapon.h"
#include "ValorantMechanics/ValorantMechanics.h"
#include "ValorantMechanics/Core/Val_DefaultGameMode.h"

#include "PlayerComponents/Val_CharacterMovementComponent.h"
#include "PlayerComponents/Val_PlayerInventory.h"
#include "Controller/Val_PlayerController.h"

#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SkeletalMeshComponent.h"
// #include "Engine/LocalPlayer.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "ValorantMechanics/Weapon/Val_WeaponAnimConfig.h"


AVal_Character::AVal_Character(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer.SetDefaultSubobjectClass<UVal_CharacterMovementComponent>(CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    _inventory = CreateDefaultSubobject<UVal_PlayerInventory>(TEXT("Player Inventory"));
    

    RootComponent = GetCapsuleComponent();
    sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    sceneComponent->SetupAttachment(RootComponent);
    
    characterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Character Mesh"));
    characterMesh->SetupAttachment(sceneComponent);
    characterMesh->CastShadow = false;
    characterMesh->bCastDynamicShadow = true;
    characterMesh->SetSimulatePhysics(false);
    
    characterMeshCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Character Mesh Camera"));
    characterMeshCamera->SetupAttachment(characterMesh, socketData.cameraSocket);
    characterMeshCamera->bUsePawnControlRotation = false;

    audioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
    audioComponent->SetupAttachment(RootComponent);
    audioComponent->bAutoActivate = false;
    audioComponent->bAutoRegister = true;
    audioComponent->bIsUISound = true;
    audioComponent->bAllowSpatialization = false;
    

    bUseControllerRotationYaw = false;
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;

    bReplicates = true;

    // disable physics
    GetMesh()->SetSimulatePhysics(false);
    characterMesh->SetSimulatePhysics(false);
    characterMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    characterMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

    
}


void AVal_Character::BeginPlay()
{
    Super::BeginPlay();
    
    _charMovementComponent = Cast<UVal_CharacterMovementComponent>(GetCharacterMovement());
    _playerAnimInstance = Cast<UVal_PlayerAnimInstance>(characterMesh->GetAnimInstance());
    if (!_charMovementComponent) LOG(Val_Player, Error, "_charMovementComponent is likely a nullptr.");
    if (!_playerAnimInstance) LOG(Val_Player, Error, "_playerAnimInstance is likely a nullptr.");

    if (const auto* e = GetWorld()->GetAuthGameMode<AVal_DefaultGameMode>())
    {
        if (e->meleeToSpawn)     SpawnWeapon(e->meleeToSpawn,     !e->secondaryToSpawn && !e->primaryToSpawn);
        if (e->secondaryToSpawn) SpawnWeapon(e->secondaryToSpawn, !e->primaryToSpawn);
        if (e->primaryToSpawn)   SpawnWeapon(e->primaryToSpawn,   true);
    }

    
    /*
     * make mesh render always on top
     *
     * this requires the base mesh material to have
     * MF_FOV_and_clip_fix applied to the world position offset node
     */
    UMaterialInterface* const base_mat = characterMesh->GetMaterial(0);
    if (base_mat)
    {
        UMaterialInstanceDynamic* const mid = UMaterialInstanceDynamic::Create(base_mat, this, FName("AVal_Character"));
        characterMesh->SetMaterial(0, mid);
        mid->SetScalarParameterValue(TEXT("Target FOV"), targetFOV);
        mid->SetScalarParameterValue(TEXT("Scale in Depth"), targetRenderScaleInDepth);
        
        LOG(Val_Player, Warning, "[AVal_Character] FOV and clip fix material created at for characterMesh");
        
    } else LOG(Val_Player, Warning, "[AVal_Character] characterMesh has no material at index 0");
}




void AVal_Character::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}


void AVal_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void AVal_Character::SpawnWeapon(const TSubclassOf<ACommonWeapon>& weaponToSpawn, bool shouldAutoEquip)
{
    if (!weaponToSpawn) return;
    
    if (ACommonWeapon* spawned_weapon = GetWorld()->SpawnActor<ACommonWeapon>(weaponToSpawn))
    {
        spawned_weapon->SetOwner(this);
        spawned_weapon->AttachToComponent(characterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, socketData.rightWeaponMasterSocket);
        spawned_weapon->SetActorHiddenInGame(true); // hidden by default, EquipWeapon(...) will unhide
        spawned_weapon->tryWeaponPickUp(this);

        _inventory->addWeaponToInventory(spawned_weapon);

        if (shouldAutoEquip) this->EquipWeapon(spawned_weapon->getWeaponType(), EEquipType::EquipDefault);
        
    }
    
}


void AVal_Character::EquipWeapon(const EWeaponType weaponType, const EEquipType equipType = EEquipType::EquipDefault)
{
    if (const auto& e = _inventory->getEquippedWeapon())
    {
        // return if trying to switch to the already equipped weapon
        if (e->getWeaponType() == weaponType) return;
        if (_inventory->hasWeapon(weaponType)) UnequipWeapon(e->getWeaponType());
    }
    
    ACommonWeapon* inv_weapon = _inventory->getWeaponByType(weaponType);
    if (!inv_weapon)
    {
        UE_LOG(LogTemp, Warning, TEXT("EquipWeapon: no weapon of type %d in inventory"), static_cast<int32>(weaponType));
        return;
    }
    inv_weapon->weaponEquip(equipType);
    _inventory->switchEquippedWeapon(weaponType);
    
}


void AVal_Character::UnequipWeapon(const EWeaponType weaponType) const
{
    // TODO: equip next level available weapon type
    ACommonWeapon* weapon = _inventory->getWeaponByType(weaponType);
    if (weapon) weapon->weaponUnequip();
}

// void AVal_Character::pickupWeapon(EWeaponType weaponType)
// {
//     
// }


void AVal_Character::DropWeapon(EWeaponType weaponType)
{
    // FIX: null-check getEquippedWeapon() before dereferencing
    ACommonWeapon* equipped = _inventory->getEquippedWeapon();
    if (!equipped) return;

    if (equipped->getWeaponType() == weaponType)
    {
        // assuming that the player has a melee weapon at all times
        const bool has_primary = _inventory->hasWeapon(EWeaponType::Primary);
        const bool has_secondary = _inventory->hasWeapon(EWeaponType::Secondary);  
        
        switch (weaponType)
        {
            case EWeaponType::Melee:
            case EWeaponType::Empty:
            default:
                // cannot drop weapon of type empty or melee
                return;
            case EWeaponType::Secondary:
                // when secondary is dropped, equip primary if it exists, or equip melee
                this->EquipWeapon(has_primary ? EWeaponType::Primary : EWeaponType::Melee);
                return;
            case EWeaponType::Primary:
                // when primary is dropped, equip secondary if it exists, or equip melee
                this->EquipWeapon(has_secondary ? EWeaponType::Secondary : EWeaponType::Melee);
                return;
        }
    }

    // TODO: change this when implementing weapon drop and physics for weapon 
    ACommonWeapon* weapon_to_drop = _inventory->getWeaponByType(weaponType);
    weapon_to_drop->tryWeaponDrop();
    if (weapon_to_drop) weapon_to_drop->Destroy();
    _inventory->removeWeaponFromInventory(weaponType);
    
}

void AVal_Character::PlayLocalSound(USoundBase* sound) const
{
    if (audioComponent && sound)
    {
        audioComponent->SetSound(sound);
        audioComponent->Play();
    }
}


AVal_PlayerController* AVal_Character::GetValPlayerController() const { return Cast<AVal_PlayerController>(GetController()); }
AVal_Character* AVal_Character::GetValCharacter() { return this; }
UVal_CharacterMovementComponent* AVal_Character::GetValMovementComponent() const { return Cast<UVal_CharacterMovementComponent>(GetCharacterMovement()); }
UVal_PlayerAnimInstance* AVal_Character::GetValAnimInstance() const { return Cast<UVal_PlayerAnimInstance>(characterMesh->GetAnimInstance()); }
UVal_PlayerInventory* AVal_Character::GetPlayerInventory() const { return _inventory; }

UVal_WeaponAnimConfig* AVal_Character::getCurrentAnimAsset() const
{
    ACommonWeapon* w = _inventory->getEquippedWeapon();
    return w ? w->getAnimAsset() : nullptr;
}
