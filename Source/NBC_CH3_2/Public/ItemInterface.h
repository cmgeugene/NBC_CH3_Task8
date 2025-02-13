#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"


UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};


class NBC_CH3_2_API IItemInterface
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,	// ������ �߻��� �ڱ� �ڽ�
		AActor* OtherActor,						// �浹�� �߻��� ����
		UPrimitiveComponent* OtherComp,			// �浹�� �߻��� ������ ������Ʈ
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) = 0;
	
	UFUNCTION()
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) = 0;
	
	virtual void ActivateItem(AActor* Activator) = 0;
	virtual FName GetItemName() const = 0;
};
