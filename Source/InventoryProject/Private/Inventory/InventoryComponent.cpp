﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryProject/Public/Inventory/InventoryComponent.h"
#include "Inventory/PickUp/Item.h"
#include "Inventory/Widgets/OutsideItemWidget.h"
#include "Kismet/GameplayStatics.h"


UInventoryComponent::UInventoryComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
	
}

void UInventoryComponent::GetAllItem()
{
	TArray<AActor*> AllItems;
	UGameplayStatics::GetAllActorsOfClass(GetWorld() , AItem::StaticClass() , AllItems);
	
	for(auto i : AllItems)
	{
		if(FVector::Dist(i -> GetActorLocation() , GetOwner() -> GetActorLocation()) <= PickUpDistance)
		{
			AddItemInDistance(Cast<AItem>(i));
			Cast<AItem>(i) -> SetWidgetVisibility(true);
		}
		else
		{
			DeleteItemNotInDistance(Cast<AItem>(i));
			Cast<AItem>(i) -> SetWidgetVisibility(false);

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
	
	for(auto i : AllInDistanceItems)
	{
		FOutsideItem NewOutsideItem;
		NewOutsideItem.ItemDetails = i -> GetItemDetails();
		NewOutsideItem.Widget = nullptr;
		OutsideItemBox.Add(NewOutsideItem);

		
		UE_LOG(LogTemp , Error , TEXT("Add Item -> %s") , *i ->GetName());
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

