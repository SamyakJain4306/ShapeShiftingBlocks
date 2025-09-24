
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
    // Grid dimensions (must accommodate the center platform)
    UPROPERTY(EditAnywhere, Category = "Maze")
    int32 Rows = 10;

    UPROPERTY(EditAnywhere, Category = "Maze")
    int32 Cols = 10;

    // Base block size (used for spacing; actual block is scaled 2×2)
    UPROPERTY(EditAnywhere, Category = "Maze")
    float BlockSize = 100.0f;

    // Width of the center platform (e.g. 5 for a 5×5 center block)
    UPROPERTY(EditAnywhere, Category = "Maze")
    int32 CenterPlatformSize = 5;

    // Blueprint class to use for each block (BP_Block)
    UPROPERTY(EditAnywhere, Category = "Maze")
    TSubclassOf<AActor> BlockClass;

    // Holds references to all spawned block actors
    UPROPERTY(VisibleAnywhere, Category = "Maze")
    TArray<AActor*> SpawnedBlocks;
};