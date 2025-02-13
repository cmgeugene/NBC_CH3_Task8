
#include "SlowingItem.h"
#include "SpartaCharacter.h"

ASlowingItem::ASlowingItem()
{

	PrimaryActorTick.bCanEverTick = false;
	ItemDuration = 5.0f;
	SlowingRate = 0.5;
	
}

void ASlowingItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(Activator))
		{
			PlayerCharacter->NormalSpeed = PlayerCharacter->NormalSpeed * SlowingRate;
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Speed decreased by 50%.")));

			PlayerCharacter->GetSlowEffect(Activator, ItemDuration, SlowingRate);
			DestroyItem();

		}
	}
}


