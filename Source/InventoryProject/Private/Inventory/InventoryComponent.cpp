﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryProject/Public/Inventory/InventoryComponent.h"


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

		UE_LOG(LogTemp , Error , TEXT("Add Item -> %s  Num %d") , *AllInDistanceItems[i] ->GetName() , AllInDistanceItems.Num());
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
		GetWorld() ->GetGameViewport() -> SetMouseCaptureMode(EMouseCaptureMode::NoCapture);
	}	
}

void UInventoryComponent::AddToInsideBox(FItemDetails Detail)
{
	if(CheckAlreadyInInventory(Detail)) return;
	UE_LOG(LogTemp , Error , TEXT("调用添加到背包"))
	if(FindIndexCouldSaveItem() == -1)
	{
		return;
	}
	UE_LOG(LogTemp , Error , TEXT("背包位置 %d") , FindIndexCouldSaveItem())
	InsideItemBox[FindIndexCouldSaveItem()].ItemDetails = Detail;
	
	InventoryWidget -> RefreshInsideItemBox(InsideItemBox);
	UpdateOutsideBox();
}

int32 UInventoryComponent::FindIndexCouldSaveItem()
{
	for(size_t i = 0 ; i < InsideItemBox.Num() ; i ++)
	{
		if(InsideItemBox[i].ItemDetails.ID == -1)
		{
			return i;
		}
	}

	return -1;
	
}


AItem* UInventoryComponent::FindCloseItem(int32 ItemID)
{
	double CloseDistance = PickUpDistance;
	AItem* CloseItem = nullptr;
	if(ItemID == -1)
	{
		for(size_t i = 0 ; i < AllInDistanceItems.Num() ; i ++)
		{
			if(FVector::Dist(AllInDistanceItems[i] -> GetActorLocation() , GetOwner() -> GetActorLocation()) < CloseDistance)
			{
				CloseDistance = FVector::Dist(AllInDistanceItems[i] -> GetActorLocation() , GetOwner() -> GetActorLocation());
				CloseItem = AllInDistanceItems[i];
			}
		}
	}
	else
	{
		for(size_t i = 0 ; i < AllInDistanceItems.Num() ; i ++)
		{
			if(FVector::Dist(AllInDistanceItems[i] -> GetActorLocation() , GetOwner() -> GetActorLocation()) < CloseDistance && AllInDistanceItems[i]->GetItemDetails().ID == ItemID)
			{
				CloseDistance = FVector::Dist(AllInDistanceItems[i] -> GetActorLocation() , GetOwner() -> GetActorLocation() );
				CloseItem = AllInDistanceItems[i];
			}
		}
	}
	
	return CloseItem;
}

void UInventoryComponent::PickUpByKey()
{
	if(FindCloseItem(-1) == nullptr) return;

	AItem* Item = FindCloseItem(-1);

	if(Item -> GetItemDetails().Num == 1)
	{
		AddToInsideBox(Item -> GetItemDetails());
		Item -> Destroy();

		DeleteItemNotInDistance(Item);
	}
	else if(Item -> GetItemDetails().Num > 1)
	{
		auto Temp = Item -> GetItemDetails();
		Temp.Num -= 1;
		Item -> SetItemDetails(Temp);
		
		Temp.Num = 1;
		AddToInsideBox(Temp);
		ConvertItemToOutsideItem();
	}
}

bool UInventoryComponent::CheckAlreadyInInventory(FItemDetails Details)
{
	for(size_t i = 0 ; i < InsideItemBox.Num() ; i ++)
	{
		if(InsideItemBox[i].ItemDetails.ID == Details.ID)
		{
			InsideItemBox[i].ItemDetails.Num += Details.Num;
			InventoryWidget -> RefreshInsideItemBox(InsideItemBox);
			return true;
		}
	}
	return false;
}

void UInventoryComponent::PickUpByWidget(UOutsideItemWidget* OutsideItemWidget)
{
	int32 ID = 0;
	for(size_t i = 0 ; i < OutsideItemBox.Num() ; i ++)
	{
		if(OutsideItemBox[i].Widget == OutsideItemWidget)
		{
			ID = OutsideItemBox[i].ItemDetails.ID;
		}
	}
	
	while(FindCloseItem(ID) != nullptr)
	{
		AddToInsideBox(FindCloseItem(ID) -> GetItemDetails());
		FindCloseItem(ID) -> Destroy();
		DeleteItemNotInDistance(FindCloseItem(ID));
	}
	
}

void UInventoryComponent::SwitchTwoItemDetail(UInsideItemWidget* OriginalWidget, UInsideItemWidget* TargetWidget)
{
	int32 IndexOfOriginalWidget;
	int32 IndexOfTargetWidget;

	for(size_t i = 0 ; i < InsideItemBox.Num() ; i ++)
	{
		if(InsideItemBox[i].Widget == OriginalWidget)
		{
			IndexOfOriginalWidget = i;
		}
		if(InsideItemBox[i].Widget == TargetWidget)
		{
			IndexOfTargetWidget = i;
		}
	}

	//交换后刷新
	auto Temp = InsideItemBox[IndexOfOriginalWidget];

	InsideItemBox[IndexOfOriginalWidget].ItemDetails = InsideItemBox[IndexOfTargetWidget].ItemDetails;
	InsideItemBox[IndexOfTargetWidget].ItemDetails = Temp.ItemDetails;

	InventoryWidget -> RefreshInsideItemBox(InsideItemBox);
}

void UInventoryComponent::DropThisItem(UInsideItemWidget* Widget)
{
	for(size_t i = 0 ; i < InsideItemBox.Num() ; i ++)
	{
		if(Widget == InsideItemBox[i].Widget)
		{
			SpawnItem(InsideItemBox[i].ItemDetails);
			InsideItemBox[i].ItemDetails.ID = -1;
			InventoryWidget -> RefreshInsideItemBox(InsideItemBox);
		}
	}
}

void UInventoryComponent::SpawnItem(FItemDetails Details)
{
	AItem* DropItem =  GetWorld() -> SpawnActor<AItem>(AItem::StaticClass() , GetOwner() -> GetActorTransform());
	DropItem -> SetItemDetails(Details);
	DropItem -> GetMeshComponent() -> SetSimulatePhysics(true);
}

FItemDetails UInventoryComponent::GetItemDetails(UInsideItemWidget* Widget)
{
	FItemDetails Empty;
	
	for(size_t i = 0 ; i < InsideItemBox.Num() ; i ++)
	{
		if(InsideItemBox[i].Widget == Widget)
		{
			return InsideItemBox[i].ItemDetails;
		}
	}

	return Empty; 
}

void UInventoryComponent::SetItemDetails(UInsideItemWidget* Widget, FItemDetails NewItemDetails)
{
	for(size_t i = 0 ; i < InsideItemBox.Num() ; i ++)
	{
		if(InsideItemBox[i].Widget == Widget)
		{
			InsideItemBox[i].ItemDetails = NewItemDetails;
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
	InventoryWidget -> TargetWidgetFollowMouse();
}

