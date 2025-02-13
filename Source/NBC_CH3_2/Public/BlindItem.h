
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseItem.h"
#include "BlindItem.generated.h"

UCLASS()
class NBC_CH3_2_API ABlindItem : public ABaseItem
{
	GENERATED_BODY()
	
public:	
	ABlindItem();
	virtual void ActivateItem(AActor* Activator);

protected:
	//virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties")
	float ItemDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties")
	float BlurStrength;
};
