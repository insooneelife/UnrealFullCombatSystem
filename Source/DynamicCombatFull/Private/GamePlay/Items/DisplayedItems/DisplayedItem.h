// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameCore/DataTypes.h"
#include "DisplayedItem.generated.h"

class USceneComponent;
class UEquipmentComponent;
class UPrimitiveComponent;

UCLASS()
class ADisplayedItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADisplayedItem();

protected:
    virtual void NativeInit(UEquipmentComponent* InEquipmentComponent, EItemType InType, int InSlotIndex);

public:
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override { Super::NotifyActorBeginOverlap(OtherActor); }

    void Init(UEquipmentComponent* InEquipmentComponent, EItemType InType, int InSlotIndex);

    UFUNCTION(BlueprintImplementableEvent, Category = "Blueprint", meta = (DisplayName = "Init", ScriptName = "Init"))
    void K2_Init();

    virtual UPrimitiveComponent* GetPrimaryComponent() const;

    virtual FName GetAttachmentSocket() const { return AttachmentSocket; }

    virtual bool Attach();

    virtual void SimulatePhysics();

public:
    UFUNCTION(BlueprintCallable)
    UEquipmentComponent* GetEquipmentComponent() const { return EquipmentComponent; }
    EItemType GetType() const { return Type; }
    int GetSlotIndex() const { return SlotIndex; }

protected:

    UPROPERTY(EditAnywhere, Category = "Sockets")
        FName AttachmentSocket;

    UPROPERTY(EditAnywhere, Category = "Equipment")
        UEquipmentComponent* EquipmentComponent;


    UPROPERTY(EditAnywhere, Category = "Equipment")
        EItemType Type;

    UPROPERTY(EditAnywhere, Category = "Equipment")
        int SlotIndex;



};
