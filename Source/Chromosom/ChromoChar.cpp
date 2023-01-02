// Fill out your copyright notice in the Description page of Project Settings.


#include "ChromoChar.h"

// Sets default values
AChromoChar::AChromoChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChromoChar::BeginPlay()
{
	Super::BeginPlay();

	// Spawn homologous pair of chromosomes
	CreateChromatid(ESide::LEFT);
	CreateChromatid(ESide::RIGHT);
}

// Called every frame
void AChromoChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SelectedChromatid != nullptr) {
		SelectedChromatid = nullptr;
	}
}

// Called to bind functionality to input
void AChromoChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Create new Chromosom
void AChromoChar::CreateChromatid(ESide side)
{
	if (side == ESide::LEFT) {
		LeftChromatid = GetWorld()->SpawnActor<AChromatid>(ChromatidClass, left_loc, default_rotation);
		LeftChromatid->side = ESide::LEFT;
		LeftChromatid->ChromosomProperty = LeftChromatid->GenerateRandomChromosom(LeftChromatid->side);
		LeftChromatid->ApplyProperty();
		LeftChromatid->ChangeColor(FColor::Red);
	}
	else if (side == ESide::RIGHT) {
		RightChromatid = GetWorld()->SpawnActor<AChromatid>(ChromatidClass, right_loc, default_rotation);
		RightChromatid->side = ESide::RIGHT;
		RightChromatid->ChromosomProperty = RightChromatid->GenerateRandomChromosom(RightChromatid->side);
		RightChromatid->ApplyProperty();
		RightChromatid->ChangeColor(FColor::Blue);
	}
	else { }
}

// Initiative
void AChromoChar::StartTeilung()
{
	current_state = EState::TEILUNG;
	LeftChromatid->Teilung();
	RightChromatid->Teilung();
}