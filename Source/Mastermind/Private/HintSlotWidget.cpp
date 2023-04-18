// MIT License - Copyright (c) 2023 João Casimiro


#include "HintSlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UHintSlotWidget::NativeUpdateText()
{
	FString IntAsString = FString::FromInt(Number);
	N_TextObject->SetText(FText::FromString(IntAsString));
}

void UHintSlotWidget::SetHintImage(int i)
{
	FSlateBrush brush;

	if (i == 0) { brush = nativeWrongBrush; }
	else if (i == 1) { brush = nativeWarningBrush; }
	else { brush = nativeCorrectBrush; }

	if (!brush.HasUObject())
		return;

	nativeHintImage->SetBrush(brush);
}
