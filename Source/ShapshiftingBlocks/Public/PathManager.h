#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathManager.generated.h"

UCLASS()
class SHAPSHIFTINGBLOCKS_API APathManager : public AActor
{
	GENERATED_BODY()

public:
	APathManager();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
	int32 Rows = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
	int32 Cols = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
	float BlockSize = 100.f;

	// Blueprint class reference (works with BP_Block)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
	TSubclassOf<AActor> BlockClass;

	// Store spawned blocks as generic AActor references
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze")
	TArray<AActor*> Blocks;

private:
	TArray<TArray<int32>> MazeArray;

	void InitializeMaze();
	void GenerateMaze(int32 x, int32 y);
	bool IsValid(int32 x, int32 y);
	void SpawnMaze();
};