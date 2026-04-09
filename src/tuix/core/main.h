#ifndef TUIX_main_H
#define TUIX_main_H

#include "types.h"

typedef struct TuixCoreLoopStats {
	unsigned long long frame_counter;
	double batch_commit_ms;
	double cache_refresh_ms;
	double input_ms;
	double routing_ms;
	double buffer_ms;
	double composite_ms;
	double python_commit_ms;
	double total_ms;
	int composite_skipped;
	int composite_forced;
	int traversal_cache_hit;
	int traversal_cache_miss;
	int size_changed;
	int has_input;
	int has_scene;
	int has_subcycles;
} TuixCoreLoopStats;

void tuix_main_loop(void);
void tuix_get_terminal_size(int* width, int* height);
void tuix_get_core_loop_stats(TuixCoreLoopStats* out_stats);

#endif