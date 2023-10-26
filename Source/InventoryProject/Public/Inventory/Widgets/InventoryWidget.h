// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UButton;
class UInventoryComponent;
struct FInsideItem;
class UInsideItemWidget;
struct FOutsideItem;
class UOutsideItemWidget;
class UScrollBox;
class UCanvasPanel;
UCLASS()
class INVENTORYPROJECT_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init();
	
	void CreateNewOutsideItemWidget(FOutsideItem& OutsideItem);

	void ClearWidgetBox();

	void InitInsideItemBox(FVector2D InventoryMaxStorage , TArray<FInsideItem>& InsideItemBox);

	void RefreshInsideItemBox(TArray<FInsideItem>& InsideItemBox);

	void SetTargetWidget(UInsideItemWidget* Widget);

	/**
	 * UI跟随鼠标
	 */
	void TargetWidgetFollowMouse();

	bool GetIsHadWidgetFollowMouse(){return bIsHadWidgetFollowMouse;}

	void ResetTargetWidget();
	
	void SwitchToWidget(UInsideItemWidget* InsideWidget);

	UFUNCTION(BlueprintCallable)
	void DropThisItem();
protected:
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	TSubclassOf<UOutsideItemWidget> OutsideWidgetsClass;

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	TSubclassOf<UInsideItemWidget> InsideWidgetClass;
	

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	FVector2D InsideItemBoxSize = FVector2D(100 , 100);

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	FVector2D InsideBoxBeginPoint = FVector2D(10 , 10);
private:
	UScrollBox* ScrollBox;
	UCanvasPanel* CanvasPanel;

	UButton* DropButton;
	
	bool bIsHadWidgetFollowMouse = false;
	
	UInsideItemWidget* TargetWidget = nullptr;
	UInventoryComponent* InventoryComp;
};
