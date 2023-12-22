#include "camera.h"
#include "super_header.h"
#include "window.h"

CGLM_ALIGN_MAT mat4 g_mat_projection = GLM_MAT4_IDENTITY_INIT;
CGLM_ALIGN_MAT mat4 g_mat_view = GLM_MAT4_IDENTITY_INIT;
CGLM_ALIGN_MAT mat4 g_mat_view_project = GLM_MAT4_IDENTITY_INIT;

CGLM_ALIGN_MAT mat4 g_mat_view_reset = GLM_MAT4_IDENTITY_INIT;

int g_cam_movement_flags = 0;
float g_camera_yaw = 0.0f;
float g_camera_pitch = 0.0f;
vec3 g_camera_position = { 0, 8, 25 };
POINT initial_cursor_position;

void init_camera()
{
    glm_perspective(glm_rad(60.0f), 1024.0f / 768.0f, 0.1f, 100.0f, g_mat_projection);
    //glm_perspective(glm_rad(60.0f), 800.0f / 600.0f, 0.0f, 65535.0f, g_mat_projection);
    //glm_look(vec3{ 0, 8,25 }, vec3{ 0,0, -1 }, vec3{ 0, -1,0 }, g_mat_camera);
    glm_look(g_camera_position, vec3{ 0, 0, -1 }, vec3{ 0, -1,0 }, g_mat_view);
    memcpy(g_mat_view_reset, g_mat_view, sizeof(mat4));
    glm_mat4_mul(g_mat_projection, g_mat_view, g_mat_view_project);

    GetCursorPos(&initial_cursor_position);
}

void camera_process_keys(int msg, int key)
{
    if (key == 'W')
    {
        if (msg == WM_KEYDOWN)
            g_cam_movement_flags |= camera_movement_flags::cam_move_forward;
        else if (msg == WM_KEYUP)
            g_cam_movement_flags &= ~camera_movement_flags::cam_move_forward;
    }

    if (key == 'S')
    {
        if (msg == WM_KEYDOWN)
            g_cam_movement_flags |= camera_movement_flags::cam_move_backward;
        else if (msg == WM_KEYUP)
            g_cam_movement_flags &= ~camera_movement_flags::cam_move_backward;
    }

    if (key == 'A')
    {
        if (msg == WM_KEYDOWN)
            g_cam_movement_flags |= camera_movement_flags::cam_move_left;
        else if (msg == WM_KEYUP)
            g_cam_movement_flags &= ~camera_movement_flags::cam_move_left;
    }

    if (key == 'D')
    {
        if (msg == WM_KEYDOWN)
            g_cam_movement_flags |= camera_movement_flags::cam_move_right;
        else if (msg == WM_KEYUP)
            g_cam_movement_flags &= ~camera_movement_flags::cam_move_right;
    }

    if (key == 'Z' || key == 'Q')
    {
        if (msg == WM_KEYDOWN)
            g_cam_movement_flags |= camera_movement_flags::cam_move_down;
        else if (msg == WM_KEYUP)
            g_cam_movement_flags &= ~camera_movement_flags::cam_move_down;
    }

    if (key == VK_SPACE || key == 'E')
    {
        if (msg == WM_KEYDOWN)
            g_cam_movement_flags |= camera_movement_flags::cam_move_up;
        else if (msg == WM_KEYUP)
            g_cam_movement_flags &= ~camera_movement_flags::cam_move_up;
    }
    if (key == 'R')
    {
        g_camera_yaw = 0.0f;
        g_camera_pitch = 0.0f;
        glm_vec3_copy(vec3{ 0.0f, 8.0f, 25 }, g_camera_position);

    }
}

void camera_update_control(float move_speed = 1.0f)
{
    // 1. mouse update rotation for camera
    const float mouse_sensitivity = 0.15f;

    static vec3 forward;
    static vec3 backward;
    static vec3 up = { 0.0f, -1.0f, 0.0f };
    static vec3 down;
    static vec3 right;
    static vec3 left;

    vec2 diff = { g_mouse_delta_x , g_mouse_delta_y };
    float deltaLen = sqrtf(glm_vec2_dot(diff, diff));

    if (deltaLen > 0.1f)
    {
        //printf("debug mouse move: delta move len = %0.2f diff = %0.2f %0.2f\n", len, diff[0], diff[1]);

        g_camera_yaw += mouse_sensitivity * diff[0] * 0.02f;
        g_camera_pitch -= mouse_sensitivity * diff[1] * 0.02f;
        g_camera_pitch = glm_clamp(g_camera_pitch, glm_rad(-89.0f), glm_rad(89.0f));
        mat4 dest = GLM_MAT4_IDENTITY_INIT;
        glm_euler_xyz(vec3{ g_camera_pitch, g_camera_yaw, 0 }, dest);
        
        versor q;
        glm_mat4_quat(dest, q);
        glm_quat_normalize(q);

        vec3 forward_world_alligned = { 0, 0, -1 };
        glm_vec3_normalize(forward_world_alligned);
        glm_quat_rotatev(q, forward_world_alligned, forward);
        glm_vec3_normalize(forward);

        glm_vec3_negate_to(forward, backward);
        glm_vec3_cross(forward, up, right);
        glm_vec3_negate_to(right, left);
        glm_vec3_negate_to(up, down);

        //printf("debug g_camera_position vec3: %0.2f %0.2f %0.2f\n", g_camera_position[0], g_camera_position[1], g_camera_position[2]);
        //printf("debug g_camera_yaw float: %0.2f\n", g_camera_yaw);
        //printf("debug g_camera_pitch float: %0.2f\n", g_camera_pitch);
        //printf("debug forward vec3: %0.2f %0.2f %0.2f\n", forward[0], forward[1], forward[2]);
    }

    // 1. update camera transforms with new input
    vec3 movement = GLM_VEC3_ZERO_INIT;

    if (g_cam_movement_flags & camera_movement_flags::cam_move_forward) 
    {
        glm_vec3_add(movement, forward, movement);
    }

    if (g_cam_movement_flags & camera_movement_flags::cam_move_backward)
    {
        glm_vec3_add(movement, backward, movement);
    }

    if (g_cam_movement_flags & camera_movement_flags::cam_move_left)
    {
        glm_vec3_add(movement, left, movement);
    }

    if (g_cam_movement_flags & camera_movement_flags::cam_move_right)
    {
        glm_vec3_add(movement, right, movement);
    }

    if (g_cam_movement_flags & camera_movement_flags::cam_move_up)
    {
        glm_vec3_add(movement, up, movement);
    }

    if (g_cam_movement_flags & camera_movement_flags::cam_move_down)
    {
        glm_vec3_add(movement, down, movement);
    }

    glm_vec3_add(g_camera_position, movement, g_camera_position);
    glm_look(g_camera_position, forward, up, g_mat_view);
    
    //glm_lookat_rh(g_camera_position, forward, up, g_mat_view);
    //glm_look_anyup(g_camera_position, forward, up, g_mat_view);
    //glm_translate(g_mat_view, movement);

    // update every frame g_mat_view_project matrix, we use it to transform geometry on scene
    glm_mat4_mul(g_mat_projection, g_mat_view, g_mat_view_project);

    // consume movement
    g_mouse_delta_x = 0;
    g_mouse_delta_y = 0;
}
