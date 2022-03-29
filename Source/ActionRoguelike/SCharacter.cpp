// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "SInteractComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASCharacter::ASCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(GetRootComponent());
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractComp = CreateDefaultSubobject<USInteractComponent>("InteractComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

void ASCharacter::Heal(float Amount)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}

void ASCharacter::Kill(AActor* Target)
{
	if (!Target)
	{
		Target = this;
	}
	USAttributeComponent* Attributes = USAttributeComponent::GetAttributes(Target);
	if (Attributes)
	{
		Attributes->ApplyHealthChange(this, -Attributes->GetHealthMax());
	}
}

void ASCharacter::God(bool bEnabled)
{
	SetCanBeDamaged(!bEnabled);
}

void ASCharacter::MoveForward(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.f;
	ControlRot.Roll = 0.f;
	
	AddMovementInput(ControlRot.Vector(), value);
}

void ASCharacter::MoveRight(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.f;
	ControlRot.Roll = 0.f;
	
	AddMovementInput(FRotationMatrix(ControlRot).GetUnitAxis(EAxis::Y), value);
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	for (const TSubclassOf<USAction> DefaultAction : DefaultActions)
	{
		ActionComp->AddAction(GetOwner(), DefaultAction);
	}
}

void ASCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}

void ASCharacter::SecondaryAttack()
{
	ActionComp->StartActionByName(this, "SecondaryAttack");
}

void ASCharacter::PrimaryInteract()
{
	InteractComp->PrimaryInteract();
}

void ASCharacter::Teleport()
{
	ActionComp->StartActionByName(this, "Teleport");
}

void ASCharacter::HandleHealthChanged(USAttributeComponent* OwningComp, AActor* InstigatorActor, float HealthNew,
                                      float HealthDelta)
{
	Super::HandleHealthChanged(OwningComp, InstigatorActor, HealthNew, HealthDelta);
	
	if (HealthDelta < 0.f)
	{
		// hit flash
		GetMesh()->SetScalarParameterValueOnMaterials("TimeOfHit", GetWorld()->GetTimeSeconds());
	}
	
	if (HealthNew <= 0.f && HealthDelta < 0.f)
	{
		DisableInput(Cast<APlayerController>(GetController()));
	}
}

// Called every frame

void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input

void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Lookup", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", EInputEvent::IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("SecondaryAttack", EInputEvent::IE_Pressed, this, &ASCharacter::SecondaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", EInputEvent::IE_Pressed, this, &ASCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Teleport", IE_Pressed, this, &ASCharacter::Teleport);
	PlayerInputComponent->BindAction("Parry", IE_Pressed, this, &ASCharacter::Parry);

	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &ASCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &ASCharacter::StopSprint);

	PlayerInputComponent->BindAction("CancelActions", IE_Pressed, this, &ASCharacter::CancelActions);
}

void ASCharacter::Parry()
{
	ActionComp->StartActionByName(this, "Parry");
}

void ASCharacter::CancelActions()
{
	ActionComp->StopRunningActions(this);
}

void ASCharacter::StartSprint()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void ASCharacter::StopSprint()
{
	ActionComp->StopActionByName(this, "Sprint");
}

