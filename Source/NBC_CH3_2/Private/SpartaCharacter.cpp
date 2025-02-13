#include "SpartaCharacter.h"
#include "SpartaPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Components/BackgroundBlur.h"
#include "SpartaGameState.h"

ASpartaCharacter::ASpartaCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComp->SetupAttachment(RootComponent);
    SpringArmComp->TargetArmLength = 300.0f;
    SpringArmComp->bUsePawnControlRotation = true;

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
    CameraComp->bUsePawnControlRotation = false;

    OverHeadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverHeadWidget"));
    OverHeadWidget->SetupAttachment(GetMesh());
    OverHeadWidget->SetWidgetSpace(EWidgetSpace::Screen);

    ItemEffectWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ItemEffectWidget"));
    ItemEffectWidget->SetupAttachment(GetMesh());
    ItemEffectWidget->SetWidgetSpace(EWidgetSpace::Screen);

    NormalSpeed = 600.0f;
    OriginSpeed = 600.0f;
    SprintSpeedMultiplier = 1.5f;
    SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

    MaxHealth = 100;
    Health = MaxHealth;
    bIsMoveInputReversed = false;
    
}

void ASpartaCharacter::BeginPlay()
{
    Super::BeginPlay();
    UpdateOverheadHP();
}

float ASpartaCharacter::GetHealth() const
{
    return Health;
}
float ASpartaCharacter::TakeDamage(
    float DamageAmount,							// 받은 데미지
    struct FDamageEvent const& DamageEvent,		// 데미지 유형
    AController* EventInstigator,				// 데미지 유발자(ex지뢰 심은 사람)
    AActor* DamaegeCauser						// 데미지 유발원(ex지뢰)
)
{
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamaegeCauser); // 필수 호출은 아님

    Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
    UE_LOG(LogTemp, Warning, TEXT("Health decreased to : %d"), Health);
    if (Health <= 0.0f)
    {
        OnDeath();
    }
    UpdateOverheadHP();
    return ActualDamage;
}
void ASpartaCharacter::OnDeath() const
{
    // 게임 종료 로직
    ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>(): nullptr;
    if (SpartaGameState)
    {
        SpartaGameState->OnGameOver();
    }
}
void ASpartaCharacter::AddHealth(float amount)
{
    Health = FMath::Clamp(Health + amount, 0.0f, MaxHealth);
    UE_LOG(LogTemp, Warning, TEXT("Health recovered to : %d"), Health);
    UpdateOverheadHP();
}
void ASpartaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Enhanced InputComponent로 캐스팅
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // IA를 가져오기 위해 현재 소유 중인 Controller를 ASpartaPlayerController로 캐스팅
        if (ASpartaPlayerController* PlayerController = Cast<ASpartaPlayerController>(GetController()))
        {
            if (PlayerController->MoveAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->MoveAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ASpartaCharacter::Move
                );
            }

            if (PlayerController->LookAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ASpartaCharacter::Look
                );
            }
            if (PlayerController->JumpAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ASpartaCharacter::StartJump
                );

                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Completed,
                    this,
                    &ASpartaCharacter::StopJump
                );
            }

        }
    }
}
void ASpartaCharacter::Look(const FInputActionValue& value)
{
    // 마우스의 X, Y 움직임을 2D 축으로 가져옴
    FVector2D LookInput = value.Get<FVector2D>();

    // X는 좌우 회전 (Yaw), Y는 상하 회전 (Pitch)
    // 좌우 회전
    AddControllerYawInput(LookInput.X);
    // 상하 회전
    AddControllerPitchInput(LookInput.Y);
}

void ASpartaCharacter::Move(const FInputActionValue& value)
{
    // 컨트롤러가 있어야 방향 계산이 가능
    if (!Controller) return;

    // Value는 Axis2D로 설정된 IA_Move의 입력값 (WASD)을 담고 있음
// 예) (X=1, Y=0) → 전진 / (X=-1, Y=0) → 후진 / (X=0, Y=1) → 오른쪽 / (X=0, Y=-1) → 왼쪽
    FVector2D MoveInput = value.Get<FVector2D>();

    if (bIsMoveInputReversed)
    {
        MoveInput = -MoveInput;
    }

    if (!FMath::IsNearlyZero(MoveInput.X))
    {
        // 캐릭터가 바라보는 방향(정면)으로 X축 이동
        AddMovementInput(GetActorForwardVector(), MoveInput.X);
    }

    if (!FMath::IsNearlyZero(MoveInput.Y))
    {
        // 캐릭터의 오른쪽 방향으로 Y축 이동
        AddMovementInput(GetActorRightVector(), MoveInput.Y);
    }
}
void ASpartaCharacter::StartJump(const FInputActionValue& Value)
{
    if (Value.Get<bool>())
        Jump();
}
void ASpartaCharacter::StopJump(const FInputActionValue& Value)
{
    if (!Value.Get<bool>())
        StopJumping();
}
void ASpartaCharacter::UpdateOverheadHP()
{
    if (!OverHeadWidget) return;
    
    UUserWidget* OverHeadWidgetInstance = OverHeadWidget->GetUserWidgetObject();
    if (!OverHeadWidgetInstance) return;

    if (UTextBlock* HPText = Cast<UTextBlock>(OverHeadWidgetInstance->GetWidgetFromName(TEXT("OverHeadHP"))))
    {
        HPText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), Health, MaxHealth)));
    }
}

void ASpartaCharacter::GetSlowEffect(AActor* Activator, float ItemDuration, float SlowingRate)
{
    if (Activator && Activator->ActorHasTag("Player"))
    {
        if (ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(Activator))
        {
            UCharacterMovementComponent* MovementComponent = PlayerCharacter->GetCharacterMovement();
            if (MovementComponent)
            {
                if (!GetWorldTimerManager().IsTimerActive(SlowDurationTimerHandle))
                {
                    OriginSpeed = MovementComponent->MaxWalkSpeed;
                    MovementComponent->MaxWalkSpeed = OriginSpeed * SlowingRate;
                }

                GetWorld()->GetTimerManager().SetTimer(
                    SlowDurationTimerHandle,
                    [MovementComponent, this]()
                    {
                        if (IsValid(MovementComponent))
                        {
                            MovementComponent->MaxWalkSpeed = OriginSpeed;
                        }
                    },
                    ItemDuration,
                    false
                );
            }
        }
    }
}

void ASpartaCharacter::GetReverseControllEffect(float ItemDuration)
{
    if (!GetWorldTimerManager().IsTimerActive(ReverseControlTimerHandle))
    {
        bIsMoveInputReversed = true;
    }

    GetWorld()->GetTimerManager().SetTimer(
     ReverseControlTimerHandle,
     [this]()
     {
         if (IsValid(this))
         {
             this->bIsMoveInputReversed = false;
         }
     },
     ItemDuration,
     false);
}

void ASpartaCharacter::GetBlindEffect(float ItemDuration, float BlurStrength)
{
    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
        {
            if (UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget())
            {
                if (UBackgroundBlur* BlindEffect = Cast<UBackgroundBlur>(HUDWidget->GetWidgetFromName(TEXT("BlindEffect"))))
                {
                    BlindEffect->SetBlurStrength(BlurStrength);
                    GetWorld()->GetTimerManager().SetTimer(
                        BlindTimerHandle,
                        [BlindEffect]()
                        {
                            if (BlindEffect)
                            {
                                BlindEffect->SetBlurStrength(0.0f);
                            }
                        },
                        ItemDuration,
                        false);
                }
            }
        }
    }
}