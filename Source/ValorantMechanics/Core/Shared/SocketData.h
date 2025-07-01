// Copyright 2025 Swaroop. Personal Unreal engine project inspired BY VALORANT.


#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SocketData.generated.h"




/**
 * bones/socket names on the weapon's skeletal mesh
 */
UCLASS()
class VALORANTMECHANICS_API UWeaponSocketData : public UObject
{
    GENERATED_BODY()

public: 

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Mesh|Sockets")
    FName leftHandTargetSocket = "Left_Hand_Target";

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Mesh|Sockets")
    FName magazineMainSocket = "Magazine_Main";

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Mesh|Sockets")
    FName magazineExtraSocket = "Magazine_Extra";
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Mesh|Sockets")
    FName muzzleSocket = "Muzzle";
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Mesh|Sockets", meta=(DisplayName="ADS/Scope Socket"))
    FName reflexSocket = "Reflex"; // this socket is for ADS/scope mesh
    
};


/**
 * bones/socket names on the player's skeletal mesh
 */
UCLASS()
class VALORANTMECHANICS_API UPlayerSocketNames : public UObject
{
    GENERATED_BODY()

public: 
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Mesh|Sockets")
    FName leftHandTargetSocket = "L_Weapon_HandTargetSocket";
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Mesh|Sockets")
    FName leftHandOffsetSocket = "L_Weapon_HandOffsetSocket";
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Mesh|Sockets")
    FName leftWeaponMasterSocket = "L_WeaponMasterSocket";

    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Mesh|Sockets")
    FName rightHandTargetSocket = "R_Weapon_HandTargetSocket";
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Mesh|Sockets")
    FName rightWeaponMasterSocket = "R_WeaponMasterSocket";

    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Mesh|Sockets")
    FName weaponSwitcherSocket = "R_WeaponSwitcherSocket";
    
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Mesh|Sockets")
    FName cameraSocket = "Camera";

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Valorant Character|Mesh|Sockets")
    FName cameraVFXSocket = "VFX_Camera";

    
};
