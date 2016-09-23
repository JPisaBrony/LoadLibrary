#include <stdio.h>
#include <windows.h>
#include <SDL2/SDL.h>

typedef int (__stdcall *f_sdl_init)(Uint32);
typedef void (__stdcall *f_sdl_quit)();
extern char binary_SDL2_dll_start[];
extern char binary_SDL2_dll_end[];

int main(int argc, char* args[]) {
    char *sdl_dll;
    FILE *dll_file;

    dll_file = fopen("mydll.dll", "wb");

    if(dll_file == NULL) {
        printf("can't write file");
        exit(1);
    }

    for(sdl_dll = binary_SDL2_dll_start; sdl_dll != binary_SDL2_dll_end; sdl_dll++) {
        fputc(*sdl_dll, dll_file);
    }

    fclose(dll_file);

    HINSTANCE dll = LoadLibrary("mydll.dll");

    if(!dll) {
        printf("couldn't load dll");
        exit(1);
    }

    f_sdl_init sdl_init = (f_sdl_init)GetProcAddress(dll, "_SDL_Init");

    if(sdl_init) {
        printf("couldn't load SDL_Init");
        exit(1);
    }

    f_sdl_quit sdl_quit = (f_sdl_quit)GetProcAddress(dll, "_SDL_Quit");

    if(sdl_quit) {
        printf("couldn't load SDL_Quit");
        exit(1);
    }

    sdl_init(SDL_INIT_EVERYTHING);
    sdl_quit();

    return 0;
}
