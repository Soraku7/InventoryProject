
#include "InventoryProject\Public\Inventory\Widgets\InventoryWidget.h"

#include "Components/ScrollBox.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/Widgets/OutsideItemWidget.h"

void UInventoryWidget::Init()
{
	ScrollBox = Cast<UScrollBox>(this -> GetWidgetFromName("OutsideItemScrollBox"));
	
}

void UInventoryWidget::CreateNewOutsideItemWidget(FOutsideItem& OutsideItem)
{
	auto OutsideWidget = Cast<UOutsideItemWidget>(CreateWidget(ScrollBox , OutsideWidgetsClass));

	OutsideItem.Widget = OutsideWidget;

	ScrollBox -> AddChild(OutsideWidget); 
}

void UInventoryWidget::ClearWidgetNotInBox()
{
	auto Widgets = ScrollBox -> GetAllChildren();
	
	for(size_t i = 0; i < Widgets.Num() ; i ++)
	{

		Widgets[i]->ConditionalBeginDestroy();
		Widgets.RemoveAt(i);
	}

	ScrollBox->ClearChildren();
	
}
