// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Widgets/MenuBarWidget.h"

#include "Components/TextBlock.h"
#include "Inventory/InventoryComponent.h"

void UMenuBarWidget::SetInventoryComponent(UInventoryComponent* Component)
{
	InventoryComponent = Component;
}

void UMenuBarWidget::SetWidget(UInsideItemWidget* Widget)
{
	this -> HoveredWidget = Widget;
}

void UMenuBarWidget::Init()
{
	ItemName = Cast<UTextBlock>(GetWidgetFromName("ItemName"));
	
	auto Details =  InventoryComponent -> GetItemDetails(HoveredWidget);
	ItemName -> SetText(Details.ItemName);
}
