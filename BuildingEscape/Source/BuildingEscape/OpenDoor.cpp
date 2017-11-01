// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
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
		UE_LOG(LogTemp, Error, TEXT("%s missing PressurePlate Trigger Volume"), *(Owner->GetName()));
	}
}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (GetTotalMassOfActorsOnPlate() > TriggerMass)
		OnOpen.Broadcast();
	else
		OnClose.Broadcast();
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float totalMass = 0.0f;
	
	TArray<AActor*> overLappingActors;
	if(!PressurePlate){return 0.0f;}
	PressurePlate->GetOverlappingActors(OUT overLappingActors);

	for (auto& actor : overLappingActors)
	{
		totalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}


	return totalMass;
}

