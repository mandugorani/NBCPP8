// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"

ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->SetupAttachment(Scene);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnItemOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnItemEndOverlap);

	PickupParticle = nullptr;
	PickupSound = nullptr;
}

void ABaseItem::OnItemOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Item Overlap")));
		ActivateItem(OtherActor);
	}
}

void ABaseItem::OnItemEndOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
}

void ABaseItem::ActivateItem(AActor* Activator)
{
	TWeakObjectPtr<UParticleSystemComponent> WeakParticle = nullptr;

	if (PickupParticle)
	{
		WeakParticle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			PickupParticle,
			GetActorLocation(),
			GetActorRotation(),
			true
		);
	}
	TWeakObjectPtr<UAudioComponent> WeakPickupAudio = nullptr;

	if (PickupSound)
	{
		WeakPickupAudio = UGameplayStatics::SpawnSoundAtLocation(
			GetWorld(),
			PickupSound,
			GetActorLocation()
		);

		if (!WeakPickupAudio.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("spawn PickupAudio Fail"));
		}
	}else{
		UE_LOG(LogTemp, Warning, TEXT("PickupSound null"));
	}

	if (WeakParticle.IsValid())
	{
		FTimerHandle DestroyParticleTimerHandle;

		GetWorld()->GetTimerManager().SetTimer(
			DestroyParticleTimerHandle,
			[WeakParticle]()
			{
				if (WeakParticle.IsValid())
				{
					WeakParticle->DestroyComponent();
				}
			},
			2.0f,
			false
		);
	}
	if (WeakPickupAudio.IsValid())
	{
		FTimerHandle StopSoundTimerHandle;

		GetWorld()->GetTimerManager().SetTimer(
			StopSoundTimerHandle,
			[WeakPickupAudio]()
			{
				if (WeakPickupAudio.IsValid())
				{
					WeakPickupAudio->Stop();
					WeakPickupAudio->DestroyComponent();
				}
			},
			1.0f,
			false
		);
	}
}

FName ABaseItem::GetItemType() const
{
	return ItemType;
}


void ABaseItem::DestroyItem()
{
	Destroy();
}