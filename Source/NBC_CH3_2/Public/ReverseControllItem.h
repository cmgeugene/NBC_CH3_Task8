// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseItem.h"
#include "ReverseControllItem.generated.h"

UCLASS()
class NBC_CH3_2_API AReverseControllItem : public ABaseItem
{
	GENERATED_BODY()
	
public:	
	AReverseControllItem();
	virtual void ActivateItem(AActor* Activator);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties")
	float ItemDuration;
	

};
