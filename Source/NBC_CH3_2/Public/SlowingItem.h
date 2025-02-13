// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseItem.h"
#include "SlowingItem.generated.h"

UCLASS()
class NBC_CH3_2_API ASlowingItem : public ABaseItem
{
	GENERATED_BODY()
	
public:	
	ASlowingItem();
	virtual void ActivateItem(AActor* Activator);
	

protected:
	//virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item properties")
	float ItemDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item properties")
	float SlowingRate;
	
};
