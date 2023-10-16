// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

struct FOutsideItem;
class UOutsideItemWidget;
class UScrollBox;
UCLASS()
class INVENTORYPROJECT_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init();
	
	void CreateNewOutsideItemWidget(FOutsideItem& OutsideItem);

	void ClearWidgetNotInBox();
protected:
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	TSubclassOf<UOutsideItemWidget> OutsideWidgetsClass;

private:
	UScrollBox* ScrollBox;
};
