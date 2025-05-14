// Fill out your copyright notice in the Description page of Project Settings.


#include "TacticalKnife.h"


// Sets default values
ATacticalKnife::ATacticalKnife()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATacticalKnife::BeginPlay()
{
    Super::BeginPlay();
    
}

// Called every frame
void ATacticalKnife::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

