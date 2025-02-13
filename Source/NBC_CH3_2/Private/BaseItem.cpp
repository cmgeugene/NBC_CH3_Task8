
#include "BaseItem.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

ABaseItem::ABaseItem()
{
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(Scene);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetupAttachment(Scene);
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);

	//이벤트 바인딩
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnItemOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnItemEndOverlap);
}

void ABaseItem::OnItemOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Overlapped.")));
		ActivateItem(OtherActor);
	}
}
void ABaseItem::OnItemEndOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
}

void ABaseItem::ActivateItem(AActor* Activator)
{
	if (IsPendingKillPending()) return;

	UParticleSystemComponent* Particle = nullptr;
	if (PickupParticle)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			PickupParticle,
			GetActorLocation(),
			GetActorRotation(),
			true
		);
	}
	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			PickupSound,
			GetActorLocation()
		);
		
	}
	if (Particle)
	{
		FTimerHandle DestroyParticleTimerHandle;

		GetWorld()->GetTimerManager().SetTimer(
			DestroyParticleTimerHandle,
			[this, Particle]()
			{
				if (IsValid(this))							// 람다 = 익명함수 , 이름이 없는 함수 대괄호는 캡쳐리스트
				{											// 람다를 실행할 때 파티클 변수를 바깥에서 가져다가 사용할 수 있도록 만드는
					Particle->DestroyComponent();			// 직접 구현하기는 번거로울 때 람다식 많이 사용
				}						
			},									
			2.0f,								
			false
		);
	}
}
FName ABaseItem::GetItemName() const
{
	return ItemType;
}
void ABaseItem::DestroyItem()
{
	if (this->IsPendingKillPending()) return;
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	UStaticMeshComponent* Mesh = this->FindComponentByClass<UStaticMeshComponent>();
	if (Mesh)
	{
		Mesh->SetVisibility(false);
	}
	USphereComponent* Colli = this->FindComponentByClass<USphereComponent>();
	if (Colli)
	{
		Colli->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}