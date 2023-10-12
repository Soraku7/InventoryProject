﻿#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class UWidgetComponent;
struct FItemDetails
{
	int32 ID;
	int32 Num;

	FText ItemName = FText::FromString("None");
	FText ItemDescription = FText::FromString("None");
	UTexture2D* ItemIcon = nullptr;
	UStaticMesh* MeshComponent = nullptr;
};

UCLASS()
class INVENTORYPROJECT_API AItem : public AActor
{
	GENERATED_BODY()

public:
	AItem();

	void SetItemDetails(FItemDetails NewDetails);

	FItemDetails GetItemDetails();

	void SetWidgetVisbility(bool NewValue);

protected:
	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	FText ItemName;

	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	FText ItemDescription;

	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	int32 ID;

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	int32 Num;

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	UWidgetComponent* WidgetComponent;
};
