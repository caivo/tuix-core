#include "../types.h"
#include "../tuix_registry.h"
#include "builders/canvas_builder.h"
#include "builders/text_builder.h"
#include "builders/box_builder.h"
#include "builders/divider_builder.h"
#include "builders/badge_builder.h"
#include "builders/button_builder.h"
#include "builders/tag_builder.h"
#include "builders/status_builder.h"
#include "builders/menu_builder.h"
#include "builders/scroll_container_builder.h"
#include "builders/choice_builder.h"
#include "builders/input_builder.h"
#include "builders/progressbar_builder.h"
#include "builders/stack_builder.h"
#include "builders/splitpane_builder.h"
#include "builders/grid_builder.h"
#include "builders/checkbox_builder.h"
#include "builders/listview_builder.h"
#include "builders/textarea_builder.h"
#include "builders/dialog_builder.h"
#include <stdlib.h>
#include <stdio.h>

int tuix_register_standart_builders() {
    const TuixBuilder* b_canvas      = tuix_canvas_init();
    const TuixBuilder* b_text        = tuix_text_init();
    const TuixBuilder* b_box         = tuix_box_init();
    const TuixBuilder* b_divider     = tuix_divider_init();
    const TuixBuilder* b_badge       = tuix_badge_init();
    const TuixBuilder* b_button      = tuix_button_init();
    const TuixBuilder* b_tag         = tuix_tag_init();
    const TuixBuilder* b_status      = tuix_status_init();
    const TuixBuilder* b_menu        = tuix_menu_init();
    const TuixBuilder* b_scroll      = tuix_scroll_container_init();
    const TuixBuilder* b_choice      = tuix_choice_init();
    const TuixBuilder* b_input       = tuix_input_init();
    const TuixBuilder* b_progressbar = tuix_progressbar_init();
    const TuixBuilder* b_row         = tuix_row_init();
    const TuixBuilder* b_column      = tuix_column_init();
    const TuixBuilder* b_splitpane   = tuix_splitpane_init();
    const TuixBuilder* b_grid        = tuix_grid_init();
    const TuixBuilder* b_checkbox    = tuix_checkbox_init();
    const TuixBuilder* b_listview    = tuix_listview_init();
    const TuixBuilder* b_textarea    = tuix_textarea_init();
    const TuixBuilder* b_dialog      = tuix_dialog_init();

    if (!b_canvas || !b_text || !b_box || !b_divider || !b_badge || !b_button || !b_tag || !b_status || !b_menu || !b_scroll || !b_choice || !b_input || !b_progressbar || !b_row || !b_column || !b_splitpane || !b_grid || !b_checkbox || !b_listview || !b_textarea || !b_dialog) {
        printf("Failed to init standard builders\n");
        return -1;
    }

    int add_count = 21;
    int new_count = tuix_registry.builders.count + add_count;

    TuixBuilder** tmp = realloc(tuix_registry.builders.builders, new_count * sizeof(TuixBuilder*));
    if (!tmp) {
        printf("Failed to allocate builders array\n");
        return -1;
    }
    tuix_registry.builders.builders = tmp;

    tuix_registry.builders.builders[tuix_registry.builders.count++] = (TuixBuilder*)b_canvas;
    tuix_registry.builders.builders[tuix_registry.builders.count++] = (TuixBuilder*)b_text;
    tuix_registry.builders.builders[tuix_registry.builders.count++] = (TuixBuilder*)b_box;
    tuix_registry.builders.builders[tuix_registry.builders.count++] = (TuixBuilder*)b_divider;
    tuix_registry.builders.builders[tuix_registry.builders.count++] = (TuixBuilder*)b_badge;
    tuix_registry.builders.builders[tuix_registry.builders.count++] = (TuixBuilder*)b_button;
    tuix_registry.builders.builders[tuix_registry.builders.count++] = (TuixBuilder*)b_tag;
    tuix_registry.builders.builders[tuix_registry.builders.count++] = (TuixBuilder*)b_status;
    tuix_registry.builders.builders[tuix_registry.builders.count++] = (TuixBuilder*)b_menu;
    tuix_registry.builders.builders[tuix_registry.builders.count++] = (TuixBuilder*)b_scroll;
    tuix_registry.builders.builders[tuix_registry.builders.count++] = (TuixBuilder*)b_choice;
    tuix_registry.builders.builders[tuix_registry.builders.count++] = (TuixBuilder*)b_input;
    tuix_registry.builders.builders[tuix_registry.builders.count++] = (TuixBuilder*)b_progressbar;
    tuix_registry.builders.builders[tuix_registry.builders.count++] = (TuixBuilder*)b_row;
    tuix_registry.builders.builders[tuix_registry.builders.count++] = (TuixBuilder*)b_column;
    tuix_registry.builders.builders[tuix_registry.builders.count++] = (TuixBuilder*)b_splitpane;
    tuix_registry.builders.builders[tuix_registry.builders.count++] = (TuixBuilder*)b_grid;
    tuix_registry.builders.builders[tuix_registry.builders.count++] = (TuixBuilder*)b_checkbox;
    tuix_registry.builders.builders[tuix_registry.builders.count++] = (TuixBuilder*)b_listview;
    tuix_registry.builders.builders[tuix_registry.builders.count++] = (TuixBuilder*)b_textarea;
    tuix_registry.builders.builders[tuix_registry.builders.count++] = (TuixBuilder*)b_dialog;

    return 0;
}
