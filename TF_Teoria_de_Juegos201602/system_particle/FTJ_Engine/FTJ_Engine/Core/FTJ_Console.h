#ifndef FTJ_CONSOLE
#define FTJ_CONSOLE

#include <Windows.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>

#include "Log\Log.h"
#include "Log\DXLog.h"

namespace FTJ
{
	class Console
	{
	public:
		static void CreateConsoleWindow()
		{
			int hConHandle;
			long lStdHandle;
			CONSOLE_SCREEN_BUFFER_INFO coninfo;

			FILE *fp;

			// allocate a console for this app

			AllocConsole();

			// set the screen buffer to be big enough to let us scroll text

			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
			coninfo.dwSize.Y = 500;
			SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

			// redirect unbuffered STDOUT to the console

			lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
			hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
			fp = _fdopen(hConHandle, "w");
			*stdout = *fp;
			setvbuf(stdout, NULL, _IONBF, 0);

			// redirect unbuffered STDIN to the console

			lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
			hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
			fp = _fdopen(hConHandle, "r");
			*stdin = *fp;
			setvbuf(stdin, NULL, _IONBF, 0);

			// redirect unbuffered STDERR to the console

			lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
			hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
			fp = _fdopen(hConHandle, "w");
			*stderr = *fp;
			setvbuf(stderr, NULL, _IONBF, 0);

			// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
			// point to console as well
			freopen_s(&fp, "CONIN$", "r", stdin);
			
			freopen_s(&fp, "CONOUT$", "r", stdout);
			
			freopen_s(&fp, "CONOUT$", "r", stderr);


			//freopen("CONIN$", "r", stdin);
			//freopen("CONOUT$", "w", stdout);
			//freopen("CONOUT$", "w", stderr);

			std::ios::sync_with_stdio();

		}

		static void DestroyConsole()
		{
			FreeConsole();
		}

		static void Print(const char* _message)
		{
			std::cout << _message << std::endl;
		}
	};
}
#endif