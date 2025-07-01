// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.


#pragma once

#include "CoreMinimal.h"
#include "Logging/StructuredLog.h"


VALORANTMECHANICS_API DECLARE_LOG_CATEGORY_EXTERN(Val_Player, Log, All);

#define LOG(Category, Verbosity, Message, ...) UE_LOGFMT(Category, Verbosity, Message, ##__VA_ARGS__)