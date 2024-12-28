// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineSession/KKSessionSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSessionSettings.h"



// 创建Session
void UKKSessionSubsystem::KKCreateSession(APlayerController* PlayerController,FKKSessionSettings Settings,TMap<FName,FString> CustomData,FKKSessionOneParam OnCreateSessionFinish)
{
	check(PlayerController != nullptr)
	IOnlineSubsystem * Subsystem = Online::GetSubsystem(PlayerController->GetWorld());
	IOnlineSessionPtr OnlineSessionPtr = Subsystem->GetSessionInterface();
	check(OnlineSessionPtr.IsValid() != false)
	
	//
	OnlineSessionPtr->OnCreateSessionCompleteDelegates.Clear();
	OnlineSessionPtr->OnCreateSessionCompleteDelegates.AddLambda([OnCreateSessionFinish](FName SessionName,bool bSuccess)
	{
		OnCreateSessionFinish.ExecuteIfBound(bSuccess);
	});
	
	FOnlineSessionSettings OnlineSessionSettings;

	OnlineSessionSettings.NumPublicConnections = Settings.PublicNum;
	OnlineSessionSettings.NumPrivateConnections = Settings.PrivateNum;
	OnlineSessionSettings.bIsLANMatch = Settings.bIsLAN;

	OnlineSessionSettings.bShouldAdvertise = true;
	OnlineSessionSettings.bAllowJoinInProgress = true;
	OnlineSessionSettings.bIsDedicated = false;
	OnlineSessionSettings.bUsesStats = true;
	OnlineSessionSettings.bAllowInvites = true;
	OnlineSessionSettings.bUsesPresence = true;
	OnlineSessionSettings.bAllowJoinViaPresence = true;
	OnlineSessionSettings.bAllowJoinViaPresenceFriendsOnly = false;
	OnlineSessionSettings.bAntiCheatProtected = false;
	
	for (TTuple<FName,FString> it : CustomData)
	{
		// 传递二进制数据
		// UE_LOG(LogTemp,Warning,TEXT("Test Convert"));
		FString Str = it.Value;
		// 处理分隔符
		Str.ReplaceInline(TEXT("|"),TEXT(""));
		if (Str.Len() >= 26)
		{
			Str.LeftInline(26);
		}
		// UE_LOG(LogTemp,Warning,TEXT("进来的数据 : %s"),*Str);
		
		// UE_LOG(LogTemp,Warning,TEXT("字符串转换为uint8数组"));
		TArray<uint8> Data(reinterpret_cast<const uint8*>(TCHAR_TO_UTF8(*Str)),60);
		
		
		FString B;
		for (auto i : Data)
		{
			B += FString::FromInt(i) + TEXT("|");
		}
		// UE_LOG(LogTemp,Warning,TEXT("将uint8数组拼接为字符串,长度=%d | 数据=%s"),Data.Num(),*B);
		
		OnlineSessionSettings.Set(it.Key,B,EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	}
	OnlineSessionPtr->CreateSession(0,NAME_GameSession,OnlineSessionSettings);
}

// 寻找Session
void UKKSessionSubsystem::KKFindSession(APlayerController* PlayerController,bool bIsLAN,FKKFindSessionFinish OnFindSessionFinish)
{
	check(PlayerController!=nullptr)
	IOnlineSubsystem * Subsystem = Online::GetSubsystem(PlayerController->GetWorld());
	IOnlineSessionPtr OnlineSessionPtr = Subsystem->GetSessionInterface();
	check(OnlineSessionPtr.IsValid() != false)
	
	OnlineSessionPtr->OnFindSessionsCompleteDelegates.Clear();
	OnlineSessionPtr->OnFindSessionsCompleteDelegates.AddLambda([OnFindSessionFinish,OnlineSessionPtr,this](bool bSuccess)
	{
		TArray<FKKSessionInfo> FindSessionInfo;
		for (int i = 0; i <  OnlineSessionSearch->SearchResults.Num() ; ++i)
		{
			FKKSessionInfo Tmp;
			Tmp.Index = i;
			Tmp.Ping = OnlineSessionSearch->SearchResults[i].PingInMs;
			Tmp.MaxPlayerNum = OnlineSessionSearch->SearchResults[i].Session.SessionSettings.NumPublicConnections;
			Tmp.CurrentPlayerNum = Tmp.MaxPlayerNum - OnlineSessionSearch->SearchResults[i].Session.NumOpenPublicConnections;
			FindSessionInfo.Add(Tmp);
		}
		OnFindSessionFinish.ExecuteIfBound(bSuccess,FindSessionInfo);
	});
	OnlineSessionSearch = MakeShareable(new FOnlineSessionSearch);
	OnlineSessionSearch->MaxSearchResults = 999;
	OnlineSessionSearch->bIsLanQuery = bIsLAN;

	OnlineSessionSearch->QuerySettings.Set(SEARCH_PRESENCE,true,EOnlineComparisonOp::Equals);
	
	OnlineSessionPtr->FindSessions(0,OnlineSessionSearch.ToSharedRef());
}

// 加入 Session
void UKKSessionSubsystem::KKJoinSession(APlayerController* PlayerController, int32 Index)
{
	check(PlayerController!=nullptr)
	IOnlineSubsystem * Subsystem = Online::GetSubsystem(PlayerController->GetWorld());
	IOnlineSessionPtr OnlineSessionPtr = Subsystem->GetSessionInterface();
	check(OnlineSessionPtr.IsValid() != false)

	OnlineSessionPtr->OnJoinSessionCompleteDelegates.Clear();
	OnlineSessionPtr->OnJoinSessionCompleteDelegates.AddLambda([PlayerController,OnlineSessionPtr](FName SessionName, EOnJoinSessionCompleteResult::Type Type)
	{
		if (Type == EOnJoinSessionCompleteResult::Success)
		{
			FString URL;
			OnlineSessionPtr->GetResolvedConnectString(NAME_GameSession,URL);
			PlayerController->ClientTravel(URL,ETravelType::TRAVEL_Absolute);
		}
	});

	if (OnlineSessionSearch.IsValid() && Index >=0 && Index < OnlineSessionSearch->SearchResults.Num())
	{
		OnlineSessionPtr->JoinSession(0,NAME_GameSession,OnlineSessionSearch->SearchResults[Index]);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Join Session Failed , the index is not valid or no Session Search Result"));
	}
}

// 获取Session的自定义数据
FString UKKSessionSubsystem::KKGetCustomDataFromSession(int32 Index, FName KeyName)
{
	FString Data;
	if (OnlineSessionSearch.IsValid() && Index >=0 && Index < OnlineSessionSearch->SearchResults.Num())
	{
		OnlineSessionSearch->SearchResults[Index].Session.SessionSettings.Get(KeyName,Data);
	}

	TArray<FString> C;
	Data.ParseIntoArray(C,TEXT("|"));
	FString D = FString::Join(C,TEXT("|"));
	// UE_LOG(LogTemp,Warning,TEXT("将字符串拆分为uint8数组,长度=%d | 数据=%s"),C.Num(),*D);

	// 
	TArray<uint8> E;
	FString F;
	for (auto j : C)
	{
		uint8 i = FCString::Atoi(*j);
		E.Add(i);
		F += j + TEXT("|");
	}
	
	// UE_LOG(LogTemp,Warning,TEXT("构建uint8的真实数组,长度=%d | 数据=%s"),E.Num(),*F);

	FString G(UTF8_TO_TCHAR(reinterpret_cast<const char*>(E.GetData())));
	// UE_LOG(LogTemp,Warning,TEXT("最终结果数据: %s"),*G);
	
	return G;
}

// 销毁Session
void UKKSessionSubsystem::KKDestorySession(APlayerController* PlayerController)
{
	check(PlayerController!=nullptr)
	IOnlineSubsystem * Subsystem = Online::GetSubsystem(PlayerController->GetWorld());
	IOnlineSessionPtr OnlineSessionPtr = Subsystem->GetSessionInterface();
	check(OnlineSessionPtr.IsValid() != false)

	OnlineSessionPtr->DestroySession(NAME_GameSession,FOnDestroySessionCompleteDelegate::CreateLambda([](FName SessionName,bool bSuccess)
	{
		
	}));
}

// 获取Steam玩家昵称
FString UKKSessionSubsystem::KKGetSteamPlayerName(APlayerController * PlayerController)
{
	check(PlayerController!=nullptr)
	IOnlineSubsystem * Subsystem = Online::GetSubsystem(PlayerController->GetWorld());
	IOnlineIdentityPtr OnlineIdentityPtr = Subsystem->GetIdentityInterface();
	// if is not steam , will crash 
	// check(OnlineIdentityPtr.IsValid() != false)
	if (OnlineIdentityPtr.IsValid())
	{
		return  OnlineIdentityPtr->GetPlayerNickname(0);
	}
	return FString::Printf(TEXT("Player%d"),FMath::RandRange(11111111,99999999));
}

// 显示Steam的邀请UI
void UKKSessionSubsystem::KKShowInviteFriendsUI(APlayerController * PlayerController)
{
	check(PlayerController!=nullptr)
	IOnlineSubsystem * Subsystem = Online::GetSubsystem(PlayerController->GetWorld());
	// IOnlineSessionPtr OnlineSessionPtr = Subsystem->GetSessionInterface();
	IOnlineExternalUIPtr OnlineExternalUIPtr = Subsystem->GetExternalUIInterface();
	// this can be null , if is not steam subsystem , so need to disable check
	// check(OnlineExternalUIPtr.IsValid() != false)
	if (OnlineExternalUIPtr.IsValid())
	{
		OnlineExternalUIPtr->ShowInviteUI(0);
	}
}

// 获取朋友列表
void UKKSessionSubsystem::KKGetFriendsList(APlayerController* PlayerController,FKKGetFriendsListFinish OnGetFriendsList)
{
	check(PlayerController!=nullptr)
	IOnlineSubsystem * Subsystem = Online::GetSubsystem(PlayerController->GetWorld());
	IOnlineFriendsPtr OnlineFriendsPtr = Subsystem->GetFriendsInterface();
	check(OnlineFriendsPtr.IsValid() != false)
	FString FriendsList;
	OnlineFriendsPtr->ReadFriendsList(0,FriendsList,FOnReadFriendsListComplete::CreateLambda(
		[OnlineFriendsPtr,OnGetFriendsList](int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr)
		{
			UE_LOG(LogTemp,Warning,TEXT("Read Friends List : %s"),bWasSuccessful?TEXT("Success"):TEXT("failed"));
			if (bWasSuccessful)
			{
				UE_LOG(LogTemp,Warning,TEXT("firends list : %s"),*ListName);
				TArray<TSharedRef<FOnlineFriend>> Find_Friends;
				OnlineFriendsPtr->GetFriendsList(LocalUserNum,ListName,Find_Friends);

				TArray<FKKFriendInfo> Tmp;
				for (TSharedRef<FOnlineFriend> it : Find_Friends)
				{
					Tmp.Add(FKKFriendInfo(it));
				}
				OnGetFriendsList.ExecuteIfBound(bWasSuccessful,Tmp);
			}
			else
			{
				UE_LOG(LogTemp,Warning,TEXT("error str : %s"),*ErrorStr);
			}
			
		}));
}

// 读取好友名
FString UKKSessionSubsystem::KKGetFriendsName(FKKFriendInfo FriendInfo)
{
	return FriendInfo.OnlineFriend->GetRealName();
}

// 发送邀请给好友
void UKKSessionSubsystem::KKSendInviteToFriends(APlayerController* PlayerController, FKKFriendInfo SDFriendInfo)
{
	check(PlayerController != nullptr)
	IOnlineSubsystem * Subsystem = Online::GetSubsystem(PlayerController->GetWorld());
	IOnlineSessionPtr OnlineSessionPtr = Subsystem->GetSessionInterface();
	check(OnlineSessionPtr.IsValid() != false)
	OnlineSessionPtr->SendSessionInviteToFriend(0,NAME_GameSession,SDFriendInfo.OnlineFriend->GetUserId().Get());
	UE_LOG(LogTemp,Warning,TEXT("SDSendInviteToFriends : %s"),*SDFriendInfo.OnlineFriend->GetRealName());
}

// 绑定接收邀请的代理;
void UKKSessionSubsystem::KKBindInviteDelegate(APlayerController* PlayerController)
{
	UE_LOG(LogTemp,Warning,TEXT("Bind Invite Delegate"));
	check(PlayerController!=nullptr)
	IOnlineSubsystem * Subsystem = Online::GetSubsystem(PlayerController->GetWorld());
	IOnlineSessionPtr OnlineSessionPtr = Subsystem->GetSessionInterface();
	check(OnlineSessionPtr.IsValid() != false)
	OnlineSessionPtr->OnSessionUserInviteAcceptedDelegates.Clear();
	OnlineSessionPtr->OnSessionUserInviteAcceptedDelegates.AddLambda([OnlineSessionPtr,PlayerController](
		const bool bWasSuccessful, const int32 ControllerId,
		TSharedPtr<const FUniqueNetId> UserId, const FOnlineSessionSearchResult& InviteResult)
	{

		OnlineSessionPtr->OnJoinSessionCompleteDelegates.Clear();
		OnlineSessionPtr->OnJoinSessionCompleteDelegates.AddLambda([OnlineSessionPtr,PlayerController](FName SessionName, EOnJoinSessionCompleteResult::Type Type)
		{
			if (Type == EOnJoinSessionCompleteResult::Success)
			{
				FString URL;
				OnlineSessionPtr->GetResolvedConnectString(NAME_GameSession,URL);
				PlayerController->ClientTravel(URL,ETravelType::TRAVEL_Absolute);
			}
		});
		UE_LOG(LogTemp,Warning,TEXT("********* OnSessionUserInviteAcceptedDelegates **********"));
		bool ret = OnlineSessionPtr->JoinSession(0,NAME_GameSession,InviteResult);
		UE_LOG(LogTemp,Warning,TEXT("join Invite session : %s"),ret?TEXT("Success"):TEXT("Failed"));
	});

	OnlineSessionPtr->OnSessionInviteReceivedDelegates.Clear();
	OnlineSessionPtr->OnSessionInviteReceivedDelegates.AddLambda([](const FUniqueNetId& UserId, const FUniqueNetId& FromId, const FString& AppId, const FOnlineSessionSearchResult& InviteResult)
	{
		UE_LOG(LogTemp,Warning,TEXT("********* OnSessionInviteReceivedDelegates **********"));
	});
}

void UKKSessionSubsystem::KKCheckAndHandleSessionExist(APlayerController* PlayerController)
{
	check(PlayerController!=nullptr)
	IOnlineSubsystem * Subsystem = Online::GetSubsystem(PlayerController->GetWorld());
	IOnlineSessionPtr OnlineSessionPtr = Subsystem->GetSessionInterface();
	check(OnlineSessionPtr.IsValid() != false)
	FUniqueNetIdRepl NetID = PlayerController->GetLocalPlayer()->GetUniqueNetIdFromCachedControllerId();
	bool IsSessionExist = OnlineSessionPtr->IsPlayerInSession(NAME_GameSession,*NetID.GetUniqueNetId().Get());
	UE_LOG(LogTemp,Warning,TEXT("Check And Handle Session Exist : is Session Exist : %d"),IsSessionExist);
	if (IsSessionExist)
	{
		KKDestorySession(PlayerController);
	}
}

FString UKKSessionSubsystem::KKGetUniqueHouseID(APlayerController * PlayerController)
{
	IOnlineSubsystem * Subsystem = Online::GetSubsystem(PlayerController->GetWorld());
	IOnlineSessionPtr OnlineSessionPtr = Subsystem->GetSessionInterface();
	check(OnlineSessionPtr.IsValid() != false)
	FUniqueNetIdRepl NetID = PlayerController->GetLocalPlayer()->GetUniqueNetIdFromCachedControllerId();
	return NetID.ToString();
}

void UKKSessionSubsystem::KKServerTravel(APlayerController * PlayerController,const TSoftObjectPtr<UWorld> Level,bool bAbsolute,bool bSkipGameNotify)
{
	check(PlayerController != nullptr)
	const FString LevelName(*FPackageName::ObjectPathToPackageName(Level.ToString()));
	UE_LOG(LogTemp,Warning,TEXT("Travel URL : %s"),*LevelName);
	PlayerController->GetWorld()->ServerTravel(LevelName,bAbsolute,bSkipGameNotify);
}
