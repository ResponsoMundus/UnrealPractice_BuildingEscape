// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	AActor* Owner;
	UWorld* World;

	UPROPERTY(EditAnywhere)
	float TraceLineLength = 100.f;
	
	UPhysicsHandleComponent* PhyscisHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	void Grab();
	void Release();
	void FindAndSetPhysicsHandle();
	void FindAndSetInputComponent();
	AActor* GetHitActor() const;
};
