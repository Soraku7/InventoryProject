// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuBarWidget.generated.h"

class UInventoryWidget;
class UEditableText;
class UInsideItemWidget;
class UInventoryComponent;
class UTextBlock;
/** 
 * 
 */
UCLASS()
class INVENTORYPROJECT_API UMenuBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitWidget(UInventoryComponent* Component , UInventoryWidget* Widget);
	void RefreshItemDetails(UInsideItemWidget* Widget);

	int GetNum();
	
	UFUNCTION(BlueprintCallable)
	void UseItem();
	
private:
	UTextBlock* ItemName;

	UInventoryComponent* InventoryComponent;

	UEditableText* NumText;
	
	UInsideItemWidget* TargetWidget;

	UInventoryWidget* InventoryWidget;
};
