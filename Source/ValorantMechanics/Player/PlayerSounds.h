// © 2026 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ValorantMechanics/Core/Shared/PlayerStates.h"
#include "PlayerSounds.generated.h"


UENUM(BlueprintType)
enum class ECharMovementSounds : uint8
{
    Bass, // isn't played standalone. usually overlayed with landed or footsteps for extra thud
    Footstep,
    JumpUp,
    JumpLand,
    Effort,
    Damage,
    Death,
    
};

USTRUCT(BlueprintType)
struct FCharMovementSoundCue
{
    GENERATED_BODY()
    
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<TSoftObjectPtr<USoundBase>> soundVariations;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float volumeMin = 1.f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float volumeMax = 1.f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float pitchMin = 1.f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float pitchMax = 1.f;
};



/**
 * movement sound cues related to any player character 
 */
UCLASS(BlueprintType)
class VALORANTMECHANICS_API UPlayerMovementSounds : public UDataAsset
{
    GENERATED_BODY()
    
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<ECharMovementSounds, FCharMovementSoundCue> cues;
};
