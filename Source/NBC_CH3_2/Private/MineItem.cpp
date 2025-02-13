
#include "MineItem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AMineItem::AMineItem()
{
	ExplosionDamage = 30;
	ExplosionDelay = 5.0f;
	ExplosionRadius = 300.0f;
	ItemType = "Mine";
	bHasActivated = false;

	ExplodeCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Explode Collision"));
	ExplodeCollision->InitSphereRadius(ExplosionRadius);
	ExplodeCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplodeCollision->SetupAttachment(Scene);
}
void AMineItem::ActivateItem(AActor* Activator)
{
	if (bHasActivated) return;
	Super::ActivateItem(Activator);
	//타이머 핸들러
	// 게임 월드 -> 여러개의 핸들러 존재 가능 (타이머 매니저에 의해 관리)
	GetWorld()->GetTimerManager().SetTimer(
		ExplosionTimer,
		this,
		&AMineItem::Explode,
		ExplosionDelay,
		false);
	bHasActivated = true;
}
void AMineItem::Explode()
{
	if (IsPendingKillPending()) return;
	
	UParticleSystemComponent* Particle = nullptr;
	if (ExplosionParticle)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ExplosionParticle,
			GetActorLocation(),
			GetActorRotation(),
			false
		);
		
	}
	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			ExplosionSound,
			GetActorLocation()
		);
	}

	TArray<AActor*> OverlappingActors;
	ExplodeCollision->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		UGameplayStatics::ApplyDamage(
			Actor,
			ExplosionDamage,
			nullptr,
			this,
			UDamageType::StaticClass()
		);
	}
	DestroyItem();

	if (Particle)
	{
		FTimerHandle DestroyParticleTimerHandle;

		GetWorld()->GetTimerManager().SetTimer(
			DestroyParticleTimerHandle,
			[this, Particle]()
			{
				if (IsValid(this))
				{									// 람다 = 익명함수 , 이름이 없는 함수 대괄호는 캡쳐리스트
					Particle->DestroyComponent();	// 람다를 실행할 때 파티클 변수를 바깥에서 가져다가 사용할 수 있도록 만드는	
				}									// 직접 구현하기는 번거로울 때 람다식 많이 사용
												
			},									
			2.0f,								
			false
		);
	}
}

void AMineItem::DeactivateTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(ExplosionTimer);
}