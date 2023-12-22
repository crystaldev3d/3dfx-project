#ifndef FRAMESTAT_H
#define FRAMESTAT_H
#pragma once

struct g_render_stat_t
{
	int tris_rendered;
	int tris_clipped;
};

extern g_render_stat_t g_render_stat;

void FrameStatisticReset();


#endif /*FRAMESTAT_H*/