#include "glide_utils.h"

int CheckDeviceInfo()
{
    std::cout << "GR_HARDWARE :" << grGetString(GR_HARDWARE) << std::endl;
    std::cout << "GR_RENDERER :" << grGetString(GR_RENDERER) << std::endl;
    std::cout << "GR_VENDOR :" << grGetString(GR_VENDOR) << std::endl;
    std::cout << "GR_VERSION :" << grGetString(GR_VERSION) << std::endl;
    std::cout << "GR_EXTENSION :" << grGetString(GR_EXTENSION) << std::endl;

    const int megabyte = 1048576;

    FxI32 num_fb = 0;
    grGet(GR_NUM_FB, sizeof(FxI32), &num_fb);
    LOGn("GR_NUM_FB", num_fb);

    FxI32 num_tmu = 0;
    grGet(GR_NUM_TMU, sizeof(FxI32), &num_tmu);
    LOGn("GR_NUM_TMU", num_tmu);

    FxI32 max_texture_size = 0;
    grGet(GR_MAX_TEXTURE_SIZE, sizeof(FxI32), &max_texture_size);
    LOGn("GR_MAX_TEXTURE_SIZE", max_texture_size);

    FxI32 max_texture_aspect_ratio = 0;
    grGet(GR_MAX_TEXTURE_ASPECT_RATIO, sizeof(FxI32), &max_texture_aspect_ratio);
    LOGn("GR_MAX_TEXTURE_ASPECT_RATIO", max_texture_aspect_ratio);

    FxI32 memory_fb = 0;
    grGet(GR_MEMORY_FB, sizeof(FxI32), &memory_fb);
    LOGns("GR_MEMORY_FB", memory_fb / megabyte, "Mb");

    FxI32 memory_tmu = 0;
    grGet(GR_MEMORY_TMU, sizeof(FxI32), &memory_tmu);
    LOGns("GR_MEMORY_TMU", memory_tmu / megabyte, "Mb");

    FxI32 memory_uma = 0;
    grGet(GR_MEMORY_UMA, sizeof(FxI32), &memory_uma);
    LOGns("GR_MEMORY_UMA", memory_uma / megabyte, "Mb");

    return 1;
}

int CheckForSupportedResolutions(bool withZBuffer)
{
    GrResolution query;
    GrResolution* list;

    int listSize;

    /* find all possible modes that include a z-bufer */

    query.resolution = GR_QUERY_ANY;
    query.refresh = GR_REFRESH_60Hz;
    query.numColorBuffers = 2;
    query.numAuxBuffers = withZBuffer ? 1 : 0;

    listSize = grQueryResolutions(&query, NULL);
    list = (GrResolution*)malloc(listSize);
    grQueryResolutions(&query, list);

    std::string supportedResolution;

    //GrResolution* r;
    for (int i = 0; i < listSize; i++)
    {
        switch (list[i].resolution)
        {
        case GR_RESOLUTION_320x200: supportedResolution.append("GR_RESOLUTION_320x200\n"); break;
        case GR_RESOLUTION_320x240: supportedResolution.append("GR_RESOLUTION_320x240\n"); break;
        case GR_RESOLUTION_400x256: supportedResolution.append("GR_RESOLUTION_400x256\n"); break;
        case GR_RESOLUTION_512x384: supportedResolution.append("GR_RESOLUTION_512x384\n"); break;
        case GR_RESOLUTION_640x200: supportedResolution.append("GR_RESOLUTION_640x200\n"); break;
        case GR_RESOLUTION_640x350: supportedResolution.append("GR_RESOLUTION_640x350\n"); break;
        case GR_RESOLUTION_640x400: supportedResolution.append("GR_RESOLUTION_640x400\n"); break;
        case GR_RESOLUTION_640x480: supportedResolution.append("GR_RESOLUTION_640x480\n"); break;
        case GR_RESOLUTION_800x600: supportedResolution.append("GR_RESOLUTION_800x600\n"); break;
        case GR_RESOLUTION_856x480: supportedResolution.append("GR_RESOLUTION_856x480\n"); break;
        case GR_RESOLUTION_512x256: supportedResolution.append("GR_RESOLUTION_512x256\n"); break;
        case GR_RESOLUTION_1024x768: supportedResolution.append("GR_RESOLUTION_1024x768\n"); break;
        case GR_RESOLUTION_1280x1024: supportedResolution.append("GR_RESOLUTION_1280x1024\n"); break;
        case GR_RESOLUTION_1600x1200: supportedResolution.append("GR_RESOLUTION_1600x1200\n"); break;
        case GR_RESOLUTION_400x300: supportedResolution.append("GR_RESOLUTION_400x300\n"); break;
        {
        };
        default:
            break;
        }

    }

    if (list != NULL) { free(list); };

    std::cout << supportedResolution << std::endl;

    return 1;
}