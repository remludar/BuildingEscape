// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGrabber();
	virtual void BeginPlay() override;
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;	

private:
	float Reach = 100.0f;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	void Grab();
	void Release();
	void FindPhysicsHandleComponent();
	void SetupInputComponent();
	FHitResult GetFirstPhysicsBodyInReach();
	FVector GetReachLineStart();
	FVector GetReachLineEnd();
};
