#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "list.h"

void lookup(list *file_list, boolean *flag);
void harakiri();

int main() {
    list file_list;
    init_list(&file_list);
    boolean flag = 0;
    lookup(&file_list, &flag);
    if(flag) {
        
    }
    //harakiri(); CUIDADO! SOLO DESCOMENTAR CUANDO EL .exe FUNCIONE CORRECTAMENTE
    return 0;
}

void lookup(list *file_list, boolean *flag) {
    char exePath[MAX_PATH];
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);

    // Obtener el nombre del ejecutable
    const char *exeName = strrchr(exePath, '\\');
    if (exeName) {
        exeName++; // Saltar el '\'
    } else {
        exeName = exePath;
    }

    // Copiar el path original y recortarlo para quedarnos con la carpeta
    strcpy(path, exePath);
    for (int i = strlen(path) - 1; i >= 0; --i) {
        if (path[i] == '\\') {
            path[i + 1] = '\0';
            break;
        }
    }

    // Construir patrón de búsqueda
    char searchPath[MAX_PATH];
    snprintf(searchPath, MAX_PATH, "%s*", path);

    WIN32_FIND_DATAA findFileData;
    HANDLE hFind = FindFirstFileA(searchPath, &findFileData);

    *flag = 0; // Inicialmente asumimos que no se encontraron archivos

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (strstr(findFileData.cFileName, ".exe") != NULL && strcmp(findFileData.cFileName, exeName) != 0) {
                add_node(file_list, findFileData.cFileName);
                *flag = 1;
            }
        } while (FindNextFileA(hFind, &findFileData));
        FindClose(hFind);
    }
}

void harakiri() {
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);

    // Comando: espera un poco (ping) y luego borra el ejecutable
    char commandLine[MAX_PATH * 2];
    snprintf(commandLine, sizeof(commandLine),
        "cmd.exe /C ping 127.0.0.1 -n 2 > nul && del \"%s\"", exePath);

    STARTUPINFOA si = { 0 };
    PROCESS_INFORMATION pi = { 0 };
    si.cb = sizeof(si);

    BOOL success = CreateProcessA(
        NULL,           // No hay nombre de ejecutable (se toma del cmdline)
        commandLine,    // Línea de comando
        NULL, NULL,     // Seguridad (por defecto)
        FALSE,          // No heredar handles
        CREATE_NO_WINDOW, // No mostrar ventana de consola
        NULL,           // Entorno
        NULL,           // Directorio actual
        &si, &pi        // Info de inicio y proceso
    );

    if (success) {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    } else {
        fprintf(stderr, "No se pudo crear el proceso de autodestrucción.\n");
    }
}