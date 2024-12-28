// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KKLobbyType.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LibLobbyHelper.generated.h"

/**
 * 
 */
UCLASS()
class STEAMNETLOBBY_API ULibLobbyHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void LibUpdatePlayerReadyState(UPARAM(ref) TArray<FLobbyPlayerInfo> & PlayerList,FText PlayerName);

	UFUNCTION(BlueprintCallable)
	static void LibRemovePlayerByName(UPARAM(ref) TArray<FLobbyPlayerInfo> & PlayerList,FText PlayerName);

	UFUNCTION(BlueprintCallable)
	static void LibServerTravel(APlayerController * PlayerController,const TSoftObjectPtr<UWorld> Level);
};
