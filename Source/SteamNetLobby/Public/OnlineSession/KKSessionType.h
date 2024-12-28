#pragma once
#include "OnlineSubsystemTypes.h"
#include "KKSessionType.generated.h"

USTRUCT(BlueprintType)
struct FKKSessionSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 PublicNum;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 PrivateNum;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsLAN;

	FKKSessionSettings():PublicNum(4),PrivateNum(0),bIsLAN(true){}
};

USTRUCT(BlueprintType)
struct FKKSessionInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 Index;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 Ping;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 MaxPlayerNum;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 CurrentPlayerNum;
};



USTRUCT(BlueprintType)
struct FKKFriendInfo
{
	GENERATED_USTRUCT_BODY()
	FKKFriendInfo(){}
	TSharedPtr<FOnlineFriend> OnlineFriend = nullptr;
	FKKFriendInfo(TSharedRef<FOnlineFriend> InFriend):OnlineFriend(InFriend){}
};
