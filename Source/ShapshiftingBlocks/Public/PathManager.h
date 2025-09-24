#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathManager.generated.h"

// Struct to track block and its grid position
USTRUCT(BlueprintType)
struct FMazeBlock
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* BlockActor = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Row = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Col = 0;
};

UCLASS()
class SHAPSHIFTINGBLOCKS_API APathManager : public AActor
{
	GENERATED_BODY()

public:
	APathManager();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
	int32 Rows = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
	int32 Cols = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
	float BlockSize = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
	int32 CenterPlatformSize = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
	TSubclassOf<AActor> BlockClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze")
	TArray<FMazeBlock> MazeBlocks;

	// Spawn maze initially
	void SpawnMaze();

	// Called when player steps on a block
	UFUNCTION(BlueprintCallable)
	void UpdateMazeNear(int32 PlayerRow, int32 PlayerCol);

	// Randomize block types in clusters
	void RandomizeBlockTypes();

private:
	// Helper to call Blueprint function and set enum type
	void SetBlockTypeOnBlock(AActor* BlockActor, uint8 EnumValue);

	// Spawn player at center platform
	void SpawnPlayerAtCenter();
};