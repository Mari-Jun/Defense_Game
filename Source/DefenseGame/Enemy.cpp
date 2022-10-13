// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyStatusWidget.h"
#include "EnemyController.h"
#include "BaseCharacter.h"
#include "EnemyDamageWidget.h"
#include "DefenseBase.h"
#include "Item.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation({ 0.0f, 0.0f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight() });
	GetMesh()->SetRelativeRotation({ 0.0f, -90.0f, 0.0f });

	GetMesh()->SetCollisionProfileName("Enemy");
	GetMesh()->SetGenerateOverlapEvents(true);

	bUseControllerRotationYaw = false;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	
	GetCharacterMovement()->bOrientRotationToMovement = true;

	AddNewAbility<USphereComponent>("DefaultAttack", 2.0f, 100'000);
	
	EnemyStatusWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("status widget");
	EnemyStatusWidgetComponent->SetupAttachment(GetRootComponent());
	EnemyStatusWidgetComponent->SetDrawSize({ 200.f, 35.f });
	HideStatusWidget();

	DamageWidgetSpawnPoint = CreateDefaultSubobject<USceneComponent>("DamageWidgetSpawnPoint");
	DamageWidgetSpawnPoint->SetupAttachment(GetRootComponent());

	DropCoinEffect = Cast<UNiagaraSystem>(StaticLoadObject(UNiagaraSystem::StaticClass(), nullptr,
		TEXT("NiagaraSystem'/Game/sA_PickupSet_1/Fx/NiagaraSystems/NS_CoinBurst.NS_CoinBurst'")));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	if (EnemyStatusTable != nullptr)
	{
		FString RowName = FString::Printf(TEXT("Level%d"), EnemyStatusData.Level);
		FEnemyStatusTable* EnemyDataRow = EnemyStatusTable->FindRow<FEnemyStatusTable>(FName(*RowName), "");
		if (EnemyDataRow != nullptr)
		{
			CombatStatus.Attack = EnemyDataRow->Attack;
			CombatStatus.Defense = EnemyDataRow->Defense;
			CombatStatus.CurrentHP = CombatStatus.MaxHP = EnemyDataRow->HP;
			CombatStatus.CurrentShield = CombatStatus.MaxShield = EnemyDataRow->Shield;
			CombatStatus.ReactionValue = EnemyDataRow->ReactionValue;
			DropCoin = EnemyDataRow->DropCoin;
		}

	}
	
	if (EnemyStatusWidgetComponent != nullptr)
	{
		EnemyStatusWidget = Cast<UEnemyStatusWidget>(EnemyStatusWidgetComponent->GetWidget());
		if (EnemyStatusWidget == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not cast GetWidget() to UEnemyStatusWidget"));
			GetWorld()->DestroyActor(this);
		}
		else
		{
			ChangeHPDelegate.AddDynamic(EnemyStatusWidget, &UEnemyStatusWidget::OnChangeHP);
			ChangeShieldDelegate.AddDynamic(EnemyStatusWidget, &UEnemyStatusWidget::OnChangeShield);
			ChangeHPDelegate.Broadcast(CombatStatus.CurrentHP, CombatStatus.MaxHP);
			ChangeShieldDelegate.Broadcast(CombatStatus.CurrentShield, CombatStatus.MaxShield);

		}
	}
	BroadcastDamageInfoDelegate.AddDynamic(this, &AEnemy::AddDamageNumber);

	EnemyController = Cast<AEnemyController>(GetController());
	if (EnemyController == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not cast GetController() to AEnemyController"));
		GetWorld()->DestroyActor(this);
	}

	//Initialize AbilityMap
	for (const auto& [Name, Data] : AbilityMap)
	{
		Data.AbilityEnableRange->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnAbilityRangeBeginOverlap);
		Data.AbilityEnableRange->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnAbilityRangeEndOverlap);
	}
	SetAbilityCollision(false);
	AbilityOrder.ValueSort([](int32 A, int32 B) { return A < B; });


	GetCharacterMovement()->MaxWalkSpeed = EnemyStatusData.DefaultSpeed;
}

void AEnemy::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyStatusWidgetComponent != nullptr && EnemyStatusWidgetComponent->IsVisible())
	{
		FVector WidgetLocation = EnemyStatusWidgetComponent->GetComponentLocation();
		FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
		FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(WidgetLocation, CameraLocation);
		EnemyStatusWidgetComponent->SetWorldRotation({ 0.0f, NewRotation.Yaw, 0.0f });
	}

	Attack();

	RenderHitNumbers();
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ShowStatusWidget();

	return DamageAmount;
}

void AEnemy::FinishDeath()
{
	GetMesh()->GlobalAnimRateScale = 0.0f;
	HideStatusWidget();
	GetWorldTimerManager().SetTimer(DestoryActorTimerHandle, this, &AEnemy::DestoryEnemy, DeathTime, false);
}

void AEnemy::GetNewTarget()
{
	SetAbilityCollision(true);
}

void AEnemy::LoseTarget()
{
	SetAbilityCollision(false);
}

void AEnemy::KillObject()
{
	EnemyController->KilledControlledPawn();
	DisableCollision();
	ChangeEnemyState(EEnemyState::EDeath);
	if (DeadAnimMontage != nullptr)
	{
		PlayAnimMontage(DeadAnimMontage);
	}
	else
	{
		GetWorld()->DestroyActor(this);
	}
	DropItem();
	KillEnemyEventDelegate.Broadcast();
}

void AEnemy::DestoryEnemy()
{
	GetWorld()->DestroyActor(this);
}

void AEnemy::DropItem()
{
	if (DropItems.IsEmpty()) return;
	if (ItemDropProbability == 0) return;
	if (ItemDropProbability < FMath::RandRange(0, 100)) return;

	TArray<TSubclassOf<AItem>> ItemArray;
	for (const auto& [ItemClass, Count] : DropItems)
	{
		for (int i = 0; i < Count; ++i) ItemArray.Add(ItemClass);
	}

	TSubclassOf<AActor> SelectItemClass = ItemArray[FMath::RandRange(0, ItemArray.Num() - 1)];
	AItem* DropItem = Cast<AItem>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, SelectItemClass, FTransform(GetActorLocation())));
	if (DropItem != nullptr)
	{
		UGameplayStatics::FinishSpawningActor(DropItem, FTransform(GetActorLocation()));
	}

	if (DropCoinEffect != nullptr)
	{
		UNiagaraComponent* Effect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, DropCoinEffect, GetActorLocation());
		Effect->SetRelativeScale3D(DropCoinEffectScale);
	}

	if (EnemyController != nullptr && EnemyController->GetLastDamageCauser() != nullptr)
	{
		ABaseCharacter* DamageCauserCharacter = Cast<ABaseCharacter>(EnemyController->GetLastDamageCauser());
		if (DamageCauserCharacter != nullptr)
		{
			DamageCauserCharacter->ChangeCoin(DropCoin);
		}
	}

}

void AEnemy::ChangeEnemyState(EEnemyState State)
{
	switch (State)
	{
	case EEnemyState::ENone: 
		GetCharacterMovement()->MaxWalkSpeed = EnemyStatusData.DefaultSpeed;
		break;
	case EEnemyState::EReaction: 
		GetCharacterMovement()->MaxWalkSpeed = EnemyStatusData.ReactionSpeed;
		break;
	case EEnemyState::EAttack: 
		GetCharacterMovement()->MaxWalkSpeed = EnemyStatusData.AttackSpeed;
		break;
	case EEnemyState::EDeath: break;
	default: break;
	}
	EnemyState = State;
}

void AEnemy::ShowStatusWidget()
{
	EnemyStatusWidgetComponent->SetHiddenInGame(false);
	GetWorldTimerManager().SetTimer(ShowStatusWidgetTimerHandle, this, &AEnemy::HideStatusWidget, ShowStatusWidgetTime, false);
}

void AEnemy::HideStatusWidget()
{
	EnemyStatusWidgetComponent->SetHiddenInGame(true);
}

void AEnemy::PlayHitReaction(float HitYaw)
{
	UAnimMontage* HitReaction = HitReactionFWDAnimMontage;

	if (HitYaw >= -45.f && HitYaw <= 45.f)
	{
		if (HitReactionFWDAnimMontage != nullptr)
		{
			HitReaction = HitReactionFWDAnimMontage;
		}
	}
	else if (HitYaw >= -135.f && HitYaw < -45.f)
	{
		if (HitReactionLeftAnimMontage != nullptr)
		{
			HitReaction = HitReactionLeftAnimMontage;
		}
	}
	else if (HitYaw <= 135.f && HitYaw > 45.f)
	{
		if (HitReactionRightAnimMontage != nullptr)
		{
			HitReaction = HitReactionRightAnimMontage;
		}
	}
	else
	{
		if (HitReactionBWDAnimMontage != nullptr)
		{
			HitReaction = HitReactionBWDAnimMontage;
		}
	}

	if (HitReaction != nullptr)
	{
		PlayAnimMontage(HitReaction);
		ChangeEnemyState(EEnemyState::EReaction);
	}
}

bool AEnemy::CheckAttack()
{
	if (EnemyController == nullptr) return false;
	if (EnemyController->HasTarget() == false) return false;
	if (EnemyState != EEnemyState::ENone) return false;

	return true;
}

void AEnemy::Attack()
{
	if (CheckAttack() == false) return;

	ABaseCharacter* TargetCharacter = EnemyController->GetTargetCharacter();
	ADefenseBase* TargetBase = EnemyController->GetTargetDefenseBase();

	if (AbilityMap["DefaultAttack"].CanCastAbility)
	{
		if (AttackAnimMontange.IsEmpty() == false)
		{
			int32 index = FMath::RandRange(0, AttackAnimMontange.Num() - 1);
			const auto& attack_montage = AttackAnimMontange[index];
			PlayAnimMontage(attack_montage);
			ChangeEnemyState(EEnemyState::EAttack);
		}
	}
}

void AEnemy::SetAbilityCollision(bool bEnable)
{
	for (auto& [Name, Data] : AbilityMap)
	{
		if (Data.AbilityEnableRange != nullptr)
		{
			if (bEnable)
			{
				Data.AbilityEnableRange->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				Data.AbilityEnableRange->SetGenerateOverlapEvents(true);
			}
			else
			{
				Data.AbilityEnableRange->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				Data.AbilityEnableRange->SetGenerateOverlapEvents(false);
			}
		}
	}
}

void AEnemy::OnAbilityRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (auto& [Name, Data] : AbilityMap)
	{
		if (Data.AbilityEnableRange != nullptr && Data.AbilityEnableRange == OverlappedComponent)
		{
			Data.CanCastAbility = true;
			break;
		}
	}
}

void AEnemy::OnAbilityRangeEndOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	for (auto& [Name, Data] : AbilityMap)
	{
		if (Data.AbilityEnableRange != nullptr && Data.AbilityEnableRange == OverlappedComponent)
		{
			Data.CanCastAbility = false;
			break;
		}
	}
}

void AEnemy::DisableCollision()
{
	Super::DisableCollision();
	SetAbilityCollision(false);
}

void AEnemy::AddDamageNumber(float HPDamage, float ShieldDamage, bool IsCritical)
{
	if (DamageNumberWidgetClass != nullptr)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController == nullptr) return;

		UEnemyDamageWidget* DamageWidget = CreateWidget<UEnemyDamageWidget>(PlayerController, DamageNumberWidgetClass);
		if (DamageWidget != nullptr)
		{
			DamageWidget->SetDamageText(HPDamage, ShieldDamage, IsCritical);
			DamageWidget->SetTextAlpha(ShowDamageWidgetTime);

			FVector Location = GetActorLocation();
			DamageNumbers.Add(DamageWidget, Location);
			DamageWidget->AddToViewport();
			DamageWidget->SetVisibility(ESlateVisibility::Hidden);

			FTimerHandle DamageNumberTimerHandle;
			FTimerDelegate DamageNumberTimerDelegate;

			DamageNumberTimerDelegate.BindUFunction(this, FName("DestoryDamageNumber"), DamageWidget);
			GetWorldTimerManager().SetTimer(DamageNumberTimerHandle, DamageNumberTimerDelegate, ShowDamageWidgetTime, false);
		}
	}
}

void AEnemy::DestoryDamageNumber(UEnemyDamageWidget* DamageWidget)
{
	DamageNumbers.Remove(DamageWidget);
	DamageWidget->RemoveFromParent();
}

void AEnemy::RenderHitNumbers()
{
	for (auto& DamageNumber : DamageNumbers)
	{
		UEnemyDamageWidget* DamageNumberWidget = DamageNumber.Key;
		const FVector HitLocation = DamageNumber.Value;
		DamageNumber.Value += FVector(0.0f, 0.0f, 50.0f * GetWorld()->GetDeltaSeconds());

		FVector2D ScreenPosition;
		if (UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), HitLocation, ScreenPosition))
		{
			DamageNumberWidget->SetVisibility(ESlateVisibility::Visible);
			DamageNumberWidget->SetPositionInViewport(ScreenPosition);
		}
		else
		{
			DamageNumberWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AEnemy::AttackEnd()
{
	ChangeEnemyState(EEnemyState::ENone);
}

void AEnemy::ReactionEnd()
{
	ChangeEnemyState(EEnemyState::ENone);
}
