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
		scriptUnregister(hInstance);
		keyboardHandlerUnregister(ChaosMod::OnKeyboardMessage);
		break;
	}
	return TRUE;
}
