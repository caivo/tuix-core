#include "builders_router.h"
#include "../tuix_registry.h"
#include <string.h>
#include "../types.h"

TuixBuilder* tuix_get_builder_by_name(char* name) {
    for (int i = 0; i < tuix_registry.builders.count; i++) {
        if (strcmp(tuix_registry.builders.builders[i]->name, name) == 0) {
            return tuix_registry.builders.builders[i];
        }
    }
    return NULL;
}
