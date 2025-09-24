#include "ShapeShiftBlock.h"
#include "Components/StaticMeshComponent.h"


AShapeShiftBlock::AShapeShiftBlock()
{
    PrimaryActorTick.bCanEverTick = true;

    BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh"));
    RootComponent = BlockMesh;

    CurrentState = EBlockState::Normal;
}

void AShapeShiftBlock::BeginPlay()
{
    Super::BeginPlay();
    SetBlockState(CurrentState);
}

void AShapeShiftBlock::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AShapeShiftBlock::SetBlockState(EBlockState NewState)
{
    CurrentState = NewState;

    switch (CurrentState)
    {
    case EBlockState::Normal:
        BlockMesh->SetPhysMaterialOverride(NormalMaterial);
        break;

    case EBlockState::Bouncy:
        BlockMesh->SetPhysMaterialOverride(BouncyMaterial);
        break;

    case EBlockState::Slippery:
        BlockMesh->SetPhysMaterialOverride(SlipperyMaterial);
        break;

    case EBlockState::Sticky:
        BlockMesh->SetPhysMaterialOverride(StickyMaterial);
        break;

    default:
        break;
    }
}

void AShapeShiftBlock::OnTouched()
{
    SetBlockState(EBlockState::Bouncy);
}

void AShapeShiftBlock::OnJumpedOn()
{
    SetBlockState(EBlockState::Sticky);
}

void AShapeShiftBlock::OnShot()
{
    SetBlockState(EBlockState::Slippery);
}
