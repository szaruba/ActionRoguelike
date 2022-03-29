// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect.h"

#include "SActionComponent.h"

USActionEffect::USActionEffect()
{
	bAutoStart = true;
}

void USActionEffect::StartAction_Implementation(AActor* ActionInstigator)
{
	Super::StartAction_Implementation(ActionInstigator);

	if (Period > 0.f)
	{
		FTimerDelegate PeriodDelegate;
		PeriodDelegate.BindUObject(this, &USActionEffect::ExecutePeriodicEffect, ActionInstigator);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Period, PeriodDelegate, Period, true);	
	}

	if (Duration > 0.f)
	{
		FTimerDelegate DurationDelegate;
		DurationDelegate.BindUObject(this, &USActionEffect::StopAction, ActionInstigator);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Duration, DurationDelegate, Duration, false);
	}
}

void USActionEffect::StopAction_Implementation(AActor* ActionInstigator)
{
	// If the duration timer runs out just before the period, trigger the periodic effect one last time.
	if (GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle_Period) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(ActionInstigator);
	}
	
	Super::StopAction_Implementation(ActionInstigator);

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Period);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Duration);

	if (USActionComponent* OwningComp = GetOwningComponent())
	{
		OwningComp->RemoveAction(this);
	}
}

void USActionEffect::ExecutePeriodicEffect_Implementation(AActor* ActionInstigator)
{
	
}
