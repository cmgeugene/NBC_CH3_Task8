
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
	//Ÿ�̸� �ڵ鷯
	// ���� ���� -> �������� �ڵ鷯 ���� ���� (Ÿ�̸� �Ŵ����� ���� ����)
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
				{									// ���� = �͸��Լ� , �̸��� ���� �Լ� ���ȣ�� ĸ�ĸ���Ʈ
					Particle->DestroyComponent();	// ���ٸ� ������ �� ��ƼŬ ������ �ٱ����� �����ٰ� ����� �� �ֵ��� �����	
				}									// ���� �����ϱ�� ���ŷο� �� ���ٽ� ���� ���
												
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