// Fill out your copyright notice in the Description page of Project Settings.


#include "BlindItem.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"

ABlindItem::ABlindItem()
{
    ItemType = TEXT("BlindItem");
}

void ABlindItem::ActivateItem(AActor* Activator)
{
    if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        if (UUserWidget* BlindWidget = CreateWidget<UUserWidget>(PlayerController, LoadClass<UUserWidget>(nullptr, TEXT("/Game/UI/Widget/WBP_BlindEffect.WBP_BlindEffect_C"))))
        {
            BlindWidget->AddToViewport();

            FTimerHandle TimerHandle;
            GetWorld()->GetTimerManager().SetTimer(TimerHandle, [BlindWidget]()
                {
                    if (BlindWidget)
                    {
                        BlindWidget->RemoveFromParent();
                    }
                }, 3.0f, false);
        }
    }
    DestroyItem();
}