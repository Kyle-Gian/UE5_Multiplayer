// Fill out your copyright notice in the Description page of Project Settings.


#include "OverheadWidget.h"

#include "Components/TextBlock.h"

void UOverheadWidget::SetDisplayText(FString a_textToDisplay)
{
	if (DisplayText)
	{
		DisplayText->SetText(FText::FromString(a_textToDisplay));
	}
}

void UOverheadWidget::ShowPlayerNetRole(APawn* a_inPawn)
{
	ENetRole localRole = a_inPawn->GetRemoteRole();
	FString role;

	switch (localRole)
	{
	case ENetRole::ROLE_Authority:
		role = FString("Authority");
		break;
	case ENetRole::ROLE_SimulatedProxy:
		role = FString("Simulated Proxy");
		break;
	case ENetRole::ROLE_AutonomousProxy:
		role = FString("Autonomous Proxy");
		break;
	case ENetRole::ROLE_None:
		role = FString("None");
		break;
	}

	FString RemoteRoleString = FString::Printf(TEXT("RemoteRole: %s"), *role);
	SetDisplayText(RemoteRoleString);
}

void UOverheadWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	RemoveFromParent();
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}
