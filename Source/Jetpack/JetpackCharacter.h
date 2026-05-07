
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
    //jetpack FUnctions and variables and particlesystem
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
		UNiagaraComponent* LeftThrusterFX;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
		UNiagaraComponent* RightThrusterFX;

    bool bIsJetpacking = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jetpack")
    float JetpackForce = 100000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jetpack")
    float MaxFuel = 1000.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Jetpack")
    float CurrentFuel = 1000.0f;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jetpack")
    float FuelBurnRate = 30.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jetpack")
    float RechargeRate = 15.0f;

    void StartJetpack();
    void StopJetpack();

	virtual void Tick(float DeltaTime) override;
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

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

