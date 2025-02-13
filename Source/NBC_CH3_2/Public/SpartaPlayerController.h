// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SpartaPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class NBC_CH3_2_API ASpartaPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASpartaPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InputSettings")
	UInputMappingContext* InputMappingContext;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InputSettings")
	//UInputMappingContext* InputMappingContext_UI;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InputSettings")
	//UInputAction* ChangeInputMappingContextAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InputSettings")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InputSettings")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InputSettings")
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	UUserWidget* HUDWidgetInstance;
	UFUNCTION(BlueprintCallable, Category = "HUD")
	UUserWidget* GetHUDWidget() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	UUserWidget* MainMenuWidgetInstance;
	/*UFUNCTION(BlueprintPure, Category = "HUD")
	UUserWidget* GetMainMenuWidget() const;*/

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowGameHUD();
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowMainMenu(bool bIsRestart);
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void StartGame();
	//UInputAction* SprintAction;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InputSettings")
	//bool bDefaultContext;

	virtual void BeginPlay() override;
	//virtual void SetupInputComponent() override;
	//UFUNCTION()
	//void ChangeIMC();
};
