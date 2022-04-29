#include <main.h>
#include "script.h"
#include <string>

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID lpReserved)
{
	switch (reason)
	{
		case DLL_PROCESS_ATTACH:
			ChaosMod::hInstance = hInstance;
			scriptRegister(hInstance, ChaosMod::ScriptMain);
			keyboardHandlerRegister(ChaosMod::OnKeyboardMessage);
			break;
		case DLL_PROCESS_DETACH:
			
			if (ChaosMod::Singleton)
			{
				if (ChaosMod::Singleton->wsServer)
				{
					ChaosMod::Singleton->wsServer->Stop();
				}
			}
			
			ChaosMod::TerminateNodeProcess();
			
			scriptUnregister(hInstance);
			keyboardHandlerUnregister(ChaosMod::OnKeyboardMessage);
			break;
	}
	return TRUE;
}
