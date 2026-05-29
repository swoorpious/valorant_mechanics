// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#pragma once

#include "CoreMinimal.h"

#include "ValorantMechanics/Core/Shared/PlayerDelegateDefinition.h"
#include "ValorantMechanics/Core/Shared/SocketData.h"
#include "ValorantMechanics/Core/Shared/PlayerStates.h"

#include "GameFramework/Character.h"
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

    /*
     * getter functions cuz im lazy
     */
    AVal_PlayerController* GetValPlayerController() const;
    AVal_Character* GetValCharacter();
    UVal_CharacterMovementComponent* GetValMovementComponent() const;
    UVal_PlayerAnimInstance* GetValAnimInstance() const;
    UVal_PlayerInventory* GetPlayerInventory() const;

	FOnWeaponChanged& getOnWeaponChangedDelegate() { return _onWeaponChangedDelegate_; }
	FOnWeaponStateChanged& getOnWeaponStateChangedDelegate() { return _onWeaponStateChangedDelegate_; }
	

#pragma region COMPONENT SETUP
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Valorant Character|Character Setup|Scene")
    TObjectPtr<USceneComponent> sceneComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Valorant Character|Character Setup|Scene|Mesh", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USkeletalMeshComponent> characterMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Valorant Character|Character Setup|Scene|Mesh|Camera", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UCameraComponent> characterMeshCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Valorant Character|Character Setup|Scene|Mesh|Camera", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAudioComponent> audioComponent;
	
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Valorant Character|Mesh|Sockets")
    FPlayerSocketNames socketData;

#pragma endregion COMPONENT SETUP
	
	
    // TODO: move these functions to protected scope and add try<action>weapon type functions
    void SpawnWeapon(const TSubclassOf<ACommonWeapon>& weaponToSpawn, bool shouldAutoEquip);
    void EquipWeapon(const EWeaponType weaponType, const EEquipType equipType);
    void UnequipWeapon(const EWeaponType weaponType) const;
    void pickupWeapon(EWeaponType weaponType);
    void DropWeapon(EWeaponType weaponType);

    void PlayLocalSound(USoundBase* sound) const;
    
    
protected:
    virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	
    // caching for some reason
    UPROPERTY() TObjectPtr<UVal_CharacterMovementComponent> _charMovementComponent = nullptr;
    UPROPERTY() TObjectPtr<UVal_PlayerAnimInstance> _playerAnimInstance = nullptr;
    UPROPERTY() TObjectPtr<UVal_PlayerInventory> _inventory = nullptr;

private:
	FOnWeaponChanged _onWeaponChangedDelegate_;
	FOnWeaponStateChanged _onWeaponStateChangedDelegate_;

};




