// © 2026 swaroop. Personal Unreal Engine project inspired by VALORANT.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ValorantMechanics/Core/Shared/PlayerStates.h"
#include "PlayerSounds.generated.h"


USTRUCT(BlueprintType)
struct FCharMovementSoundCue
{
    GENERATED_BODY()
    
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<USoundBase*> soundVariations;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float volumeMin = 1.f, volumeMax = 1.f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float pitchMin = 1.f, pitchMax = 1.f;
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
    TMap<EMovementState, FCharMovementSoundCue> cues;
};
