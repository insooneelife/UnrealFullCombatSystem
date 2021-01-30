// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"

UItemBase::UItemBase()
{
    UE_LOG(LogTemp, Error, TEXT("UItemBase Ctor  : %s"), *GetName());
}