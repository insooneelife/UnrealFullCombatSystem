// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_IgnoreRootMotion.h"
#include "Animation/AnimInstance.h"

void UANS_IgnoreRootMotion::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    MeshComp->GetAnimInstance()->SetRootMotionMode(ERootMotionMode::Type::IgnoreRootMotion);
}

void UANS_IgnoreRootMotion::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    MeshComp->GetAnimInstance()->SetRootMotionMode(ERootMotionMode::Type::RootMotionFromMontagesOnly);
}