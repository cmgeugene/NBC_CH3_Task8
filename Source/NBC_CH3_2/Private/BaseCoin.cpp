// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCoin.h"
#include "Engine/World.h"
#include "SpartaGameState.h"

ABaseCoin::ABaseCoin()
{
	PointValue = 0;
	ItemType = "Default Coin";
}

void ABaseCoin::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (UWorld* World = GetWorld())
		{
			if (ASpartaGameState* GameState = World->GetGameState<ASpartaGameState>())
			{
				GameState->AddScore(PointValue);
				//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Player gained %d points."), PointValue));
				GameState->OnCoinCollected();
				DestroyItem();
			}
		}
		
	}
}
