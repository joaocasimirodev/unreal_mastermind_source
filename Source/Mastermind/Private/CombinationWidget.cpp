// MIT License - Copyright (c) 2023 João Casimiro


#include "CombinationWidget.h"
#include "Components/Button.h"
#include "Blueprint/WidgetTree.h"
#include "DialWidget.h"
#include "DialDataSet.h"
#include "DialData.h"
#include "HintSlotWidget.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Engine/DataTable.h"
#include "Components/PanelWidget.h"
#include "FCTween.h"
#include "FCTweenInstance.h"
#include "HintWidget.h"
#include "Components/ProgressBar.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "Misc/DateTime.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"


void UCombinationWidget::Init()
{
    NativeSubmitButton->OnClicked.AddDynamic(this, &UCombinationWidget::CheckForResults);
    NativeClearButton->OnClicked.AddDynamic(this, &UCombinationWidget::DebugClearHints);
    NativeResetButton->OnClicked.AddDynamic(this, &UCombinationWidget::ResetGame);
    NativeCreditsButton->OnClicked.AddDynamic(this, &UCombinationWidget::CreditsAction);
    
    PlayerController = GetWorld()->GetFirstPlayerController();
    PlayerController->EnableInput(PlayerController);
    FInputModeGameAndUI InputMode;
    PlayerController->SetInputMode(InputMode);
    UInputComponent* InputReference = GetOwningPlayer()->InputComponent;
    InputReference->BindKey(EKeys::Escape, IE_Pressed, this, &UCombinationWidget::Quit);

    if(TimerManagerPtr)   
    TimerManagerPtr->SetTimer(TimerHandle, this, &UCombinationWidget::UpdateClock, 1.0f, true, 0.0f);


    StoredCombinations.Init(nullptr, MaxAttempts);
    SetUpDataTable();  
    UpdateAttemptsLeft();
    PopulateDials();
}

void UCombinationWidget::Quit()
{   
    PlayerController->ConsoleCommand("quit");
}
void UCombinationWidget::CreditsAction()
{   
   UKismetSystemLibrary::LaunchURL("https://joaocasimiro.pt/");
}

#pragma region  DataHandling
void UCombinationWidget::SetUpDataTable() 
{
    if (CodeTable == nullptr)
        return;

    FNativeCodeRow* row = nullptr;

    TArray<FName> names = CodeTable->GetRowNames();
    TArray<FNativeCodeRow*> rows;
    
    for (int i = 0; i < names.Num(); i++)
    {     
        row = CodeTable->FindRow<FNativeCodeRow>(names[i], "ReadCodeContext", true);

        if(row)
            CodeRowArray.Add(*row);

    }

    SetCurrentCode(CodeRowArray[CurrentIndex]);
    
}

FString UCombinationWidget::StringFromCodeRow(FNativeCodeRow codeRow) 
{
    FString s = FString::FromInt(codeRow.digit1) 
              + FString::FromInt(codeRow.digit2) 
              + FString::FromInt(codeRow.digit3) 
              + FString::FromInt(codeRow.digit4) 
              + FString::FromInt(codeRow.digit5);

    return s;
}

void UCombinationWidget::SetCurrentCode(FNativeCodeRow codeRow)
{
    SecretCode.Empty();
    SecretCode.Add(codeRow.digit1);
    SecretCode.Add(codeRow.digit2);
    SecretCode.Add(codeRow.digit3);
    SecretCode.Add(codeRow.digit4);
    SecretCode.Add(codeRow.digit5);

    FString str = " Solution :  " + StringFromCodeRow(codeRow);
    DebugText->SetText(FText::FromString(str));
}

void UCombinationWidget::UpdateClock()
{
    if (NativeClockText) 
    {
        FString Hours = FString::FromInt(FDateTime::Now().GetHour());
        FString Minutes = FString::FromInt(FDateTime::Now().GetMinute());

        if (Minutes.Len() == 1)
           Minutes = "0" + FString::FromInt(FDateTime::Now().GetMinute());

        if(Hours.Len() == 1)
            Hours = "0" + FString::FromInt(FDateTime::Now().GetHour());
            
        FString time = Hours + ":" + Minutes;

        NativeClockText->SetText(FText::FromString(time));      
    } 
}
#pragma endregion DataHandling

#pragma region  GamePlay
void UCombinationWidget::PopulateDials()
{
    TArray<UWidget*> Children;  
    NativeDialLine->WidgetTree->GetAllWidgets(Children);
    for (auto Child : Children)
    {    
        if (UDialWidget* Dial = Cast<UDialWidget>(Child))
        {
            NativeDials.Add(Dial);          
        }
    }
    
    //debug
    /*
    for (int i = 0; i < NativeDials.Num(); i++) 
    {
        FString sab = "Dial " + NativeDials[i]->GetFName().ToString() + " Stored in position " + FString::FromInt(i);        
        UE_LOG(LogTemp , Warning, TEXT("%s"),*sab);       
        
    }
    */
}

void UCombinationWidget::CheckForResults()
{
    FString str = "CheckForResults Called";
    UE_LOG(LogTemp, Warning, TEXT("%s"), *str);

    if (!bCanSubmit)
        return;

   
    //create a temp DialDataSet to store current SecretCode information and later add it to DialDataSets
    UDialDataSet* tempDialDataSet = NewObject<UDialDataSet>();

    int32 Success = 0;
 
    FString count = "DialsCount : " + FString::FromInt(NativeDials.Num());
    UE_LOG(LogTemp, Warning, TEXT("%s"), *count);
  
    for (int i = 0; i < SecretCode.Num(); i++) 
    {
        UDialData* TempDialData = NewObject<UDialData>();
        TempDialData->Number = NativeDials[i]->Number;
        
        if (SecretCode[i] != NativeDials[i]->Number) 
        {
            Success--;
            
            UDialWidget* TempDial = NativeDials[i];
            int32 storedDigit = NativeDials[i]->Number;

            if (CheckForDigitInSequence(storedDigit)) 
            {
                TempDialData->warning = true;
            }
            else 
            {
                TempDialData->wrong = true;
            }         
        }
        else 
        {           
            TempDialData->correct = true;
            Success++;           
        }

        tempDialDataSet->Data.Add(TempDialData);
        StoredCombinations.Add(tempDialDataSet);          
    }
    
    if (Success == SecretCode.Num())
    {
        ClearHints(ECleanHintMode::SequenceClean);
        if(CurrentIndex < CodeRowArray.Num()-1)
        {
            ShowLoadingScreen();
            ResetAttempts();
            CurrentIndex++;
            SetCurrentCode(CodeRowArray[CurrentIndex]);           
        }
        else {
            ShowWinScreen();
        }
    }
    else
    {
        CurrentAttempt--;

        if (CurrentAttempt > 0)
        {
            CreateHint();
        }
        else 
        {       
            ShowLooseScreen();
        }
    }
    UpdateAttemptsLeft();
}

void UCombinationWidget::ShowWinScreen()
{
    FString str = "<### Y0U 4R3 1N ###>";
    NativeTextFeedback->SetText(FText::FromString(str));
    NativeOutcomePanel->SetVisibility(ESlateVisibility::Visible);
}
void UCombinationWidget::ShowLooseScreen()
{
    FString str = "You Ran Out Of Tries";
    NativeTextFeedback->SetText(FText::FromString(str));
    NativeOutcomePanel->SetVisibility(ESlateVisibility::Visible);
    ClearHints(ECleanHintMode::Asap);   
}

void UCombinationWidget::ResetGame()
{
    CurrentIndex = 0;
    ResetAttempts();
    NativeOutcomePanel->SetVisibility(ESlateVisibility::Hidden);
    RefreshDials();  
    SetCurrentCode(CodeRowArray[CurrentIndex]);
}

void UCombinationWidget::ClearHints(ECleanHintMode mode)
{
    if(mode == ECleanHintMode::SequenceClean)
    SequentialHideHintTweens();
    else
    AsapCleanHints();     
}

void UCombinationWidget::AsapCleanHints() 
{
    FString h = "CombinationWidget -> " + this->GetName();
    UE_LOG(LogTemp, Warning, TEXT("ASAP CLEAN HINTS CALLED ! ""%s"), *h);
   
    for(int j = 0; j < HintEntries.Num(); j++)
    {             
        HandleHintCleanUp(Cast<UHintWidget>(HintEntries[j]));             
    }
    
    HintEntries.Empty();
     
}
void UCombinationWidget::DebugClearHints()
{
    ClearHints(ECleanHintMode::SequenceClean);
    ResetAttempts();
}

void UCombinationWidget::CreateHint()
{
    //Get editor created UMG class reference
    FSoftClassPath ClassPath(TEXT("/Game/UI/HintWidgetBP.HintWidgetBP_C"));
    UClass* ClassReference = ClassPath.TryLoadClass<UHintWidget>();

    if (ClassReference)
    {        
        UHintWidget* TempHintWidget = CreateWidget<UHintWidget>(this->GetGameInstance(), ClassReference);
       
        ScrollBox->AddChild(TempHintWidget);
        ScrollBox->ScrollToEnd();
        TempHintWidget->SetRenderTranslation(FVector2D(450, 0));      
       
        //Get all HintSlots Children of current hintwidgetBP 
        TArray<UHintSlotWidget*> HintSlots;
        TArray<UWidget*> Children;
        TempHintWidget->WidgetTree->GetAllWidgets(Children);
        for (auto Child : Children)
        {
            UE_LOG(LogTemp, Warning, TEXT("child : ""%s"), *Child->GetFName().ToString());
            UE_LOG(LogTemp, Warning, TEXT("Class : ""%s"), *Child->GetClass()->GetName());

            UHintSlotWidget* TempSlot = Cast<UHintSlotWidget>(Child);
            if (TempSlot)
            {           
                HintSlots.Add(TempSlot);
                FString SlotName = "Slot" + TempSlot->GetFName().ToString();
                UE_LOG(LogTemp, Warning, TEXT("%s"), *SlotName);               
            }
           
            UTextBlock* textBlock = Cast<UTextBlock>(Child);
            if (textBlock)
            {
                FString str = " Try " + FString::FromInt(HintEntries.Num()+1);
                textBlock->SetText(FText::FromString(str));              
                UE_LOG(LogTemp, Warning, TEXT("Should Show : ""%s"), *str);
            }

        }    
        HintEntries.Add(TempHintWidget);
        

        for (int i = 0; i < HintSlots.Num(); i++) 
        {
            UDialData* TempDialData = StoredCombinations[StoredCombinations.Num() -1]->Data[i];
            HintSlots[i]->Number = TempDialData->Number;
            
            if (TempDialData->wrong) HintSlots[i]->SetHintImage(0);
            else if (TempDialData->warning) HintSlots[i]->SetHintImage(1);
            else 
            { 
                HintSlots[i]->SetHintImage(2); 
                //disable matching dial aswell
                NativeDials[i]->DisableDial();
            }

            HintSlots[i]->NativeUpdateText();
        }   

        TempHintWidget->PlayShowTween();
        TempHintWidget->OnHiddenDelegate.AddDynamic(this, &UCombinationWidget::ClearHintAndHideNextHint);      
    }  
}

void UCombinationWidget::RefreshDials()
{
    for(int i = 0; i < NativeDials.Num(); i++)
    {      
            NativeDials[i]->EnableDial();
            NativeDials[i]->Number = 0;
            NativeDials[i]->NativeUpdateText();
    }
}

bool UCombinationWidget::CheckForDigitInSequence(int32 digit)
{
    for (int i = 0; i < SecretCode.Num(); i++)
    {
        if (SecretCode[i] == digit)
            return true;
    }
    return false;
}
void UCombinationWidget::ResetAttempts() 
{ 
    CurrentAttempt = MaxAttempts; 
    UpdateAttemptsLeft();
}
void UCombinationWidget::UpdateAttemptsLeft()
{
    FString TempString = FString::FromInt(CurrentAttempt) + " Attempts Left !";
    AttemptsLeftText->SetText(FText::FromString(TempString));

}
#pragma endregion  GamePlay
//450 -> 0
#pragma region Tweens
void UCombinationWidget::HandleHintTween(UUserWidget* hintwidget)
{
    int32 indexador = 0;
    indexador = HintEntries.Num()-1;

    if (ShowHintTween != nullptr)
        ShowHintTween = nullptr;
        
    //Note : a global var pointer reference (CurrentHintReference) was needed for the tween to work, 
    //trying to handle local var would result violation access write point.
    CurrentHintReference = HintEntries[indexador];

    ShowHintTween = FCTween::Play(450, 0, [&](float t)
    {
            if (CurrentHintReference)
            {
                CurrentHintReference->SetRenderTranslation(FVector2D(t, 0));
            }
    }
        , 1.0f, EFCEase::OutExpo);
        ShowHintTween->SetLoops(0);
        ShowHintTween->SetAutoDestroy(true);
}


void UCombinationWidget::SequentialHideHintTweens() 
{  
   
    //Sequential works by Hiding the first in the list and once it has finished
    //it makes a handshake triggering the ClearHintAndHideNextHint() 
    //it continues to do it subsequently until the all hints are cleared    

    if(HintEntries.Num() > 0)
    {
        if (bCanSubmit)
            bCanSubmit = !bCanSubmit;


        ScrollBox->ScrollToStart();        
        Cast<UHintWidget>(HintEntries[0])->PlayHideTween();   
    }
    else 
    {       
        bCanSubmit = true;       
    }
   
}
void UCombinationWidget::ClearHintAndHideNextHint(UHintWidget* owner)
{
    UE_LOG(LogTemp, Warning, TEXT("CallBack called from : ""%s"), *owner->GetFName().ToString());
    HandleHintCleanUp(owner, true);     
    SequentialHideHintTweens();
}
void UCombinationWidget::HandleHintCleanUp(UHintWidget* owner, bool bRemoveArrayEntry) 
{
    owner->OnHiddenDelegate.RemoveDynamic(this, &UCombinationWidget::ClearHintAndHideNextHint);
    ScrollBox->RemoveChild(owner);

    if(bRemoveArrayEntry)
    HintEntries.Remove(owner);

    owner->RemoveFromParent();
}

void UCombinationWidget::ShowLoadingScreen() 
{
    NativeLoadingPanel->SetVisibility(ESlateVisibility::Visible);
 
        LoadingBarTween = FCTween::Play(0,1, [&](float t)
            {               
                LoadingBar->SetPercent(t);
                FString tempString = FString::FromInt(FMath::RoundToInt32(t * 100)); 
                FText tempText = FText::FromString(tempString);
                LoadingText->SetText(tempText);

            } ,2.0f, EFCEase::OutExpo);
        LoadingBarTween->SetOnComplete([&] { OnFinishedLoading(); });
        LoadingBarTween->SetAutoDestroy(true);

}

void UCombinationWidget::OnFinishedLoading()
{
    NativeLoadingPanel->SetVisibility(ESlateVisibility::Hidden);
    RefreshDials();
    UpdateAttemptsLeft();
}

#pragma endregion Tweens

void UCombinationWidget::BeginDestroy()
{
    Super::BeginDestroy();

    if (ShowHintTween != nullptr)
    {   ShowHintTween->Destroy();
        ShowHintTween = nullptr;
    }

    if(LoadingBarTween != nullptr)
    {
        LoadingBarTween->Destroy();
        LoadingBarTween = nullptr;
    }
    
    TimerManagerPtr->ClearTimer(TimerHandle);
}
