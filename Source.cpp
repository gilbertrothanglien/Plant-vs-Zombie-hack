
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

using namespace std;

DWORD pid;
DWORD sun = 0x0019926c;

DWORD GetPID(const char* ProcessName) {
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);


	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processesSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	Process32First(processesSnapshot, &processInfo);
	if (!strcmp(processInfo.szExeFile, ProcessName))
	{
		CloseHandle(processesSnapshot);
	}

	while (Process32Next(processesSnapshot, &processInfo))
	{
		if (!strcmp(processInfo.szExeFile, ProcessName))
		{
			CloseHandle(processesSnapshot);
		}
	}
	return processInfo.th32ProcessID;
}

MODULEENTRY32 GetModule(const char* moduleName, unsigned long ProcessID)
{
	MODULEENTRY32 modEntry = { 0 };

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, ProcessID);

	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 curr = { 0 };

		curr.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(hSnapshot, &curr))
		{
			do
			{
				if (!strcmp(curr.szModule, moduleName))
				{
					modEntry = curr;
					break;
				}
			} while (Module32Next(hSnapshot, &curr));
		}
		CloseHandle(hSnapshot);
	}
	return modEntry;
}

int main()
{	
	HWND hwnd = FindWindowA(NULL, "Plants vs. Zombies");

	GetWindowThreadProcessId(hwnd, &pid);
	cout << "Process Id of the game: " << pid << endl;;
	MODULEENTRY32 module = GetModule("Plants vs. Zombies", pid);
	HANDLE phandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	unsigned long long result;
	
	ReadProcessMemory(phandle, (void*)((unsigned long long)module.modBaseAddr + 0x0019926c), &result, sizeof(result), 0);
	//ReadProcessMemory(phandle, (void*)((unsigned long long)result + 0x5560), &result, sizeof(result), 0);
	//ReadProcessMemory(phandle, (void*)((unsigned long long)result + 0x50), &result, sizeof(result), 0);
	//ReadProcessMemory(phandle, (void*)((unsigned long long)result + 0x4A0), &result, sizeof(result), 0);
	int ans;
	cout << "Please select the hack to be executed" << endl << "1. Sun" << endl << "2. Unlimited sun" << endl << "3. Quit"<< endl << "Ans: " ;
	cin >> ans;
	switch (ans)
	{
	case 1:
		{
		cout << "Input Sun"<< endl;
		int ammodesiredvalue;
		cout << "Enter your desired sun(max value 9500): ";
		cin >> ammodesiredvalue;
		WriteProcessMemory(phandle, (void*)((unsigned long long)result + 0x5560), &ammodesiredvalue, sizeof(ammodesiredvalue), 0);
		break;
		}
	case 2:
	{
		cout << "Unlimited Sun" << endl;
		while(1)
		{
			int unammo = 1000;
			if (GetAsyncKeyState(1) == TRUE)
			{
				break;
			}
			else
			{
				WriteProcessMemory(phandle, (void*)((unsigned long long)result + 0x5560), &unammo, sizeof(unammo), 0);
			}
		}
		break;
	}
	case 3:
	{
		exit;
	}
	return 0;
	}
	
	system("pause");
}