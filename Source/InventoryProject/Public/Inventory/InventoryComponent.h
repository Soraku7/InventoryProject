// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PickUp/Item.h"
#include "InventoryComponent.generated.h"


class UInventoryWidget;
class UOutsideItemWidget;
class AItem;

struct FOutsideItem
{
	FItemDetails ItemDetails;
	UOutsideItemWidget* Widget = nullptr;
	
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

	void ConvertItemToOutsideItem();

	void UpdateOutsideBox();

	void ToggleInventoryWidget();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	double PickUpDistance = 300.f;
private:
	TArray<AItem*> AllInDistanceItems;
	TArray<FOutsideItem> OutsideItemBox;

	UInventoryWidget* InventoryWidget;
};
