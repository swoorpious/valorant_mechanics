// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Val_Character.generated.h"

class UVal_AnimInstance;
class UVal_InputComponent;
class AVal_PlayerController;
class USkeletalMeshComponent;
class UCameraComponent;
class ACommonWeapon;
class UAnimMontage;
class UVal_CharacterMovementComponent;

UCLASS()
class VALORANTMECHANICS_API AVal_Character : public ACharacter
{
	GENERATED_BODY()

public:
	explicit AVal_Character(const FObjectInitializer& ObjectInitializer);

	bool isJumping = false;
	bool isLanded = false;
	
	
	virtual void Jump() override;
	virtual void Landed(const FHitResult& Hit) override;
	void Walk();
	void Unwalk();


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UVal_InputComponent> valInputComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wushu")
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mesh", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> characterMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> characterMeshCamera;

	UPROPERTY(EditDefaultsOnly, Category = "Equipped Weapons")
	TSubclassOf<ACommonWeapon> meleeWeaponToEquip = nullptr;

	// can be used to spawn with weapons
	UPROPERTY(EditDefaultsOnly, Category = "Equipped Weapons")
	TSubclassOf<ACommonWeapon> secondaryWeaponToEquip = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Equipped Weapons")
	TSubclassOf<ACommonWeapon> primaryWeaponToEquip = nullptr;


	UFUNCTION(BlueprintCallable, Category="Equipped Weapons")
	ACommonWeapon* SpawnWeapon(TSubclassOf<ACommonWeapon> weaponToEquip, FName socketName, bool isHidden);
	

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	// UCameraComponent* Wushu_GameplayCaptureCamera;

protected:
	virtual void BeginPlay() override;

	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;


private:
	UPROPERTY()	TObjectPtr<AVal_PlayerController> playerController = nullptr;
	UPROPERTY()	TObjectPtr<UVal_CharacterMovementComponent> movementComponent = nullptr;
	
	UPROPERTY() TObjectPtr<ACommonWeapon> currentEquippedWeapon = nullptr;
	UPROPERTY() TObjectPtr<ACommonWeapon> meleeWeapon = nullptr;
	UPROPERTY() TObjectPtr<ACommonWeapon> secondaryWeapon = nullptr;
	UPROPERTY() TObjectPtr<ACommonWeapon> primaryWeapon = nullptr;
	
	
	
	float RunSpeed = 750.0f;
	float WalkSpeed = 400.0f;
	float RegularAcceleration = 3072.0f;

	// jump
	float TimeSinceLanded = 0.0f;
	float BunnyHopTimeThreshold = 0.08f;
	float AfterJumpWalkSpeed = 300.0f; // could be 150.0f
	float JumpStunDuration = 0.2f;
	float WhileJumpingMovementAcceleration = 1024.0f;
	float WhileJumpingWalkSpeed = 150.0f;

};
