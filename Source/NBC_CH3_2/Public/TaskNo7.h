// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TaskNo7.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USkeletalMeshComponent;
class UCapsuleComponent;
struct FInputActionValue;

UCLASS()
class NBC_CH3_2_API ATaskNo7 : public APawn
{
	GENERATED_BODY()

public:
	
	ATaskNo7();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Properties|Components")
	UCapsuleComponent* CapsuleComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Properties|Components")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Properties|Components")
	UCameraComponent* CameraComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Properties|Components")
	USkeletalMeshComponent* SkeletalMeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Properties|Movement")
	float MouseSensitivity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Properties|Movement")
	float AccelerationRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Properties|Movement")
	float MaxSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Properties|Movement")
	FVector CurrentVelocity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Properties|Movement")
	FVector InputAcceleration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Properties|Movement")
	float Friction;	// 마찰력. 0 - 1 사이. 1에 가까울 수록 자연감소폭 작음
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Properties|Movement")
	float RollSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Properties|Movement")
	float InterpSpeed;
	float ForwardInputValue;
	float RightInputValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Properties|Movement")
	float Gravity;
	

public:	
	
	virtual void Tick(float DeltaTime) override;

	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION()
	void MoveForward(const FInputActionValue& value);
	UFUNCTION()
	void MoveRight(const FInputActionValue& value);
	//UFUNCTION()
	//void Look(const FInputActionValue& value);
	UFUNCTION()
	void MoveUp(const FInputActionValue& value);
	UFUNCTION()
	void MoveDown(const FInputActionValue& value);
	UFUNCTION()
	void MoveRollLeft(const FInputActionValue& value);
	UFUNCTION()
	void MoveRollRight(const FInputActionValue& value);
	UFUNCTION()
	void MoveYaw(const FInputActionValue& value);
	UFUNCTION()
	void MovePitch(const FInputActionValue& value);
};
