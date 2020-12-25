/** CSci-4611 Assignment 6: Harold
 */

#include "sky.h"


Sky::Sky() {
    
}


Sky::~Sky() {
    
}

void Sky::Init(ShaderProgram *stroke3d_shaderprog) {
    stroke3d_shaderprog_ = stroke3d_shaderprog;
}


/// Projects a 2D normalized screen point (e.g., the mouse position in normalized
/// device coordinates) to a 3D point on the "sky", which is really a huge sphere
/// (radius = 1500) that the viewer is inside.  This function should always return
/// true since any screen point can successfully be projected onto the sphere.
/// sky_point is set to the resulting 3D point.  Note, this function only checks
/// to see if the ray passing through the screen point intersects the sphere; it
/// does not check to see if the ray hits the ground or anything else first.
bool Sky::ScreenPtHitsSky(const Matrix4 &view_matrix, const Matrix4 &proj_matrix,
                        const Point2 &normalized_screen_pt, Point3 *sky_point)
{
    // TODO: Stitch together your worksheet implementation of this method
    // follow the worsheet I define a inermefiate matrix
    Matrix4 Inte_matrix = view_matrix.Inverse();
    Point3 eye = Inte_matrix.ColumnToPoint3(3);  //find the position in world space
    //then construct a mouse pointer location inworld space
    Point3 mouseIn3d = GfxMath::ScreenToNearPlane(view_matrix, proj_matrix, normalized_screen_pt);
    //create a ray through the world-space
    Ray eyeThroughMouse = Ray(eye, (mouseIn3d - eye).ToUnit());
    float x;
    return eyeThroughMouse.IntersectSphere(Point3::Origin(), 1500.0, &x, sky_point);  //radius = 1500.0   use &x to set the value set
}




/// Creates a new sky stroke mesh by projecting each vertex of the 2D mesh
/// onto the sky dome and saving the result as a new 3D mesh.
void Sky::AddSkyStroke(const Matrix4 &view_matrix, const Matrix4 &proj_matrix,
                       const Mesh &stroke2d_mesh, const Color &stroke_color)
{
    // TODO: Create a new SkyStroke and add it to the strokes_ array.
    Mesh m = stroke2d_mesh; // like the one given in the handout

    // loop through the vertices in the new meshand convert them to 3D points that lie on the sky sphere.
    int temp = m.num_vertices();
    Point3 pt;
    std::vector<Point3> verti;
    for (int i = 0; i < temp; i++) {
        ScreenPtHitsSky(view_matrix, proj_matrix, Point2(m.vertexReadOnly(i)[0], m.vertexReadOnly(i)[1]), &pt);  //2d to 3d alter the vertices
        verti.push_back(pt);
    }             
    //keep the same indices, and akter the vertices.
    struct SkyStroke new_str;  
    m.SetVertices(verti);   // store the result in a new skystoke
    new_str.mesh = m;
    new_str.color = stroke_color;  // store the current stroke color 
    strokes_.push_back(new_str);     


}


/// Draws all of the sky strokes
void Sky::Draw(const Matrix4 &view_matrix, const Matrix4 &proj_matrix) {

    // Precompute matrices needed in the shader
    Matrix4 model_matrix; // identity
    Matrix4 modelview_matrix = view_matrix * model_matrix;
    
    // Draw sky meshes
    stroke3d_shaderprog_->UseProgram();
    stroke3d_shaderprog_->SetUniform("modelViewMatrix", modelview_matrix);
    stroke3d_shaderprog_->SetUniform("projectionMatrix", proj_matrix);
    for (int i=0; i<strokes_.size(); i++) {
        stroke3d_shaderprog_->SetUniform("strokeColor", strokes_[i].color);
        strokes_[i].mesh.Draw();
    }
    stroke3d_shaderprog_->StopProgram();
}

