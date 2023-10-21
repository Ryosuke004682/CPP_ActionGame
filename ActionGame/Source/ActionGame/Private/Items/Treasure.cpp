// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"
#include "Character/PlayerController_Core.h"
#include "Kismet/GameplayStatics.h"

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerController_Core* PlayerController = Cast<APlayerController_Core>(OtherActor);

	if (PlayerController)
	{
		if (GetItemSound)
		{
			UGameplayStatics::PlaySoundAtLocation
			(
				this,
				GetItemSound,
				GetActorLocation()
			);
		}
		Destroy();
	}

}
