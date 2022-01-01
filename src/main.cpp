#include <main.h>
#include "script.h"

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID lpReserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		scriptRegister(hInstance, ChaosMod::ScriptMain);
		keyboardHandlerRegister(ChaosMod::OnKeyboardMessage);
		break;
	case DLL_PROCESS_DETACH:

		if (ChaosMod::Singleton && ChaosMod::Singleton->wsServer)
		{
			ChaosMod::Singleton->wsServer->Stop();
		}

		scriptUnregister(hInstance);
		keyboardHandlerUnregister(ChaosMod::OnKeyboardMessage);
		break;
	}
	return TRUE;
}
