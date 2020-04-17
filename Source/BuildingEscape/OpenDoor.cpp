// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "GameFramework/Actor.h"

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
	TargetYaw += Owner->GetActorRotation().Yaw;

	if (!PressurePlate) {
		UE_LOG(LogTemp, Warning, TEXT("The PressurePlate in OpenDoor component of %s has not been set."), *Owner->GetName());
	}
	if (!Actor) {
		UE_LOG(LogTemp, Warning, TEXT("The Actor in OpenDoor component of %s has not been set."), *Owner->GetName());
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate && Actor && PressurePlate->IsOverlappingActor(Actor)) {
		OpenDoor(DeltaTime);
	}
}

void UOpenDoor::OpenDoor(float DeltaTime) {
	FRotator OpenDoor(Owner->GetActorRotation());
	OpenDoor.Yaw = FMath::Lerp(OpenDoor.Yaw, TargetYaw, DeltaTime * 1.f);
	Owner->SetActorRotation(OpenDoor);
}
