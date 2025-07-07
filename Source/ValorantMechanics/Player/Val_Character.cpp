// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "Val_Character.h"

#include "ValorantMechanics/Anim/Val_PlayerAnimInstance.h"
#include "ValorantMechanics/Weapons/CommonWeapon.h"
#include "ValorantMechanics/ValorantMechanics.h"
#include "ValorantMechanics/Core/Val_DefaultGameMode.h"

#include "PlayerComponents/Val_CharacterMovementComponent.h"
#include "PlayerComponents/Val_PlayerInventory.h"
#include "Controller/Val_PlayerController.h"

#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/LocalPlayer.h"
#include "Materials/MaterialInstanceDynamic.h"


AVal_Character::AVal_Character(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer.SetDefaultSubobjectClass<UVal_CharacterMovementComponent>(CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    socketData = CreateDefaultSubobject<UPlayerSocketNames>(TEXT("Player Socket Names"));
    pInventory = CreateDefaultSubobject<UVal_PlayerInventory>(TEXT("Player Inventory"));
    

    RootComponent = GetCapsuleComponent();
    sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    sceneComponent->SetupAttachment(RootComponent);
    
    characterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Character Mesh"));
    characterMesh->SetupAttachment(sceneComponent);
    characterMesh->CastShadow = false;
    characterMesh->bCastDynamicShadow = true;
    characterMesh->SetSimulatePhysics(false);
    
    characterMeshCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Character Mesh Camera"));
    characterMeshCamera->SetupAttachment(characterMesh, socketData->cameraSocket);
    characterMeshCamera->bUsePawnControlRotation = false;
    

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
    
    pMovement = Cast<UVal_CharacterMovementComponent>(GetCharacterMovement());
    pAnimInstance = Cast<UVal_PlayerAnimInstance>(characterMesh->GetAnimInstance());
    if (!pMovement) LOG(Val_Player, Error, "pMovement is likely a nullptr.");
    if (!pAnimInstance) LOG(Val_Player, Error, "pAnimInstance is likely a nullptr.");

    if (const auto* e = GetWorld()->GetAuthGameMode<AVal_DefaultGameMode>())
    {
        if (e->meleeToSpawn) SpawnWeapon(e->meleeToSpawn, !e->secondaryToSpawn && !e->primaryToSpawn);
        if (e->secondaryToSpawn) SpawnWeapon(e->secondaryToSpawn, !e->primaryToSpawn);
        if (e->primaryToSpawn) SpawnWeapon(e->primaryToSpawn, true);
    }
    

    // tryWeaponEquip.AddUObject(this, &AVal_Character::EquipWeapon);
    // tryWeaponSpawn.AddUObject(this, &AVal_Character::SpawnWeapon);
    // tryWeaponDrop.AddUObject(this, &AVal_Character::DropWeapon);
    
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

    
    if (ACommonWeapon* spawnedWeapon = GetWorld()->SpawnActor<ACommonWeapon>(weaponToSpawn))
    {
        spawnedWeapon->SetOwner(this);
        spawnedWeapon->AttachToComponent(characterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, socketData->rightWeaponMasterSocket);
        spawnedWeapon->SetActorHiddenInGame(true); // hidden by default, EquipWeapon(...) will unhide
        pInventory->UpdateInventoryWeapon(spawnedWeapon);

        pAnimInstance->UpdateAnimDataAsset(spawnedWeapon);

        if (shouldAutoEquip) this->EquipWeapon(spawnedWeapon->GetWeaponType(), EWeaponLogicState::Equip_Default);
        
    }
    
}


void AVal_Character::EquipWeapon(const EWeaponType weaponType, const EWeaponLogicState equipType = EWeaponLogicState::Equip_Default)
{
    if (equipType != EWeaponLogicState::Equip_Default && equipType != EWeaponLogicState::Equip_Fast) return;
    if (pInventory->GetEquippedWeapon()->GetWeaponType() == weaponType) return;

    UnequipWeapon(weaponType);
    
    const TObjectPtr<ACommonWeapon>* invWeapon = &pInventory->GetWeaponByType(weaponType);
    const TObjectPtr<UWeaponAnimDataAsset>* animDataInstance = &pAnimInstance->GetAnimDataAsset(weaponType);
    const TObjectPtr<UWeaponAnimDataAsset>* animDataWeapon = invWeapon ? &invWeapon->GetAnimAsset() : nullptr;

    bool const validAnim = pAnimInstance && animDataWeapon == animDataInstance;

    if (!validAnim || !pInventory->HasWeapon(weaponType)) return;

    for (const auto& pair : pInventory->GetInventory())
        if (ACommonWeapon* e = pair.Value) e->SetActorHiddenInGame(true);


    auto* weaponLSM = invWeapon->GetPrimaryStateManager();
    weaponLSM->TryTransitionToState(EWeaponLogicState::None);
    

    pInventory->UpdateEquippedWeapon(weaponType);
    pAnimInstance->UpdateCurrentWeapon(weaponType);
    weaponLSM->TryTransitionToState(equipType);

    for (const auto& pair : pInventory->GetInventory())
        if (ACommonWeapon* e = pair.Value) e->SetActorHiddenInGame(pair.Key != weaponType);
    
}



void AVal_Character::UnequipWeapon(const EWeaponType weaponType)
{
}

void AVal_Character::DropWeapon(EWeaponType weaponType)
{
    if (pInventory->GetEquippedWeapon()->GetWeaponType() == weaponType)
    {
        switch (weaponType)
        {
            case EWeaponType::Melee:
            case EWeaponType::Empty:
            default:
                // cannot drop an empty, or...the melee lol
                return;
            case EWeaponType::Secondary:
                // when secondary is dropped, equip primary if it exists, or equip melee
                this->EquipWeapon(pInventory->HasWeapon(EWeaponType::Primary) ? EWeaponType::Primary : EWeaponType::Melee);
                break;
            case EWeaponType::Primary:
                // when primary is dropped, equip secondary if it exists, or equip melee
                this->EquipWeapon(pInventory->HasWeapon(EWeaponType::Secondary) ? EWeaponType::Secondary : EWeaponType::Melee);
                break;
        }
    }

    // TODO: change this when implementing weapon drop and physics for weapon 
    pInventory->GetWeaponByType(weaponType)->Destroy();
    pInventory->DropWeaponByType(weaponType);
    
}



AVal_PlayerController* AVal_Character::GetValPlayerController() const { return Cast<AVal_PlayerController>(GetController()); }
AVal_Character* AVal_Character::GetValCharacter() { return this; }
UVal_CharacterMovementComponent* AVal_Character::GetValMovementComponent() const { return Cast<UVal_CharacterMovementComponent>(GetCharacterMovement()); }
UVal_PlayerAnimInstance* AVal_Character::GetValAnimInstance() const { return Cast<UVal_PlayerAnimInstance>(characterMesh->GetAnimInstance()); }

