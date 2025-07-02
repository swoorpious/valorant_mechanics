// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#pragma once

#include "CoreMinimal.h"

#include "ValorantMechanics/Core/Shared/PlayerDelegateDefinition.h"
// #include "Shared/WeaponData/WeaponProperties.h"
#include "ValorantMechanics/Core/Shared/SocketData.h"

#include "GameFramework/Character.h"
#include "ValorantMechanics/Core/Shared/StateController/PlayerStates.h"
#include "Val_Character.generated.h"


// forward declarations
class UVal_PlayerAnimInstance;
class UVal_InputComponent;
class UVal_CharacterMovementComponent;
class UVal_PlayerInventory;
class AVal_PlayerController;

class USkeletalMeshComponent;
class UCameraComponent;
class ACommonWeapon;





UCLASS()
class VALORANTMECHANICS_API AVal_Character : public ACharacter
{
	GENERATED_BODY()

public:
	explicit AVal_Character(const FObjectInitializer& ObjectInitializer);

	AVal_PlayerController* GetValPlayerController() const;
	AVal_Character* GetValCharacter();
	UVal_CharacterMovementComponent* GetValMovementComponent() const;
	UVal_PlayerAnimInstance* GetValAnimInstance() const;

	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Valorant Character|Character Setup|Scene")
	TObjectPtr<USceneComponent> sceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Valorant Character|Character Setup|Scene|Mesh", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> characterMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Valorant Character|Character Setup|Scene|Mesh|Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> characterMeshCamera;

		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Valorant Character|Mesh|Sockets")
	TObjectPtr<UPlayerSocketNames> socketData;

		
	void SpawnWeapon(const TSubclassOf<ACommonWeapon>& weaponToSpawn, bool shouldAutoEquip);
	void EquipWeapon(const EWeaponType weaponType, const EWeaponState equipType); // this might break functionality if called with anything other than Equip_Default or Equip_Fast
	void DropWeapon(EWeaponType weaponType);
	

protected:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// caching for some reason
	UPROPERTY()	TObjectPtr<UVal_CharacterMovementComponent> pMovement = nullptr;
	UPROPERTY()	TObjectPtr<UVal_PlayerAnimInstance> pAnimInstance = nullptr;
	UPROPERTY()	TObjectPtr<UVal_PlayerInventory> pInventory = nullptr;
	
	

	float TimeSinceLanded = 0.0f;
	
};




