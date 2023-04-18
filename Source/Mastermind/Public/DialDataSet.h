// MIT License - Copyright (c) 2023 João Casimiro

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DialDataSet.generated.h"

/**
 * 
 */
class UDialData;

UCLASS()
class MASTERMIND_API UDialDataSet : public UObject
{
	GENERATED_BODY()
public :
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = DialDataSet)
		TArray<UDialData*> Data;
};
