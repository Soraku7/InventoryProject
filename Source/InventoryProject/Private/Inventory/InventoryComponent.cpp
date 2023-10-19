// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryProject/Public/Inventory/InventoryComponent.h"

#include "IDetailTreeNode.h"
#include "PropertyEditorModule.h"
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

	FItemDetails NewDetail;
	FInsideItem NewInsideItem;

	NewInsideItem.ItemDetails = NewDetail;

	for(size_t i = 0 ; i < InventoryMaxStorage.X * InventoryMaxStorage.Y ; i ++)
	{
		InsideItemBox.Add(NewInsideItem);
	}

	InventoryWidget -> InitInsideItemBox(InventoryMaxStorage , InsideItemBox);
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
		if(LeachSameIDItem(AllInDistanceItems[i] -> GetItemDetails()))
		{
		}
		else
		{
			FOutsideItem NewOutsideItem;
			NewOutsideItem.ItemDetails = AllInDistanceItems[i] -> GetItemDetails();
			NewOutsideItem.Widget = nullptr;
			OutsideItemBox.Add(NewOutsideItem);
		}

		UE_LOG(LogTemp , Error , TEXT("Add Item -> %s") , *AllInDistanceItems[i] ->GetName());
	}

	UpdateOutsideBox();
}

void UInventoryComponent::UpdateOutsideBox()
{
	InventoryWidget -> ClearWidgetBox();
	
	for(size_t i = 0 ; i < OutsideItemBox.Num() ; i ++)
	{
		if(OutsideItemBox[i].Widget == nullptr)
		{
			InventoryWidget -> CreateNewOutsideItemWidget(OutsideItemBox[i]);
			UE_LOG(LogTemp , Error , TEXT("ItemDetails.Num -> %d") , OutsideItemBox[i].ItemDetails.Num)
		}
	}
}

bool UInventoryComponent::LeachSameIDItem(FItemDetails ItemDetail)
{
	
	for(size_t i = 0 ; i < OutsideItemBox.Num() ; i ++)
	{
		if(OutsideItemBox[i].ItemDetails.ID == ItemDetail.ID)
		{
			OutsideItemBox[i].ItemDetails.Num += ItemDetail.Num;
			return true;
		}
	}

	return false;
}

void UInventoryComponent::ToggleInventoryWidget()
{
	auto WidgetComponent = Cast<AInventoryProjectCharacter>(GetOwner()) -> GetWidgetComponent();
	if(WidgetComponent->IsVisible())
	{
		WidgetComponent -> SetVisibility(false);
		Cast<APlayerController>(Cast<AInventoryProjectCharacter>(GetOwner())
			-> GetController()) -> SetInputMode(FInputModeGameOnly());
		Cast<APlayerController>(Cast<AInventoryProjectCharacter>
			(GetOwner()) -> GetController()) ->SetShowMouseCursor(false);
	}
	else
	{
		WidgetComponent ->SetVisibility(true);
		Cast<APlayerController>(Cast<AInventoryProjectCharacter>(GetOwner())
			-> GetController()) -> SetInputMode(FInputModeGameAndUI());
		Cast<APlayerController>(Cast<AInventoryProjectCharacter>
			(GetOwner()) -> GetController()) ->SetShowMouseCursor(true);
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

