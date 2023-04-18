// MIT License - Copyright (c) 2023 João Casimiro

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialWidget.generated.h"

class UButton;
class UTextBlock;
class FCTweenInstanceVector;
class FCTweenInstance;
/**
 * 
 */
UCLASS()
class MASTERMIND_API UDialWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = DialWidget)
	UButton* DialUpBT;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = DialWidget)
	UButton* DialDownBT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Number;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DialWidget)
	FString N_NumberString;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DialWidget)
	UTextBlock* N_TextObject;

	FCTweenInstanceVector* manualTween = nullptr;
	FCTweenInstance* dialTween;
	FCTweenInstance* upArrowTween;
	FCTweenInstance* downArrowTween;
	

	int32 min = 0;
	int32 max = 9;
protected:
	virtual void NativeConstruct() override;

public:	
	UFUNCTION(BlueprintCallable)
	void Init();

	UFUNCTION()
	void DialUp();
	UFUNCTION()
	void DialDown();
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateUIText();

	void EnableDial();
	void DisableDial();
	
	void NativeUpdateText();

	void PlayHighLight();
	void CreateDialArrowTweens();
	void PlayTween(FCTweenInstance* TweenRef);

	virtual void BeginDestroy() override;
};
