#include <windows.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // Build argument list: curl.exe -k [user args]
    char *curl_path = "C:\\Windows\\System32\\curl.exe";
    // Allocate space for argv: curl_path, -k, user args, NULL
    char **args = (char **)malloc(sizeof(char *) * (argc + 2));
    if (!args) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    args[0] = curl_path;
    args[1] = "-k";
    for (int i = 1; i < argc; ++i) {
        args[i + 1] = argv[i];
    }
    args[argc + 1] = NULL;

    // Output the command and arguments
    printf("Executing: %s", curl_path);
    for (int i = 1; i < argc + 1; ++i) {
        printf(" %s", args[i]);
    }
    printf("\n");

    // Use _spawnv to run curl.exe directly with argument array
    int ret = _spawnv(_P_WAIT, curl_path, args);
    if (ret == -1) {
        fprintf(stderr, "Failed to launch curl.exe\n");
        free(args);
        return 1;
    }
    free(args);
    return ret;
}