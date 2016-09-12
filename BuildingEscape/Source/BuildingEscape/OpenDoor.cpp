// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"
#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing Pressure Plate."), *GetOwner()->GetName());
	}
}

void UOpenDoor::OpenDoor()
{
	//Set door rotation
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}

void UOpenDoor::CloseDoor()
{
	//Set door rotation
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	//Pool the trigger volume every frame
	if (GetTotalMassOfActorsOnPlate() > 30.0f)//TODO make into parameter
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	//Check if its time to close the door
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}
}
float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float totalMass = 0.0f;

	//Find overlapping actors
	TArray<AActor*>OverlappingActors;
	if (!PressurePlate) { return totalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	//iterate through them and add their masses
	for (const auto &Actor : OverlappingActors)
	{
		totalMass+=Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT(" %s on pressure plate"), *Actor->GetName());
	}

	return totalMass;
}

