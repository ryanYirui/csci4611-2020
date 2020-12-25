/** CSci-4611 Assignment 3:  Earthquake
 */

#include "earth.h"
#include "config.h"

#include <vector>

 // for M_PI constant
#define _USE_MATH_DEFINES
#include <math.h>


Earth::Earth() {
}

Earth::~Earth() {
}

void Earth::Init(const std::vector<std::string>& search_path) {
    // init shader program
    shader_.Init();

    // init texture: you can change to a lower-res texture here if needed
    earth_tex_.InitFromFile(Platform::FindFile("earth-2k.png", search_path));

    // init geometry
    const int nslices = 10;
    const int nstacks = 10;

    // TODO: This is where you need to set the vertices and indiceds for earth_mesh_.

    //set up x ,y and longitude and latitude
    float x = 2 * M_PI / nslices;
    float y = M_PI / nstacks;
    float longitude = 360 / nslices;
    float latidute = 180 / nstacks;

    //set up vector like the given demo
    std::vector<unsigned int> indices;
    std::vector<Point2> vertices;

    //Create all of the vertices in order of left to right, top to bottom
    //Also creates the normals
    //Does Plane and Globe
    for (int i = 0; i <= nstacks; i++) {
        for (int j = 0; j <= nslices; j++) {
            
            plane_vertices.push_back(Point3(-M_PI + j * x, -M_PI / 2 + i * y, 0));    //this was set up the plane
            plane_normals.push_back(Vector3(0, 0, 1));

            
            globe_vertices.push_back(LatLongToSphere(-90 + i * latidute, -180 + j * longitude));     //this was set up the globe
            globe_normals.push_back(LatLongToSphere(-90 + i * latidute, -180 + j * longitude) - Point3(0, 0, 0));

        }
    }


    //Add all of the indices two triangles at a time, row-wise
    int count = 0;
    for (int j = 0; j < nstacks; j++) {
        for (int i = 0; i < nslices; i++) {
            indices.push_back(count);
            indices.push_back(count + nslices + 2);
            indices.push_back(count + nslices + 1);


            indices.push_back(count);
            indices.push_back(count + 1);
            indices.push_back(count + nslices + 2);
            count++;
        }
        count++;
    }

    //Add all of the vertices to the texture map
    float tempx;
    float tempy;
    for (int i = nstacks; i >= 0; i--) {
        for (int j = 0; j <= nslices; j++) {
            tempx = (float)j / nslices;
            tempy = (float)i / nstacks;
            vertices.push_back(Point2(tempx,tempy));
        }
    }


    earth_mesh_.SetVertices(plane_vertices);
    earth_mesh_.SetIndices(indices);
    earth_mesh_.SetTexCoords(0, vertices);
    earth_mesh_.SetNormals(plane_normals);
    earth_mesh_.UpdateGPUMemory();
}



void Earth::Draw(const Matrix4& model_matrix, const Matrix4& view_matrix, const Matrix4& proj_matrix) {
    // Define a really bright white light.  Lighting is a property of the "shader"
    DefaultShader::LightProperties light;
    light.position = Point3(10, 10, 10);
    light.ambient_intensity = Color(1, 1, 1);
    light.diffuse_intensity = Color(1, 1, 1);
    light.specular_intensity = Color(1, 1, 1);
    shader_.SetLight(0, light);

    // Adust the material properties, material is a property of the thing
    // (e.g., a mesh) that we draw with the shader.  The reflectance properties
    // affect the lighting.  The surface texture is the key for getting the
    // image of the earth to show up.
    DefaultShader::MaterialProperties mat;
    mat.ambient_reflectance = Color(0.5, 0.5, 0.5);
    mat.diffuse_reflectance = Color(0.75, 0.75, 0.75);
    mat.specular_reflectance = Color(0.75, 0.75, 0.75);
    mat.surface_texture = earth_tex_;

    // Draw the earth mesh using these settings
    if (earth_mesh_.num_triangles() > 0) {
        shader_.Draw(model_matrix, view_matrix, proj_matrix, &earth_mesh_, mat);
    }
}


Point3 Earth::LatLongToSphere(double latitude, double longitude) const {
    // TODO: We recommend filling in this function to put all your
    // lat,long --> sphere calculations in one place.
    return Point3(cos(GfxMath::ToRadians(latitude)) * sin(GfxMath::ToRadians(longitude)),
        sin(GfxMath::ToRadians(latitude)),
        cos(GfxMath::ToRadians(latitude)) * cos(GfxMath::ToRadians(longitude)));
}

Point3 Earth::LatLongToPlane(double latitude, double longitude) const {
    // TODO: We recommend filling in this function to put all your
    // lat,long --> plane calculations in one place.
    return Point3(-M_PI + (((2 * M_PI) * (longitude + 180)) / 360),
        -M_PI / 2 + (((M_PI) * (latitude + 90)) / 180),
        0);
}



void Earth::DrawDebugInfo(const Matrix4& model_matrix, const Matrix4& view_matrix, const Matrix4& proj_matrix) {
    // This draws a cylinder for each line segment on each edge of each triangle in your mesh.
    // So it will be very slow if you have a large mesh, but it's quite useful when you are
    // debugging your mesh code, especially if you start with a small mesh.
    for (int t = 0; t < earth_mesh_.num_triangles(); t++) {
        std::vector<unsigned int> indices = earth_mesh_.triangle_verticesReadOnly(t);
        std::vector<Point3> loop;
        loop.push_back(earth_mesh_.vertexReadOnly(indices[0]));
        loop.push_back(earth_mesh_.vertexReadOnly(indices[1]));
        loop.push_back(earth_mesh_.vertexReadOnly(indices[2]));
        quick_shapes_.DrawLines(model_matrix, view_matrix, proj_matrix,
            Color(1, 1, 0), loop, QuickShapes::LinesType::LINE_LOOP, 0.005);
    }
}
//two method were seted as below
//plane to globe
//update smesh
void Earth::PlaneToGlobe(const float& num) {
    std::vector<Point3> t_vertices;   //set up a new vector that accpet vertices and normal since i am going to transform it 
    std::vector<Vector3> t_normals;
    //transofrm case
    if (num == false) {
        {
            int size1 = globe_vertices.size();
            int i = 0;
            while(i<size1){
                t_vertices.push_back(plane_vertices[i].Lerp(globe_vertices[i], num));
                t_normals.push_back(plane_normals[i].Lerp(globe_normals[i], num));
                i += 1;
            }
            earth_mesh_.SetVertices(t_vertices);
            earth_mesh_.SetNormals(t_normals);
        }
    }
    //stay as is case
    else {
        earth_mesh_.SetVertices(globe_vertices);
        earth_mesh_.SetNormals(globe_normals);
    }

    earth_mesh_.UpdateGPUMemory();
}
//Globe change to plane
void Earth::GlobeToPlane(const float& num) {
    std::vector<Point3> t_vertices;
    std::vector<Vector3> t_normals;
    //transform case
    int size = plane_vertices.size();
    int j = 0;
    if (num == false) {
        while(j<size){
            t_vertices.push_back(globe_vertices[j].Lerp(plane_vertices[j], num));
            t_normals.push_back(globe_normals[j].Lerp(plane_normals[j], num));
            j += 1;
        }
        earth_mesh_.SetVertices(t_vertices);
        earth_mesh_.SetNormals(t_normals);
    }
    //stay as is
    else 
    {
        earth_mesh_.SetVertices(plane_vertices);
        earth_mesh_.SetNormals(plane_normals);
    }

    earth_mesh_.UpdateGPUMemory();
}
