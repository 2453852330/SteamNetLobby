#pragma once

#include "KKLobbyType.generated.h"

USTRUCT(BlueprintType)
struct FLobbyPlayerInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FText PlayerName;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsReady;
};
