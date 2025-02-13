
#include "SpartaGameInstance.h"

USpartaGameInstance::USpartaGameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
}

void USpartaGameInstance::AddtoScore(int32 amount)
{
	TotalScore += amount;
	UE_LOG(LogTemp, Display, TEXT("Total Score Updated : %d"), TotalScore);
}