/*
For now isn't used 
Would be finished in v0.3
*/

#include <stdio.h>
#include <stdlib.h>
#include "cJson/cJSON.h"
#include "types.h"
#include <string.h>
#include "tuix_registry.h"

static const char* TUIX_SCENES_CACHE = "cache_scenes.json";

void tuix_cache_scenes() {
    if (scenes.count == 0) {
        return;
    }

    cJSON* root = cJSON_CreateArray();

    for (int i = 0; i < scenes.count; i++) {
        TuixScene* scene = scenes.scenes[i];
        if (!scene) continue;

        cJSON* json_scene = cJSON_CreateObject();

        cJSON_AddStringToObject(json_scene, "name", scenes.names[i]);

        cJSON* json_buffers = cJSON_CreateArray();
        for (int b = 0; b < scene->count; b++) {
            TuixBuffer* buf = scene->buffers[b];
            if (!buf) continue;

            cJSON* json_buf = cJSON_CreateObject();
            cJSON_AddNumberToObject(json_buf, "uid", buf->obj.uid);
            cJSON_AddNumberToObject(json_buf, "id", buf->obj.id);
            cJSON_AddNumberToObject(json_buf, "width", buf->width);
            cJSON_AddNumberToObject(json_buf, "height", buf->height);

            cJSON_AddNumberToObject(json_buf, "width_mod", buf->obj.width_mod);
            cJSON_AddNumberToObject(json_buf, "height_mod", buf->obj.height_mod);

            ////////////////////////
            //   PIXELS -> JSON   //
            ////////////////////////
            cJSON* json_pixels = cJSON_CreateArray();

            for (int y = 0; y < buf->height; y++) {
                cJSON* json_row = cJSON_CreateArray();

                for (int x = 0; x < buf->width; x++) {
                    TuixPixel* px = &buf->pixels[(size_t)y * buf->width + x];

                    cJSON* json_px = cJSON_CreateObject();

                    // Symbol
                    char symstr[2] = { px->sym, '\0' };
                    cJSON_AddStringToObject(json_px, "sym", symstr);

                    // Styles
                    cJSON* js = cJSON_CreateObject();
                    cJSON_AddNumberToObject(js, "fg_r", px->styles.fg.r);
                    cJSON_AddNumberToObject(js, "fg_g", px->styles.fg.g);
                    cJSON_AddNumberToObject(js, "fg_b", px->styles.fg.b);

                    cJSON_AddNumberToObject(js, "bg_r", px->styles.bg.r);
                    cJSON_AddNumberToObject(js, "bg_g", px->styles.bg.g);
                    cJSON_AddNumberToObject(js, "bg_b", px->styles.bg.b);

                    cJSON_AddNumberToObject(js, "bold", px->styles.bold);
                    cJSON_AddNumberToObject(js, "underlined", px->styles.underlined);
                    cJSON_AddNumberToObject(js, "italic", px->styles.italic);
                    cJSON_AddNumberToObject(js, "dim", px->styles.dim);

                    cJSON_AddItemToObject(json_px, "styles", js);

                    cJSON_AddItemToArray(json_row, json_px);
                }

                cJSON_AddItemToArray(json_pixels, json_row);
            }

            cJSON_AddItemToObject(json_buf, "pixels", json_pixels);

            cJSON_AddItemToArray(json_buffers, json_buf);
        }

        cJSON_AddItemToObject(json_scene, "buffers", json_buffers);
        cJSON_AddItemToArray(root, json_scene);
    }

    char* json_data = cJSON_Print(root);
    FILE* f = fopen(TUIX_SCENES_CACHE, "w");
    if (f) {
        fputs(json_data, f);
        fclose(f);
    }

    free(json_data);
    cJSON_Delete(root);
}

static void tuix_append_scene(TuixScene* scene, const char* name) {
    scenes.count += 1;

    scenes.scenes = realloc(
        scenes.scenes,
        sizeof(TuixScene*) * scenes.count
    );
    scenes.names = realloc(
        scenes.names,
        sizeof(char*) * scenes.count
    );

    scenes.scenes[scenes.count - 1] = scene;
    scenes.names[scenes.count - 1] = strdup(name);
}

void tuix_restore_scenes() {
    FILE* f = fopen(TUIX_SCENES_CACHE, "r");
    if (!f) {
        return;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char* data = malloc(size + 1);
    fread(data, 1, size, f);
    data[size] = '\0';
    fclose(f);

    cJSON* root = cJSON_Parse(data);
    free(data);

    if (!root || !cJSON_IsArray(root)) {
        printf("[tuix] Corrupted scene cache.\n");
        return;
    }

    int count = cJSON_GetArraySize(root);

    for (int i = 0; i < count; i++) {
        cJSON* json_scene = cJSON_GetArrayItem(root, i);
        if (!json_scene) continue;

        const char* name = cJSON_GetObjectItem(json_scene, "name")->valuestring;
        cJSON* json_buffers = cJSON_GetObjectItem(json_scene, "buffers");

        int buf_count = cJSON_GetArraySize(json_buffers);

        TuixScene* scene = malloc(sizeof(TuixScene));
        scene->count = buf_count;
        scene->buffers = malloc(sizeof(TuixBuffer*) * buf_count);

        for (int b = 0; b < buf_count; b++) {
            cJSON* jb = cJSON_GetArrayItem(json_buffers, b);

            TuixBuffer* buf = malloc(sizeof(TuixBuffer));
            memset(buf, 0, sizeof(TuixBuffer));

            buf->obj.uid    = cJSON_GetObjectItem(jb, "uid")->valueint;
            buf->obj.id     = cJSON_GetObjectItem(jb, "id")->valueint;
            buf->width      = cJSON_GetObjectItem(jb, "width")->valueint;
            buf->height     = cJSON_GetObjectItem(jb, "height")->valueint;

            buf->obj.width_mod  = cJSON_GetObjectItem(jb, "width_mod")->valuedouble;
            buf->obj.height_mod = cJSON_GetObjectItem(jb, "height_mod")->valuedouble;

            buf->pixels = calloc((size_t)buf->height * buf->width, sizeof(TuixPixel));
            buf->pixels_owned = 1;

            cJSON* json_pixels = cJSON_GetObjectItem(jb, "pixels");

            for (int y = 0; y < buf->height; y++) {
                cJSON* json_row = cJSON_GetArrayItem(json_pixels, y);

                for (int x = 0; x < buf->width; x++) {
                    cJSON* jp = cJSON_GetArrayItem(json_row, x);

                    TuixPixel* px = &buf->pixels[(size_t)y * buf->width + x];

                    const char* sym = cJSON_GetObjectItem(jp, "sym")->valuestring;
                    px->sym = sym[0];

                    cJSON* js = cJSON_GetObjectItem(jp, "styles");

                    // FG
                    px->styles.fg.r = cJSON_GetObjectItem(js, "fg_r")->valueint;
                    px->styles.fg.g = cJSON_GetObjectItem(js, "fg_g")->valueint;
                    px->styles.fg.b = cJSON_GetObjectItem(js, "fg_b")->valueint;

                    // BG
                    px->styles.bg.r = cJSON_GetObjectItem(js, "bg_r")->valueint;
                    px->styles.bg.g = cJSON_GetObjectItem(js, "bg_g")->valueint;
                    px->styles.bg.b = cJSON_GetObjectItem(js, "bg_b")->valueint;

                    px->styles.bold       = cJSON_GetObjectItem(js, "bold")->valueint;
                    px->styles.underlined = cJSON_GetObjectItem(js, "underlined")->valueint;
                    px->styles.italic     = cJSON_GetObjectItem(js, "italic")->valueint;
                    px->styles.dim        = cJSON_GetObjectItem(js, "dim")->valueint;
                }
            }

            scene->buffers[b] = buf;
        }

        tuix_append_scene(scene, name);
    }

    cJSON_Delete(root);
}