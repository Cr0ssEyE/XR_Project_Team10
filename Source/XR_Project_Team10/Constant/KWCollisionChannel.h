
#pragma once

#include "CoreMinimal.h"

// Object Channel
#define ECC_PLAYER ECC_GameTraceChannel11
#define ECC_ENEMY ECC_GameTraceChannel12

// Trace Channel
#define ECC_PLAYER_ONLY ECC_GameTraceChannel13
#define ECC_ENEMY_ONLY ECC_GameTraceChannel14

// Collision Profile
#define CP_STATIC_ONLY TEXT("IgnoreOnlyPawn")
#define CP_GIMMICK TEXT("Gimmick")
#define CP_ENEMY TEXT("Enemy")
#define CP_PLAYER TEXT("Player")