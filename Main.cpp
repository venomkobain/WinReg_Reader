#define _WIN32_WINNT 0x0500

#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include <Lmcons.h>
#include <winbase.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>

void read_reg()
{
	HKEY hKey;

	LONG error = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
		NULL,
		KEY_READ,
		&hKey);

	if (error == ERROR_SUCCESS)
	{
		std::string show_OS;
		std::string show_Ver;

		char buffer_OS[128];
		DWORD buffer_OS_size = 128;

		char buffer_Ver[128];
		DWORD buffer_Ver_size = 128;

		char username_buffer[UNLEN + 1];
		DWORD username_buffer_len = UNLEN + 1;

		TCHAR buffer_PC[256] = TEXT("");
		TCHAR szDescription[8][32] = { TEXT("NetBIOS"),
			TEXT("DNS hostname"),
			TEXT("DNS domain"),
			TEXT("DNS fully-qualified"),
			TEXT("Physical NetBIOS"),
			TEXT("Physical DNS hostname"),
			TEXT("Physical DNS domain"),
			TEXT("Physical DNS fully-qualified") };
		int cnf = 0;
		DWORD dwSize = _countof(buffer_PC);

		LONG result = RegQueryValueEx(hKey,
			L"ProductName",
			NULL,
			NULL,
			(LPBYTE)&buffer_OS,
			&buffer_OS_size);

		LONG result_display_ver = RegQueryValueEx(hKey,
			L"DisplayVersion",
			NULL,
			NULL,
			(LPBYTE)&buffer_Ver,
			&buffer_Ver_size);


		if (result == ERROR_SUCCESS)
		{
			GetUserNameA(username_buffer, &username_buffer_len);
			show_OS = std::string(reinterpret_cast<char*>(buffer_OS), reinterpret_cast<char*>(buffer_OS + buffer_OS_size));
			show_Ver = std::string(reinterpret_cast<char*>(buffer_Ver), reinterpret_cast<char*>(buffer_Ver + buffer_Ver_size));
			std::cout << "OS: " << show_OS << std::endl;
			std::cout << "Version: " << show_Ver << std::endl;
			std::cout << "Username: " << std::string(username_buffer) << std::endl;
			
		}
		else
		{
			std::cout << "Error:" << result << std::endl;
		}

		for (cnf = 0; cnf < ComputerNameMax; cnf++)
		{
			if (!GetComputerNameEx((COMPUTER_NAME_FORMAT)cnf, buffer_PC, &dwSize))
			{
				_tprintf(TEXT("GetComputerNameEx failed (%d)\n"), GetLastError());
				return;
			}
			else _tprintf(TEXT("%s: %s\n"), szDescription[cnf], buffer_PC);

			dwSize = _countof(buffer_PC);
			ZeroMemory(buffer_PC, dwSize);
		}
	}
}


int main() 
{
	printf("		PC Information\n");
	printf("------------------------------------------------\n");
	read_reg();
	return 0;
}


