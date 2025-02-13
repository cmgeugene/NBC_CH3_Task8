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
    float DamageAmount,							// ���� ������
    struct FDamageEvent const& DamageEvent,		// ������ ����
    AController* EventInstigator,				// ������ ������(ex���� ���� ���)
    AActor* DamaegeCauser						// ������ ���߿�(ex����)
)
{
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamaegeCauser); // �ʼ� ȣ���� �ƴ�

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
    // ���� ���� ����
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

    // Enhanced InputComponent�� ĳ����
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // IA�� �������� ���� ���� ���� ���� Controller�� ASpartaPlayerController�� ĳ����
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
    // ���콺�� X, Y �������� 2D ������ ������
    FVector2D LookInput = value.Get<FVector2D>();

    // X�� �¿� ȸ�� (Yaw), Y�� ���� ȸ�� (Pitch)
    // �¿� ȸ��
    AddControllerYawInput(LookInput.X);
    // ���� ȸ��
    AddControllerPitchInput(LookInput.Y);
}

void ASpartaCharacter::Move(const FInputActionValue& value)
{
    // ��Ʈ�ѷ��� �־�� ���� ����� ����
    if (!Controller) return;

    // Value�� Axis2D�� ������ IA_Move�� �Է°� (WASD)�� ��� ����
// ��) (X=1, Y=0) �� ���� / (X=-1, Y=0) �� ���� / (X=0, Y=1) �� ������ / (X=0, Y=-1) �� ����
    FVector2D MoveInput = value.Get<FVector2D>();

    if (bIsMoveInputReversed)
    {
        MoveInput = -MoveInput;
    }

    if (!FMath::IsNearlyZero(MoveInput.X))
    {
        // ĳ���Ͱ� �ٶ󺸴� ����(����)���� X�� �̵�
        AddMovementInput(GetActorForwardVector(), MoveInput.X);
    }

    if (!FMath::IsNearlyZero(MoveInput.Y))
    {
        // ĳ������ ������ �������� Y�� �̵�
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