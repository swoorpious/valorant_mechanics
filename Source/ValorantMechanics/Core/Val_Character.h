// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"

#include "Shared/PlayerDelegateDefinition.h"
// #include "Shared/WeaponData/WeaponProperties.h"
#include "Shared/SocketData.h"
#include "PlayerInventory.h"

#include "GameFramework/Character.h"

#include "Val_Character.generated.h"


// forward declarations
class UVal_AnimInstance;
class UVal_InputComponent;
class UVal_CharacterMovementComponent;
class AVal_PlayerController;
// class UPlayerInventory;

class USkeletalMeshComponent;
class UCameraComponent;
class ACommonWeapon;





UCLASS()
class VALORANTMECHANICS_API AVal_Character : public ACharacter
{
	GENERATED_BODY()

public:
	explicit AVal_Character(const FObjectInitializer& ObjectInitializer);

	// getter functions defined from IVal_PlayerInterface
	AVal_PlayerController* GetValPlayerController() const;
	AVal_Character* GetValCharacter();
	UVal_CharacterMovementComponent* GetValMovementComponent() const;
	UVal_AnimInstance* GetValAnimInstance() const;


	bool isJumping = false;
	bool isLanded = false;

	
	DRequestWeaponSpawn tryWeaponSpawn;
	DSuccessWeaponSpawn onWeaponSpawn;
	
	DRequestWeaponDrop tryWeaponDrop;
	DSuccessWeaponEquip onWeaponDrop;
	
	DRequestWeaponEquip tryWeaponEquip;
	DSuccessWeaponDrop onWeaponEquip;


	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Valorant Character|Character Setup|Scene")
	TObjectPtr<USceneComponent> sceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Valorant Character|Character Setup|Scene|Mesh", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> characterMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Valorant Character|Character Setup|Scene|Mesh|Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> characterMeshCamera;

	
	
	// can be used to spawn with weapons

		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Valorant Character|Mesh|Sockets")
	TObjectPtr<UPlayerSocketNames> socketData;

	

	virtual void Jump() override;
	virtual void Landed(const FHitResult& Hit) override;
	
	void Walk();
	void Unwalk();
	
	void SpawnWeapon(const TSubclassOf<ACommonWeapon>& weaponToSpawn, bool shouldAutoEquip);
	void EquipWeapon(const EWeaponType weaponType);
	void DropWeapon(EWeaponType weaponType);
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Valorant Weapons")
	TObjectPtr<UPlayerInventory> playerInventory;
	
protected:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// caching for some reason
	UPROPERTY()	TObjectPtr<UVal_CharacterMovementComponent> movementComponent = nullptr;
	UPROPERTY()	TObjectPtr<UVal_AnimInstance> playerAnimInstance = nullptr;
	
	

	float TimeSinceLanded = 0.0f;
	
};




