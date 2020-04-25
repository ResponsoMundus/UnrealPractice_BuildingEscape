// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"

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

	World = GetWorld();
	Owner = GetOwner();

	FindAndSetPhysicsHandle();
	FindAndSetInputComponent();

	RefreshTransform();
}

void UGrabber::Grab() {
	// UE_LOG(LogTemp, Warning, TEXT("Grabber Pressed!"));

	RefreshTransform();
	FHitResult HitResult = GetHitResult();

	if (PhyscisHandle && HitResult.GetActor()) {
		PhyscisHandle->GrabComponentAtLocation(HitResult.GetComponent(), NAME_None, LineTraceEnd);
	}
}

void UGrabber::Release() {
	// UE_LOG(LogTemp, Warning, TEXT("Grabber Released!"));

	if (PhyscisHandle && PhyscisHandle->GrabbedComponent) {
		PhyscisHandle->ReleaseComponent();
	}
}

void UGrabber::FindAndSetPhysicsHandle() {
	PhyscisHandle = Owner->FindComponentByClass<UPhysicsHandleComponent>();

	if (!PhyscisHandle) {
		UE_LOG(LogTemp, Error, TEXT("Physcis Handle NOT Found on %s"), *Owner->GetName());
	}
}

void UGrabber::FindAndSetInputComponent() {
	InputComponent = Owner->FindComponentByClass<UInputComponent>();

	InputComponent->BindAction(TEXT("Grab"), IE_Pressed, this, &UGrabber::Grab);
	InputComponent->BindAction(TEXT("Grab"), IE_Released, this, &UGrabber::Release);
}

FHitResult UGrabber::GetHitResult() const {
	// DrawDebugLine(
	// 	World,
	// 	PlayerViewPointLocation,
	// 	LineTraceEnd,
	// 	FColor(16, 240, 16),
	// 	false,
	// 	0.f,
	// 	0,
	// 	0.5f
	// );

	FHitResult Hit;
	FCollisionQueryParams TraceParams(
		FName(TEXT("")),
		false,
		Owner
	);

	World->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	return Hit;
	
	// AActor* ActorHit = Hit.GetActor();
	// if (ActorHit) {
	// 	UE_LOG(LogTemp, Warning, TEXT("Traced Item: %s"), *ActorHit->GetName());
	// }

	// return ActorHit;
}

void UGrabber::RefreshTransform() {
	World->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	
	LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * TraceLineLength;
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhyscisHandle && PhyscisHandle->GrabbedComponent) {
		RefreshTransform();
		PhyscisHandle->SetTargetLocation(LineTraceEnd);
	}
}
