// dllmain.cpp : Defines the entry point for the DLL application.
#pragma once
#include "framework.h"
#include "discord.h"
#include <MCADINCL.H>
#include <WinUser.h>
#include <string>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

constexpr std::chrono::nanoseconds timestep(900ms);

#define  INTERRUPTED            1
#define  INSUFFICIENT_MEMORY    2
#define  MUST_BE_REAL           3
#define  NUMBER_OF_ERRORS       3

// table of error messages
// if your function never returns an error -- you do not need to create this table
char* myErrorMessageTable[NUMBER_OF_ERRORS] =
{
    (char*)"interrupted",
    (char*)"insufficient memory",
    (char*)"must be real"
};

// this code executes the multiplication
    // see the information of MathcadUserFunction
    // to find out more
LRESULT  MultiplyRealArrayByRealScalar(COMPLEXARRAY* const Product,
    LPCCOMPLEXSCALAR Scalar, LPCCOMPLEXARRAY Array)
{
    unsigned int row, col;

    MessageBox(NULL, L"Hello world", L"multiply", 0);

    if (isUserInterrupted())
    {
        return INTERRUPTED;
    }
    
    // normal return
    return 0;
}

FUNCTIONINFO multiply =
{
	// name by which Mathcad will recognize the function
	(char*)"multiply",

	// description of "multiply" parameters to be used
	(char*)"a,M",

	// description of the function
	(char*)"returns the product of real scalar a and real array M",

	// pointer to the executable code
	// i.e. code that should be executed when a user types "multiply(a,M)="
	(LPCFUNCTION)MultiplyRealArrayByRealScalar,

    // multiply(a, M) returns a complex array
    COMPLEX_ARRAY,

    // multiply takes on two arguments
    2,

    // the first is a complex scalar, the second a complex array
    { COMPLEX_SCALAR, COMPLEX_ARRAY }
};

discord::Core* core{};

void InitDiscord()
{
    auto result = discord::Core::Create(938426837233729626, DiscordCreateFlags_Default, &core);
    discord::Activity activity{};
    activity.SetState("Fil: test.mcdx");
    activity.SetDetails("Redigerer dokument");
    activity.GetAssets().SetLargeImage("mathcad_logo");

    core->ActivityManager().UpdateActivity(activity, [](discord::Result) {
        MessageBox(NULL, L"Hello world", L"hej", 0);
    });
}

void CallbackLoop() {
    while (true)
    {
        std::this_thread::sleep_for(timestep);

        // update
        core->RunCallbacks();
    }
}

void InitCallbackLoop() {
    std::thread t1(CallbackLoop);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    MessageBox(NULL, L"Hello world", L"main1", 0);

    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            MessageBox(NULL, L"Hello world", L"main", 0);

            //MessageBox(NULL, L"Hello world", L"Hi", 0);
            if (CreateUserErrorMessageTable(hModule, NUMBER_OF_ERRORS, myErrorMessageTable))
                // and if the errors register properly, register the user function
                CreateUserFunction(hModule, &multiply);

            InitDiscord();
            InitCallbackLoop();
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            // er det her nødvendigt?
            core->~Core();
            break;
    }

    return TRUE;
}

