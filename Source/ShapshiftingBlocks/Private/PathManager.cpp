#include "PathManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "UObject/UnrealType.h" // For setting BP variables dynamically

APathManager::APathManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APathManager::BeginPlay()
{
	Super::BeginPlay();

	if (!BlockClass || Rows <= 0 || Cols <= 0 || CenterPlatformSize <= 0) return;

	SpawnMaze();
	RandomizeBlockTypes();
	SpawnPlayerAtCenter();
}

void APathManager::SpawnMaze()
{
	MazeBlocks.Empty();

	int32 CenterStartRow = (Rows - CenterPlatformSize) / 2;
	int32 CenterEndRow = CenterStartRow + CenterPlatformSize - 1;
	int32 CenterStartCol = (Cols - CenterPlatformSize) / 2;
	int32 CenterEndCol = CenterStartCol + CenterPlatformSize - 1;

	FVector Origin = GetActorLocation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (int32 i = 0; i < Rows; ++i)
	{
		for (int32 j = 0; j < Cols; ++j)
		{
			bool bIsInCenter = (i >= CenterStartRow && i <= CenterEndRow &&
				j >= CenterStartCol && j <= CenterEndCol);

			bool bSpawnBlock = bIsInCenter || (FMath::RandRange(0, 1) > 0);

			AActor* SpawnedBlock = nullptr;
			if (bSpawnBlock)
			{
				FVector SpawnLocation = Origin + FVector(i * BlockSize, j * BlockSize, 0.f);
				SpawnedBlock = GetWorld()->SpawnActor<AActor>(BlockClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);

				// Set Row and Col variables in BP_Block
				if (SpawnedBlock)
				{
					// Row
					FProperty* RowProp = SpawnedBlock->GetClass()->FindPropertyByName(FName("Row"));
					if (FIntProperty* IntProp = CastField<FIntProperty>(RowProp))
					{
						IntProp->SetPropertyValue_InContainer(SpawnedBlock, i);
					}

					// Col
					FProperty* ColProp = SpawnedBlock->GetClass()->FindPropertyByName(FName("Col"));
					if (FIntProperty* IntProp = CastField<FIntProperty>(ColProp))
					{
						IntProp->SetPropertyValue_InContainer(SpawnedBlock, j);
					}
					FProperty* Prop = SpawnedBlock->GetClass()->FindPropertyByName("PathManagerRef");
					if (FObjectProperty* ObjProp = CastField<FObjectProperty>(Prop))
					{
						ObjProp->SetPropertyValue_InContainer(SpawnedBlock, this);
					}

				}
			}

			FMazeBlock BlockStruct;
			BlockStruct.BlockActor = SpawnedBlock;
			BlockStruct.Row = i;
			BlockStruct.Col = j;
			MazeBlocks.Add(BlockStruct);
		}
	}
}

void APathManager::SetBlockTypeOnBlock(AActor* BlockActor, uint8 EnumValue)
{
	if (!BlockActor) return;

	FName FuncName = FName("SetBlockType"); // Blueprint function
	UFunction* Func = BlockActor->FindFunction(FuncName);
	if (Func)
	{
		struct FBlockTypeParam { uint8 Type; };
		FBlockTypeParam Param;
		Param.Type = EnumValue;
		BlockActor->ProcessEvent(Func, &Param);
	}
}

void APathManager::RandomizeBlockTypes()
{
	TArray<uint8> Types = { 1,2,3,4 }; // Slippery, Slime, Magma, Sticky
	int32 ClusterSize = 5;

	for (int32 ClusterNum = 0; ClusterNum < MazeBlocks.Num() / 10; ClusterNum++)
	{
		uint8 Type = Types[FMath::RandRange(0, Types.Num() - 1)];
		int32 StartIndex = FMath::RandRange(0, MazeBlocks.Num() - ClusterSize);

		for (int32 i = 0; i < ClusterSize; i++)
		{
			FMazeBlock& Block = MazeBlocks[StartIndex + i];
			if (Block.BlockActor)
				SetBlockTypeOnBlock(Block.BlockActor, Type);
		}
	}
}

void APathManager::UpdateMazeNear(int32 PlayerRow, int32 PlayerCol)
{
	int32 Radius = 3; // affects 7x7 area around player

	for (int32 r = PlayerRow - Radius; r <= PlayerRow + Radius; r++)
	{
		for (int32 c = PlayerCol - Radius; c <= PlayerCol + Radius; c++)
		{
			if (r < 0 || r >= Rows || c < 0 || c >= Cols) continue;

			int32 Index = r * Cols + c;
			if (!MazeBlocks.IsValidIndex(Index)) continue;

			FMazeBlock& Block = MazeBlocks[Index];

			if (FMath::RandBool()) // 50% chance
			{
				if (Block.BlockActor)
				{
					Block.BlockActor->Destroy();
					Block.BlockActor = nullptr;
				}
			}
			else
			{
				if (!Block.BlockActor)
				{
					FVector SpawnLocation = GetActorLocation() + FVector(r * BlockSize, c * BlockSize, 0.f);
					Block.BlockActor = GetWorld()->SpawnActor<AActor>(BlockClass, SpawnLocation, FRotator::ZeroRotator);
				}
			}
		}
	}

	RandomizeBlockTypes();
}

void APathManager::SpawnPlayerAtCenter()
{
	int32 CenterRow = Rows / 2;
	int32 CenterCol = Cols / 2;

	FVector SpawnLocation = GetActorLocation() + FVector(CenterRow * BlockSize , CenterCol * BlockSize , BlockSize + 50.f);
	ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerChar)
	{
		PlayerChar->SetActorLocation(SpawnLocation);
	}
}