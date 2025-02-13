
#include "BlindItem.h"
#include "SpartaCharacter.h"

ABlindItem::ABlindItem()
{
	PrimaryActorTick.bCanEverTick = false;
	ItemDuration = 5.0f;
	BlurStrength = 30.0f;
}
void ABlindItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(Activator))
		{
			PlayerCharacter->GetBlindEffect(ItemDuration, BlurStrength);
			
			DestroyItem();

		}
	}
}

