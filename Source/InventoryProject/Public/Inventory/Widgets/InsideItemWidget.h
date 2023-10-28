// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InsideItemWidget.generated.h"

class UInventoryWidget;
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

	void SetInventoryWidget(UInventoryWidget* Widget){InventoryWidget = Widget;}

	FVector2D GetWhenPressedMousePosition(){return WhenPressMousePosition;}

	FVector2D GetSelfPosition(){return SelfPosition;}
	
	UFUNCTION(BlueprintCallable)
	void BeClick();

	UFUNCTION(BlueprintCallable)
	void BeHovered();

protected:
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	UTexture2D* EmptyImg;
	
private:
	UImage* ItemIcon;
	UTextBlock* ItemNum;
	UButton* ItemButton;

	FVector2D WhenPressMousePosition;
	FVector2D SelfPosition;
	
	UInventoryWidget* InventoryWidget;
};
