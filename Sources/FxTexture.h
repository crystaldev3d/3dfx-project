#pragma once
#include "super_header.h"

#define NO_TABLE ((GrTexTable_t)(~0))

typedef FxU32 TlPalette[256];
typedef struct {
    FxU8  yRGB[16];
    FxI16 iRGB[4][3];
    FxI16 qRGB[4][3];
    FxU32 packed_data[12];
} TlNCCTable;

typedef union {
    TlPalette  palette;
    TlNCCTable nccTable;
} TlTextureTable;

typedef struct {
    GrTexInfo      info;
    GrTexTable_t   tableType;
    TlTextureTable tableData;
} TlTexture;

//extern TlTexture texture;

int tlLoadTexture(const char* filename, GrTexInfo* info, GrTexTable_t* tableType, void* table);

//void setupTextures();

class FxTexture 
{
public:
    bool Load(std::string pathToFile, GrChipID_t tmu = GR_TMU0);
    void SetTexture(GrChipID_t tmu = GR_TMU0);
    static void InitTextureMemManager();

    bool m_enableMipmap = false;
    bool m_enableBilinear = false;
    
    TlTexture m_texture;
    FxU32 texTMUAddress;

private:
    long getStartAddress(FxU32 evenOdd, GrTexInfo* info, GrChipID_t tmu = GR_TMU0);
    static FxU32 nextTextureTMU[2];
    static FxU32 lastTextureTMU[2];
};
