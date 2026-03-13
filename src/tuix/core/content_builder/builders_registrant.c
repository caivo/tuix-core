#include "../types.h"
#include "../tuix_registry.h"
#include "builders/canvas_builder.h"
#include "builders/choice_builder.h"
#include "builders/input_builder.h"
#include "builders/progressbar_builder.h"
#include <stdlib.h>
#include <stdio.h>

int tuix_register_standart_builders() {
    const TuixBuilder* b_canvas      = tuix_canvas_init();
    const TuixBuilder* b_choice      = tuix_choice_init();
    const TuixBuilder* b_input       = tuix_input_init();
    const TuixBuilder* b_progressbar = tuix_progressbar_init();

    if (!b_canvas || !b_choice || !b_input || !b_progressbar) {
        printf("Failed to init standard builders\n");
        return -1;
    }

    int add_count = 4;
    int new_count = tuix_registry.builders.count + add_count;

    TuixBuilder** tmp = realloc(tuix_registry.builders.builders, new_count * sizeof(TuixBuilder*));
    if (!tmp) {
        printf("Failed to allocate builders array\n");
        return -1;
    }
    tuix_registry.builders.builders = tmp;

    tuix_registry.builders.builders[tuix_registry.builders.count++] = (TuixBuilder*)b_canvas;
    tuix_registry.builders.builders[tuix_registry.builders.count++] = (TuixBuilder*)b_choice;
    tuix_registry.builders.builders[tuix_registry.builders.count++] = (TuixBuilder*)b_input;
    tuix_registry.builders.builders[tuix_registry.builders.count++] = (TuixBuilder*)b_progressbar;

    return 0;
}

