// Copyright Kamil Kołodziejczak 2021


#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

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

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle = CurrentYaw + OpenAngle;

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component on it, but no pressureplate set!"), *GetOwner()->GetHumanReadableName());
	}

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() >= DoorLastOpened + DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float& DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, OpenAngle, DeltaTime * DoorOpenSpeed);

	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	CurrentRotation.Yaw = CurrentYaw;

	GetOwner()->SetActorRotation(CurrentRotation);
}

void UOpenDoor::CloseDoor(float& DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * DoorCloseSpeed);

	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	CurrentRotation.Yaw = CurrentYaw;

	GetOwner()->SetActorRotation(CurrentRotation);
}

