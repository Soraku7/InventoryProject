// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InsideItemWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;
struct FItemDetails;
/**
 * 
 */
UCLASS()
class INVENTORYPROJECT_API UInsideItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init();

	void SetWidgetStyle(FItemDetails& Details);
	
private:
	UImage* ItemIcon;
	UTextBlock* ItemNum;
	UButton* ItemButton;
};
