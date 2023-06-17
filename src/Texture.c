#include "Texture.h"
#include "pd_api.h"
#include <string.h>
#include <stdio.h>

Texture* LoadTexture(PlaydateAPI* pd, const char* dir)
{
    Texture* t = pd->system->realloc(NULL, sizeof(Texture));
    for (int i = 0; i != TEXTURE_WIDTH; ++i) {
        //generate the directory
        int len = strlen(dir);
        char* filename = pd->system->realloc(NULL, len + 7);
        strcpy(filename, dir);

        char buff[10] = { 0 };
        sprintf(buff, "_%d.png", i + 1);
        strcpy(filename+len, buff);

        char* err;
        LCDBitmap* bmp = pd->graphics->loadBitmap(filename, &err);
        if (bmp == NULL) {
            pd->system->logToConsole("Unable to find texture '%s'", filename);
            // free already loaded textures
            --i;
            while (i >= 0) {
                pd->graphics->freeBitmap(t->bmps[i]);
                --i;
            }
            // free the texture itself
            pd->system->realloc(t, 0);
            return NULL;
        }
        t->bmps[i] = bmp;
    }
    return t;
}

void DeleteTexture(PlaydateAPI* pd, Texture* texture) {
    for (int i = 0; i != TEXTURE_WIDTH; ++i) {
        // free the bmps
    }
    pd->system->realloc(texture, 0);
}
