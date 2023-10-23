// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OutsideItemWidget.generated.h"

struct FItemDetails;
class UImage;
class UTextBlock;

UCLASS()
class INVENTORYPROJECT_API UOutsideItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init();
	
	void SetWidgetStyle(FItemDetails& Details);

	UFUNCTION(BlueprintCallable)
	void WhenClick();
protected:
	UImage* ItemIcon;
	UTextBlock* ItemName;
	UTextBlock* ItemDescription;
	UTextBlock* ItemNum;
	
};
