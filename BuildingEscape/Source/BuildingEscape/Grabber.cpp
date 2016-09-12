// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}
// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetUpAttachedInputComponent();
}
void UGrabber::SetUpAttachedInputComponent()
{
	///Look for attached input component
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing input component."), *GetOwner()->GetName());
	}
}

void UGrabber::FindPhysicsHandleComponent()
{
	///Look for attached physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle==nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component."), *GetOwner()->GetName());
	}
}
void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Pressed"))
		//LINE TRACE and see if we reach any actors with physics body channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	//If we hit something attach physics handle
	//TODO Attach physics handle
	if(ActorHit!=nullptr)
	PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true);
}
void UGrabber::Release()
{
	if (!PhysicsHandle) { return; }
	UE_LOG(LogTemp, Warning, TEXT("Grab Released"))
		//TODO Release Physics
		PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	GetReachLineEnd();

	if (!PhysicsHandle) { return; }
	//If the physics handle is attached 
	if (PhysicsHandle->GrabbedComponent)
	{
		//move the object that we are holding
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
	

}
const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	//Draw a red trace line
	//DrawDebugLine(GetWorld(), playerViewPointLocation, LineTraceEnd, FColor(255, 0, 0), false, 0.0f, 0.0f, 10.0f);

	///Line-cast out to reach distance
	FHitResult HitResult;
	FCollisionQueryParams TraceParameters(FName(TEXT(" ")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	//See what we hit
	AActor* hitActor = HitResult.GetActor();
	if (hitActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line Trace Hit :%s"), *hitActor->GetName());
	}
	return HitResult;
}

FVector UGrabber::GetReachLineEnd()
{
	//Get player view point
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT playerViewPointLocation,
		OUT playerViewPointRotation
	);
	//TODO Log Out to test
	/*UE_LOG(LogTemp, Warning, TEXT("Location: %s Rotation: %s"), *playerViewPointLocation.ToString(), *playerViewPointRotation.ToString());*/

	return playerViewPointLocation + playerViewPointRotation.Vector()*Reach;
}
FVector UGrabber::GetReachLineStart()
{
	//Get player view point
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT playerViewPointLocation,
		OUT playerViewPointRotation
	);
	//TODO Log Out to test
	/*UE_LOG(LogTemp, Warning, TEXT("Location: %s Rotation: %s"), *playerViewPointLocation.ToString(), *playerViewPointRotation.ToString());*/

	return playerViewPointLocation;
}