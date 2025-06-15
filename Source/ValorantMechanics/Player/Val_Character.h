// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ValorantMechanics/Weapons/SharedWeapon.h"
#include "Val_Character.generated.h"

class UVal_AnimInstance;
class UVal_InputComponent;
class AVal_PlayerController;
class USkeletalMeshComponent;
class UCameraComponent;
class ACommonWeapon;
class UAnimMontage;
class UVal_CharacterMovementComponent;



#define MASTER_SOCKET TEXT("R_WeaponMasterSocket")




// basic code for inventory
USTRUCT()
struct FPlayerInventory
{
	GENERATED_BODY();

private:
	UPROPERTY() EWeaponType equippedWeaponType = EWeaponType::Empty; // set to EWeaponType::Empty by default, updated to whichever the player spawns with
	UPROPERTY() TMap<EWeaponType, TObjectPtr<ACommonWeapon>> inventoryMap; // cannot have EWeaponType::Empty

public:

	// does not update equippedWeaponType
	// updates if weapon of EWeaponType exists
	// otherwise adds weapon of EWeaponType
	void UpdateInventoryWeapon(TObjectPtr<ACommonWeapon> weapon);

	// only updates equippedWeaponType if weapon type exists in inventoryMap
	// use UpdateInventoryWeapon to add/update inventory slots
	void UpdateCurrentWeapon(EWeaponType weaponType);
	
	TObjectPtr<ACommonWeapon> GetInventoryWeapon(EWeaponType weaponType) const;
	TObjectPtr<ACommonWeapon> GetCurrentWeapon() const;
	TMap<EWeaponType, TObjectPtr<ACommonWeapon>> GetInventory() const { return inventoryMap; }
	
	bool HasWeapon(EWeaponType weaponType) const { return inventoryMap.FindRef(weaponType) != nullptr; } 
};




UCLASS()
class VALORANTMECHANICS_API AVal_Character : public ACharacter
{
	GENERATED_BODY()

public:
	explicit AVal_Character(const FObjectInitializer& ObjectInitializer);
	UVal_InputComponent* GetValInputComponent() { return valInputComponent; }
	

	bool isJumping = false;
	bool isLanded = false;
	
	
	virtual void Jump() override;
	virtual void Landed(const FHitResult& Hit) override;
	void Walk();
	void Unwalk();


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UVal_InputComponent> valInputComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wushu")
	TObjectPtr<USceneComponent> sceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mesh", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> characterMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> characterMeshCamera;

	UPROPERTY(EditDefaultsOnly, Category = "Equipped Weapons")
	TSubclassOf<ACommonWeapon> meleeWeaponToSpawn = nullptr;

	// can be used to spawn with weapons
	UPROPERTY(EditDefaultsOnly, Category = "Equipped Weapons")
	TSubclassOf<ACommonWeapon> secondaryWeaponToSpawn = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Equipped Weapons")
	TSubclassOf<ACommonWeapon> primaryWeaponToSpawn = nullptr;


	UFUNCTION(BlueprintCallable, Category="Equipped Weapons")
	void SpawnWeapon(TSubclassOf<ACommonWeapon> weaponToSpawn, FName socketName, bool shouldAutoEquip);
	
	UFUNCTION(BlueprintCallable, Category="Equipped Weapons")
	void EquipWeapon(ACommonWeapon* weapon);

protected:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY()	TObjectPtr<AVal_PlayerController> playerController = nullptr;
	UPROPERTY()	TObjectPtr<UVal_CharacterMovementComponent> movementComponent = nullptr;
	UPROPERTY()	TObjectPtr<UVal_AnimInstance> playerAnimInstance = nullptr;
	
	UPROPERTY() FPlayerInventory playerInventory;

	
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







