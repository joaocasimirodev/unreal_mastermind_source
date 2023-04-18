// MIT License - Copyright (c) 2023 João Casimiro

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HintSlotWidget.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class MASTERMIND_API UHintSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Number;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HintSlotWidget)
		UTextBlock* N_TextObject;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HintSlotWidget)
		FSlateBrush nativeWarningBrush;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HintSlotWidget)
		FSlateBrush nativeWrongBrush;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HintSlotWidget)
		FSlateBrush nativeCorrectBrush;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HintSlotWidget)
		UImage* nativeHintImage;
	
	void NativeUpdateText();

	void SetHintImage(int i);
};
