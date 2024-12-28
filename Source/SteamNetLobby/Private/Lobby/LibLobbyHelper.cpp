// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LibLobbyHelper.h"

void ULibLobbyHelper::LibUpdatePlayerReadyState(TArray<FLobbyPlayerInfo> & PlayerList, FText PlayerName)
{
	for (FLobbyPlayerInfo & it : PlayerList)
	{
		if (it.PlayerName.EqualTo(PlayerName))
		{
			it.bIsReady = !it.bIsReady;
			return;
		}
	}
}

void ULibLobbyHelper::LibRemovePlayerByName(TArray<FLobbyPlayerInfo>& PlayerList, FText PlayerName)
{
	int32 index = -1;
	for (int32 i = 0 ; i < PlayerList.Num(); ++i)
	{
		if (PlayerList[i].PlayerName.EqualTo(PlayerName))
		{
			index = i;
			break;
		}
	}
	if (index != -1)
	{
		PlayerList.RemoveAtSwap(index);
	}
}

void ULibLobbyHelper::LibServerTravel(APlayerController* PlayerController, const TSoftObjectPtr<UWorld> Level)
{
	check(PlayerController != nullptr)
	PlayerController->GetWorld()->ServerTravel(FPackageName::ObjectPathToPackageName(Level.ToString()) + TEXT("?Listen"),true);
}

