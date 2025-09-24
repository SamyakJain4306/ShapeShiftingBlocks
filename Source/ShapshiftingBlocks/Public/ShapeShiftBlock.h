// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShapeShiftBlock.generated.h"

UENUM(BlueprintType)
enum class EBlockState : uint8
{
    Normal      UMETA(DisplayName = "Normal"),
    Bouncy      UMETA(DisplayName = "Bouncy"),
    Slippery    UMETA(DisplayName = "Slippery"),
    Sticky      UMETA(DisplayName = "Sticky")
};

UCLASS()
class SHAPSHIFTINGBLOCKS_API AShapeShiftBlock : public AActor
{
    GENERATED_BODY()

public:
    AShapeShiftBlock();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* BlockMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block")
    EBlockState CurrentState;

    // Physics Materials for each state
    UPROPERTY(EditAnywhere, Category = "Block")
    UPhysicalMaterial* BouncyMaterial;

    UPROPERTY(EditAnywhere, Category = "Block")
    UPhysicalMaterial* SlipperyMaterial;

    UPROPERTY(EditAnywhere, Category = "Block")
    UPhysicalMaterial* StickyMaterial;

    UPROPERTY(EditAnywhere, Category = "Block")
    UPhysicalMaterial* NormalMaterial;

public:
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Block")
    void SetBlockState(EBlockState NewState);

    // Interaction functions
    void OnTouched();
    void OnJumpedOn();
    void OnShot();

};
