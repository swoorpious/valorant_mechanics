// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "Val_Character.h"
#include "Val_CharacterMovementComponent.h"
#include "Controller/Val_PlayerController.h"
#include "ValorantMechanics/Anim/Val_AnimInstance.h"
#include "ValorantMechanics/Weapons/CommonWeapon.h"
#include "ValorantMechanics/ValorantMechanics.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/LocalPlayer.h"
#include "Materials/MaterialInstanceDynamic.h"




AVal_Character::AVal_Character(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer.SetDefaultSubobjectClass<UVal_CharacterMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	socketData = CreateDefaultSubobject<UPlayerSocketNames>(TEXT("Player Socket Names"));	
	
	
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
	

	// Wushu_GameplayCaptureCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Wushu_GameplayCaptureCamera"));
	// Wushu_GameplayCaptureCamera->SetupAttachment(Wushu_Mesh, TEXT("Camera"));
	// // Wushu_GameplayCaptureCamera->AttachToComponent(Wushu_Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Camera"));
	// Wushu_GameplayCaptureCamera->bUsePawnControlRotation = true;
	

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	bReplicates = true;
	
}


void AVal_Character::BeginPlay()
{
	Super::BeginPlay();
	
	movementComponent = GetValMovementComponent();
	playerAnimInstance = Cast<UVal_AnimInstance>(characterMesh->GetAnimInstance());
	if (!movementComponent) LOG(Val_Player, Error, "movementComponent is likely a nullptr.");
	if (!playerAnimInstance) LOG(Val_Player, Error, "playerAnimInstance is likely a nullptr.");
	

	if (meleeWeaponToSpawn) SpawnWeapon(meleeWeaponToSpawn, !secondaryWeaponToSpawn && !primaryWeaponToSpawn);
	if (secondaryWeaponToSpawn) SpawnWeapon(secondaryWeaponToSpawn, !primaryWeaponToSpawn);
	if (primaryWeaponToSpawn) SpawnWeapon(primaryWeaponToSpawn, true);

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
		playerInventory.UpdateInventoryWeapon(spawnedWeapon);

		playerAnimInstance->UpdateAnimDataAsset(spawnedWeapon);
		onWeaponSpawn.Broadcast(spawnedWeapon);

		if (shouldAutoEquip) this->EquipWeapon(spawnedWeapon->GetWeaponType());
		
	}
	
}



void AVal_Character::EquipWeapon(const EWeaponType weaponType)
{

	/*
	 * check if the animation data asset for the give type in playerAnimInstance is the same as the weapon in playerInventory.
	 * if not, we don't really care, we just return. 
	 */
	TObjectPtr<ACommonWeapon> const invWeapon = playerInventory.GetWeaponByType(weaponType);
	TObjectPtr<UDataAsset> const animDataInstance = playerAnimInstance->GetAnimDataAsset(weaponType);
	TObjectPtr<UDataAsset> const animDataWeapon = invWeapon ? invWeapon->GetAnimAsset() : nullptr;

	bool const validAnim = playerAnimInstance && animDataWeapon == animDataInstance;
	
	if (!validAnim || !playerInventory.HasWeapon(weaponType)) return;
	
	
	playerAnimInstance->UpdateCurrentWeapon(weaponType);
	playerAnimInstance->UpdateWeaponState(EWeaponState::Equipping);
	
	playerInventory.UpdateEquippedWeapon(weaponType);
	for (const auto& pair : playerInventory.GetInventory())
	{
		if (ACommonWeapon* e = pair.Value) e->SetActorHiddenInGame(pair.Key != weaponType);
	}
	
	onWeaponEquip.Broadcast(weaponType);
}



void AVal_Character::DropWeapon(EWeaponType weaponType)
{
	if (playerInventory.GetEquippedWeapon()->GetWeaponType() == weaponType)
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
				this->EquipWeapon(playerInventory.HasWeapon(EWeaponType::Primary) ? EWeaponType::Primary : EWeaponType::Melee);
				break;
			case EWeaponType::Primary:
				// when primary is dropped, equip secondary if it exists, or equip melee
				this->EquipWeapon(playerInventory.HasWeapon(EWeaponType::Secondary) ? EWeaponType::Secondary : EWeaponType::Melee);
				break;
		}
	}

	// TODO: change this when implementing weapon drop and physics for weapon 
	playerInventory.GetWeaponByType(weaponType)->Destroy();
	playerInventory.DropWeaponByType(weaponType);
	onWeaponDrop.Broadcast(weaponType);
}



void AVal_Character::Jump()
{
	Super::Jump();

	isLanded = false;
	isJumping = true;
	

	// movementComponent->MaxAcceleration = WhileJumpingMovementAcceleration;
	// movementComponent->MaxWalkSpeed = WhileJumpingWalkSpeed;
}

void AVal_Character::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	// // PlayerController->OnLanded();
	//
	// isLanded = true;
	// isJumping = false;
	// UE_LOG(LogTemp, Warning, TEXT("Landed")); 
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

	
#pragma region Jump Logic
	/*if (isLanded && !isJumping)
	{
		if (TimeSinceLanded < BunnyHopTimeThreshold)
		{
			UE_LOG(LogTemp, Warning, TEXT("Landed, before BunnyHopTimeThreshold"));

			TimeSinceLanded += DeltaTime;
			movementComponent->MaxAcceleration = RegularAcceleration;
			movementComponent->MaxWalkSpeed = RunSpeed;
		}
	    else if (TimeSinceLanded < JumpStunDuration)
	    {
	    	UE_LOG(LogTemp, Warning, TEXT("Landed, before JumpStunDuration"));
		    TimeSinceLanded += DeltaTime;
	    	movementComponent->MaxAcceleration = RegularAcceleration;
	    	movementComponent->MaxWalkSpeed = AfterJumpWalkSpeed;
	    }
		else if (TimeSinceLanded >= JumpStunDuration)
		{
			UE_LOG(LogTemp, Warning, TEXT("Landed, just after JumpStunDuration"));
	
			TimeSinceLanded = 0.0f;
			isLanded = false;
			isJumping = false;
			movementComponent->MaxAcceleration = RegularAcceleration;
			movementComponent->MaxWalkSpeed = RunSpeed;
		}
	}*/
#pragma endregion
}



void AVal_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}



#pragma region Inventory Implementation

void FPlayerInventory::UpdateInventoryWeapon(const TObjectPtr<ACommonWeapon>& weapon)
{
	// this definition is defined in .cpp due to ACommonWeapon being incomplete
	EWeaponType weaponType = weapon->GetWeaponType();
	if (!weapon || weaponType == EWeaponType::Empty) return;
	this->HasWeapon(weaponType) ?
		inventoryMap[weaponType] = weapon :
		inventoryMap.Add(weaponType, weapon);
}

void FPlayerInventory::UpdateEquippedWeapon(EWeaponType weaponType)
{
	if (!this->HasWeapon(weaponType)) return;
	equippedWeaponType = weaponType;
}


TObjectPtr<ACommonWeapon> FPlayerInventory::GetWeaponByType(EWeaponType weaponType) const
{
	if (weaponType == EWeaponType::Empty || !this->HasWeapon(weaponType)) return nullptr;
	return inventoryMap[weaponType];
}


void FPlayerInventory::DropWeaponByType(EWeaponType weaponType)
{
	if (weaponType == EWeaponType::Empty || !this->HasWeapon(weaponType)) return;
	inventoryMap[weaponType] = nullptr;
}

#pragma endregion Inventory Implementation




AVal_PlayerController* AVal_Character::GetValPlayerController() const { return Cast<AVal_PlayerController>(GetController()); }
AVal_Character* AVal_Character::GetValCharacter() { return this; }
UVal_CharacterMovementComponent* AVal_Character::GetValMovementComponent() const { return Cast<UVal_CharacterMovementComponent>(GetCharacterMovement()); }
UVal_AnimInstance* AVal_Character::GetValAnimInstance() const { return Cast<UVal_AnimInstance>(characterMesh->GetAnimInstance()); }

