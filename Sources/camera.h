#pragma once
#include "super_header.h"

extern mat4 g_mat_projection;
extern mat4 g_mat_view;
extern mat4 g_mat_view_project;

enum camera_movement_flags
{
    cam_move_forward = 1,
    cam_move_backward = 2,
    cam_move_left = 4,
    cam_move_right = 8,
    cam_move_up = 16,
    cam_move_down = 32,
    cam_move_max
};

extern int g_cam_movement_flags;
extern float g_camera_yaw;
extern float g_camera_pitch;
extern vec3 g_camera_position;


void init_camera();
void camera_process_keys(int msg, int key);
void camera_update_control(float move_speed);
