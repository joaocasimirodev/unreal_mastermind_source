// MIT License - Copyright (c) 2023 João Casimiro

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NativeCodeRow.h"
#include "CombinationWidget.generated.h"

class UButton;
class UDialWidget;
class UDialDataSet;
class UScrollBox;
class UDataTable;
class UTextBlock;
class UPanelWidget;
class FCTweenInstance;
class UHintWidget;
class UProgressBar;
class APlayerController;

UENUM()
enum ECleanHintMode 
{
	SequenceClean  UMETA(DisplayName = "SequenceClean"),
	Asap          UMETA(DisplayName = "ASAP"),	
};

/**
 * 
 */
UCLASS()
class MASTERMIND_API UCombinationWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CombinationWidget)
	UButton* NativeSubmitButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CombinationWidget)
    UButton* NativeClearButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CombinationWidget)
	UButton* NativeCreditsButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CombinationWidget)
	TArray<UDialWidget*> NativeDials;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CombinationWidget)
	UUserWidget* Instance;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CombinationWidget)
	TArray<UDialDataSet*> StoredCombinations;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CombinationWidget)
	UScrollBox* ScrollBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CombinationWidget)
	TArray<UUserWidget*> HintEntries;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CombinationWidget)
	TArray<FNativeCodeRow> CodeRowArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CombinationWidget)
	UDataTable* CodeTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CombinationWidget)
	UTextBlock*  DebugText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CombinationWidget)
	UTextBlock* AttemptsLeftText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CombinationWidget)
	UUserWidget* NativeDialLine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CombinationWidget)
	UTextBlock* NativeClockText;

	//OutcomePanel
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CombinationWidget)
	UPanelWidget* NativeOutcomePanel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CombinationWidget)
	UButton* NativeResetButton;
	//text used for sucess message or failure message
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CombinationWidget)
	UTextBlock* NativeTextFeedback;
	
	//loadingScreen
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CombinationWidget)
	UPanelWidget* NativeLoadingPanel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CombinationWidget)
	UTextBlock* LoadingText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CombinationWidget)
	UProgressBar* LoadingBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CombinationWidget)
	APlayerController* PlayerController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
	TEnumAsByte<ECleanHintMode> CleanMode;

	FTimerHandle TimerHandle;
	FTimerManager* TimerManagerPtr;

	TArray<FCTweenInstance*> TweenArray;
	UUserWidget* CurrentHintReference;

	FCTweenInstance* ShowHintTween;
	FCTweenInstance* LoadingBarTween;

	int32 CurrentIndex = 0;
	TArray<int32> SecretCode;
	int32 CurrentAttempt = 6;
	int32 MaxAttempts = 6;
	bool bCanSubmit = true;
	int32 CurrentHintIndex = 0;

	UFUNCTION(BlueprintCallable)
	void Init();
	UFUNCTION(BlueprintCallable)
	void PopulateDials();

	UFUNCTION()
	void CheckForResults();
	UFUNCTION(BlueprintCallable) //fordebug
	void ShowWinScreen();
	UFUNCTION()
	void ResetGame();

	UFUNCTION()
	void ClearHints(ECleanHintMode mode);
	UFUNCTION()
	void DebugClearHints();

	UFUNCTION()
	void CreateHint();

	void RefreshDials();

	UFUNCTION()
	void SetUpDataTable();

	UFUNCTION()
	FString StringFromCodeRow(FNativeCodeRow codeRow);
	UFUNCTION()
	void SetCurrentCode(FNativeCodeRow codeRow);

	bool CheckForDigitInSequence(int32 digit);

	void HandleHintTween(UUserWidget* hintwidget);
	UFUNCTION()
	void SequentialHideHintTweens();	
	UFUNCTION()
	void ClearHintAndHideNextHint(UHintWidget* owner);
	void AsapCleanHints();
	void HandleHintCleanUp(UHintWidget* owner, bool bRemoveArrayEntry = false);
	void ShowLoadingScreen();
	void OnFinishedLoading();
	void ShowLooseScreen();
	void ResetAttempts();
	void UpdateAttemptsLeft();
	void UpdateClock();

	UFUNCTION()
	void CreditsAction();
	UFUNCTION()
	void Quit();

	virtual void BeginDestroy() override;
	
};
