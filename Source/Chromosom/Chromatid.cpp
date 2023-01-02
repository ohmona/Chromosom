// Fill out your copyright notice in the Description page of Project Settings.


#include "Chromatid.h"
#include "ChromoChar.h"

// Sets default values
AChromatid::AChromatid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initalize Poseable Mesh Component
	PoseMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("Pose Mesh"));
	PoseMesh->SetWorldScale3D(FVector(0.2, 0.2, 0.2));
	
	// Initalize chromosom property
	ChromosomProperty.Top.Init(FRotator::ZeroRotator, 3);
	ChromosomProperty.Bottom.Init(FRotator::ZeroRotator, 3);
}

// Called when the game starts or when spawned
void AChromatid::BeginPlay()
{
	Super::BeginPlay();
	
	// Starting up by applying the color
	ChangeColor(DefaultColor);

	ApplyProperty();
}

// Called every frame
void AChromatid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Change the color of mesh
void AChromatid::ChangeColor(FColor color)
{
	// Create dynamic material
	UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(Material, this);

	// Set parameter value
	DynMaterial->SetVectorParameterValue("Color", color);

	// Apply dynamic material
	PoseMesh->SetMaterial(0, DynMaterial);
}

// Fold the mesh
void AChromatid::Fold(EChromosomBone bone, FRotator rotation)
{
	FRotator original = PoseMesh->GetBoneRotationByName(GetNameBone(bone), EBoneSpaces::ComponentSpace);
	PoseMesh->SetBoneRotationByName(GetNameBone(bone), original + rotation, EBoneSpaces::ComponentSpace);
	if (GetIndexbone(bone) < 4) { // Top
		if (ChromosomProperty.Top[GetIndexbone(bone) - 1] != rotation) { // if not from apply property
			ChromosomProperty.Top[GetIndexbone(bone) - 1] = rotation;
		}
	}
	if (GetIndexbone(bone) > 3) { // Bottom
		if (ChromosomProperty.Bottom[GetIndexbone(bone) - 4] != rotation) { // if not from apply property
			ChromosomProperty.Bottom[GetIndexbone(bone) - 4] = rotation;
		}
	}
}

// Convert Enum to Fname
FName AChromatid::GetNameBone(EChromosomBone bone)
{
	if (bone == EChromosomBone::TOP1) return FName("Top_001");
	if (bone == EChromosomBone::TOP2) return FName("Top_002");
	if (bone == EChromosomBone::TOP3) return FName("Top_003");
	if (bone == EChromosomBone::BOTTOM1) return FName("Bottom_001");
	if (bone == EChromosomBone::BOTTOM2) return FName("Bottom_002");
	if (bone == EChromosomBone::BOTTOM3) return FName("Bottom_003");
	return FName("Error");
}

// Convert int to Fname
EChromosomBone AChromatid::GetEnumBone(int32 bone)
{
	if (bone == 1) return EChromosomBone::TOP1;
	if (bone == 2) return EChromosomBone::TOP2;
	if (bone == 3) return EChromosomBone::TOP3;
	if (bone == 4) return EChromosomBone::BOTTOM1;
	if (bone == 5) return EChromosomBone::BOTTOM2;
	if (bone == 6) return EChromosomBone::BOTTOM3;
	return EChromosomBone::TOP1;
}

// Convert enum to int
int32 AChromatid::GetIndexbone(EChromosomBone bone)
{
	if (bone == EChromosomBone::TOP1) return 1;
	if (bone == EChromosomBone::TOP2) return 2;
	if (bone == EChromosomBone::TOP3) return 3;
	if (bone == EChromosomBone::BOTTOM1) return 4;
	if (bone == EChromosomBone::BOTTOM2) return 5;
	if (bone == EChromosomBone::BOTTOM3) return 6;
	return 0;
}

// Apply properties
void AChromatid::ApplyProperty()
{
	// For Top ones
	for (int32 i = 0; i < 3; i++) {
		Fold(GetEnumBone(i + 1), ChromosomProperty.Top[i]);
	}
	// For Bottom ones
	for (int32 i = 0; i < 3; i++) {
		Fold(GetEnumBone(i + 4), ChromosomProperty.Bottom[i]);
	}
}

// Generate Random homologous pair of chromosomes
FChromosomProperty AChromatid::GenerateRandomChromosom(ESide desired_side)
{
	// variable to return
	FChromosomProperty prop;
	prop.Top.Init(FRotator::ZeroRotator, 3);
	prop.Bottom.Init(FRotator::ZeroRotator, 3);

	if (desired_side == ESide::LEFT) {
		prop.Top[0] = FRotator(FMath::RandRange(0, 50), 0, 0);
		prop.Top[1] = FRotator(FMath::RandRange(-30, 0), 0, 0);
		prop.Top[2] = FRotator(FMath::RandRange(0, 20), 0, 0);
		prop.Bottom[0] = FRotator(-1 * FMath::RandRange(0, 50), 0, 0);
		prop.Bottom[1] = FRotator(-1 * FMath::RandRange(-30, 0), 0, 0);
		prop.Bottom[2] = FRotator(-1 * FMath::RandRange(0, 20), 0, 0);

	}
	else if (desired_side == ESide::RIGHT) { // Right
		prop.Top[0] = FRotator(-1 * FMath::RandRange(0, 50), 0, 0);
		prop.Top[1] = FRotator(-1 * FMath::RandRange(-30, 0), 0, 0);
		prop.Top[2] = FRotator(-1 * FMath::RandRange(0, 20), 0, 0);
		prop.Bottom[0] = FRotator(FMath::RandRange(0, 50), 0, 0);
		prop.Bottom[1] = FRotator(FMath::RandRange(-30, 0), 0, 0);
		prop.Bottom[2] = FRotator(FMath::RandRange(0, 20), 0, 0);
	}

	return prop;
}

// Make this Chromatid disapear from screen
void AChromatid::Disapear(bool originalOnly)
{
	if (originalOnly && NewChromatid != nullptr) {
		// Check if this chromatid belongs to player
		AChromoChar* player = Cast<AChromoChar>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		if (this == player->LeftChromatid) {
			player->LeftChromatid = NewChromatid;
		}
		else if (this == player->RightChromatid) {
			player->RightChromatid = NewChromatid;
		}

		// THIS METHOD SHOULD COME AT LAST
		Destroy();
	}
	else {
		// Check if this chromatid belongs to player
		AChromoChar* player = Cast<AChromoChar>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		if (this == player->LeftChromatid) {
			player->LeftChromatid = nullptr;
		}
		else if (this == player->RightChromatid) {
			player->RightChromatid = nullptr;
		}

		// Check if this chromatid has a copy
		if (hasGeteilt()) DisapearCopy();

		ResetAllTimer();

		if(NewChromatid != nullptr) NewChromatid->Destroy();
		// THIS METHOD SHOULD COME AT LAST
		Destroy();
	}
}

void AChromatid::DisapearCopy()
{
	NewChromatid->Destroy();
	NewChromatid = nullptr;
}

// Duplicate!
void AChromatid::Teilung()
{
	GetWorld()->GetTimerManager().SetTimer(TeilungTimerHandle, this, &AChromatid::TeilungTimer, dt, true);
}

// Part 1
void AChromatid::TeilungTimer()
{
	// Useful boolean
	bool bLeft = true;
	if (side == ESide::RIGHT) bLeft = false;

	FVector new_chromatid_loc_left = left_loc;
	FVector new_chromatid_loc_right = right_loc;

	const float move_x = -400;
	const float move_y = 200;

	// Preset for spawn location
	FTransform preset;
	preset.SetLocation(bLeft
		? left_loc + FVector(move_x, move_y, 0)
		: right_loc + FVector(move_x, move_y * (-1), 0));
	preset.SetRotation(FQuat(default_rotation + FRotator(0, 180, 0)));
	preset.SetScale3D(FVector(0, 0, 0));

	// Move & Duplicate
	if (time == 0) {
		// Create new chromatid (size 0, flipped, same property). zweichromatidig!
		AChromoChar* p = Cast<AChromoChar>(GetWorld()->GetFirstPlayerController()->GetCharacter());

		NewChromatid = GetWorld()->SpawnActor<AChromatid>(p->ChromatidClass, FVector::ZeroVector, FRotator::ZeroRotator);

		NewChromatid->SetActorTransform(preset);

		// Copy every necessery values
		NewChromatid->side = side;
		NewChromatid->ChromosomProperty = ChromosomProperty;
		NewChromatid->ApplyProperty();
		NewChromatid->ChangeColor(bLeft ? FColor::Red : FColor::Blue);
	}
	else if (time > 0 && time <= duration_1) {

		// Calculate desired location
		FVector desired_loc;
		FVector target_loc = bLeft ? left_loc + FVector(move_x, move_y, 0) : right_loc + FVector(move_x, move_y * (-1), 0);
		float delta_time = time / duration_1;

		desired_loc = FMath::Lerp(bLeft ? left_loc : right_loc, target_loc, delta_time);

		SetActorLocation(desired_loc);
	}
	else if (time > duration_1 + 1.0f && time <= duration_2) {
		float delta_time = time / duration_2;

		// For new chromatid
		FVector target_loc_new;
		FVector desired_scale;

		FVector loc_end = bLeft ? left_loc + FVector(move_x, move_y - 40, 0) : right_loc + FVector(move_x, move_y * (-1) + 40, 0);

		desired_scale = FMath::Lerp(FVector(0.0, 0.0, 0.0), FVector(0.2, 0.2, 0.2), delta_time);
		target_loc_new = FMath::Lerp(preset.GetLocation(), loc_end, delta_time);

		NewChromatid->SetActorScale3D(desired_scale);
		NewChromatid->SetActorLocation(target_loc_new);
	}
	else if (time > duration_2 + 0.1f && time <= duration_3) {
		float delta_time = time / duration_3;

		FVector start_loc = bLeft ? left_loc + FVector(move_x, move_y, 0) : right_loc + FVector(move_x, move_y * (-1), 0);
		FVector start_loc_new = bLeft ? left_loc + FVector(move_x, move_y - 40, 0) : right_loc + FVector(move_x, move_y * (-1) + 40, 0);
		FVector target_loc = bLeft ? left_loc + FVector(move_x, move_y + 30, 0) : right_loc + FVector(move_x, move_y * (-1) - 30, 0);
		FVector target_loc_new = bLeft ? left_loc + FVector(move_x, move_y - 45, 0) : right_loc + FVector(move_x, move_y * (-1) + 45, 0);;
		FVector desired_loc;
		FVector desired_loc_new;

		desired_loc = FMath::Lerp(start_loc, target_loc, delta_time);
		desired_loc_new = FMath::Lerp(start_loc_new, target_loc_new, delta_time);
	
		SetActorLocation(desired_loc);
		NewChromatid->SetActorLocation(desired_loc_new);
	}
	else if(time > duration_3) {
		// Reset Timer
		time = 0;
		GetWorld()->GetTimerManager().ClearTimer(TeilungTimerHandle);

		// Update State
		AChromoChar* player = Cast<AChromoChar>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		player->current_state = EState::GETEILT;
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("%f"), time)

	time += dt;
}

bool AChromatid::hasGeteilt()
{
	return NewChromatid != nullptr;
}

// Flip chromatid
void AChromatid::Flip(bool bCopy)
{
	if (!bCopy) {
		SetActorRotation(GetActorRotation() + FRotator(0, 180, 0));
	}
	else {
		if(NewChromatid != nullptr) NewChromatid->SetActorRotation(GetActorRotation() + FRotator(0, 180, 0));
	}
}

// Place this chromatid to original location
void AChromatid::ReturnToOriginal()
{
	return_start_location = GetActorLocation();
	return_start_rotation = GetActorRotation();
	GetWorld()->GetTimerManager().SetTimer(ReturnTimerHandle, this, &AChromatid::ReturnTimer, dt, true);
}

void AChromatid::ReturnTimer()
{
	// Useful boolean
	bool bLeft = true;
	if (side == ESide::RIGHT) bLeft = false;

	if (time_return == 0 || time_return <= duration_return) {
		UE_LOG(LogTemp, Log, TEXT("%f"), time_return)
		// Location
		FVector start_loc = return_start_location;
		FVector end_loc = bLeft ? left_loc : right_loc;
		FVector desired_loc;

		float delta_time = time_return / duration_return;

		desired_loc = FMath::Lerp(start_loc, end_loc, delta_time);

		UE_LOG(LogTemp, Log, TEXT("%s"), *desired_loc.ToString())
		SetActorLocation(desired_loc);

		// Rotation
		FRotator start_rot = return_start_rotation;
		FRotator end_rot = default_rotation;
		FRotator desired_rot;

		desired_rot = FMath::Lerp(start_rot, end_rot, delta_time);

		SetActorRotation(desired_rot);
	}
	else if(time_return > duration_return) {
		// Reset Timer
		time_return = 0;
		GetWorld()->GetTimerManager().ClearTimer(ReturnTimerHandle);
	}

	time_return += dt;
}

// Should always be awared of decprecation and unexpected events
void AChromatid::ResetAllTimer()
{
	// Reset Timer
	time = 0;
	GetWorld()->GetTimerManager().ClearTimer(TeilungTimerHandle);

	// Return timer
	time_return = 0;
	GetWorld()->GetTimerManager().ClearTimer(ReturnTimerHandle);
}