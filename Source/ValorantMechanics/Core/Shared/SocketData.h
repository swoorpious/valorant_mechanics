// © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SocketData.generated.h"




/**
 * bones/socket names on the weapon's skeletal mesh
 */
UCLASS()
class VALORANTMECHANICS_API USocketData : public UDataAsset
{
    GENERATED_BODY()

public: 

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Mesh|Sockets")
    FName leftHandTargetSocket;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Mesh|Sockets")
    FName magazineMainSocket;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Mesh|Sockets")
    FName magazineExtraSocket;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Mesh|Sockets")
    FName muzzleSocket;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Mesh|Sockets", meta=(DisplayName="ADS/Scope Socket"))
    FName reflexSocket; // this socket is for ADS/scope mesh
    
};


/**
 * bones/socket names on the player's skeletal mesh
 */
UCLASS()
class VALORANTMECHANICS_API UPlayerSocketNames : public UDataAsset
{
    GENERATED_BODY()

public: 
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Mesh|Sockets")
    FName leftHandTargetSocket;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Mesh|Sockets")
    FName leftHandOffsetSocket;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Mesh|Sockets")
    FName leftWeaponMasterSocket;

    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Mesh|Sockets")
    FName rightHandTargetSocket;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Mesh|Sockets")
    FName rightWeaponMasterSocket;

    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Mesh|Sockets")
    FName weaponSwitcherSocket;
    
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Mesh|Sockets")
    FName cameraSocket;

    
};
