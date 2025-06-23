// © 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WeaponSocketNames.generated.h"


USTRUCT(BlueprintType)
struct FWeaponSkeletonSockets
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Mesh|Sockets")
    FString leftHandTargetSocket;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Mesh|Sockets")
    FString magazineMainSocket;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Mesh|Sockets")
    FString magazineExtraSocket;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Mesh|Sockets")
    FString muzzleSocket;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Mesh|Sockets")
    FString reflexSocket; // this socket is for ADS/scope mesh


};

USTRUCT(BlueprintType)
struct FPlayerSkeletonSockets
{
    GENERATED_BODY()
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Mesh|Sockets")
    FString leftHandTargetSocket;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Mesh|Sockets")
    FString leftHandOffsetSocket;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Mesh|Sockets")
    FString leftWeaponMasterSocket;

    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Mesh|Sockets")
    FString rightHandTargetSocket;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Mesh|Sockets")
    FString rightWeaponMasterSocket;

    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Mesh|Sockets")
    FString weaponSwitcherSocket;
    
};



/**
 * 
 */
UCLASS()
class VALORANTMECHANICS_API UWeaponSocketNames : public UObject
{
    GENERATED_BODY()

public: 
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Mesh|Sockets")
    FPlayerSkeletonSockets playerSkelSockets;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Mesh|Sockets")
    FWeaponSkeletonSockets weaponSkelSockets;
    
};
