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
	// 이동 속도 관련 프로퍼티들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed; // 기본 걷기 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float OriginSpeed; // 기본 걷기 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;  // "기본 속도" 대비 몇 배로 빠르게 달릴지 결정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed; 	// 실제 스프린트 속도
	FTimerHandle SlowDurationTimerHandle;
	bool bIsMoveInputReversed;
	FTimerHandle ReverseControlTimerHandle;
	FTimerHandle BlindTimerHandle;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(
		float DamageAmount,							// 받은 데미지
		struct FDamageEvent const& DamageEvent,		// 데미지 유형
		AController* EventInstigator,				// 데미지 유발자(ex지뢰 심은 사람)
		AActor* DamaegeCauser						// 데미지 유발원(ex지뢰)
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
