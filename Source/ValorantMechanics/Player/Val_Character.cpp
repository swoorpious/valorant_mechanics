// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "Val_Character.h"

#include "Val_CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/LocalPlayer.h"
#include "Val_PlayerController.h"
#include "ValorantMechanics/Input/Val_InputComponent.h"
#include "ValorantMechanics/Anim/Val_AnimInstance.h"


#include "ValorantMechanics/Weapons/CommonWeapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"


AVal_Character::AVal_Character(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer.SetDefaultSubobjectClass<UVal_CharacterMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = GetCapsuleComponent();

	sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	sceneComponent->SetupAttachment(GetCapsuleComponent());
	
	characterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Character Mesh"));
	characterMesh->SetupAttachment(sceneComponent);
	characterMesh->CastShadow = false;
	characterMesh->bCastDynamicShadow = true;
	
	characterMeshCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Character Mesh Camera"));
	characterMeshCamera->SetupAttachment(characterMesh, TEXT("Camera"));
	characterMeshCamera->bUsePawnControlRotation = false;

	valInputComponent = CreateDefaultSubobject<UVal_InputComponent>(TEXT("Valorant Input Component"));
	valInputComponent->SetIsReplicated(true);

	// Wushu_GameplayCaptureCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Wushu_GameplayCaptureCamera"));
	// Wushu_GameplayCaptureCamera->SetupAttachment(Wushu_Mesh, TEXT("Camera"));
	// // Wushu_GameplayCaptureCamera->AttachToComponent(Wushu_Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Camera"));
	// Wushu_GameplayCaptureCamera->bUsePawnControlRotation = true;

	movementComponent = Cast<UVal_CharacterMovementComponent>(GetCharacterMovement());
	playerController = Cast<AVal_PlayerController>(GetController());
	playerAnimInstance = Cast<UVal_AnimInstance>(characterMesh->GetAnimInstance());
	

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	
}


// TODO: make common list for socket names, and remove @param socketName
void AVal_Character::SpawnWeapon(TSubclassOf<ACommonWeapon> weaponToSpawn, FName socketName, bool shouldAutoEquip)
{
	if (!weaponToSpawn) return;

	UE_LOG(LogTemp, Display, TEXT("SpawnWeapon is spawning weapon"));

	if (ACommonWeapon* spawnedWeapon = GetWorld()->SpawnActor<ACommonWeapon>(weaponToSpawn))
	{
		spawnedWeapon->SetOwner(this);
		spawnedWeapon->AttachToComponent(characterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, socketName);
		spawnedWeapon->SetActorHiddenInGame(true); // hidden by default, EquipWeapon(...) will unhide 

		EWeaponType type = spawnedWeapon->GetWeaponType();
		if (playerInventory.HasWeapon(type)) playerInventory.UpdateInventoryWeapon(spawnedWeapon);
		if (shouldAutoEquip) this->EquipWeapon(spawnedWeapon);

		
	}
	
}

void AVal_Character::EquipWeapon(ACommonWeapon* weaponToEquip)
{
	if (!playerAnimInstance || !weaponToEquip) return;
	
	
}


void AVal_Character::BeginPlay()
{
	Super::BeginPlay();
	movementComponent->MaxAcceleration = RegularAcceleration;
	movementComponent->MaxWalkSpeed = RunSpeed;


	if (meleeWeaponToSpawn) SpawnWeapon(meleeWeaponToSpawn, MASTER_SOCKET, secondaryWeaponToSpawn || primaryWeaponToSpawn);
	if (secondaryWeaponToSpawn) SpawnWeapon(secondaryWeaponToSpawn, MASTER_SOCKET, !secondaryWeaponToSpawn || primaryWeaponToSpawn);
	if (primaryWeaponToSpawn) SpawnWeapon(primaryWeaponToSpawn, MASTER_SOCKET, !primaryWeaponToSpawn);
	
}


void AVal_Character::Jump()
{
	Super::Jump();

	isLanded = false;
	isJumping = true;
	
	if (TimeSinceLanded < BunnyHopTimeThreshold) TimeSinceLanded = 0.0f;

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
	movementComponent->MaxWalkSpeed = WalkSpeed;
}

void AVal_Character::Unwalk() 
{
	UE_LOG(LogTemp, Warning, TEXT("Unwalking"));
	movementComponent->MaxWalkSpeed = RunSpeed;
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



// inventory implementation
void FPlayerInventory::UpdateInventoryWeapon(TObjectPtr<ACommonWeapon> weapon)
{
	EWeaponType weaponType = weapon->GetWeaponType();
	if (!weapon || weaponType == EWeaponType::Empty) return;
	this->HasWeapon(weaponType) ?
		inventoryMap[weaponType] = weapon :
		inventoryMap.Add(weaponType, weapon);
}

void FPlayerInventory::UpdateCurrentWeapon(EWeaponType weaponType)
{
	if (!this->HasWeapon(weaponType)) return;
	equippedWeaponType = weaponType;
}


TObjectPtr<ACommonWeapon> FPlayerInventory::GetInventoryWeapon(EWeaponType weaponType) const
{
	if (!this->HasWeapon(weaponType)) return nullptr;
	return inventoryMap[weaponType];
}

TObjectPtr<ACommonWeapon> FPlayerInventory::GetCurrentWeapon() const
{
	if (equippedWeaponType == EWeaponType::Empty) return nullptr;
	return inventoryMap[equippedWeaponType];
}


