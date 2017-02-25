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

	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate"), *GetOwner()->GetName());
	}
	
}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll the trigger volume
	if (GetTotalMassOnActorsOnPlate() > TriggerMass)//TODO make into a parameter
	{
		OnOpen.Broadcast();
	}
	else
	{
		OnClose.Broadcast();
	}

}

float UOpenDoor::GetTotalMassOnActorsOnPlate() {

	float TotalMass = 0.f;

	//Find overlapping Actors
	TArray<AActor*> OverlappingActors;

	//If no reference to pointer Trigger Mass, return
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	//Iterate through them adding their masses
	for (auto* Actor : OverlappingActors)
	{		
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("This is %s"), *Actor->GetName() );
	}

	return TotalMass;
}


