#include "UStatAttributesComponent.h"

UStatAttributesComponent::UStatAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UStatAttributesComponent::BeginPlay()
{
	Super::BeginPlay();
}

float UStatAttributesComponent::CalculateDamage(UStatAttributesComponent* Other, float Damage) const
{
	return 0.0f;
}