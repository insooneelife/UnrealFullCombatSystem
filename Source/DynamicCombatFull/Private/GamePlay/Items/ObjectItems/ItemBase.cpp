// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"


void UItemBase::BeginDestroy()
{
    Super::BeginDestroy();

    Item.Image = nullptr;
}