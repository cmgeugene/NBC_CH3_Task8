// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskNo7.h"
#include "TaskNo7PlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"


ATaskNo7::ATaskNo7()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));	// ĸ�� ������Ʈ ��Ʈ ������Ʈ�� ����
	SetRootComponent(CapsuleComp);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(CapsuleComp);
	SpringArmComp->bUsePawnControlRotation = false;
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->SocketOffset = FVector(97, 7, 71);	// ��� 3��Ī�� �ǵ��� ���� ��ġ ����
	

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	SkeletalMeshComp->SetupAttachment(CapsuleComp);
	
	CapsuleComp->SetSimulatePhysics(false);			// ��Ʈ ������Ʈ�� ���̷�Ż �޽� ���� �浹 ��Ȱ��ȭ
	SkeletalMeshComp->SetSimulatePhysics(false);	//

	MouseSensitivity = 10.0f;
	AccelerationRate = 50.0f;
	MaxSpeed = 1200.f;
	CurrentVelocity = FVector(0.0f, 0.0f, 0.0f);
	InputAcceleration = FVector(0.0f, 0.0f, 0.0f);
	Friction = 0.7f;
	RollSpeed = 5.0f;
	InterpSpeed = 3.0f;
	ForwardInputValue = 0.0f;
	RightInputValue = 0.0f;
	Gravity = 50.0f;
}

void ATaskNo7::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATaskNo7::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentVelocity += InputAcceleration * DeltaTime;					// ���ӵ��� ���� �ӵ� ���
	CurrentVelocity = CurrentVelocity * (1.0f - Friction * DeltaTime);	// ������ ����
	FVector ApplyGravity = FVector(0.0f, 0.0f, -Gravity * DeltaTime); 
	CurrentVelocity = CurrentVelocity.GetClampedToMaxSize(MaxSpeed);	// �ִ� �ӵ� ����
	FVector Movement = CurrentVelocity * DeltaTime;						// �̵� �Ÿ� ���
	
	// ---- ��н� ������, ���ӵ� ��� ������ ���� �Լ� -------- //
	float TargetPitch = ForwardInputValue * 15.0f;
	float TargetRoll = RightInputValue * 15.0f;
	FRotator CurrentRotation = SkeletalMeshComp->GetRelativeRotation();	// GetRelativeRotation() �� ���� ��ǥ ���� ȸ�� GetComponentRotation() �� ���� ��ǥ ���� ȸ��
	float NewPitch = FMath::FInterpTo(CurrentRotation.Roll, TargetPitch, DeltaTime, InterpSpeed);	// �޽��� x,y ���� �ݴ�� ���� roll, pitch ���� ������ �����ؾ���
	CurrentRotation.Roll = FMath::Clamp(NewPitch, -15.0f, 15.0f);									
	float NewRoll = FMath::FInterpTo(CurrentRotation.Pitch, TargetRoll, DeltaTime, InterpSpeed);
	CurrentRotation.Pitch = FMath::Clamp(NewRoll, -15.0f, 15.0f);
	SkeletalMeshComp->SetRelativeRotation(CurrentRotation);
	if (!FMath::IsNearlyZero(GetActorLocation().Z) && GetActorLocation().Z > 20)	// �߷� ���� ���� ����
	{
		AddActorWorldOffset(ApplyGravity, true);
	}
	AddActorWorldOffset(Movement, true);
	
	// ����׿� �α� ���
	//if (!FMath::IsNearlyZero(InputAcceleration.X) || !FMath::IsNearlyZero(InputAcceleration.Y))
	//{
	//	UE_LOG(LogTemp, Display, TEXT("TargetPitch : %f / TargetRoll : %f"), TargetPitch, TargetRoll);	
	//}
	
	// �Է� ���ӵ� �ʱ�ȭ
	InputAcceleration = FVector::ZeroVector;
	ForwardInputValue = 0.0f;
	RightInputValue = 0.0f;
}

void ATaskNo7::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ATaskNo7PlayerController* PlayerController = Cast<ATaskNo7PlayerController>(GetController()))
		{
			if (PlayerController->MoveForward)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveForward,
					ETriggerEvent::Triggered,
					this,
					&ATaskNo7::MoveForward
				);
			}

			if (PlayerController->MoveRight)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveRight,
					ETriggerEvent::Triggered,
					this,
					&ATaskNo7::MoveRight
				);
			}

			if (PlayerController->MoveYaw)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveYaw,
					ETriggerEvent::Triggered,
					this,
					&ATaskNo7::MoveYaw
				);
			}
			if (PlayerController->MovePitch)
			{
				EnhancedInput->BindAction(
					PlayerController->MovePitch,
					ETriggerEvent::Triggered,
					this,
					&ATaskNo7::MovePitch
				);
			}

			if (PlayerController->MoveUp)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveUp,
					ETriggerEvent::Triggered,
					this,
					&ATaskNo7::MoveUp
				);
			}

			if (PlayerController->MoveDown)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveDown,
					ETriggerEvent::Triggered,
					this,
					&ATaskNo7::MoveDown
				);
			}
			if (PlayerController->MoveRollLeft)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveRollLeft,
					ETriggerEvent::Triggered,
					this,
					&ATaskNo7::MoveRollLeft
				);
			}
			if (PlayerController->MoveRollRight)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveRollRight,
					ETriggerEvent::Triggered,
					this,
					&ATaskNo7::MoveRollRight
				);
			}
		}
	}
}

void ATaskNo7::MoveForward(const FInputActionValue& value)
{
	if (!Controller) return;
	const FVector2D MoveInput = value.Get<FVector2D>();
	if (FMath::IsNearlyZero(MoveInput.X)) return;	// �ε��Ҽ��� ó��

	ForwardInputValue = MoveInput.X;
	const FVector Forward = GetActorForwardVector();
	InputAcceleration += Forward * ForwardInputValue * AccelerationRate;
}

void ATaskNo7::MoveRight(const FInputActionValue& value)
{
	if (!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();
	if (FMath::IsNearlyZero(MoveInput.Y)) return;	// �ε��Ҽ��� ó��

	RightInputValue = MoveInput.Y;
	const FVector Right = GetActorRightVector();
	InputAcceleration += Right * RightInputValue * AccelerationRate;
}

void ATaskNo7::MoveUp(const FInputActionValue& value)
{
	if (!Controller) return;
	const FVector Up = GetActorUpVector();
	InputAcceleration += Up * AccelerationRate;
}
void ATaskNo7::MoveDown(const FInputActionValue& value)
{
	if (!Controller) return;
	const FVector Down = GetActorUpVector();
	InputAcceleration += -Down * AccelerationRate;
}
void ATaskNo7::MoveRollLeft(const FInputActionValue& value)
{
	if (!Controller) return;
	FRotator RollLeft = FRotator(0.0f, 0.0f, -1.0f * RollSpeed * GetWorld()->GetDeltaSeconds());
	AddActorLocalRotation(RollLeft, true);
}
void ATaskNo7::MoveRollRight(const FInputActionValue& value)
{
	if (!Controller) return;
	FRotator RollRight = FRotator(0.0f, 0.0f, 1.0f * RollSpeed * GetWorld()->GetDeltaSeconds());
	AddActorLocalRotation(RollRight,true);
}

void ATaskNo7::MoveYaw(const FInputActionValue& value)
{
	const FVector2D LookInput = value.Get<FVector2D>();
	if (!FMath::IsNearlyZero(LookInput.X))
	{
		FRotator CurrentRotation(0.0f, LookInput.X * MouseSensitivity, 0.0f);
		FQuat QuatYaw = FQuat(CurrentRotation);
		AddActorWorldRotation(QuatYaw,true);
	}
}
void ATaskNo7::MovePitch(const FInputActionValue& value)
{
	const FVector2D LookInput = value.Get<FVector2D>();
	if (!FMath::IsNearlyZero(LookInput.Y))
	{
		FRotator CurrentRotation(FMath::Clamp(LookInput.Y * MouseSensitivity, -80.0f, 80.0f), 0.0f, 0.0f);
		FQuat QuatPitch = FQuat(CurrentRotation);
		AddActorLocalRotation(QuatPitch,true);
		
	}
}