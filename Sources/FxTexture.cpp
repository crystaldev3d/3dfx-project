#include "FxTexture.h"

#include "matrix.h"
#include "super_header.h"


FxU32 FxTexture::nextTextureTMU[2] = {};
FxU32 FxTexture::lastTextureTMU[2] = {};

static GrTexTable_t texTableType(GrTextureFormat_t format) {
    GrTexTable_t rv = (GrTexTable_t)NO_TABLE;
    switch (format) {
    case GR_TEXFMT_YIQ_422:
    case GR_TEXFMT_AYIQ_8422:
        rv = GR_TEXTABLE_NCC0;
        break;
    case GR_TEXFMT_P_8:
    case GR_TEXFMT_AP_88:
        rv = GR_TEXTABLE_PALETTE;
        break;
    }
    return rv;
}

int tlLoadTexture(const char* filename, GrTexInfo* info, GrTexTable_t* tableType, void* table)
{
    Gu3dfInfo tdfInfo;
    int rv = 0;

    assert(filename);
    assert(info);
    assert(tableType);
    assert(table);
    if (gu3dfGetInfo(filename, &tdfInfo)) {
        tdfInfo.data = malloc(tdfInfo.mem_required);
        assert(tdfInfo.data);
        if (gu3dfLoad(filename, &tdfInfo)) {
            info->smallLodLog2 = tdfInfo.header.small_lod;
            info->largeLodLog2 = tdfInfo.header.large_lod;
            info->aspectRatioLog2 = tdfInfo.header.aspect_ratio;
            info->format = tdfInfo.header.format;
            info->data = tdfInfo.data;
            *tableType = texTableType(info->format);
            switch (*tableType) {
            case GR_TEXTABLE_NCC0:
            case GR_TEXTABLE_NCC1:
            case GR_TEXTABLE_PALETTE:
                memcpy(table, &(tdfInfo.table), sizeof(TlTextureTable));
                break;
            default:
                break;
            }
            rv = 1;
        }
    }
    return rv;
}

long FxTexture::getStartAddress(FxU32 evenOdd, GrTexInfo* info, GrChipID_t tmu)
{
#define TEXMEM_1MB_EDGE 1024000
#define TEXMEM_2MB_EDGE 2048000
#define TEXMEM_4MB_EDGE 4096000

    FxU32 start;
    FxU32 textureSize = grTexTextureMemRequired(evenOdd, info);
    start = nextTextureTMU[tmu];

    /* check for 2MB/4MB edge and space past it if necessary */
    if ((start < TEXMEM_4MB_EDGE) && (start + textureSize > TEXMEM_4MB_EDGE))
        start = TEXMEM_4MB_EDGE;

    nextTextureTMU[tmu] += textureSize;
    if (nextTextureTMU[tmu] <= lastTextureTMU[tmu])
    {
        return start;
    }
    else
    {
        nextTextureTMU[tmu] = start;
        return -1;
    }
}

bool FxTexture::Load(std::string pathToFile, GrChipID_t tmu)
{
    /* Load texture data into system ram */
    assert(tlLoadTexture(pathToFile.c_str(), &m_texture.info, &m_texture.tableType, &m_texture.tableData));

    texTMUAddress = getStartAddress(GR_MIPMAPLEVELMASK_BOTH, &m_texture.info, tmu);
    if (texTMUAddress != -1)
    {
        /* Download texture data to TMU */
        grTexDownloadMipMap(tmu, texTMUAddress, GR_MIPMAPLEVELMASK_BOTH, &m_texture.info);

        if (m_texture.tableType != NO_TABLE) {
            grTexDownloadTable(m_texture.tableType, &m_texture.tableData);
        }

#if FXDEBUG
        FxU32 textureSize = grTexCalcMemRequired(m_texture.info.smallLodLog2, m_texture.info.largeLodLog2, m_texture.info.aspectRatioLog2, m_texture.info.format);
        printf("FxTexture::Load file = %s vram used = %d\n", pathToFile.c_str(), textureSize);

        FxI32 memory_tmu = 0;
        grGet(GR_MEMORY_TMU, sizeof(FxI32), &memory_tmu);
        FxU32 memUsed = nextTextureTMU[tmu] / 1024;
        FxU32 memFree = (memory_tmu - nextTextureTMU[tmu]) / 1024;
        printf("VRAM STAT: TMU0 Used = %d Kb TMU[0] Free = %d Kb TotalMem TMU0 = %d Kb\n", memUsed, memFree, memory_tmu / 1024);
#endif        
    }

    //FxU32 textureSize, startAddress;
    //textureSize = grTexCalcMemRequired(m_texture.info.smallLodLog2, m_texture.info.largeLodLog2, m_texture.info.aspectRatioLog2, m_texture.info.format);
    //startAddress = grTexMinAddress(tmu);
    //if (startAddress + textureSize <= grTexMaxAddress(GR_TMU0))
    //{
    //    /* Download texture data to TMU */
    //    grTexDownloadMipMap(tmu, grTexMinAddress(tmu), GR_MIPMAPLEVELMASK_BOTH, &m_texture.info);

    //    if (texture.tableType != NO_TABLE) {
    //        grTexDownloadTable(texture.tableType, &texture.tableData);
    //    }
    //}

    return false;
}

void FxTexture::SetTexture(GrChipID_t tmu)
{

    if (m_texture.info.data && texTMUAddress >= 0)
        grTexSource(tmu, texTMUAddress, GR_MIPMAPLEVELMASK_BOTH, &m_texture.info);

    if (m_enableMipmap)
        grTexMipMapMode(tmu, GR_MIPMAP_NEAREST, FXTRUE);

    if (m_enableBilinear)
        grTexFilterMode(tmu, GR_TEXTUREFILTER_BILINEAR, GR_TEXTUREFILTER_BILINEAR);

}

void FxTexture::InitTextureMemManager()
{
    // TODO: TMU count should be init by engine core, and get count here
    nextTextureTMU[0] = grTexMinAddress(GR_TMU0);
    lastTextureTMU[0] = grTexMaxAddress(GR_TMU0);

    nextTextureTMU[1] = grTexMinAddress(GR_TMU1);
    lastTextureTMU[1] = grTexMaxAddress(GR_TMU1);

}
