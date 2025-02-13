// Fill out your copyright notice in the Description page of Project Settings.


#include "SlowingItem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SpartaCharacter.h"
#include "TimerManager.h"

ASlowingItem::ASlowingItem()
{
    ItemType = TEXT("SlowingItem");
}

void ASlowingItem::ActivateItem(AActor* Activator)
{
    if (ASpartaCharacter* Player = Cast<ASpartaCharacter>(Activator))
    {
        float OriginalSpeed = Player->GetCharacterMovement()->MaxWalkSpeed;
        Player->GetCharacterMovement()->MaxWalkSpeed *= 0.5f;

        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, [Player, OriginalSpeed]()
            {
                if (Player)
                {
                    Player->GetCharacterMovement()->MaxWalkSpeed = OriginalSpeed;
                }
            }, 3.0f, false);
    }
    DestroyItem();
}