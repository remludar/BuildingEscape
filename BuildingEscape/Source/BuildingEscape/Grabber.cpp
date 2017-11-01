// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"


#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	SetupInputComponent();
}
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		FString OwnerName = GetOwner()->GetName();
		UE_LOG(LogTemp, Error, TEXT("%s missing PhysicsHandle"), *OwnerName);
	}
}
void UGrabber::SetupInputComponent()
{
	///Look for attached Input Component at runtime
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!InputComponent)
	{
		FString OwnerName = GetOwner()->GetName();
		UE_LOG(LogTemp, Error, TEXT("%s missing InputComponent"), *OwnerName);
	}

	InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
	InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	GetReachLineEnd();
	if(!PhysicsHandle) {return;}
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
	
}
FVector UGrabber::GetReachLineEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);

}

void UGrabber::Grab()
{
	FHitResult hitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* componentToGrab = hitResult.GetComponent();
	AActor* actorHit = hitResult.GetActor();
	if (actorHit)
	{
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponent(
			componentToGrab,
			NAME_None,
			componentToGrab->GetOwner()->GetActorLocation(),
			true);
	}
}
FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Raycast (AKA Line trace) out to a distance
	FHitResult Hit;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	return Hit;
}

void UGrabber::Release()
{
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

FVector UGrabber::GetReachLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}

