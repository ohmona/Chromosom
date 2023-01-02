// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Engine/Classes/Components/PoseableMeshComponent.h>
#include "Chromatid.generated.h"

UENUM(BlueprintType)
enum class EChromosomBone : uint8 
{
	TOP1 UMETA(DisplayName = "Top.001"),
	TOP2 UMETA(DisplayName = "Top.002"),
	TOP3 UMETA(DisplayName = "Top.003"),
	BOTTOM1 UMETA(DisplayName = "Bottom.001"),
	BOTTOM2 UMETA(DisplayName = "Bottom.002"),
	BOTTOM3 UMETA(DisplayName = "Bottom.003"),
};

UENUM(BlueprintType)
enum class ESide : uint8
{
	LEFT UMETA(DisplayName = "left"),
	RIGHT UMETA(DisplayName = "right"),
};

// Never forget to apply all properties after changing them
USTRUCT(BlueprintType, Atomic)
struct FChromosomProperty 
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRotator> Top;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRotator> Bottom;
};

UCLASS()
class CHROMOSOM_API AChromatid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChromatid();

	// Default Values
	const FVector left_loc = FVector(-390, 30, 300);
	const FVector right_loc = FVector(-390, -10, 300);
	const FRotator default_rotation = FRotator(0, -90, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPoseableMeshComponent* PoseMesh;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;

	UPROPERTY(EditAnywhere)
	FColor DefaultColor = FColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESide side;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FChromosomProperty ChromosomProperty;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Change the color of mesh
	UFUNCTION(BlueprintCallable)
	void ChangeColor(FColor color);

	// Fold the mesh
	UFUNCTION(BlueprintCallable)
	void Fold(EChromosomBone bone, FRotator rotation);

	// Convert Enum to Fname
	UFUNCTION(BlueprintCallable)
	FName GetNameBone(EChromosomBone bone);

	// Convert int to Enum
	UFUNCTION(BlueprintCallable)
	EChromosomBone GetEnumBone(int32 bone);

	// Convert enum to int
	UFUNCTION(BlueprintCallable)
	int32 GetIndexbone(EChromosomBone bone);

	// Apply properties
	UFUNCTION(BlueprintCallable)
	void ApplyProperty();

	// Generate Random homologous pair of chromosomes
	UFUNCTION(BlueprintCallable)
	FChromosomProperty GenerateRandomChromosom(ESide desired_side);

	// Make this Chromatid disapear from screen
	UFUNCTION(BlueprintCallable)
	void Disapear(bool originalOnly);

	UFUNCTION(BlueprintCallable)
	void DisapearCopy();

	// Duplicate!
	UFUNCTION(BlueprintCallable)
	void Teilung();

	UFUNCTION(BlueprintCallable)
	bool hasGeteilt();

	// Flip chromatid
	UFUNCTION(BlueprintCallable)
	void Flip(bool bCopy);

	// Place this chromatid to original location
	UFUNCTION(BlueprintCallable)
	void ReturnToOriginal();

	// Should always be awared of decprecation and unexpected events
	UFUNCTION(BlueprintCallable)
	void ResetAllTimer();

private:
	FTimerHandle TeilungTimerHandle;
	float time = 0.0f;
	AChromatid* NewChromatid;
	
	float duration_1 = 0.5f;
	float duration_2 = 7.5f;
	float duration_3 = 8.0f;
	float dt = 0.01f;

	void TeilungTimer();

private:
	FTimerHandle ReturnTimerHandle;
	float time_return = 0.0f;
	FVector return_start_location;
	FRotator return_start_rotation;

	float duration_return = 1.0f;

	void ReturnTimer();
};
