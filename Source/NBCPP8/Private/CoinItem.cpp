// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinItem.h"
#include "Engine/World.h"
#include "SpartaGameState.h"

ACoinItem::ACoinItem()
{

	PointValue = 10;

	ItemType = "DefaultCoin";

}


void ACoinItem::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (UWorld* World = GetWorld())
		{
			if (ASpartaGameState* GameState = World->GetGameState<ASpartaGameState>())
			{
				GameState->AddScore(PointValue);
				GameState->OnCoinCollected();
			}
		}
		DestroyItem();
	}
}