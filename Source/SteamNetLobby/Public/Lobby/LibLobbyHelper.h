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
	// 更新玩家的准备状态
	UFUNCTION(BlueprintCallable,Category="KKSession")
	static void LibUpdatePlayerReadyState(UPARAM(ref) TArray<FLobbyPlayerInfo> & PlayerList,FText PlayerName);

	// 通过玩家名称踢出玩家
	UFUNCTION(BlueprintCallable,Category="KKSession")
	static void LibRemovePlayerByName(UPARAM(ref) TArray<FLobbyPlayerInfo> & PlayerList,FText PlayerName);

	// 服务器关卡切换
	UFUNCTION(BlueprintCallable,Category="KKSession")
	static void LibServerTravel(APlayerController * PlayerController,const TSoftObjectPtr<UWorld> Level);
};
