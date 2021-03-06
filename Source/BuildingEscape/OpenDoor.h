// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "Components/AudioComponent.h"

#include "OpenDoor.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOnPressurePlate() const;
	void FindAudioComponent();

private:

	UWorld *World = nullptr;
	AActor *Owner = nullptr;
	float InitalYaw = 0.f;
	float LastOpenTime = 0.f;
//   float CurrentYaw = 0.f;
	bool DoorOpened = false;
	
	UPROPERTY(EditAnywhere)
	float TargetYaw = 90.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 2.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 3.f;
	
	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 1.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;

	// UPROPERTY(EditAnywhere)
	// AActor* Actor;

	UPROPERTY(EditAnywhere)
	float Threshold = 50.f;

};
