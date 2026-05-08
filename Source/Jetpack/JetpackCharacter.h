
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "JetpackCharacter.generated.h"

class UNiagaraComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;
class UUserWidget;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class AJetpackCharacter : public ACharacter
{
	GENERATED_BODY()

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
protected:
virtual void BeginPlay() override;
virtual void Tick(float DeltaTime) override;


protected:
    // particle systems and widget

	// widget for jetpack fuel
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
		TSubclassOf<UUserWidget> JetpackHUDClass;

	UPROPERTY() UUserWidget* JetpackHUD;




protected:
	// jetpack variables and functions 

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jetpack")
    float JetpackForce = 100000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jetpack")
    float MaxFuel = 50.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Jetpack")
    float CurrentFuel = 50.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Jetpack", meta=(AllowPrivateAccess="true"))
	bool bIsJetpacking = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jetpack")
    float FuelBurnRate = 10.0f;
//recharge variables
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jetpack")
    float RechargeRate = 5.0f;
	float RechargeTimer = 0.0f;
	float RechargeDelay = 2.0f;

    void StartJetpack();
    void StopJetpack();
	void HandleJetpack(float DeltaTime);




	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;
	UPROPERTY(EditAnywhere, Category="Input")	UInputAction* JetpackAction;

public:

	/** Constructor */
	AJetpackCharacter();	

protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

public:

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

public:
	UFUNCTION(BlueprintImplementableEvent, Category="Jetpack")
	void JetpackStarted();

	UFUNCTION(BlueprintImplementableEvent, Category="Jetpack")
	void JetpackStopped();

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

