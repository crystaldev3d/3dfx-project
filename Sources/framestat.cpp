#include "framestat.h"

g_render_stat_t g_render_stat = {};

void FrameStatisticReset()
{
	g_render_stat.tris_clipped = 0;
	g_render_stat.tris_rendered = 0;
}
