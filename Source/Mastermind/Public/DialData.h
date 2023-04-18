// MIT License - Copyright (c) 2023 João Casimiro

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DialData.generated.h"

/**
 * 
 */
UCLASS()
class MASTERMIND_API UDialData : public UObject
{
	GENERATED_BODY()
public:
		bool warning;
		bool wrong;
		bool correct;
		int32 Number;
		int32 Position;

};
