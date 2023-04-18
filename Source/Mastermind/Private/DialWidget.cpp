// MIT License - Copyright (c) 2023 João Casimiro

#include "DialWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "FCTween.h"
#include "FCTweenInstance.h"


void UDialWidget::NativeConstruct() 
{
	Super::NativeConstruct();	
}

void UDialWidget::Init()
{
	Number = min;
	DialUpBT->OnClicked.AddDynamic(this, &UDialWidget::DialUp);
	DialDownBT->OnClicked.AddDynamic(this, &UDialWidget::DialDown);
	CreateDialArrowTweens();
}

void UDialWidget::EnableDial()
{
	DialUpBT->SetIsEnabled(true);
	DialDownBT->SetIsEnabled(true);
}

void UDialWidget::DisableDial()
{
	DialUpBT->SetIsEnabled(false);
	DialDownBT->SetIsEnabled(false);
}

void UDialWidget::DialUp()
{

	if (Number == max)
		Number = min;
	else
		Number++;

	NativeUpdateText();
	PlayTween(upArrowTween);
}

void UDialWidget::DialDown()
{
	if (Number == min)
		Number = max;
	else
		Number--;
	
	NativeUpdateText();
	PlayTween(downArrowTween);	
}

void UDialWidget::NativeUpdateText()
{
	FString IntAsString = FString::FromInt(Number);
	N_TextObject->SetText(FText::FromString(IntAsString));
}

void UDialWidget::CreateDialArrowTweens() 
{
		upArrowTween = FCTween::Play(0, -20, [&](float t)
		{						
			DialUpBT->SetRenderTranslation(FVector2D(0.0f,t));
		}
		, 0.05f, EFCEase::Linear)
		->SetLoops(1)->SetYoyo(true);
		upArrowTween->Pause();
		
		upArrowTween->SetAutoDestroy(false);

		downArrowTween = FCTween::Play(0, 20, [&](float t)
		{				
			DialDownBT->SetRenderTranslation(FVector2D(0.0f, t));
		}
		, 0.05f, EFCEase::Linear)
		->SetLoops(1)->SetYoyo(true);
		downArrowTween->Pause();

		downArrowTween->SetAutoDestroy(false);
}

void UDialWidget::PlayHighLight()
{	
	if (dialTween)
	{
		if(dialTween->bIsActive)
		dialTween->Restart();			
	}
	else 
	{
		// If the tween var is not assigned, Create a new tween

		UE_LOG(LogTemp, Warning, TEXT("CREATED NEW TWEEN !"));

		dialTween =	FCTween::Play(1, 1.5f, [&](float t) 			
		{
			if (N_TextObject) 							
				N_TextObject->SetRenderScale(FVector2D(t,t));
			
		}, .05f, EFCEase::Linear)
			->SetLoops(1)->SetYoyo(true);

			dialTween->SetAutoDestroy(false);
	}	
}

void UDialWidget::PlayTween(FCTweenInstance* TweenRef)
{
	if (TweenRef)
	TweenRef->Restart();			
}

 void UDialWidget::BeginDestroy() 
{	
	 Super::BeginDestroy();
	
	 if (dialTween != nullptr)
	 {
		 // tells FCTween to recycle this tween
		 dialTween->Destroy();
		 dialTween = nullptr;
	 }

	 if(upArrowTween !=nullptr)
	 {
		 upArrowTween->Destroy();
		 upArrowTween = nullptr;
	 }
	 if(downArrowTween != nullptr)
	 {
		 downArrowTween->Destroy();
		 downArrowTween = nullptr;
	 
	 }

	 if (N_TextObject != nullptr) {	
		 N_TextObject = nullptr;	
	 }
}


