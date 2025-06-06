// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "Val_Character.h"

#include "Val_CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/LocalPlayer.h"
#include "Val_PlayerController.h"
#include "ValorantMechanics/Input/Val_InputComponent.h"


#include "ValorantMechanics/Weapons/CommonWeapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"


AVal_Character::AVal_Character(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer.SetDefaultSubobjectClass<UVal_CharacterMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = GetCapsuleComponent();

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SceneComponent->SetupAttachment(GetCapsuleComponent());
	
	characterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Character Mesh"));
	characterMesh->SetupAttachment(SceneComponent);
	characterMesh->CastShadow = false;
	characterMesh->bCastDynamicShadow = true;
	
	characterMeshCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Character Mesh Camera"));
	characterMeshCamera->SetupAttachment(characterMesh, TEXT("Camera"));
	characterMeshCamera->bUsePawnControlRotation = false;

	// Wushu_GameplayCaptureCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Wushu_GameplayCaptureCamera"));
	// Wushu_GameplayCaptureCamera->SetupAttachment(Wushu_Mesh, TEXT("Camera"));
	// // Wushu_GameplayCaptureCamera->AttachToComponent(Wushu_Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Camera"));
	// Wushu_GameplayCaptureCamera->bUsePawnControlRotation = true;

	movementComponent = Cast<UVal_CharacterMovementComponent>(GetCharacterMovement());
	playerController = Cast<AVal_PlayerController>(GetController());
	valInputComponent = CreateDefaultSubobject<UVal_InputComponent>(TEXT("Valorant Input Component"));
	valInputComponent->SetIsReplicated(true);
	

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
}


ACommonWeapon* AVal_Character::SpawnWeapon(TSubclassOf<ACommonWeapon> weaponToEquip, FName socketName, bool isHidden)
{
	if (!weaponToEquip) return nullptr;

	UE_LOG(LogTemp, Display, TEXT("Spawning Weapon"));
	ACommonWeapon* spawnedWeapon = GetWorld()->SpawnActor<ACommonWeapon>(weaponToEquip);
	
	if (spawnedWeapon)
	{
		spawnedWeapon->SetOwner(this);
		spawnedWeapon->AttachToComponent(characterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, socketName);
		if (isHidden) spawnedWeapon->SetActorHiddenInGame(true);
	}

	return spawnedWeapon;
}


void AVal_Character::BeginPlay()
{
	Super::BeginPlay();
	movementComponent->MaxAcceleration = RegularAcceleration;
	movementComponent->MaxWalkSpeed = RunSpeed;

	meleeWeapon ? meleeWeapon = SpawnWeapon(meleeWeaponToEquip, TEXT("R_WeaponMasterSocket"), false) : meleeWeapon = nullptr;
	secondaryWeapon ? secondaryWeapon = SpawnWeapon(secondaryWeaponToEquip, TEXT("R_WeaponMasterSocket"), true) : secondaryWeapon = nullptr;
	primaryWeapon ? primaryWeapon = SpawnWeapon(primaryWeaponToEquip, TEXT("R_WeaponMasterSocket"), true) : primaryWeapon = nullptr;
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
