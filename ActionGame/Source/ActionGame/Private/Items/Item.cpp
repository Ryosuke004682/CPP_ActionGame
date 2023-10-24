// Fill out your copyright notice in the Description page of Project Settings.
#include "Items/Item.h"
#include "../../../../Intermediate/ProjectFiles/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Character/PlayerController_Core.h"
#include "NiagaraComponent.h"


AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;
	
	Sphere	 = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

	EmbersEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
	EmbersEffect->SetupAttachment(GetRootComponent());

}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this , &AItem::OnSphereEndOverlap);
}

float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItem::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//‹…‚Ì“–‚½‚è”»’è‚ğæ“¾B

	APlayerController_Core* PlayerController = Cast<APlayerController_Core>(OtherActor);
	if (PlayerController) { PlayerController->SetOverlappingItem(this); }
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//‹…‚Ì“–‚½‚è”»’è‚ğæ“¾‚µ‚½ŒãANull‚É•Ô‚·B

	APlayerController_Core* PlayerController = Cast<APlayerController_Core>(OtherActor);
	if (PlayerController) { PlayerController ->SetOverlappingItem(nullptr); }
}


void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunningTime += DeltaTime;
	
	if (ItemState == EItemState::EIS_Hovering)
	{
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
	}
}