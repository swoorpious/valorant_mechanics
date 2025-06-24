// Copyright © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"

#include "Shared/PlayerDelegateDefinition.h"
#include "Shared/WeaponData/WeaponProperties.h"
#include "Shared/SocketData.h"

#include "GameFramework/Character.h"
#include "Val_Character.generated.h"


// forward declarations
class UVal_AnimInstance;
class UVal_InputComponent;
class UVal_CharacterMovementComponent;
class AVal_PlayerController;

class USkeletalMeshComponent;
class UCameraComponent;
class ACommonWeapon;




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
	void UpdateInventoryWeapon(const TObjectPtr<ACommonWeapon>& weapon);

	// only updates equippedWeaponType if weapon type exists in inventoryMap
	// use UpdateInventoryWeapon to add/update inventory slots
	void UpdateEquippedWeapon(EWeaponType weaponType);
	
	TObjectPtr<ACommonWeapon> GetWeaponByType(EWeaponType weaponType) const;

	void DropWeaponByType(EWeaponType weaponType);
	bool HasWeapon(EWeaponType weaponType) const { return inventoryMap.FindRef(weaponType) != nullptr; }
	
	FORCEINLINE TObjectPtr<ACommonWeapon> GetEquippedWeapon() const { return this->GetWeaponByType(equippedWeaponType); }
	FORCEINLINE TMap<EWeaponType, TObjectPtr<ACommonWeapon>> GetInventory() const { return inventoryMap; }

};



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
	
	DRequestWeaponEquip tryWeaponEquip;
	DSuccessWeaponEquip onWeaponDrop;

	DRequestWeaponDrop tryWeaponDrop;
	DSuccessWeaponDrop onWeaponEquip;

	

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

		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Mesh|Sockets")
	TObjectPtr<UPlayerSocketNames> socketData;

	

	virtual void Jump() override;
	virtual void Landed(const FHitResult& Hit) override;
	
	void Walk();
	void Unwalk();
	
	void SpawnWeapon(const TSubclassOf<ACommonWeapon>& weaponToSpawn, bool shouldAutoEquip);
	void EquipWeapon(const EWeaponType weaponType);
	void DropWeapon(EWeaponType weaponType);
	

	
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




