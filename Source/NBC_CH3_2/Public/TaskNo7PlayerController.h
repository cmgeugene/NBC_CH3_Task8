// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TaskNo7PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
/**
 * 
 */
UCLASS()
class NBC_CH3_2_API ATaskNo7PlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ATaskNo7PlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Properties")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Properties")
	UInputAction* MoveForward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Properties")
	UInputAction* MoveRight;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Properties")
	//UInputAction* Look;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Properties")
	UInputAction* MoveUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Properties")
	UInputAction* MoveDown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Properties")
	UInputAction* MovePitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Properties")
	UInputAction* MoveYaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Properties")
	UInputAction* MoveRollLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Properties")
	UInputAction* MoveRollRight;

	virtual void BeginPlay() override;

};
