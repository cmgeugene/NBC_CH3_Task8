
#include "ReverseControllItem.h"
#include "SpartaPlayerController.h"
#include "SpartaCharacter.h"

AReverseControllItem::AReverseControllItem()
{
	PrimaryActorTick.bCanEverTick = false;
	ItemDuration = 5.0f;
}

void AReverseControllItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(Activator))
		{
			PlayerCharacter->GetReverseControlEffect(ItemDuration);
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Move Input reversed.")));
			DestroyItem();

		}
	}
}



