// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"


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
	
}

void UOpenDoor::OpenDoor()
{
	//Find owning actor
	AActor* Owner = GetOwner();

	//Create roator
	FRotator newRotation = FRotator(0.0f, 45.0f, 0.0f);
	//Set door rotation
	Owner->SetActorRotation(newRotation);

}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	//Pool the trigger volume every frame
	//If the actor that opens is in the volume
	if(PressurePlate->IsOverlappingActor(ActorThatOpens))
			OpenDoor();
}

