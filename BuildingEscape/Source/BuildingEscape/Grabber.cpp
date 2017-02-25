// Copyright Tarik Awad 2017

#include "BuildingEscape.h"
#include "Grabber.h"

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

	FindAttachedPhysicsHandle();

	FindAttachedInputComponent();
}

void UGrabber::FindAttachedInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Input Component attached"));

		//Bind the input action
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
		UE_LOG(LogTemp, Error, TEXT("%s is missing Input Component"), *GetOwner()->GetName());
}

void UGrabber::FindAttachedPhysicsHandle()
{
	///Look for attached Physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) { return; }
	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("%s is missing Physics Handle"), *GetOwner()->GetName());
	}		
}


void UGrabber::Grab() {

	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();// The mesh that we are movings
	auto ActorHit = HitResult.GetActor();

	//If hit attach a physics handle
	if (ActorHit) {
		PhysicsHandle->GrabComponentAtLocationWithRotation(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), ComponentToGrab->GetOwner()->GetActorRotation());
	}
}

void UGrabber::Release() {
	//Release grabbed component
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	if (!PhysicsHandle) { return; }

	//If physics handle attached
	if (PhysicsHandle->GrabbedComponent) {
		//move the object we're holding
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	///Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	/// Ray-cast out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters

	);

	return Hit;
}

FVector UGrabber::GetReachLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	FVector LineTraceDirection = PlayerViewPointRotation.Vector();
	return PlayerViewPointLocation + LineTraceDirection * Reach;
}

