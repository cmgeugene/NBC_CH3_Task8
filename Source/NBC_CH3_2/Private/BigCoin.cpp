// Fill out your copyright notice in the Description page of Project Settings.


#include "BigCoin.h"

// Sets default values
ABigCoin::ABigCoin()
{
	PointValue = 50;
	ItemType = "Big Coin";
}

void ABigCoin::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
}