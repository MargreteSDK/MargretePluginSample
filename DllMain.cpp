﻿#include <Windows.h>
#include "DllMain.h"
#include "MargretePlugin.h"
#include "MargretePluginCommandImpl.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


DLLEXPORT void WINAPI MargretePluginGetInfo(MP_PLUGININFO* info) {
	if (!info)
		return;

	info->sdkVersion = MP_SDK_VERSION;
	if (info->nameBuffer)
		wcsncpy_s(info->nameBuffer, info->nameBufferLength, L"MargretePluginSample", info->nameBufferLength);
	if (info->descBuffer)
		wcsncpy_s(info->descBuffer, info->descBufferLength, L"プラグイン開発のサンプル", info->descBufferLength);
	if (info->developerBuffer)
		wcsncpy_s(info->developerBuffer, info->developerBufferLength, L"inonote", info->developerBufferLength);
}

DLLEXPORT MpBoolean WINAPI MargretePluginCommandCreate(IMargretePluginCommand** ppobj) {
	*ppobj = new CMargretePluginCommandImpl();
	(*ppobj)->addRef();
	return MP_TRUE;
}
