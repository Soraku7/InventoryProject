﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryProject/Public/Inventory/InventoryComponent.h"

#include "Components/WidgetComponent.h"
#include "Inventory/PickUp/Item.h"
#include "Inventory/Widgets/InventoryWidget.h"
#include "Inventory/Widgets/OutsideItemWidget.h"
#include "InventoryProject/InventoryProjectCharacter.h"
#include "Kismet/GameplayStatics.h"


UInventoryComponent::UInventoryComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
	
}

void UInventoryComponent::Init()
{
	InventoryWidget = Cast<UInventoryWidget>(Cast<AInventoryProjectCharacter>(GetOwner())-> GetWidgetComponent() -> GetWidget());
	InventoryWidget -> Init();
}

void UInventoryComponent::GetAllItem()
{
	TArray<AActor*> AllItems;
	UGameplayStatics::GetAllActorsOfClass(GetWorld() , AItem::StaticClass() , AllItems);
	
	for(size_t i = 0; i < AllItems.Num(); i++)
	{
		if(FVector::Dist(AllItems[i] -> GetActorLocation() , GetOwner() -> GetActorLocation()) <= PickUpDistance)
		{
			AddItemInDistance(Cast<AItem>(AllItems[i]));
			Cast<AItem>(AllItems[i]) -> SetWidgetVisibility(true);
		}
		else
		{
			DeleteItemNotInDistance(Cast<AItem>(AllItems[i]));
			Cast<AItem>(AllItems[i]) -> SetWidgetVisibility(false);

		}
	}
}

void UInventoryComponent::AddItemInDistance(AItem* Item)
{
	if(CheckIfAlreadyInArray(Item))
		return;

	AllInDistanceItems.Add(Item);
	ConvertItemToOutsideItem();
}

void UInventoryComponent::DeleteItemNotInDistance(AItem* Item)
{
	if(FindIndexOfInDistanceArray(Item) >= 0)
	{
		AllInDistanceItems.RemoveAt(FindIndexOfInDistanceArray(Item));
		ConvertItemToOutsideItem();
	}
}

bool UInventoryComponent::CheckIfAlreadyInArray(AItem* Item) 
{
	if(FindIndexOfInDistanceArray(Item) >=0)
	{
		return true;
	}
	return false;
}

int32 UInventoryComponent::FindIndexOfInDistanceArray(AItem* Item)
{
	for(size_t i = 0; i < AllInDistanceItems.Num() ; i ++)
	{
		if(AllInDistanceItems[i] == Item)
		{
			return i;
		}
	}

	return -1;
}

void UInventoryComponent::ConvertItemToOutsideItem()
{
	OutsideItemBox.Empty();
	
	for(size_t i = 0 ; i < AllInDistanceItems.Num() ; i ++)
	{
		FOutsideItem NewOutsideItem;
		NewOutsideItem.ItemDetails = AllInDistanceItems[i] -> GetItemDetails();
		NewOutsideItem.Widget = nullptr;
		OutsideItemBox.Add(NewOutsideItem);

		
		UE_LOG(LogTemp , Error , TEXT("Add Item -> %s") , *AllInDistanceItems[i] ->GetName());
	}

	UpdateOutsideBox();
}

void UInventoryComponent::UpdateOutsideBox()
{
	InventoryWidget -> ClearWidgetNotInBox();
	
	for(size_t i = 0 ; i < OutsideItemBox.Num() ; i ++)
	{
		if(OutsideItemBox[i].Widget == nullptr)
		{
			InventoryWidget -> CreateNewOutsideItemWidget(OutsideItemBox[i]);
		}
	}
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	
}


void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	GetAllItem();
}

