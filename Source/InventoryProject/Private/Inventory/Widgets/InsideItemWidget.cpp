// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Widgets/InsideItemWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Inventory/PickUp/Item.h"

void UInsideItemWidget::Init()
{
	ItemIcon = Cast<UImage>(this -> GetWidgetFromName("ItemIcon"));
	ItemNum = Cast<UTextBlock>(this -> GetWidgetFromName("ItemNum"));
	ItemButton = Cast<UButton>(this -> GetWidgetFromName("ItemButton"));
}

void UInsideItemWidget::SetWidgetStyle(FItemDetails& Details)
{
	if(Details.ID == -1) return;
	this -> ItemIcon -> SetBrushResourceObject(Details.ItemIcon);
	this -> ItemNum -> SetText(FText::FromString(FString::FormatAsNumber(Details.Num)));
	this -> ItemButton -> SetToolTipText(Details.ItemName);
}
