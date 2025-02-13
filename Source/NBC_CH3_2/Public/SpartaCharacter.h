#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpartaCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;
struct FInputActionValue;

UCLASS()
class NBC_CH3_2_API ASpartaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASpartaCharacter();

public:
	void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* OverHeadWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* ItemEffectWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player State")
	int32 MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player State")
	int32 Health;
	// �̵� �ӵ� ���� ������Ƽ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed; // �⺻ �ȱ� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float OriginSpeed; // �⺻ �ȱ� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;  // "�⺻ �ӵ�" ��� �� ��� ������ �޸��� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed; 	// ���� ������Ʈ �ӵ�
	FTimerHandle SlowDurationTimerHandle;
	bool bIsMoveInputReversed;
	FTimerHandle ReverseControlTimerHandle;
	FTimerHandle BlindTimerHandle;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(
		float DamageAmount,							// ���� ������
		struct FDamageEvent const& DamageEvent,		// ������ ����
		AController* EventInstigator,				// ������ ������(ex���� ���� ���)
		AActor* DamaegeCauser						// ������ ���߿�(ex����)
								) override;			
	void OnDeath() const;
	void UpdateOverheadHP();
	UFUNCTION(BlueprintCallable, Category = "Player State")
	void AddHealth(float amount);

	UFUNCTION(BlueprintPure, Category = "Player State")
	float GetHealth() const;
	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void StartJump(const FInputActionValue& Value);
	UFUNCTION()
	void StopJump(const FInputActionValue& Value);
	void GetSlowEffect(AActor* Activator, float ItemDuration, float SlowingRate);
	void GetReverseControllEffect(float ItemDuration);
	void GetBlindEffect(float ItemDuration, float BlurStrength);

};
