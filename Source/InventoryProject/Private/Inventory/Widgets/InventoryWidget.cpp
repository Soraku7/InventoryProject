
#include "InventoryProject\Public\Inventory\Widgets\InventoryWidget.h"

#include "PropertyEditorModule.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/Widgets/InsideItemWidget.h"
#include "Inventory/Widgets/OutsideItemWidget.h"
#include "Inventory/Widgets/MenuBarWidget.h"
#include "InventoryProject/InventoryProjectCharacter.h"

void UInventoryWidget::Init()
{
	ScrollBox = Cast<UScrollBox>(this -> GetWidgetFromName("OutsideItemScrollBox"));
	CanvasPanel = Cast<UCanvasPanel>(this -> GetWidgetFromName("InsideItemCanvasPanel"));

	DropButton = Cast<UButton>(this -> GetWidgetFromName("DropButton"));
	DropButton -> SetVisibility(ESlateVisibility::Hidden);

	MenuBarWidget = Cast<UMenuBarWidget>(this -> GetWidgetFromName("MenuBar"));
	MenuBarWidget -> SetVisibility(ESlateVisibility::Hidden);
	
	InventoryComp = Cast<AInventoryProjectCharacter>(GetOwningPlayer() -> GetCharacter()) -> GetInventoryComponent();

	MenuBarWidget -> InitWidget(InventoryComp , this);
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
		InsideWidget -> SetInventoryWidget(this);
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

void UInventoryWidget::RefreshInsideItemBox(TArray<FInsideItem>& InsideItemBox)
{
	for(size_t i = 0 ; i < InsideItemBox.Num() ; i ++)
	{
		InsideItemBox[i].Widget -> Init();
		InsideItemBox[i].Widget -> SetWidgetStyle(InsideItemBox[i].ItemDetails);
	}
}

void UInventoryWidget::SetTargetWidget(UInsideItemWidget* Widget)
{
	TargetWidget = Widget;

	bIsHadWidgetFollowMouse = true;
}

void UInventoryWidget::TargetWidgetFollowMouse()
{
	if(TargetWidget == nullptr) return;
	
	auto NowMousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	Cast<UCanvasPanelSlot>(TargetWidget -> Slot) -> SetPosition(NowMousePosition - TargetWidget -> GetWhenPressedMousePosition() + TargetWidget -> GetSelfPosition());

	DropButton -> SetVisibility(ESlateVisibility::Visible);
}

void UInventoryWidget::ResetTargetWidget()
{
	if(TargetWidget == nullptr) return;
	TargetWidget -> SetVisibility(ESlateVisibility::Visible);
	Cast<UCanvasPanelSlot>(TargetWidget -> Slot) -> SetPosition(TargetWidget -> GetSelfPosition());
	Cast<UCanvasPanelSlot>(TargetWidget -> Slot) -> SetZOrder(0);

	TargetWidget = nullptr;
	bIsHadWidgetFollowMouse = false;

	DropButton -> SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryWidget::SwitchToWidget(UInsideItemWidget* InsideWidget)
{
	if(TargetWidget && InsideWidget)
	{
		InventoryComp -> SwitchTwoItemDetail(TargetWidget , InsideWidget);
		ResetTargetWidget();
	}

	
}

void UInventoryWidget::DropThisItem()
{
	InventoryComp -> DropThisItem(TargetWidget);
}

void UInventoryWidget::SetHoverWidget(UInsideItemWidget* Widget)
{
	HoverWidget = Widget;
	
	MenuBarWidget -> SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryWidget::ShowMenuBar()
{
	if(!HoverWidget)
	{
		return;
	}

	if(MenuBarWidget -> IsVisible())
	{
		MenuBarWidget -> SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	
	auto NowMousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	Cast<UCanvasPanelSlot>(MenuBarWidget -> Slot) -> SetPosition(NowMousePosition -FVector2D(960 , 540));
	MenuBarWidget -> SetVisibility(ESlateVisibility::Visible);
	
	MenuBarWidget -> RefreshItemDetails(HoverWidget);
}
