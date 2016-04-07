
#include <cstdio>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>

void main()
{
	LPTSTR cmdline;
	BOOL ret;
	STARTUPINFO starti = { sizeof(starti) };
	PROCESS_INFORMATION proci;
	LPWSTR* argv;
	int argc;
	LPWSTR wssz;
	int wssi;

	printf( "starting up\n" );

	cmdline = GetCommandLine();

	wprintf( TEXT("command-line [%s]\n"), cmdline );

	argv = CommandLineToArgvW( cmdline, &argc );

	printf( "got [%d] args\n", argc );

	if( argc < 3 ) 
	{
		printf( "need at least two arguments.\n" );
		return;
	}

	cmdline += _tcslen( argv[0] );
	cmdline ++;
	cmdline += _tcslen( argv[1] );
	cmdline = _tcschr( cmdline, ' ' );

	if( !cmdline )
	{
		printf( "could not locate start of pass through commands.\n" );
		return;
	}

	cmdline ++;
	wprintf( TEXT("cmdline pass through [%s]\n"), cmdline );

	wssz = argv[1];
	wssi = _tstoi( wssz );

	if( 0 >= wssi )
	{
		printf( "couldn't make sense of WS size (first) argument, should be at least 10" );
		return;
	}

	if( 'K' == wssz[_tcslen(wssz)-1] )
		wssi *= 1024;
	else
	if( 'M' == wssz[_tcslen(wssz)-1] )
		wssi *= 1024 * 1024;
	else
		wssi *= 4 * 1024; /* pages */

	printf( "wssi solution [%i]\n", wssi );

	if( wssi < 10 * 1024 || wssi > 1024 * 1024 * 1024 )
	{
		printf( "WS parameter would be insanity [%i]\n", wssi );
		return;
	}

	ret = CreateProcess(
		NULL,		/* lpApplicationName */
		cmdline,	/* lpCmdLine */
		NULL,		/* lpProcessAttributes */
		NULL,		/* lpThreadAttributes */
		TRUE,		/* bInheritHandles */
		CREATE_SUSPENDED, /* dwCreationFlags */
		NULL,		/* lpEnvironment */
		NULL,		/* lpCurrentDirectory */
		&starti,	/* lpStartupInfo */
		&proci		/* lpProcessInformation */
	);

	if( TRUE != ret )
	{
		printf( "ERROR CreateProcess [%i]\n", GetLastError() );
		return;
	}

	printf( "process is instantiated suspended, fiddling...\n" );

	ret = SetProcessWorkingSetSizeEx( proci.hProcess, wssi, wssi * 2, QUOTA_LIMITS_HARDWS_MIN_DISABLE | QUOTA_LIMITS_HARDWS_MAX_DISABLE );

	if( TRUE == ret )
	{
		printf( ".. letting it out into the wild world, good luck!\n" );
		ResumeThread( proci.hThread );
	}
	else
	{
		printf( "ERROR SetProcessWorkingSetSizeEx [%i]\n", GetLastError() );

		// process hasn't actually executed anything yet, so this should be fine
		TerminateProcess( proci.hProcess, 0 );
	}

	CloseHandle( proci.hProcess );
	CloseHandle( proci.hThread );
}
