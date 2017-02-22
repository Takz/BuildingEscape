// Copyright Tarik Awad 2017

#include "BuildingEscape.h"
#include "OpenDoor.h"
#include "Grabber.h"

#define OUT
// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();

	
}

void UOpenDoor::OpenDoor()
{
	//Set the door rotation
	Owner->SetActorRotation(FRotator(0.0f, openAngle, 0.0f));
}

void UOpenDoor::CloseDoor()
{
	//Set the door rotation
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll the trigger volume
	if (GetTotalMassOnActorsOnPlate() > 25.f)//TODO make into a parameter
	{

		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();

	}

	//Check if time to close door
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay) {
		CloseDoor();
	}

}

float UOpenDoor::GetTotalMassOnActorsOnPlate() {

	float TotalMass = 0.f;

	//Find overlapping Actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	//Iterate through them adding their masses
	for (auto* Actor : OverlappingActors)
	{		
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("This is %s"), *Actor->GetName() );

	}

	return TotalMass;
}


