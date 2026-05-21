#include "content_builder/builders_router.h"
#include "buffers.h"

#ifdef __cplusplus
extern "C" {
#endif

int tuix_has_builder(const char* name) {
    TuixBuilder* b = tuix_get_builder_by_name((char*)name);
    return b != NULL;
}

int tuix_has_scene(const char* name) {
    TuixScene* s = tuix_get_scene(name);
    return s != NULL;
}

#ifdef __cplusplus
}
#endif
