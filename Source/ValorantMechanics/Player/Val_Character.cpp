// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "Val_Character.h"

#include "ValorantMechanics/Anim/Val_AnimInstance.h"
#include "ValorantMechanics/Weapons/CommonWeapon.h"
#include "ValorantMechanics/ValorantMechanics.h"

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
	
	movementComponent = GetValMovementComponent();
	pAnimInstance = Cast<UVal_AnimInstance>(characterMesh->GetAnimInstance());
	if (!movementComponent) LOG(Val_Player, Error, "movementComponent is likely a nullptr.");
	if (!pAnimInstance) LOG(Val_Player, Error, "pAnimInstance is likely a nullptr.");
	

	if (pInventory->meleeToSpawn) SpawnWeapon(pInventory->meleeToSpawn, !pInventory->secondaryToSpawn && !pInventory->primaryToSpawn);
	if (pInventory->secondaryToSpawn) SpawnWeapon(pInventory->secondaryToSpawn, !pInventory->primaryToSpawn);
	if (pInventory->primaryToSpawn) SpawnWeapon(pInventory->primaryToSpawn, true);

	tryWeaponEquip.AddUObject(this, &AVal_Character::EquipWeapon);
	// tryWeaponSpawn.AddUObject(this, &AVal_Character::SpawnWeapon);
	tryWeaponDrop.AddUObject(this, &AVal_Character::DropWeapon);
	
}




// TODO: make common list for socket names, and remove @param socketName
void AVal_Character::SpawnWeapon(const TSubclassOf<ACommonWeapon>& weaponToSpawn, bool shouldAutoEquip)
{
	if (!weaponToSpawn) return;

	LOG(Val_Player, Verbose, "");
	
	if (ACommonWeapon* spawnedWeapon = GetWorld()->SpawnActor<ACommonWeapon>(weaponToSpawn))
	{
		spawnedWeapon->SetOwner(this);
		spawnedWeapon->AttachToComponent(characterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, socketData->rightWeaponMasterSocket);
		spawnedWeapon->SetActorHiddenInGame(true); // hidden by default, EquipWeapon(...) will unhide
		pInventory->UpdateInventoryWeapon(spawnedWeapon);

		pAnimInstance->UpdateAnimDataAsset(spawnedWeapon);
		onWeaponSpawn.Broadcast(spawnedWeapon);

		if (shouldAutoEquip) this->EquipWeapon(spawnedWeapon->GetWeaponType());
		
	}
	
}



void AVal_Character::EquipWeapon(const EWeaponType weaponType)
{

	/*
	 * check if the animation data asset for the give type in pAnimInstance is the same as the weapon in pInventory.
	 * if not, we don't really care, we just return. 
	 */
	TObjectPtr<ACommonWeapon> const invWeapon = pInventory->GetWeaponByType(weaponType);
	TObjectPtr<UDataAsset> const animDataInstance = pAnimInstance->GetAnimDataAsset(weaponType);
	TObjectPtr<UDataAsset> const animDataWeapon = invWeapon ? invWeapon->GetAnimAsset() : nullptr;

	bool const validAnim = pAnimInstance && animDataWeapon == animDataInstance;
	
	if (!validAnim || !pInventory->HasWeapon(weaponType)) return;
	
	
	pAnimInstance->UpdateCurrentWeapon(weaponType);
	pAnimInstance->UpdateWeaponState(EWeaponState::Equipping);
	
	pInventory->UpdateEquippedWeapon(weaponType);
	for (const auto& pair : pInventory->GetInventory())
	{
		if (ACommonWeapon* e = pair.Value) e->SetActorHiddenInGame(pair.Key != weaponType);
	}
	
	onWeaponEquip.Broadcast(weaponType);
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
	onWeaponDrop.Broadcast(weaponType);
}



void AVal_Character::Jump()
{
	Super::Jump();
}

void AVal_Character::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
}


void AVal_Character::Walk()
{
	UE_LOG(LogTemp, Warning, TEXT("Walking"));
}


void AVal_Character::Unwalk() 
{
	UE_LOG(LogTemp, Warning, TEXT("Unwalking"));
}


void AVal_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void AVal_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}




AVal_PlayerController* AVal_Character::GetValPlayerController() const { return Cast<AVal_PlayerController>(GetController()); }
AVal_Character* AVal_Character::GetValCharacter() { return this; }
UVal_CharacterMovementComponent* AVal_Character::GetValMovementComponent() const { return Cast<UVal_CharacterMovementComponent>(GetCharacterMovement()); }
UVal_AnimInstance* AVal_Character::GetValAnimInstance() const { return Cast<UVal_AnimInstance>(characterMesh->GetAnimInstance()); }

