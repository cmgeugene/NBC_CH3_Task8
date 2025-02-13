// Fill out your copyright notice in the Description page of Project Settings.


#include "SmallCoin.h"

// Sets default values
ASmallCoin::ASmallCoin()
{
	PointValue = 20;
	ItemType = "Small Coin";
}

void ASmallCoin::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
}