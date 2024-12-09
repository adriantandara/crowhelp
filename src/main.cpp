#include "main.h"

#include "tools/common/Webhook.h"
#include "tools/helpers/WaitPlayerReady.h"
#include "tools/helpers/ChatInput.h"
#include "tools/helpers/GetLastLine.h"

void Thread() {
    using namespace std;

    auto chat_future = async(launch::async, WaitPlayerReady);

    if (!chat_future.get() && playerChat == nullptr) return;

    this_thread::sleep_for(chrono::seconds(1));

    const string text0 = "{a12222}>> {f9f9f9}client {a12222}crowhelp{f9f9f9} initialized.";
    const string text1 = "{a12222}>> {f9f9f9}developed by {a12222}adriantandara{f9f9f9}.";

    playerChat->AddMessage(0xa12222FF, text0.c_str());
    playerChat->AddMessage(0xa12222FF, text1.c_str());

    monitorLogFile(playerChat, playerInput);
}

int APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) std::thread{Thread}.detach();
    return TRUE;
}