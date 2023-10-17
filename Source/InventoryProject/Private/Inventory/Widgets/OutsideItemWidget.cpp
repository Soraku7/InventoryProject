// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryProject/Public/Inventory/Widgets/OutsideItemWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Inventory/InventoryComponent.h"

void UOutsideItemWidget::Init()
{
	this -> ItemIcon = Cast<UImage>(this -> GetWidgetFromName("ItemIcon"));
	this -> ItemName = Cast<UTextBlock>(this -> GetWidgetFromName("ItemName"));
	this -> ItemDescription = Cast<UTextBlock>(this -> GetWidgetFromName("ItemDescription"));
	this -> ItemNum = Cast<UTextBlock>(this -> GetWidgetFromName("ItemNum"));
}

void UOutsideItemWidget::SetWidgetStyle(FItemDetails& Details)
{
	this -> ItemIcon -> SetBrushResourceObject(Details.ItemIcon);
	this -> ItemName -> SetText(Details.ItemName);
	this -> ItemDescription -> SetText(Details.ItemDescription);
	this -> ItemNum -> SetText(FText::FromString(FString::FormatAsNumber(Details.Num)));
}
