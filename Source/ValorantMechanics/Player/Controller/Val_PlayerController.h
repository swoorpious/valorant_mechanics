// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"
#include "../Val_Character.h"
#include "Val_InputSystem.h"

#include "Val_PlayerController.generated.h"

class AVal_Character;



/**
 * wushu
 */
UCLASS()
class VALORANTMECHANICS_API AVal_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AVal_PlayerController();
	virtual TObjectPtr<UVal_InputSystem> GetInputSystem() { return valInputSystem; } 	

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	float Sensitivity = 1.0f;

	
#pragma region PLAYER ACTIONS
	void PlayerJump(const FInputActionInstance& InputActionInstance);
	void PlayerCrouch(const FInputActionInstance& InputActionInstance);
	void PlayerWalk(const FInputActionInstance& InputActionInstance);
	void PlayerUse(const FInputActionInstance& InputActionInstance);
	
	// functions called from Val_InputComponent.h
	void PlayerMove() const;
	FORCEINLINE void PlayerLook(const FVector2D lookVector) const { AddLookInput(lookVector * Sensitivity); }
#pragma endregion PLAYER ACTIONS


#pragma region WEAPON ACTIONS
	FORCEINLINE void TryWeaponEquip(const EWeaponType weaponType) const { pCharacter->EquipWeapon(weaponType); }
	// FORCEINLINE void TryWeaponDrop(const EWeaponType weaponType) const { pCharacter->DropWeapon(weaponType); }

#pragma endregion WEAPON ACTIONS
	
	
protected:
	void AddLookInput(FVector2D Look) const;

	UPROPERTY() TObjectPtr<AVal_Character> pCharacter = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category="Input")
	TObjectPtr<UVal_InputSystem> valInputSystem = nullptr;


	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

};

