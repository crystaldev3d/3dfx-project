#ifndef FXSTATICMODEL_H
#define FXSTATICMODEL_H
#pragma once
#include <tiny_obj_loader.h>

#include "super_header.h"
#include "vertex_structs.h"
#include "FxDrawable.h"

class FxStaticModel : public FxDrawable 
{
public:
    void Load(char* filename);

private:
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
};

#endif /*FXSTATICMODEL_H*/