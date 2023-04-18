// MIT License - Copyright (c) 2023 João Casimiro


#include "HintWidget.h"
#include "FCTween.h"
#include "FCTweenInstance.h"
#include "CombinationWidget.h"


void UHintWidget::PlayHideTween()
{
    HideTween = FCTween::Play(0, 450, [&](float t)
        {
            if (this)
            {
                this->SetRenderTranslation(FVector2D(t, 0));
            }
        }
    , duration, EFCEase::Linear);
    HideTween->SetLoops(0);
    HideTween->SetAutoDestroy(true);
 
    HideTween->SetOnComplete([&]() {OnHiddenDelegate.Broadcast(this);});
}

void UHintWidget::PlayShowTween()
{
    ShowTween = FCTween::Play(450, 0, [&](float t)
        {
            if (this)
            {
                this->SetRenderTranslation(FVector2D(t, 0));
            }
        }
    , 1.0f, EFCEase::OutExpo);
    ShowTween->SetLoops(0);
    ShowTween->SetAutoDestroy(true);
}

void UHintWidget::BeginDestroy()
{
    Super::BeginDestroy();

    if (ShowTween != nullptr)
    {      
        ShowTween->Destroy();
        ShowTween = nullptr;
    }


    if (HideTween != nullptr)
    {      
        HideTween->Destroy();
        HideTween = nullptr;
    }

}