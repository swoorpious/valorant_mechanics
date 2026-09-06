// Copyright 2025 swaroop. Personal Unreal Engine project inspired by VALORANT.

#ifndef VAL_LOG_H
#define VAL_LOG_H

#pragma once

#include "CoreMinimal.h"
#include "Logging/StructuredLog.h"


VALORANTMECHANICS_API DECLARE_LOG_CATEGORY_EXTERN(Val_Player, Log, All);

#define LOG(Category, Verbosity, Message, ...) UE_LOG(Category, Verbosity, Message, ##__VA_ARGS__)

/*
 * appends the class name to the log message like the following,
 * [ClassName] <message>
 * 
 * in static class members, functions may use the class name
 * in non-static class members, functions can pass in `this`
 */
#define LOGObjName(Obj, Category, Verbosity, Format, ...) \
UE_LOG(Category, Verbosity, TEXT("[%s] " Format), *(Obj)->GetClass()->GetName(), ##__VA_ARGS__)


#endif

