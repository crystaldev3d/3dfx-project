#include "matrix.h"
#include "FxCustomMesh.h"


void mat4_transform_vec3(const mat4& m, const vec3& v, vec3 out)
{
    //vec4 res;
    //res[0] = m[0][0] * v[0] + m[1][0] * v[1] + m[2][0] * v[2] + m[3][0] * v[3];
    //res[1] = m[0][1] * v[0] + m[1][1] * v[1] + m[2][1] * v[2] + m[3][1] * v[3];
    //res[2] = m[0][2] * v[0] + m[1][2] * v[1] + m[2][2] * v[2] + m[3][2] * v[3];
    //res[3] = m[0][3] * v[0] + m[1][3] * v[1] + m[2][3] * v[2] + m[3][3] * v[3];
    //glm_vec4_copy(res, out);

    vec3 res;
    res[0] = m[0][0] * v[0] + m[1][0] * v[1] + m[2][0] * v[2] + m[3][0];
    res[1] = m[0][1] * v[0] + m[1][1] * v[1] + m[2][1] * v[2] + m[3][1];
    res[2] = m[0][2] * v[0] + m[1][2] * v[1] + m[2][2] * v[2] + m[3][2];
    glm_vec3_copy(res, out);

}
void mat4_transform_GrVertex(const mat4& m, const GrVertex& v, GrVertex& out)
{
    GrVertex res;
    memcpy(&res, &v, sizeof(GrVertex));
    //res.x = m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0];  //* v.w;
    //res.y = m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1];  //* v.w;
    //res.z = m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2];  //* v.w;
    //res.w = m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3];  //* v.w;
    //res.w = 1.0f / res.z;
    //res.w = 1.0f;
    //float z = res.z;
    //res.x = (m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0] *v.w);
    //res.y = (m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1] *v.w);
    //res.z = (m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2] *v.w);
    ////res.w = (m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3] *v.w);
    ////res.w = 1.0f / res.z;
    //res.w = 1.0f / res.z;
    
    res.x = (m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0]) / 65535.0f;
    res.y = (m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1]) / 65535.0f;
    res.z = (m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2]) / 65535.0f;
    res.w = (m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3]) / 65535.0f;

    //float m[4][4];
    ///* simplified perspective proj matrix assume unit clip volume */
    //m[0][0] = 1.0f, m[0][1] = 0.0f, m[0][2] = 0.0f, m[0][3] = 0.0f;
    //m[1][0] = 0.0f, m[1][1] = 1.0f, m[1][2] = 0.0f, m[1][3] = 0.0f;
    //m[2][0] = 0.0f, m[2][1] = 0.0f, m[2][2] = 1.0f, m[2][3] = 1.0f;
    //m[3][0] = 0.0f, m[3][1] = 0.0f, m[3][2] = 0.0f, m[3][3] = 0.0f;


    //res.x = (m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0]);
    //res.y = (m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1]);
    //res.z = (m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2]);
    //res.w = (m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3]);

    //res.x /= res.w;
    //res.y /= res.w;
    //res.z /= res.w;


    
    memcpy(&out, &res, sizeof(GrVertex));
};


// Vector3 Matrix4::operator* ( const Vector3 &v ) const
// {
//     const Vector3 o {
//         v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + m[3][0],
//         v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + m[3][1],
//         v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + m[3][2]
//     };

//     const float w = v.x * m[0][3] + v.y * m[1][3] + v.z * m[2][3] + m[3][3];

//     if (w != 0.0f)
//     {
//         return {o.x/w, o.y / w, o.z / w};
//     }
//     return o;
// }
