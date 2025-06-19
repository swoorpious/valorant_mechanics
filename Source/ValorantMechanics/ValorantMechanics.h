// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Logging/StructuredLog.h"


VALORANTMECHANICS_API DECLARE_LOG_CATEGORY_EXTERN(VALORANT_PlayerMovement, Log, All);

#define LOG(Category, Verbosity, Message, ...) UE_LOGFMT(Category, Verbosity, Message, ##__VA_ARGS__)