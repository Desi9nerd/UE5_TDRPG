#pragma once
#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTDRPG, Log, All);

#define LOG_LOCALROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetLocalRole()))
#define LOG_REMOTEROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetRemoteRole()))
#define LOG_NETMODEINFO ((GetNetMode() == ENetMode::NM_Client) ? *FString::Printf(TEXT("CLIENT%d"), GPlayInEditorID) : ((GetNetMode() == ENetMode::NM_Standalone) ? TEXT("STANDALONE") : TEXT("SERVER"))) 
#define LOG_CALLINFO ANSI_TO_TCHAR(__FUNCTION__)
#define AB_LOG(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("[%s][%s/%s] %s %s"), LOG_NETMODEINFO, LOG_LOCALROLEINFO, LOG_REMOTEROLEINFO, LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define LOG_CALLINFO ANSI_TO_TCHAR(__FUNCTION__)
#define TDGAS_LOG(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("%s %s"), LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define CUSTOM_DEPTH_RED 250
#define ECC_Projectile ECollisionChannel::ECC_GameTraceChannel1
#define ECC_Target ECollisionChannel::ECC_GameTraceChannel2