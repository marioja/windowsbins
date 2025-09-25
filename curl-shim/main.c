
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Helper to quote arguments for Windows command line
void quote_arg(const char *src, char *dest) {
    int needs_quotes = strchr(src, ' ') || strchr(src, '\t') || strchr(src, '"');
    if (needs_quotes) {
        *dest++ = '"';
        while (*src) {
            if (*src == '"') {
                *dest++ = '\\';
            }
            *dest++ = *src++;
        }
        *dest++ = '"';
        *dest = '\0';
    } else {
        strcpy(dest, src);
    }
}

int main(int argc, char *argv[]) {
    char *curl_path = "C:\\Windows\\System32\\curl.exe";
    // Estimate command line length
    size_t cmd_len = strlen(curl_path) + 10;
    for (int i = 1; i < argc; ++i) {
        cmd_len += strlen(argv[i]) + 4;
    }
    char *cmd = (char *)malloc(cmd_len);
    if (!cmd) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    // Start with quoted curl_path
    char quoted[512];
    quote_arg(curl_path, quoted);
    strcpy(cmd, quoted);
    strcat(cmd, " -k");
    for (int i = 1; i < argc; ++i) {
        strcat(cmd, " ");
        quote_arg(argv[i], quoted);
        strcat(cmd, quoted);
    }
    printf("Executing: %s\n", cmd);

    STARTUPINFOA si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb = sizeof(si);
    if (!CreateProcessA(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        fprintf(stderr, "Failed to launch curl.exe\n");
        free(cmd);
        return 1;
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD exitCode = 0;
    GetExitCodeProcess(pi.hProcess, &exitCode);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    free(cmd);
    return exitCode;
}