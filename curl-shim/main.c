#include <windows.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char cmd[MAX_PATH * 4] = {0};
    char *curl_path = "C:\\Windows\\System32\\curl.exe";
    strcat(cmd, "\"");
    strcat(cmd, curl_path);
    strcat(cmd, "\" -k");
    for (int i = 1; i < argc; ++i) {
        strcat(cmd, " ");
        strcat(cmd, argv[i]);
    }
    // Run the real curl.exe with -k and all user args
    STARTUPINFOA si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb = sizeof(si);
    if (!CreateProcessA(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        fprintf(stderr, "Failed to launch curl.exe\n");
        return 1;
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD exitCode = 0;
    GetExitCodeProcess(pi.hProcess, &exitCode);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return exitCode;
}
