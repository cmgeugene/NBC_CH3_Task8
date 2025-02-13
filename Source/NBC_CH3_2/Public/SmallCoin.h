// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseCoin.h"
#include "SmallCoin.generated.h"

UCLASS()
class NBC_CH3_2_API ASmallCoin : public ABaseCoin
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASmallCoin();

	virtual void ActivateItem(AActor* Activator);

};
