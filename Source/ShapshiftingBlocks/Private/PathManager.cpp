// PathManager.cpp
#include "PathManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

APathManager::APathManager()
{
    PrimaryActorTick.bCanEverTick = false;  // No need to tick every frame
}

void APathManager::BeginPlay()
{
    Super::BeginPlay();

    // Validate parameters
    if (!BlockClass || Rows <= 0 || Cols <= 0 || CenterPlatformSize <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid configuration for PathManager."));
        return;
    }
    if (CenterPlatformSize > Rows || CenterPlatformSize > Cols)
    {
        UE_LOG(LogTemp, Warning, TEXT("CenterPlatformSize is larger than grid."));
        return;
    }

    // Determine the start/end indices of the center platform
    int32 CenterStartRow = (Rows - CenterPlatformSize) / 2;
    int32 CenterEndRow = CenterStartRow + CenterPlatformSize - 1;
    int32 CenterStartCol = (Cols - CenterPlatformSize) / 2;
    int32 CenterEndCol = CenterStartCol + CenterPlatformSize - 1;

    FVector Origin = GetActorLocation();
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    // Loop over each grid cell
    for (int32 i = 0; i < Rows; ++i)
    {
        for (int32 j = 0; j < Cols; ++j)
        {
            // Check if the cell is within the center platform region
            bool bIsInCenter = (i >= CenterStartRow && i <= CenterEndRow
                && j >= CenterStartCol && j <= CenterEndCol);

            bool bSpawnBlock = bIsInCenter;
            if (!bIsInCenter)
            {
                // Randomly decide to spawn a block or leave a gap
                bSpawnBlock = (FMath::RandRange(0, 1) > 0);  // 50% chance [oai_citation:7‡dev.epicgames.com](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Core/Math/FMath/RandRange/1#:~:text=static%20int32%20RandRange%20,Copy%20full%20snippet)
            }

            if (bSpawnBlock)
            {
                // Calculate spawn location for this block
                FVector SpawnLocation = Origin;
                SpawnLocation.X += i * BlockSize * 2.0f;
                SpawnLocation.Y += j * BlockSize * 2.0f;

                // Spawn the block actor
                AActor* Block = GetWorld()->SpawnActor<AActor>(
                    BlockClass,
                    SpawnLocation,
                    FRotator::ZeroRotator,
                    SpawnParams);

                if (Block)
                {
                    // Scale the block to 2x2 in XY
                    Block->SetActorScale3D(FVector(2.0f, 2.0f, 1.0f));
                    SpawnedBlocks.Add(Block);
                }
            }
        }
    }

    // Compute the world location of the center of the center platform
    int32 CenterIndexRow = CenterStartRow + CenterPlatformSize / 2;
    int32 CenterIndexCol = CenterStartCol + CenterPlatformSize / 2;
    FVector CenterLocation = Origin;
    CenterLocation.X += CenterIndexRow * BlockSize * 2.0f;
    CenterLocation.Y += CenterIndexCol * BlockSize * 2.0f;
    // Raise Z so the player spawns above the platform (safe drop)
    CenterLocation.Z += BlockSize;

    // Teleport the player character to the center platform
    ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (PlayerChar)
    {
        PlayerChar->SetActorLocation(CenterLocation);
    }
}