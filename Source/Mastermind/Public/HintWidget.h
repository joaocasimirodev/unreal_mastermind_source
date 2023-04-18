// MIT License - Copyright (c) 2023 João Casimiro

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HintWidget.generated.h"

class FCTweenInstance;
class UCombinationWidget;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHintDelegate, UHintWidget*, owner);

UCLASS()
class MASTERMIND_API UHintWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "HintWidget")
	FHintDelegate OnHiddenDelegate;

	FCTweenInstance* HideTween;
	FCTweenInstance* ShowTween;	
	float duration = 0.3f;

	void PlayHideTween();
	void PlayShowTween();

	virtual void BeginDestroy() override;
};
