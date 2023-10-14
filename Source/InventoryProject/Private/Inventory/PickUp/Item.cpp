#include "InventoryProject/Public/Inventory/PickUp/Item.h"
#include "Components/WidgetComponent.h"


AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));

	SetRootComponent(MeshComponent);
	WidgetComponent -> SetupAttachment(MeshComponent);
}

void AItem::SetItemDetails(FItemDetails NewDetails)
{
	this -> ID = NewDetails.ID;
	this -> Num = NewDetails.Num;
	this -> ItemName = NewDetails.ItemName;
	this -> ItemDescription = NewDetails.ItemDescription;
	this -> ItemIcon = NewDetails.ItemIcon;
	MeshComponent -> SetStaticMesh(NewDetails.MeshComponent);
}

FItemDetails AItem::GetItemDetails()
{
	FItemDetails SelfDetails;

	SelfDetails.ID = this -> ID;
	SelfDetails.Num = this -> Num;
	SelfDetails.ItemName = this -> ItemName;
	SelfDetails.ItemDescription = this -> ItemDescription;
	SelfDetails.ItemIcon = this -> ItemIcon;
	SelfDetails.MeshComponent = MeshComponent -> GetStaticMesh();
	
	return SelfDetails;
}

void AItem::SetWidgetVisibility(bool NewValue)
{
	WidgetComponent -> SetVisibility(NewValue);
}

 

