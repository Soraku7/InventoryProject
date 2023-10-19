
#include "InventoryProject\Public\Inventory\Widgets\InventoryWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ScrollBox.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/Widgets/InsideItemWidget.h"
#include "Inventory/Widgets/OutsideItemWidget.h"

void UInventoryWidget::Init()
{
	ScrollBox = Cast<UScrollBox>(this -> GetWidgetFromName("OutsideItemScrollBox"));
	CanvasPanel = Cast<UCanvasPanel>(this -> GetWidgetFromName("InsideItemCanvasPanel"));
}

void UInventoryWidget::CreateNewOutsideItemWidget(FOutsideItem& OutsideItem)
{
	auto OutsideWidget = Cast<UOutsideItemWidget>(CreateWidget(ScrollBox , OutsideWidgetsClass));

	OutsideWidget -> Init();
	OutsideWidget -> SetWidgetStyle(OutsideItem.ItemDetails);
	
	OutsideItem.Widget = OutsideWidget;

	ScrollBox -> AddChild(OutsideWidget);
}

void UInventoryWidget::ClearWidgetBox()
{
	auto Widgets = ScrollBox -> GetAllChildren();
	
	for(size_t i = 0; i < Widgets.Num() ; i ++)
	{

		Widgets[i]->ConditionalBeginDestroy();
		Widgets.RemoveAt(i);
	}

	ScrollBox->ClearChildren();
	
}

void UInventoryWidget::InitInsideItemBox(FVector2D InventoryMaxStorage ,  TArray<FInsideItem>& InsideItemBox)
{
	FVector2D SlotPosition = InsideBoxBeginPoint;
	for(auto &i : InsideItemBox)
	{
		auto InsideWidget = Cast<UInsideItemWidget>(CreateWidget(CanvasPanel , InsideWidgetClass));

		i.Widget = InsideWidget;
		CanvasPanel->AddChild(InsideWidget);
		
		Cast<UCanvasPanelSlot>(InsideWidget -> Slot) -> SetPosition(SlotPosition);
		if(SlotPosition.X - InsideBoxBeginPoint.X + InsideItemBoxSize.X >= InventoryMaxStorage.X * InsideItemBoxSize.X)
		{
			SlotPosition.Y += InsideItemBoxSize.Y;
			SlotPosition.X = InsideBoxBeginPoint.X;
		}
		else
		{
			SlotPosition.X += InsideItemBoxSize.X;
		}
	}
	
	
}
