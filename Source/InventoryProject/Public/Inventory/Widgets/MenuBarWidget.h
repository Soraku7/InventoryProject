// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuBarWidget.generated.h"

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
	void SetInventoryComponent(UInventoryComponent* Component);
	void SetWidget(UInsideItemWidget* Widget);
	void Init();
	
private:
	UTextBlock* ItemName;

	UInsideItemWidget* HoveredWidget;

	UInventoryComponent* InventoryComponent;
};
