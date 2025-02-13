#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SpawnVolume.h"
#include "SpartaGameState.generated.h"

/**
 *
 */
UCLASS()
class NBC_CH3_2_API ASpartaGameState : public AGameState
{
	GENERATED_BODY()

public:

	ASpartaGameState();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Coin")
	int32 SpawnedCoinCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Coin")
	int32 CollectedCoinCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	float LevelDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	float WaveDuration;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	FTimerHandle LevelTimerHandle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	FTimerHandle WaveTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Level")
	int32 CurrentLevelIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Level")
	int32 CurrentWaveIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Level")
	int32 MaxLevels;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Level")
	int32 MaxWaves;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	TArray<FName> LevelMapNames;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	int32 ItemToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	int32 ScoreToClear;
	ASpawnVolume* SpawnVolumeInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|Sound")
	UAudioComponent* AudioComp;

	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 amount);
	UFUNCTION(BlueprintCallable, Category = "Score")
	void OnGameOver();
	void OnWaveOver(); // 웨이브 끝, 레벨 넘어갈지 말지 판단
	void UpdateHUD();

	void OnCoinCollected();
	void StartLevel();
	void OnLevelTimeUp();
	void EndLevel();
};
