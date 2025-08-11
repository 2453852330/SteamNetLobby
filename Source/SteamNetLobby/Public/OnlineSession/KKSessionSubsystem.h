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
	// 创建一个Session房间
	// PlayerController: 传入一个有效PlayerController
	// Settings: Session房间的相关设置,比如是否是局域网?
	// CustomData: 和此房间有关的自定义数据,比如密码之类的额外信息
	// OnCreateSessionFinish: CreateSession成功之后的回调函数
	UFUNCTION(BlueprintCallable,Category="KKSession",meta=(AutoCreateRefTerm="CustomData"))
	void KKCreateSession(APlayerController * PlayerController,FKKSessionSettings Settings,TMap<FName,FString> CustomData,FKKSessionOneParam OnCreateSessionFinish);

	// 寻找房间,最大返回结果为999个
	// PlayerController: 传入一个有效PlayerController
	// bIsLAN: 是否是局域网
	// OnFindSessionFinish:寻找成功的回调函数
	UFUNCTION(BlueprintCallable,Category="KKSession")
	void KKFindSession(APlayerController * PlayerController,bool bIsLAN,FKKFindSessionFinish OnFindSessionFinish);

	// 加入一个Session房间
	// PlayerController: 传入一个有效PlayerController
	// Index: 寻找房间的回调中保存的房间Index
	UFUNCTION(BlueprintCallable,Category="KKSession")
	void KKJoinSession(APlayerController * PlayerController,int32 Index);

	// 获取创建Session房间时传入的自定义数据
	// KKCreateSession时设置的CustomData参数,可以在这里获取到
	// Index: 查找房间返回的Session索引,也就是你需要获取到哪个房间的参数
	// KeyName: 设置CustomData时设定的Key
	UFUNCTION(BlueprintCallable,BlueprintPure,Category="KKSession")
	FString KKGetCustomDataFromSession(int32 Index,FName KeyName);

	// 销毁一个Session
	// PlayerController: 传入一个有效PlayerController
	UFUNCTION(BlueprintCallable,Category="KKSession")
	void KKDestorySession(APlayerController * PlayerController);

	
	TSharedPtr<class FOnlineSessionSearch> OnlineSessionSearch = nullptr;

	// 获取玩家名称[Steam/Epic等等]
	UFUNCTION(BlueprintCallable,BlueprintPure,Category="KKSession")
	FString KKGetSteamPlayerName(APlayerController * PlayerController);

	// 显示邀请好友界面[Steam/Epic等等]
	UFUNCTION(BlueprintCallable,Category="KKSession")
	void KKShowInviteFriendsUI(APlayerController * PlayerController);

	// 获取好友列表[Steam/Epic等等]
	// OnGetFriendsList: 获取成功之后的回调函数
	UFUNCTION(BlueprintCallable,Category="KKSession")
	void KKGetFriendsList(APlayerController * PlayerController,FKKGetFriendsListFinish OnGetFriendsList);

	// 获取好友名称[Steam/Epic等等]
	UFUNCTION(BlueprintCallable,BlueprintPure,Category="KKSession")
	static FString KKGetFriendsName(FKKFriendInfo FriendInfo);

	// 发送邀请给好友 [Steam/Epic等等]
	// FriendInfo: 获取好友列表返回的好友信息
	UFUNCTION(BlueprintCallable,Category="KKSession")
	void KKSendInviteToFriends(APlayerController * PlayerController,FKKFriendInfo FriendInfo);

	// 绑定接受邀请,接受邀请后会自动加入好友Session,并切换到对应关卡
	UFUNCTION(BlueprintCallable,Category="KKSession")
	void KKBindInviteDelegate(APlayerController* PlayerController);

	// 检测当前是否存在无效Session并清理(防止加入一次Session之后,无法继续加入)
	UFUNCTION(BlueprintCallable,Category="KKSession")
	void KKCheckAndHandleSessionExist(APlayerController * PlayerController);

	// 获取唯一房间ID
	UFUNCTION(BlueprintCallable,BlueprintPure,Category="KKSession")
	FString KKGetUniqueHouseID(APlayerController * PlayerController);

	// 房主调用:用于所有玩家切换关卡
	UFUNCTION(BlueprintCallable,Category="KKSession")
	void KKServerTravel(APlayerController * PlayerController,const TSoftObjectPtr<UWorld> Level,bool bAbsolute,bool bSkipGameNotify);


	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
};


