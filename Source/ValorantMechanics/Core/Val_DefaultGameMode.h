// Copyright 2025 Swaroop. Personal Unreal engine project inspired BY VALORANT.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Val_DefaultGameMode.generated.h"


class ACommonWeapon;

/**
 * 
 */
UCLASS()
class VALORANTMECHANICS_API AVal_DefaultGameMode : public AGameMode
{
    GENERATED_BODY()

public:
    
#pragma region WEAPONS TO SPAWN
    
    UPROPERTY(EditDefaultsOnly, Category = "Weapons Options|Weapons to Spawn")
    TSubclassOf<ACommonWeapon> meleeToSpawn = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "Weapons Options|Weapons to Spawn")
    TSubclassOf<ACommonWeapon> secondaryToSpawn = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "Weapons Options|Weapons to Spawn")
    TSubclassOf<ACommonWeapon> primaryToSpawn = nullptr;
    
#pragma endregion WEAPONS TO SPAWN
    
};
