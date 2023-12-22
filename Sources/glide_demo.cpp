#include "glide_demo.h"

#include "glide_utils.h"
#include "matrix.h"
#include "camera.h"
#include "FxCustomMesh.h"
#include "vertex_structs.h"
#include "FxTexture.h"
#include "FxStaticModel.h"
#include "framestat.h"

GrContext_t context = 0;
bool done = false;

FxI32 wLimits[2];

FxCustomMesh mg_plane;
FxCustomMesh mg_tris;
FxStaticModel model;
FxStaticModel modelBox;


FxTexture tex1;
FxTexture tex2;
FxTexture tex3;


float rot = 0.0f;
FxI32 grBufferNumPending();

// ClipAndDrawTriangle
// https://github.com/Madsy/Computer-Graphics-Explained/blob/master/clipping/clipplane.cpp
// https://www.itcodet.com/cpp/cpp-clip_triangle-function-examples.html
// https://www.gamers.org/dEngine/xf3D/glide/glidepgm.htm glide2.2v spec
// https://cglm.readthedocs.io/en/latest/features.html CGLM docs
// https://github.com/Madsy/Computer-Graphics-Explained/tree/master/clipping clippnig tris example
// https://github.com/Haggarman/Software-3D-Perspective-Correct-Textured-Triangles/blob/main/Concepts/NearFrustumClipTriangleAttributes.bas clippnig tris example
// https://github.com/alextrevisan/3dfx-glide-game-engine/blob/master/src/engine/GlideEngine.cpp

// http://blog.simonrodriguez.fr/articles/2017/02/writing_a_small_software_renderer.html // clip example
// https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/overview-rasterization-algorithm.html

// https://sourceforge.net/p/psvoodoo/psVoodoo/ci/master/tree/ // psVoodoo psVoodoo


void glideErrorCallback (const char* string, FxBool fatal)
{
    LOG(string);
}

int Start(HWND hwin)
{
    FxI32 n;
    // Check for available boards
    grGet(GR_NUM_BOARDS, sizeof(n), &n);
    // if none are found
    if (n == 0)
    {
        LOG("ERROR: no 3Dfx Interactive Graphics Accelerator!");
        return 0;
    } 
    else { LOGn("Find Device:", n); }

    CheckDeviceInfo();
    CheckForSupportedResolutions();

    // Set up Glide library
    grGlideInit();
    // Pick the first available 3dfx based card
    grSstSelect(0);

    // Reset this card, open a rendering context
    context = grSstWinOpen((FxU32)hwin,				    // Handle
                           GR_RESOLUTION_1024x768,		// Resolution
                           GR_REFRESH_60Hz,			    // Refresh Rate
                           GR_COLORFORMAT_RGBA,		    // Color byte ordening
                           GR_ORIGIN_UPPER_LEFT,		// (0,0) orgin
                           2,							// Double buffered
                           1);                          // Z (16bit integer) or W(16 bit float) or Alpha buffer (can be only 0 or 1)

    if (context == NULL) return 0;

    grErrorSetCallback(&glideErrorCallback);
    //grEnable(GR_VIDEO_SMOOTHING);
    //grEnable(GR_SHAMELESS_PLUG);
    //grDitherMode(GR_DITHER_DISABLE);
    //grDitherMode(GR_DITHER_4x4);

    // Define our vertex layout
    //grVertexLayout(GR_PARAM_XY,     0,  GR_PARAM_ENABLE);	// Since we're into clipping coords, this the geometric X,Y
    ////grVertexLayout(GR_PARAM_Z,      8,  GR_PARAM_ENABLE);	// 
    //grVertexLayout(GR_PARAM_W,      8,  GR_PARAM_ENABLE);	// Geometric depth (what you would call Z)
    //grVertexLayout(GR_PARAM_Q,      12, GR_PARAM_ENABLE);	// 1 / Geometric depth
    //grVertexLayout(GR_PARAM_PARGB,  16, GR_PARAM_ENABLE);	// Color
    SetupVertexLayout();
    //setupTextures();

    //grGet(GR_GLIDE_VERTEXLAYOUT_SIZE, sizeof(FxU32), &vertexLayout4TrisSize);
    //vertexLayout4Tris = (FxU8*)calloc(vertexLayout4TrisSize, 1);
    //grGlideGetVertexLayout(vertexLayout4Tris); // when we get vl we need reset it back
    //grGlideSetVertexLayout(vertexLayout4Tris);

    /* Set up Render State - iterated alpha + constant color */
    //grColorCombine(GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_LOCAL, GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_TEXTURE, FXFALSE);
    //grColorCombine(GR_COMBINE_FUNCTION_LOCAL,
    //    GR_COMBINE_FACTOR_NONE,
    //    GR_COMBINE_LOCAL_CONSTANT,
    //    GR_COMBINE_OTHER_NONE,
    //    FXFALSE);

    //grAlphaCombine(GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE, GR_COMBINE_LOCAL_ITERATED,GR_COMBINE_OTHER_NONE, FXFALSE);
    //grAlphaBlendFunction(GR_BLEND_SRC_ALPHA, GR_BLEND_ONE_MINUS_SRC_ALPHA, GR_BLEND_ZERO, GR_BLEND_ZERO);

    // Enable clip coordinates... no need for a z-range, we're going to use w-buffering
    grCoordinateSpace(GR_CLIP_COORDS);
    //grClipWindow(0, 0, 800, 600);
    //grViewport(0, 0, 800, 600);

    //grClipWindow(0, 0, 1024, 768);
    grViewport(0, 0, 1024, 768);

    grGet(GR_WDEPTH_MIN_MAX, 8, wLimits); //[0, 65535]

    // Linear z buffering is enabled by calling grDepthBufferMode() with the constant GR_DEPTHBUFFER_ZBUFFER.
    // grDepthBufferMode(GR_DEPTHBUFFER_ZBUFFER); // the depth buffer is a 16 - bit fixed point z buffer
    
    /*
        Floating Point w Buffering
        The graphics hardware can also derive a depth value from the q/w factor computed for texture mapping
        and fog. Such an approach has many advantages over linear z buffering, including much greater
        dynamic range and less aliasing and accuracy artifacts. The graphics hardware uses a proprietary 16-
        bit floating point format for w buffering, however, an application typically does not need to manipulate
        this data directly, except when an application must read data directly from the depth buffer and then
        convert this depth value to an application dependent format. Floating point w buffering is enabled by
        calling grDepthBufferMode() with the constant GR_DEPTHBUFFER_WBUFFER.   
    */

    grDepthBufferMode(GR_DEPTHBUFFER_WBUFFER); // A 16-bit floating point w buffer is used
    grDepthBufferFunction(GR_CMP_LESS);
    grDepthMask(FXTRUE);


    FxTexture::InitTextureMemManager();

    init_camera();
    //grDepthRange(0.01, 1.0f);

    mg_plane.Begin(GR_TRIANGLES);
    glm_scale(mg_plane.m_object_transform, vec3{ 10,10,10 });
    //mg_plane.add_vertex(vec3{ 1, 0, 1 }, 0xFFFFFF00, vec2{ 1.0f, 1.0f });     //yellow
    //mg_plane.add_vertex(vec3{ 1, 0, -1 }, 0xFF00FF00, vec2{ 1.0f, 0.0f });    // green
    //mg_plane.add_vertex(vec3{ -1, 0, -1 }, 0xFFFF0000, vec2{ 0.0f, 0.0f });   // red

    ////mg.add_vertex(vec3{ 0, 0, 0 }, 0xFFFFFFFF);
    //mg_plane.add_vertex(vec3{ -1, 0, -1 }, 0xFFFF0000, vec2{ 0.0f, 0.0f });   //red
    //mg_plane.add_vertex(vec3{ -1, 0, 1 }, 0xFFFFFFFF, vec2{ 0.0f, 1.0f });    //white
    //mg_plane.add_vertex(vec3{ 1, 0, 1 }, 0xFFFFFF00, vec2{ 1.0f, 1.0f });     //yellow

    mg_plane.AddVertex(vec3{ 1, 0, 1 },    GrColor{255,255,255,255},   vec2{ 1.0f, 1.0f });     //yellow
    mg_plane.AddVertex(vec3{ 1, 0, -1 },   GrColor{ 255,255,255,255 }, vec2{ 1.0f, 0.0f });    // green
    mg_plane.AddVertex(vec3{ -1, 0, -1 },  GrColor{ 255,255,255,255 }, vec2{ 0.0f, 0.0f });   // red

    //mg.add_vertex(vec3{ 0, 0, 0 }, 0xFFFFFFFF);
    mg_plane.AddVertex(vec3{ -1, 0, -1 },  GrColor{ 255,255,255,255 }, vec2{ 0.0f, 0.0f });   //red
    mg_plane.AddVertex(vec3{ -1, 0, 1 },   GrColor{ 255,255,255,255 }, vec2{ 0.0f, 1.0f });    //white
    mg_plane.AddVertex(vec3{ 1, 0, 1 },    GrColor{ 255,255,255,255 }, vec2{ 1.0f, 1.0f });     //yellow

    mg_tris.Begin(GR_TRIANGLES);
    mg_tris.AddVertex(vec3{ 0, 1, 0 },     GrColor{ 255,255,255,255 });     //yellow
    mg_tris.AddVertex(vec3{ 1, -1, 0 },    GrColor{ 255,255,255,255 });    // white
    mg_tris.AddVertex(vec3{ -1, -1, 0 },   GrColor{ 255,255,255, 0 });   // red

    //model.load("Assets/box.obj");
    model.Load("Assets/sphere.obj");
    modelBox.Load("Assets/box.obj");

    tex1.Load("Assets/3dfxLogo_256.3df");
    tex2.Load("Assets/Grass_01.3df");
    tex3.Load("Assets/whitePuff24.3df");

    // The default alpha combine unit configuration is
    //grAlphaCombine(GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_ONE, GR_COMBINE_LOCAL_NONE, GR_COMBINE_OTHER_CONSTANT, FXFALSE);
    grColorCombine(GR_COMBINE_FUNCTION_BLEND, GR_COMBINE_FACTOR_TEXTURE_ALPHA, GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_TEXTURE, FXFALSE);
    
    // Sending "Enter + Alt" keys to switch emulator to window mode
    // INPUT in[2];
    // in[0].type = INPUT_KEYBOARD;
    // in[0].ki.wVk = 0;
    // in[0].ki.wScan = VK_EXECUTE;
    // in[0].ki.dwFlags = KEYEVENTF_UNICODE;
    // in[0].ki.time = 0;
    // in[0].ki.dwExtraInfo = GetMessageExtraInfo();

    // in[1].type = INPUT_KEYBOARD;
    // in[1].ki.wVk = 0;
    // in[1].ki.wScan = VK_RMENU;
    // in[1].ki.dwFlags = KEYEVENTF_UNICODE;
    // in[1].ki.time = 100;
    // in[1].ki.dwExtraInfo = GetMessageExtraInfo();

    // SendInput(1, &in[0], sizeof(INPUT));

    // SETUP FOG
    GrFog_t* fogtable = NULL;
    FxI32                ftsize = 0;
    grGet(GR_FOG_TABLE_ENTRIES, 4, &ftsize);
    fogtable = (GrFog_t*)malloc(sizeof(GrFog_t) * ftsize);

    grFogMode(GR_FOG_MULT2);
    grFogColorValue(0xFFFF0000);
    //guFogGenerateExp2(fogtable, 0.9f);
    guFogGenerateLinear(fogtable, 0, 10.0f);

    grFogTable(fogtable);


    return 1;
}

void End()
{
    done = true;
    grSstWinClose(context);
    // Correctly terminates Glide
    grGlideShutdown();
}

int Update()
{   

    FrameStatisticReset();
    camera_update_control(1.0f);

    // RGB
    //GrColor_t color = (16 << 16) | (16 << 8) | (128);
    GrColor_t color = 0x000008000; //GR_COLORFORMAT_RGBA

    //grBufferClear(color, 0, 0xffff);
    grBufferClear(color, 0, wLimits[1]);
    rot += 1.0f;
    // https://cglm.readthedocs.io/en/latest/affine.html
    //------> HERE DRAW OPAQUE GEOM FIRST
    //  
    //// The default color combine mode is
    //grColorCombine(GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_ONE, GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_ITERATED, FXFALSE);
    //// The default alpha combine unit configuration is
    //grAlphaCombine(GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_ONE, GR_COMBINE_LOCAL_NONE, GR_COMBINE_OTHER_CONSTANT, FXFALSE);

    //grColorCombine(GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_ONE, GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_TEXTURE, FXFALSE);
    //grAlphaCombine(GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA, GR_COMBINE_FACTOR_ZERO, GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_ITERATED, FXFALSE);


    grCullMode(GR_CULL_NEGATIVE);

    /* Set up Render State - gouraud shading */
    grColorCombine(GR_COMBINE_FUNCTION_SCALE_OTHER,
        GR_COMBINE_FACTOR_LOCAL,
        GR_COMBINE_LOCAL_ITERATED,
        GR_COMBINE_OTHER_TEXTURE,
        FXFALSE);
    
   
    mg_plane.ViewProjectTransform(g_mat_view_project);
    //grCullMode(GR_CULL_NEGATIVE);
    tex2.SetTexture();
    mg_plane.Draw();
    //grCullMode(GR_CULL_DISABLE);

    glm_mat4_identity(model.m_object_transform);
    glm_translate(model.m_object_transform, vec3{ 0.0f, 0.0f, -25.0f });
    glm_scale(model.m_object_transform, vec3{ 10,10,10 });
    glm_rotate(model.m_object_transform, glm_rad(rot), vec3{ 0.0f, 1.0f, 1.0f });
    model.ViewProjectTransform(g_mat_view_project);
    tex1.SetTexture();
    model.Draw();

    //------>HERE DRAW TRANSPARENT GEOM SECOND 
    // TODO: Need add sorting for transparent objects. Start rendering from back 2 front
    // TODO: Each object render in 2 passes. First pass cull front tris, second pass cull back tris

    grColorCombine(GR_COMBINE_FUNCTION_SCALE_OTHER,
        GR_COMBINE_FACTOR_LOCAL,
        GR_COMBINE_LOCAL_ITERATED,
        GR_COMBINE_OTHER_TEXTURE,
        FXFALSE);
    grAlphaCombine(GR_COMBINE_FUNCTION_SCALE_OTHER,
        GR_COMBINE_FACTOR_LOCAL,
        GR_COMBINE_LOCAL_ITERATED,
        GR_COMBINE_OTHER_TEXTURE,
        FXFALSE);
    grTexCombine(GR_TMU0,
        GR_COMBINE_FUNCTION_LOCAL,
        GR_COMBINE_FACTOR_NONE,
        GR_COMBINE_FUNCTION_LOCAL,
        GR_COMBINE_FACTOR_NONE,
        FXFALSE,
        FXFALSE);
    grAlphaBlendFunction(GR_BLEND_SRC_ALPHA, GR_BLEND_ONE_MINUS_SRC_ALPHA,
        GR_BLEND_ONE, GR_BLEND_ZERO);

    grAlphaTestFunction(GR_CMP_GREATER);
    grAlphaTestReferenceValue(8);

    grCullMode(GR_CULL_DISABLE);

    glm_mat4_identity(mg_tris.m_object_transform);
    glm_translate(mg_tris.m_object_transform, vec3{ 0.0f, 4.0f, 0.0f });
    glm_scale(mg_tris.m_object_transform, vec3{ 10,10,10 });
    glm_rotate(mg_tris.m_object_transform, glm_rad(rot), vec3{ 0.0f, 1.0f, 0.0f });
    mg_tris.ViewProjectTransform(g_mat_view_project);
    mg_tris.Draw();


    //grColorCombine(GR_COMBINE_FUNCTION_SCALE_OTHER,
    //    GR_COMBINE_FACTOR_ONE,
    //    GR_COMBINE_LOCAL_ITERATED,
    //    GR_COMBINE_OTHER_TEXTURE,
    //    FXFALSE);
    //grAlphaCombine(GR_COMBINE_FUNCTION_SCALE_OTHER,
    //    GR_COMBINE_FACTOR_ONE,
    //    GR_COMBINE_LOCAL_ITERATED,
    //    GR_COMBINE_OTHER_TEXTURE,
    //    FXFALSE);
    //grTexCombine(GR_TMU0,
    //    GR_COMBINE_FUNCTION_LOCAL,
    //    GR_COMBINE_FACTOR_NONE,
    //    GR_COMBINE_FUNCTION_LOCAL,
    //    GR_COMBINE_FACTOR_NONE,
    //    FXFALSE,
    //    FXFALSE);
    //grAlphaBlendFunction(GR_BLEND_SRC_ALPHA, GR_BLEND_ONE_MINUS_SRC_ALPHA,
    //    GR_BLEND_ONE, GR_BLEND_ZERO);

    //grAlphaTestFunction(GR_CMP_ALWAYS);
    //grAlphaTestReferenceValue(0x1);

    glm_mat4_identity(modelBox.m_object_transform);
    glm_translate(modelBox.m_object_transform, vec3{ 0.0f, 0.0f, -40.0f });
    glm_scale(modelBox.m_object_transform, vec3{ 10,10,10 });
    glm_rotate(modelBox.m_object_transform, glm_rad(-rot), vec3{ 0.0f, 1.0f, 0.0f });
    modelBox.ViewProjectTransform(g_mat_view_project);
    tex3.SetTexture();
    grCullMode(GR_CULL_POSITIVE);
    modelBox.Draw();
    grCullMode(GR_CULL_NEGATIVE);
    modelBox.Draw();



    grBufferSwap(1);

    while (grBufferNumPending() > 0) { Sleep(0); };
    return 1;
 }

void Input(int msg, int key)
{
    camera_process_keys(msg, key);

    if (key == 'I')
    {
        printf("RenderStatistic: tris_rendered %d\n", g_render_stat.tris_rendered);
        printf("RenderStatistic: tris_clipped %d\n", g_render_stat.tris_clipped);
    }
    
}

void Mouse(int button, int state, int x, int y) 
{
 
}

FxI32 grBufferNumPending()
{
    FxI32 t;
    grGet(GR_PENDING_BUFFERSWAPS, sizeof(t), &t);
    return t;
}
