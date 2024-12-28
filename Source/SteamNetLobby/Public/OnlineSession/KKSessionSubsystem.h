// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OnlineSession/KKSessionType.h"
#include "KKSessionSubsystem.generated.h"


DECLARE_DYNAMIC_DELEGATE_OneParam(FKKSessionOneParam,bool,bSuccess);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FKKFindSessionFinish,bool,bSuccess,const TArray<FKKSessionInfo> & , SessionList);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FKKGetFriendsListFinish,bool,bSuccess,const TArray<FKKFriendInfo> & , FriendsList);

UCLASS()
class STEAMNETLOBBY_API UKKSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable,Category="KKSessionSubsystem",meta=(AutoCreateRefTerm="CustomData"))
	void KKCreateSession(APlayerController * PlayerController,FKKSessionSettings Settings,TMap<FName,FString> CustomData,FKKSessionOneParam OnCreateSessionFinish);

	UFUNCTION(BlueprintCallable,Category="KKSessionSubsystem")
	void KKFindSession(APlayerController * PlayerController,bool bIsLAN,FKKFindSessionFinish OnFindSessionFinish);
	
	UFUNCTION(BlueprintCallable,Category="KKSessionSubsystem")
	void KKJoinSession(APlayerController * PlayerController,int32 Index);
	
	UFUNCTION(BlueprintCallable,BlueprintPure,Category="KKSessionSubsystem")
	FString KKGetCustomDataFromSession(int32 Index,FName KeyName);

	
	UFUNCTION(BlueprintCallable,Category="KKSessionSubsystem")
	void KKDestorySession(APlayerController * PlayerController);
	
	TSharedPtr<class FOnlineSessionSearch> OnlineSessionSearch = nullptr;

	UFUNCTION(BlueprintCallable,BlueprintPure,Category="KKSessionSubsystem")
	FString KKGetSteamPlayerName(APlayerController * PlayerController);


	UFUNCTION(BlueprintCallable,Category="KKSessionSubsystem")
	void KKShowInviteFriendsUI(APlayerController * PlayerController);
	
	UFUNCTION(BlueprintCallable,Category="KKSessionSubsystem")
	void KKGetFriendsList(APlayerController * PlayerController,FKKGetFriendsListFinish OnGetFriendsList);

	UFUNCTION(BlueprintCallable,BlueprintPure,Category="KKSessionSubsystem")
	static FString KKGetFriendsName(FKKFriendInfo FriendInfo);

	UFUNCTION(BlueprintCallable,Category="KKSessionSubsystem")
	void KKSendInviteToFriends(APlayerController * PlayerController,FKKFriendInfo FriendInfo);

	UFUNCTION(BlueprintCallable,Category="KKSessionSubsystem")
	void KKBindInviteDelegate(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable,Category="KKSessionSubsystem")
	void KKCheckAndHandleSessionExist(APlayerController * PlayerController);

	UFUNCTION(BlueprintCallable,BlueprintPure,Category="KKSessionSubsystem")
	FString KKGetUniqueHouseID(APlayerController * PlayerController);

	UFUNCTION(BlueprintCallable,Category="KKSessionSubsystem")
	void KKServerTravel(APlayerController * PlayerController,const TSoftObjectPtr<UWorld> Level,bool bAbsolute,bool bSkipGameNotify);
};


