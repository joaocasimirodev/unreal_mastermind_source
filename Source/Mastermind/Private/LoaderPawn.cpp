// MIT License - Copyright (c) 2023 João Casimiro

#include "LoaderPawn.h"
#include "Components/WidgetComponent.h"
#include "CombinationWidget.h"


ALoaderPawn::ALoaderPawn()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ALoaderPawn::BeginPlay()
{
	Super::BeginPlay();

	LoadPhoneWidget();	
}

void ALoaderPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALoaderPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}


void ALoaderPawn::LoadPhoneWidget()
{	
	FSoftClassPath ClassPath(TEXT("/Game/UI/PhoneUMG.PhoneUMG_C"));
	UClass* ClassReference = ClassPath.TryLoadClass<UCombinationWidget>();

	if (ClassReference)
	{
		
		UE_LOG(LogTemp, Warning, TEXT("PhoneUMG class was found and created ! "));

		UCombinationWidget* TempPhoneWidget = CreateWidget<UCombinationWidget>(this->GetGameInstance(), ClassReference);
		
		TempPhoneWidget->AddToViewport();
		TempPhoneWidget->TimerManagerPtr = &GetWorldTimerManager();
		TempPhoneWidget->Init();
		
	}else
	{
		UE_LOG(LogTemp, Warning, TEXT("PhoneUMG class failed to be found and was not created :( "));
	}

	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	
}

