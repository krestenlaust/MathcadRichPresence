#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <thread>
#include <csignal>
#include <chrono>
#include <iostream>
#include "MCADINCL.H"
#include "discord-files/discord.h"

#define  INTERRUPTED            1
#define  INSUFFICIENT_MEMORY    2
#define  MUST_BE_REAL           3
#define  NUMBER_OF_ERRORS       3

// table of error messages
// if your function never returns an
// error -- you do not need to create this
// table
char* myErrorMessageTable[NUMBER_OF_ERRORS] =
{ (char*)"interrupted",
    (char*)"insufficient memory",
    (char*)"must be real"
};

// this code executes the multiplication
    // see the information of MathcadUserFunction
    // to find out more
LRESULT MultiplyRealArrayByRealScalar(COMPLEXARRAY* const Product, LPCCOMPLEXSCALAR Scalar, LPCCOMPLEXARRAY Array)
{
    unsigned int row, col;

    // check that the scalar argument is real
    if (Scalar->imag != 0.0)
        // if it is not, display "must be real" error message 
        // under the scalar argument( the 1st argument )
        return MAKELRESULT(MUST_BE_REAL, 1);

    // check that the array argument is real
    if (Array->hImag != NULL)
        // if it is not, display "must be real" error message 
        // under the array argument( the 2nd argument )
        return MAKELRESULT(MUST_BE_REAL, 2);

    // allocate memory for the product
    if (!MathcadArrayAllocate(Product, Array->rows, Array->cols,
        TRUE,      // allocate memory for the real part 
        FALSE))    // do not allocate memory for 
                    // the imaginary part

        // if allocation is not successful
        // return with the appropriate error code
        return INSUFFICIENT_MEMORY;


    // if all is well so far -- go ahead and 
    // perform the multiplication
    for (col = 0; col < Product->cols; col++)
    {
        // check that a user has not tried to interrupt 
        // the calculation
        if (isUserInterrupted())
        {
            // if user has interrupted -- 
            // free the allocated memory
            MathcadArrayFree(Product);
            // and return with an appropriate error code
            return INTERRUPTED;
        }

        for (row = 0; row < Product->rows; row++)
            Product->hReal[col][row] = Scalar->real * Array->hReal[col][row];
    }

    // normal return
    return 0;
}

// fill out a FUNCTIONINFO structure with
// the information needed for registering the
// function with Mathcad
FUNCTIONINFO multiply =
{
    // Name by which mathcad will recognize the function
    (char*)"multiply",

    // description of "multiply" parameters to be used
    // by the Insert Function dialog box
    (char*)"a,M",

    // description of the function for the Insert Function dialog box       
    (char*)"returns the product of real scalar a and real array M",

    // pointer to the executible code
    // i.e. code that should be executed
    // when a user types in "multiply(a,M)="
    (LPCFUNCTION)MultiplyRealArrayByRealScalar,

    // multiply(a,M) returns a complex array
    COMPLEX_ARRAY,

    // multiply(a,M) takes on two arguments
    2,

    // the first is a complex scalar, the second a complex array 
    { COMPLEX_SCALAR, COMPLEX_ARRAY }
};

struct DiscordState {
    std::unique_ptr<discord::Core> core;
};


namespace {
    volatile bool interrupted{ false };
}

void init_discord() {
    DiscordState state{};
    discord::Core* core{};
    auto response = discord::Core::Create(938426837233729626, DiscordCreateFlags_Default, &core);
    state.core.reset(core);

    if (!state.core) {
        std::cout << "Failed to instantiate Discord!";
        std::exit(-1);
    }

    discord::Activity activity{};
    activity.SetState("Fil: test.mcdx");
    activity.SetDetails("Redigerer dokument");
    activity.GetAssets().SetLargeImage("mathcad_logo");
    activity.SetType(discord::ActivityType::Playing);

    state.core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
        std::cout << ((result == discord::Result::Ok) ? "Succeeded" : "Failed")
            << " updating activity!\n";
        });

    std::signal(SIGINT, [](int) {
        interrupted = true;
        });

    do {
        state.core->RunCallbacks();
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); //50
    } while (!interrupted);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    std::thread t1;

    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            if (CreateUserErrorMessageTable(hModule, NUMBER_OF_ERRORS, myErrorMessageTable))
                CreateUserFunction(hModule, &multiply);

            AllocConsole();
            freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

            std::cout << "Hello world" << std::endl;
            t1 = std::thread (init_discord);
            t1.detach();
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}

