// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseItem.h"
#include "BaseCoin.generated.h"

UCLASS()
class NBC_CH3_2_API ABaseCoin : public ABaseItem
{
	GENERATED_BODY()

public:

	ABaseCoin();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 PointValue;

	virtual void ActivateItem(AActor* Activator) override;
};
