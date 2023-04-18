// MIT License - Copyright (c) 2023 João Casimiro

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "NativeCodeRow.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FNativeCodeRow : public FTableRowBase 
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 digit1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 digit2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 digit3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 digit4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 digit5;

};
