// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#include "Wushu_Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/LocalPlayer.h"
#include "Wushu_PlayerController.h"
#include "Wushu_CharacterMovementComponent.h"

#include "ValorantMechanics/Anim/WushuAnimInstance.h"

#include "ValorantMechanics/Weapons/CommonWeapon.h"
#include "ValorantMechanics/Weapons/Melee/TacticalKnife.h"


#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"


AWushu_Character::AWushu_Character(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer.SetDefaultSubobjectClass<UWushu_CharacterMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	
	PlayerCharacterMovementComponent = GetCharacterMovement();
	RootComponent = GetCapsuleComponent();

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SceneComponent->SetupAttachment(GetCapsuleComponent());
	
	Wushu_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Wushu_Mesh"));
	Wushu_Mesh->SetupAttachment(SceneComponent);
	Wushu_Mesh->CastShadow = false;
	Wushu_Mesh->bCastDynamicShadow = true;
	
	Wushu_MeshCaptureCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Wushu_MeshCaptureCamera"));
	Wushu_MeshCaptureCamera->SetupAttachment(Wushu_Mesh, TEXT("Camera"));
	Wushu_MeshCaptureCamera->bUsePawnControlRotation = false;

	// Wushu_GameplayCaptureCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Wushu_GameplayCaptureCamera"));
	// Wushu_GameplayCaptureCamera->SetupAttachment(Wushu_Mesh, TEXT("Camera"));
	// // Wushu_GameplayCaptureCamera->AttachToComponent(Wushu_Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Camera"));
	// Wushu_GameplayCaptureCamera->bUsePawnControlRotation = true;

	PlayerController = Cast<AWushu_PlayerController>(GetController());


	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
}

ACommonWeapon* AWushu_Character::SpawnWeapon(TSubclassOf<ACommonWeapon> weaponToEquip, FName socketName)
{
	if (!weaponToEquip) return nullptr;
	
	ACommonWeapon* spawnedWeapon = GetWorld()->SpawnActor<ACommonWeapon>(weaponToEquip);
	
	if (spawnedWeapon)
	{
		spawnedWeapon->SetOwner(this);
		spawnedWeapon->AttachToComponent(Wushu_Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, socketName);
	}

	return spawnedWeapon;
}


void AWushu_Character::BeginPlay()
{
	Super::BeginPlay();
	PlayerCharacterMovementComponent->MaxAcceleration = RegularAcceleration;
	PlayerCharacterMovementComponent->MaxWalkSpeed = RunSpeed;

	meleeWeapon = SpawnWeapon(MeleeWeaponToEquip, TEXT("R_WeaponMasterSocket"));
}


void AWushu_Character::Jump()
{
	Super::Jump();

	isLanded = false;
	isJumping = true;
	
	if (TimeSinceLanded < BunnyHopTimeThreshold) TimeSinceLanded = 0.0f;

	// PlayerCharacterMovementComponent->MaxAcceleration = WhileJumpingMovementAcceleration;
	// PlayerCharacterMovementComponent->MaxWalkSpeed = WhileJumpingWalkSpeed;
}

void AWushu_Character::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	// // PlayerController->OnLanded();
	//
	// isLanded = true;
	// isJumping = false;
	// UE_LOG(LogTemp, Warning, TEXT("Landed")); 
}

void AWushu_Character::Walk()
{
	UE_LOG(LogTemp, Warning, TEXT("Walking"));
	PlayerCharacterMovementComponent->MaxWalkSpeed = WalkSpeed;
}

void AWushu_Character::Unwalk() 
{
	UE_LOG(LogTemp, Warning, TEXT("Unwalking"));
	PlayerCharacterMovementComponent->MaxWalkSpeed = RunSpeed;
}


void AWushu_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
#pragma region Jump Logic
	/*if (isLanded && !isJumping)
	{
		if (TimeSinceLanded < BunnyHopTimeThreshold)
		{
			UE_LOG(LogTemp, Warning, TEXT("Landed, before BunnyHopTimeThreshold"));

			TimeSinceLanded += DeltaTime;
			PlayerCharacterMovementComponent->MaxAcceleration = RegularAcceleration;
			PlayerCharacterMovementComponent->MaxWalkSpeed = RunSpeed;
		}
	    else if (TimeSinceLanded < JumpStunDuration)
	    {
	    	UE_LOG(LogTemp, Warning, TEXT("Landed, before JumpStunDuration"));
		    TimeSinceLanded += DeltaTime;
	    	PlayerCharacterMovementComponent->MaxAcceleration = RegularAcceleration;
	    	PlayerCharacterMovementComponent->MaxWalkSpeed = AfterJumpWalkSpeed;
	    }
		else if (TimeSinceLanded >= JumpStunDuration)
		{
			UE_LOG(LogTemp, Warning, TEXT("Landed, just after JumpStunDuration"));
	
			TimeSinceLanded = 0.0f;
			isLanded = false;
			isJumping = false;
			PlayerCharacterMovementComponent->MaxAcceleration = RegularAcceleration;
			PlayerCharacterMovementComponent->MaxWalkSpeed = RunSpeed;
		}
	}*/
#pragma endregion
	
}

void AWushu_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void AWushu_Character::AllowPlayerAirControl() const
{
	// hasPlayerAirControl = true;
	PlayerCharacterMovementComponent->AirControl = 200.0f;
	PlayerCharacterMovementComponent->AirControlBoostMultiplier = 20.0f;
	PlayerCharacterMovementComponent->AirControlBoostVelocityThreshold = 400.0f;
	PlayerCharacterMovementComponent->FallingLateralFriction = 0.5f;
}

void AWushu_Character::UnallowPlayerAirControl() const
{
	// hasPlayerAirControl = false;
	PlayerCharacterMovementComponent->AirControl = 0;
	PlayerCharacterMovementComponent->AirControlBoostMultiplier = 0;
	PlayerCharacterMovementComponent->AirControlBoostVelocityThreshold = 0;
	PlayerCharacterMovementComponent->FallingLateralFriction = 0;
}
