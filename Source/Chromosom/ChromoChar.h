// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Chromatid.h"
#include "ChromoChar.generated.h"

UENUM(BlueprintType)
enum class EState : uint8
{
	PAAR UMETA(DisplayName = "Paar"),
	TEILUNG UMETA(DisplayName = "Teilung"),
	GETEILT UMETA(DisplayName = "Geteilt"),
};

UENUM(BlueprintType)
enum class ESelectionState : uint8
{
	NONE UMETA(DisplayName = "none"),
	LEFTONLY UMETA(DisplayName = "Left only"),
	RIGHTONLY UMETA(DisplayName = "Right only"),
	BOTH UMETA(DisplayName = "Both")
};

UCLASS()
class CHROMOSOM_API AChromoChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AChromoChar();

	UPROPERTY(EditAnywhere)
	UClass* ChromatidClass;

	// Default Values
	const FVector left_loc = FVector(-390, 30, 300);
	const FVector right_loc = FVector(-390, -10, 300);
	const FRotator default_rotation = FRotator(0, -90, 0);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AChromatid* LeftChromatid;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AChromatid* RightChromatid;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AChromatid* SelectedChromatid;

	UPROPERTY(BlueprintReadWrite)
	EState current_state = EState::PAAR;

	UPROPERTY(BlueprintReadWrite)
	ESelectionState selection_state = ESelectionState::NONE;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Create new Chromosom
	UFUNCTION(BlueprintCallable)
	void CreateChromatid(ESide side);

	// Initiative
	UFUNCTION(BlueprintCallable)
	void StartTeilung();

};
