// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskNo7PlayerController.h"
#include "EnhancedInputSubsystems.h"

ATaskNo7PlayerController::ATaskNo7PlayerController()
	:	InputMappingContext(nullptr),
		MoveForward(nullptr),
		MoveRight(nullptr)
//		Look(nullptr)
{
}

void ATaskNo7PlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
				Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}