#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <conio.h>

class SunModifier {
private:
	HANDLE hProcess;
	DWORD pid, BaseAddr, SunAddrPoint;

public:
	SunModifier(): hProcess(NULL), pid(0), BaseAddr(0), SunAddrPoint(0) {}
	
	~SunModifier() {
		if (hProcess) {
			CloseHandle(hProcess);
		}
	}

	void FindProcessId() {
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		PROCESSENTRY32W pe;

		pe.dwSize = sizeof(PROCESSENTRY32W);

		if (Process32FirstW(hSnapshot, &pe)) {
			do {
				if (wcscmp(pe.szExeFile, L"PlantsVsZombies.exe") == 0) {
					pid = pe.th32ProcessID;
					break;
				}
			} while (Process32NextW(hSnapshot, &pe));
		}

		CloseHandle(hSnapshot);
	}

	void SetSunValue(DWORD SunValue) {
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

		ReadProcessMemory(hProcess, (LPCVOID)0x6A9EC0, &BaseAddr, sizeof(DWORD), NULL);
		ReadProcessMemory(hProcess, (LPCVOID)(BaseAddr + 0x768), &SunAddrPoint, sizeof(DWORD), NULL);
		WriteProcessMemory(hProcess, (LPVOID)(SunAddrPoint + 0x5560), &SunValue, sizeof(DWORD), NULL);

	}

	void LockSunValue() {
		std::cout << "正在持续锁定阳光值为99999" << std::endl;
		std::cout << "按任意键停止锁定" << std::endl;

		while (!_kbhit()) {
			SetSunValue(99999);
			Sleep(3);
		}
		_getch();
	}
};

void ClearScreen() {
	system("cls");
}

void menu() {
	std::cout << "植物大战僵尸杂交版 阳光修改器" << std::endl;
	std::cout << "1. 设置阳光值" << std::endl;
	std::cout << "2. 无限阳光" << std::endl;
	std::cout << "3. 退出" << std::endl;
}

int main() {
	int choice;
	DWORD SunValue;
	SunModifier modifier;

	modifier.FindProcessId();

	while (true) {
		ClearScreen();
		menu();
		std::cout << "请选择: ";
		std::cin >> choice;

		switch (choice) {
		case 1: 
			std::cout << "请输入阳光值:";
			std::cin >> SunValue;
			modifier.SetSunValue(SunValue);
			break;
		case 2:
			modifier.LockSunValue();
			break;
		case 3:
			return 0;
		}
	}
}