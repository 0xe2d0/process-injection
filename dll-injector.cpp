#include <windows.h>
#include <stdio.h>

HMODULE hKernel32 = NULL;

wchar_t dllPath[MAX_PATH] = L"C:\\xxx\\xxx\\xxx\\randomDLL.dll";
size_t dllSize = sizeof(dllPath);

int main(int argc, char* argv[]) {

	DWORD PID = atoi(argv[1]);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	

	hKernel32 = GetModuleHandleW(L"kernel32");

	LPTHREAD_START_ROUTINE startThis = (LPTHREAD_START_ROUTINE)GetProcAddress(hKernel32, "LoadLibraryW");

	LPVOID rBuffer = VirtualAllocEx(hProcess, NULL, dllSize, (MEM_COMMIT | MEM_RESERVE), PAGE_READWRITE);


	WriteProcessMemory(hProcess, rBuffer, dllPath, dllSize, NULL);

	DWORD TID = NULL;

	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, startThis, rBuffer, 0, &TID);

	WaitForSingleObject(hThread, INFINITE);

	CloseHandle(hThread);

	return EXIT_SUCCESS;
}
