#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <thread>
#include <csignal>
#include <chrono>
#include <iostream>
#include "MCADINCL.H"
#include "discord-files/discord.h"

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
            AllocConsole();
            freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

            std::cout << "Rich presence plugin loaded." << std::endl;
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

