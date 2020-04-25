// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

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

	World = GetWorld();
	Owner = GetOwner();

	// Actor = World->GetFirstPlayerController()->GetPawn();

	InitalYaw = Owner->GetActorRotation().Yaw;
	TargetYaw += InitalYaw;

	if (!PressurePlate) {
		UE_LOG(LogTemp, Warning, TEXT("The PressurePlate in OpenDoor component of %s has not been set."), *Owner->GetName());
	}

	FindAudioComponent();
	// if (!Actor) {
	// 	UE_LOG(LogTemp, Warning, TEXT("The Actor in OpenDoor component of %s has not been set."), *Owner->GetName());
	// }
}

float UOpenDoor::TotalMassOnPressurePlate() const {
	float TotalMass = 0.f;

	TArray<AActor*> ActorList;
	if (PressurePlate) {
		PressurePlate->GetOverlappingActors(OUT ActorList);

		for(AActor* Actor : ActorList) {
			TotalMass += Actor->FindComponentByClass<UMeshComponent>()->GetMass();
		}
	}

	return TotalMass;
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOnPressurePlate() > Threshold) {
		OpenDoor(DeltaTime);
		LastOpenTime = World->GetTimeSeconds();
	} else if (World->GetTimeSeconds() - LastOpenTime > DoorCloseDelay) {
		CloseDoor(DeltaTime);
	}
}

void UOpenDoor::OpenDoor(float DeltaTime) {
	FRotator OpenDoor(Owner->GetActorRotation());
	OpenDoor.Yaw = FMath::Lerp(OpenDoor.Yaw, TargetYaw, DeltaTime * DoorOpenSpeed);
	Owner->SetActorRotation(OpenDoor);

	if (!DoorOpened && AudioComponent) {
		AudioComponent->Play();
		DoorOpened = true;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime) {
	FRotator CloseDoor(Owner->GetActorRotation());
	CloseDoor.Yaw = FMath::Lerp(CloseDoor.Yaw, InitalYaw, DeltaTime * DoorCloseSpeed);
	Owner->SetActorRotation(CloseDoor);

	if (DoorOpened && AudioComponent) {
		AudioComponent->Play();
		DoorOpened = false;
	}
}

void UOpenDoor::FindAudioComponent() {
	AudioComponent = Owner->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent) {
		UE_LOG(LogTemp, Error, TEXT("Audio Component NOT Found on %s"), *Owner->GetName());
	}
}
