#include "PathManager.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

APathManager::APathManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APathManager::BeginPlay()
{
	Super::BeginPlay();

	// Initialize the maze
	InitializeMaze();

	// Generate random maze starting from top-left
	GenerateMaze(1, 1);

	// Spawn BP_Block actors according to maze
	SpawnMaze();
}

void APathManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Initialize maze array with all walls
void APathManager::InitializeMaze()
{
	MazeArray.SetNum(Rows);
	for (int32 i = 0; i < Rows; i++)
	{
		MazeArray[i].SetNum(Cols);
		for (int32 j = 0; j < Cols; j++)
		{
			MazeArray[i][j] = 1; // 1 = wall
		}
	}
}

// Check if cell can be turned into a path
bool APathManager::IsValid(int32 x, int32 y)
{
	if (x <= 0 || y <= 0 || x >= Cols - 1 || y >= Rows - 1)
		return false;

	int32 Count = 0;
	int dx[4] = { 0, 1, 0, -1 };
	int dy[4] = { -1, 0, 1, 0 };

	for (int i = 0; i < 4; i++)
	{
		int nx = x + dx[i];
		int ny = y + dy[i];
		if (MazeArray[ny][nx] == 0)
			Count++;
	}

	return Count <= 1; // Only carve path if <=1 neighbor
}

// Recursive Depth-First maze generation
void APathManager::GenerateMaze(int32 x, int32 y)
{
	MazeArray[y][x] = 0; // 0 = path

	TArray<int32> dirs = { 0, 1, 2, 3 };
	dirs.Sort([](const int32& A, const int32& B) { return FMath::RandBool(); }); // Shuffle directions

	int dx[4] = { 0, 1, 0, -1 };
	int dy[4] = { -1, 0, 1, 0 };

	for (int32 i = 0; i < dirs.Num(); i++)
	{
		int nx = x + dx[dirs[i]];
		int ny = y + dy[dirs[i]];

		if (IsValid(nx, ny))
		{
			GenerateMaze(nx, ny);
		}
	}
}

// Spawn BP_Block actors for walls and scale to 2x2
void APathManager::SpawnMaze()
{
	if (!BlockClass) return;

	UWorld* World = GetWorld();
	if (!World) return;

	// Destroy old blocks if any
	for (AActor* Block : Blocks)
	{
		if (Block)
			Block->Destroy();
	}
	Blocks.Empty();

	for (int32 row = 0; row < Rows; row++)
	{
		for (int32 col = 0; col < Cols; col++)
		{
			if (MazeArray[row][col] == 1) // Spawn walls only
			{
				FVector SpawnLocation = GetActorLocation()
					+ FVector(col * BlockSize * 2.f, row * BlockSize * 2.f, 0.f); // 2x2 spacing

				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				// Spawn the Blueprint actor
				AActor* SpawnedActor = World->SpawnActor<AActor>(BlockClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
				if (SpawnedActor)
				{
					// Scale block to 2x2
					SpawnedActor->SetActorScale3D(FVector(2.f, 2.f, 1.f));

					// Store reference in Blocks array
					Blocks.Add(SpawnedActor);
				}
			}
		}
	}
}