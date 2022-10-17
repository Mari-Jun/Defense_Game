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
#include "Components/BoxComponent.h"
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
	
	EnemyStatusWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("status widget");
	EnemyStatusWidgetComponent->SetupAttachment(GetRootComponent());
	EnemyStatusWidgetComponent->SetDrawSize({ 200.f, 35.f });
	HideStatusWidget();

	DamageWidgetSpawnPoint = CreateDefaultSubobject<USceneComponent>("DamageWidgetSpawnPoint");
	DamageWidgetSpawnPoint->SetupAttachment(GetRootComponent());

	DropCoinEffect = Cast<UNiagaraSystem>(StaticLoadObject(UNiagaraSystem::StaticClass(), nullptr,
		TEXT("NiagaraSystem'/Game/sA_PickupSet_1/Fx/NiagaraSystems/NS_CoinBurst.NS_CoinBurst'")));

	//Custom Depth Stencil
	GetMesh()->bRenderCustomDepth = true;
	GetMesh()->CustomDepthStencilValue = 1;
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
		if (Data.AbilityEnableRange != nullptr)
		{
			Data.AbilityEnableRange->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnAbilityRangeBeginOverlap);
			Data.AbilityEnableRange->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnAbilityRangeEndOverlap);
		}
	}
	SetAbilitysCollision(false);
	AbilityOrder.ValueSort([](int32 A, int32 B) { return A < B; });


	GetCharacterMovement()->MaxWalkSpeed = EnemyStatusData.DefaultSpeed;
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

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ShowStatusWidget();

	return DamageAmount;
}

void AEnemy::ApplyDamage(AActor* OtherActor, float Damage)
{
	float AttackRatio = 1.0f;
	if (AbilityMap.Contains(CurrentAbilityName))
	{
		AttackRatio = AbilityMap[CurrentAbilityName].AttackRatio;
	}
	Super::ApplyDamage(OtherActor, Damage * AttackRatio);
}

void AEnemy::FinishDeath()
{
	GetMesh()->GlobalAnimRateScale = 0.0f;
	HideStatusWidget();
	GetWorldTimerManager().SetTimer(DestoryActorTimerHandle, this, &AEnemy::DestoryEnemy, DeathTime, false);
}

void AEnemy::GetNewTarget()
{
	SetAbilitysCollision(true);
}

void AEnemy::LoseTarget()
{
	SetAbilitysCollision(false);
}

void AEnemy::KillObject()
{
	EnemyController->KilledControlledPawn();
	ClearAbilityMap();
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

	for (const auto& [Name, Order] : AbilityOrder)
	{
		if (AbilityMap[Name].AbilityTimerHandle.IsValid() == false && AbilityMap[Name].AbilityAnimMontange.IsEmpty() == false)
		{
			ABaseCharacter* TargetCharacter = EnemyController->GetTargetCharacter();
			ADefenseBase* TargetBase = EnemyController->GetTargetDefenseBase();

			if (AbilityMap[Name].InAttackRangeActors.Find(TargetCharacter) ||
				(TargetCharacter == nullptr && AbilityMap[Name].CanTowerAttack && AbilityMap[Name].InAttackRangeActors.Find(TargetBase)))
			{
				CurrentAbilityName = Name;
				StartAbilityCooldown(Name);
				int32 index = FMath::RandRange(0, AbilityMap[Name].AbilityAnimMontange.Num() - 1);
				const auto& attack_montage = AbilityMap[Name].AbilityAnimMontange[index];
				PlayAnimMontage(attack_montage);
				ChangeEnemyState(EEnemyState::EAttack);
				break;
			}
		}
	}
}

//AbilityTimer가 유효한지 검사하기 때문에 true의 경우 Timer를 실행하고 설정 바람
void AEnemy::SetAbilitysCollision(bool bEnable)
{
	for (auto& [Name, Data] : AbilityMap)
	{
		if (Data.AbilityEnableRange != nullptr)
		{
			if (bEnable && Data.AbilityTimerHandle.IsValid() == false)
			{
				Data.AbilityEnableRange->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				Data.AbilityEnableRange->SetGenerateOverlapEvents(true);
				Data.AbilityEnableRange->UpdateOverlaps();
			}
			else
			{
				Data.AbilityEnableRange->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				Data.AbilityEnableRange->SetGenerateOverlapEvents(false);
			}
		}
	}
}

//AbilityTimer가 유효한지 검사하기 때문에 true의 경우 Timer를 실행하고 설정 바람
void AEnemy::SetAbilityCollision(FString AbilityName, bool bEnable)
{
	if (AbilityMap.Contains(AbilityName))
	{
		FEnemyAbilityData& Data = AbilityMap[AbilityName];
		if (Data.AbilityEnableRange != nullptr)
		{
			if (bEnable && Data.AbilityTimerHandle.IsValid() == false)
			{
				Data.AbilityEnableRange->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				Data.AbilityEnableRange->SetGenerateOverlapEvents(true);
				Data.AbilityEnableRange->UpdateOverlaps();
			}
			else
			{
				Data.AbilityEnableRange->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				Data.AbilityEnableRange->SetGenerateOverlapEvents(false);
			}
		}
	}
}

void AEnemy::StartAbilityCooldown(FString AbilityName)
{
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AEnemy::ResetAbilityTimer, AbilityName);
	GetWorldTimerManager().SetTimer(AbilityMap[AbilityName].AbilityTimerHandle,
		TimerDelegate, AbilityMap[AbilityName].AbilityTime, false);
	SetAbilityCollision(AbilityName, false);
}

void AEnemy::ResetAbilityTimer(FString AbilityName)
{
	GetWorldTimerManager().ClearTimer(AbilityMap[AbilityName].AbilityTimerHandle);
	SetAbilityCollision(AbilityName, true);
}

void AEnemy::ClearAbilityMap()
{
	for (auto& [Name, Data] : AbilityMap)
	{
		GetWorldTimerManager().ClearTimer(Data.AbilityTimerHandle);
	}
	AbilityMap.Reset();
}

void AEnemy::OnAbilityRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (auto& [Name, Data] : AbilityMap)
	{
		if (Data.AbilityEnableRange != nullptr && Data.AbilityEnableRange == OverlappedComponent)
		{
			//Character와 기지 모두 충돌이 가능하니 Set으로 관리한다.
			Data.InAttackRangeActors.Add(OtherActor);
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
			Data.InAttackRangeActors.Remove(OtherActor);
			break;
		}
	}
}

void AEnemy::DisableCollision()
{
	Super::DisableCollision();
	SetAbilitysCollision(false);
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
