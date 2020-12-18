// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class DataTypes
{
public:
	DataTypes();
	~DataTypes();
};

UENUM(BlueprintType)
enum class EAbilityEndResult : uint8
{
    Finished       UMETA(DisplayName = "Finished"),
    Interrupted    UMETA(DisplayName = "Interrupted"),
    Canceled       UMETA(DisplayName = "Canceled"),
    OutOfMana      UMETA(DisplayName = "OutOfMana"),
    Destroyed      UMETA(DisplayName = "Destroyed"),
};


UENUM(BlueprintType)
enum class EActivity : uint8
{
	None                 UMETA(DisplayName = "None"),
	IsBlockingPressed    UMETA(DisplayName = "IsBlockingPressed"),
	IsAimingPressed      UMETA(DisplayName = "IsAimingPressed"),
	IsImmortal           UMETA(DisplayName = "IsImmortal"),
	CantBeInterrupted    UMETA(DisplayName = "CantBeInterrupted"),
	IsLookingForward     UMETA(DisplayName = "IsLookingForward"),
	CanParryHit          UMETA(DisplayName = "CanParryHit"),
	IsZooming            UMETA(DisplayName = "IsZooming"),
};


UENUM(BlueprintType)
enum class EAIBehavior : uint8
{
	Idle                 UMETA(DisplayName = "Idle"),
	Patrol               UMETA(DisplayName = "Patrol"),
	MeleeAttack          UMETA(DisplayName = "MeleeAttack"),
	RangeAttack          UMETA(DisplayName = "RangeAttack"),
	Approach             UMETA(DisplayName = "Approach"),
	Flee                 UMETA(DisplayName = "Flee"),
	StrafeAround         UMETA(DisplayName = "StrafeAround"),
	Hit                  UMETA(DisplayName = "Hit"),
};

UENUM(BlueprintType)
enum class EApplyEffectMethod : uint8
{
	Stack                UMETA(DisplayName = "Stack"),
	Replace              UMETA(DisplayName = "Replace")
};

UENUM(BlueprintType)
enum class EAttackResult : uint8
{
	Success             UMETA(DisplayName = "Success"),
	Failed              UMETA(DisplayName = "Failed"),
	Blocked             UMETA(DisplayName = "Blocked"),
	Parried             UMETA(DisplayName = "Parried"),
};

UENUM(BlueprintType)
enum class ECollisionPart : uint8
{
	None             UMETA(DisplayName = "None"),
	MainHandItem     UMETA(DisplayName = "MainHandItem"),
	RightHand        UMETA(DisplayName = "RightHand"),
	LeftHand         UMETA(DisplayName = "LeftHand"),
	RightFoot        UMETA(DisplayName = "RightFoot"),
	LeftFoot         UMETA(DisplayName = "LeftFoot"),
};

UENUM(BlueprintType)
enum class ECombatType : uint8
{
	Unarmed              UMETA(DisplayName = "Unarmed"),
	Melee                UMETA(DisplayName = "Melee"),
	Range                UMETA(DisplayName = "Range"),
	Magic                UMETA(DisplayName = "Magic"),
};

UENUM(BlueprintType)
enum class EDirection : uint8
{
	Front              UMETA(DisplayName = "Front"),
	Back               UMETA(DisplayName = "Back"),
	Left               UMETA(DisplayName = "Left"),
	Right              UMETA(DisplayName = "Right"),
};


UENUM(BlueprintType)
enum class EEffectType : uint8
{
	None              UMETA(DisplayName = "None"),
	Stun              UMETA(DisplayName = "Stun"),
	Knockdown         UMETA(DisplayName = "Knockdown"),
	Burning           UMETA(DisplayName = "Burning"),
	Backstab          UMETA(DisplayName = "Backstab"),
	Impact            UMETA(DisplayName = "Impact"),
	Parried           UMETA(DisplayName = "Parried"),
};

UENUM(BlueprintType)
enum class EHandleSameItemMethod : uint8
{
	Unequip              UMETA(DisplayName = "Unequip"),
	Update               UMETA(DisplayName = "Update"),
};

UENUM(BlueprintType)
enum class EInputBufferKey : uint8
{
	None                    UMETA(DisplayName = "None"),
	LightAttack             UMETA(DisplayName = "LightAttack"),
	HeavyAttack             UMETA(DisplayName = "HeavyAttack"),
	ThrustAttack            UMETA(DisplayName = "ThrustAttack"),
	SpecialAttack           UMETA(DisplayName = "SpecialAttack"),
	FallingAttack           UMETA(DisplayName = "FallingAttack"),
	Roll                    UMETA(DisplayName = "Roll"),
	Jump                    UMETA(DisplayName = "Jump"),
	Parry                   UMETA(DisplayName = "Parry"),
	ToggleCombat            UMETA(DisplayName = "ToggleCombat"),
	SwitchMainHandTypeUp    UMETA(DisplayName = "SwitchMainHandTypeUp"),
	SwitchMainHandTypeDown  UMETA(DisplayName = "SwitchMainHandTypeDown"),
	SwitchMainHandItemUp    UMETA(DisplayName = "SwitchMainHandItemUp"),
	SwitchMainHandItemDown  UMETA(DisplayName = "SwitchMainHandItemDown"),
	AbilityAttack           UMETA(DisplayName = "AbilityAttack"),
	SetSpellActiveIndex     UMETA(DisplayName = "SetSpellActiveIndex"),
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None              UMETA(DisplayName = "None"),
	Spell             UMETA(DisplayName = "Spell"),
	Shield            UMETA(DisplayName = "Shield"),
	Head              UMETA(DisplayName = "Head"),
	Top               UMETA(DisplayName = "Top"),
	Legs              UMETA(DisplayName = "Legs"),
	Hands             UMETA(DisplayName = "Hands"),
	Feet              UMETA(DisplayName = "Feet"),
	Arrows            UMETA(DisplayName = "Arrows"),
	Tool              UMETA(DisplayName = "Tool"),
	Material          UMETA(DisplayName = "Material"),
	Ring              UMETA(DisplayName = "Ring"),
	MeleeWeapon       UMETA(DisplayName = "MeleeWeapon"),
	RangeWeapon       UMETA(DisplayName = "RangeWeapon"),
	Necklace          UMETA(DisplayName = "Necklace"),
};


UENUM(BlueprintType)
enum class EMeleeAttackType : uint8
{
	None              UMETA(DisplayName = "None"),
	Light             UMETA(DisplayName = "Light"),
	Heavy             UMETA(DisplayName = "Heavy"),
	Special           UMETA(DisplayName = "Special"),
	Thrust            UMETA(DisplayName = "Thrust"),
	Falling           UMETA(DisplayName = "Falling"),
   
};

UENUM(BlueprintType)
enum class EMontageAction : uint8
{
	None              UMETA(DisplayName = "None"),
	LightAttack       UMETA(DisplayName = "LightAttack"),
	HeavyAttack       UMETA(DisplayName = "HeavyAttack"),
	ThrustAttack      UMETA(DisplayName = "ThrustAttack"),
	SpecialAttack     UMETA(DisplayName = "SpecialAttack"),
	FallingAttack     UMETA(DisplayName = "FallingAttack"),
	RollForward       UMETA(DisplayName = "RollForward"),
	RollLeft          UMETA(DisplayName = "RollLeft"),
	RollRight         UMETA(DisplayName = "RollRight"),
	RollBackward      UMETA(DisplayName = "RollBackward"),
	DrawWeapon        UMETA(DisplayName = "DrawWeapon"),
	DisarmWeapon      UMETA(DisplayName = "DisarmWeapon"),
	StunFront         UMETA(DisplayName = "StunFront"),
	StunBack          UMETA(DisplayName = "StunBack"),
	StunRight         UMETA(DisplayName = "StunRight"),
	StunLeft          UMETA(DisplayName = "StunLeft"),
	Impact            UMETA(DisplayName = "Impact"),
	Block             UMETA(DisplayName = "Block"),
	Parry             UMETA(DisplayName = "Parry"),
	Parried           UMETA(DisplayName = "Parried"),
	Backstabbed       UMETA(DisplayName = "Backstabbed"),
	Backstab          UMETA(DisplayName = "Backstab"),
	ShootArrow        UMETA(DisplayName = "ShootArrow"),

};


UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Idle             UMETA(DisplayName = "Idle"),
	Walk             UMETA(DisplayName = "Walk"),
	Jog              UMETA(DisplayName = "Jog"),
	Sprint           UMETA(DisplayName = "Sprint"),
};

UENUM(BlueprintType)
enum class ERotationMode : uint8
{
	FaceCamera             UMETA(DisplayName = "FaceCamera"),
	OrientToMovement       UMETA(DisplayName = "OrientToMovement"),
};

UENUM(BlueprintType)
enum class EState : uint8
{
	Idle              UMETA(DisplayName = "Idle"),
	Disabled          UMETA(DisplayName = "Disabled"),
	Attacking         UMETA(DisplayName = "Attacking"),
	Rolling           UMETA(DisplayName = "Rolling"),
	Falling           UMETA(DisplayName = "Falling"),
	Interacting       UMETA(DisplayName = "Interacting"),
	Backstabbing      UMETA(DisplayName = "Backstabbing"),
	Parrying          UMETA(DisplayName = "Parrying"),
	Dead              UMETA(DisplayName = "Dead"),
};

UENUM(BlueprintType)
enum class EStat : uint8
{
	None                    UMETA(DisplayName = "None"),
	Health                  UMETA(DisplayName = "Health"),
	Stamina                 UMETA(DisplayName = "Stamina"),
	Mana                    UMETA(DisplayName = "Mana"),
	Damage                  UMETA(DisplayName = "Damage"),
	Armor                   UMETA(DisplayName = "Armor"),
	CritChance              UMETA(DisplayName = "CritChance"),
	CritMultiplier          UMETA(DisplayName = "CritMultiplier"),
	AttackSpeed             UMETA(DisplayName = "AttackSpeed"),
	Block                   UMETA(DisplayName = "Block"),
	MeleeAttackStaminaCost  UMETA(DisplayName = "MeleeAttackStaminaCost"),
	MagicDamage             UMETA(DisplayName = "MagicDamage"),
	CastingSpeed            UMETA(DisplayName = "CastingSpeed"),
};


UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    None          UMETA(DisplayName = "None"),
    Sword         UMETA(DisplayName = "Sword"),
    Axe           UMETA(DisplayName = "Axe"),
    Bow           UMETA(DisplayName = "Bow"),
    Spell         UMETA(DisplayName = "Spell"),
};