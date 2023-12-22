#include "vertex_structs.h"

void SetupVertexLayout( void ) {
  grVertexLayout(GR_PARAM_XY,   0, GR_PARAM_ENABLE);        //xy
  //grVertexLayout(GR_PARAM_Z,    8, GR_PARAM_ENABLE);
  grVertexLayout(GR_PARAM_W,    8, GR_PARAM_ENABLE);        //z
  grVertexLayout(GR_PARAM_Q,    12, GR_PARAM_ENABLE);       //w
  grVertexLayout(GR_PARAM_PARGB,16, GR_PARAM_ENABLE);
  //grVertexLayout(GR_PARAM_RGB,  16, GR_PARAM_ENABLE);       //rgb
  //grVertexLayout(GR_PARAM_A,    19, GR_PARAM_ENABLE);       //a    
  grVertexLayout(GR_PARAM_ST0,  20, GR_PARAM_ENABLE);       //uv1
  grVertexLayout(GR_PARAM_ST1,  40, GR_PARAM_ENABLE);       //uv2
}