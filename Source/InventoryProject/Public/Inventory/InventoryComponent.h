// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PickUp/Item.h"
#include "InventoryComponent.generated.h"


class UInventoryWidget;
class UOutsideItemWidget;
class UInsideItemWidget;
class AItem;

struct FOutsideItem
{
	FItemDetails ItemDetails;
	UOutsideItemWidget* Widget = nullptr;
	
};

struct FInsideItem
{
	FItemDetails ItemDetails;
	UInsideItemWidget* Widget = nullptr;
};
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYPROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

	void Init();

	void GetAllItem();

	void AddItemInDistance(AItem* Item);
	
	void DeleteItemNotInDistance(AItem* Item);

	bool CheckIfAlreadyInArray(AItem* Item);

	int32 FindIndexOfInDistanceArray(AItem* Item);

	
	/**
	 * 外部Item生成
	 */
	void ConvertItemToOutsideItem();

	void UpdateOutsideBox();

	bool LeachSameIDItem(FItemDetails ItemDetail);

	void ToggleInventoryWidget();

	
	/**
	 * 添加到内部背包
	 * @param Detail 
	 */
	void AddToInsideBox(FItemDetails Detail);

	int32 FindIndexCouldSaveItem();
	
	/**
	* 找到最近的物品
	*/
	AItem* FindCloseItem(int32 ItemID);
	
	/**
	 * 拾取物品
	 */
	void PickUpByKey();
	
	bool CheckAlreadyInInventory(FItemDetails Details);

	void PickUpByWidget(UOutsideItemWidget* OutsideItemWidget);
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	double PickUpDistance = 300.f;

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	FVector2D InventoryMaxStorage = FVector2D(5 , 5);

private:
	TArray<AItem*> AllInDistanceItems;
	TArray<FOutsideItem> OutsideItemBox;
	TArray<FInsideItem> InsideItemBox;
	
	UInventoryWidget* InventoryWidget;
};
