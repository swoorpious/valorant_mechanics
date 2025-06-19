// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
// #include "Val_CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "ValorantMechanics/Weapons/SharedWeapon.h"
#include "Val_PlayerInterface.h"
#include "Val_Character.generated.h"


// forward declarations
class UVal_AnimInstance;
class UVal_InputComponent;
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
	
	TObjectPtr<ACommonWeapon> GetWeaponByType(EWeaponType weaponType) const;
	
	FORCEINLINE TObjectPtr<ACommonWeapon> GetCurrentWeapon() const { return this->GetWeaponByType(equippedWeaponType); }
	FORCEINLINE TMap<EWeaponType, TObjectPtr<ACommonWeapon>> GetInventory() const { return inventoryMap; }
	
	bool HasWeapon(EWeaponType weaponType) const { return inventoryMap.FindRef(weaponType) != nullptr; } 
};




UCLASS()
class VALORANTMECHANICS_API AVal_Character : public ACharacter, public IVal_PlayerInterface
{
	GENERATED_BODY()

public:
	explicit AVal_Character(const FObjectInitializer& ObjectInitializer);

	// getter functions defined from IVal_PlayerInterface
	virtual AVal_PlayerController* GetValPlayerController() override;
	virtual AVal_Character* GetValCharacter() override;
	virtual UVal_CharacterMovementComponent* GetValMovementComponent() override;
	virtual UVal_AnimInstance* GetValAnimInstance() override;
	virtual UVal_InputComponent* GetValInputComponent() override;


	bool isJumping = false;
	bool isLanded = false;
	
	
	virtual void Jump() override;
	virtual void Landed(const FHitResult& Hit) override;
	void Walk();
	void Unwalk();


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Input")
	TObjectPtr<UVal_InputComponent> valInputComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Valorant Character|Character Setup|Scene")
	TObjectPtr<USceneComponent> sceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Valorant Character|Character Setup|Scene|Mesh", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> characterMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Valorant Character|Character Setup|Scene|Mesh|Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> characterMeshCamera;

	
	
	// can be used to spawn with weapons
	UPROPERTY(EditDefaultsOnly, Category = "Spawn Properties|Equipped Weapons")
	TSubclassOf<ACommonWeapon> meleeWeaponToSpawn = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn Properties|Equipped Weapons")
	TSubclassOf<ACommonWeapon> secondaryWeaponToSpawn = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn Properties|Equipped Weapons")
	TSubclassOf<ACommonWeapon> primaryWeaponToSpawn = nullptr;

	

	UFUNCTION(BlueprintCallable, Category="Core")
	void SpawnWeapon(TSubclassOf<ACommonWeapon> weaponToSpawn, FName socketName, bool shouldAutoEquip);
	
	UFUNCTION(BlueprintCallable, Category="Core")
	void EquipWeapon(ACommonWeapon* weapon);

protected:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// caching for some reason
	UPROPERTY()	TObjectPtr<UVal_CharacterMovementComponent> movementComponent = nullptr;
	UPROPERTY()	TObjectPtr<UVal_AnimInstance> playerAnimInstance = nullptr;
	
	UPROPERTY() FPlayerInventory playerInventory;

	

	float TimeSinceLanded = 0.0f;
	
};







