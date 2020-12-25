/** CSci-4611 Assignment 2:  Car Soccer
 */

#include "car_soccer.h"
#include "config.h"
#include <time.h>

CarSoccer::CarSoccer() : GraphicsApp(1024,768, "Car Soccer") {
    // Define a search path for finding data files (images and shaders)
    searchPath_.push_back(".");
    searchPath_.push_back("./data");
    searchPath_.push_back(DATA_DIR_INSTALL);
    searchPath_.push_back(DATA_DIR_BUILD);
}

CarSoccer::~CarSoccer() {
}


Vector2 CarSoccer::joystick_direction() {
    Vector2 dir;
    if (IsKeyDown(GLFW_KEY_LEFT))
        dir[0]--;
    if (IsKeyDown(GLFW_KEY_RIGHT))
        dir[0]++;
    if (IsKeyDown(GLFW_KEY_UP))
        dir[1]++;
    if (IsKeyDown(GLFW_KEY_DOWN))
        dir[1]--;
    return dir;
}


void CarSoccer::OnSpecialKeyDown(int key, int scancode, int modifiers) {
    if (key == GLFW_KEY_SPACE) {
        // Here's where you could call some form of launch_ball();
        ball_.Reset();
        ball_.set_velocity(Vector3(rand() % 10, rand() % 10, rand() % 10));
        car_.Reset();

    }
}


void CarSoccer::UpdateSimulation(double timeStep) {
    // Here's where you shound do your "simulation", updating the positions of the
    // car and ball as needed and checking for collisions.  Filling this routine
    // in is the main part of the assignment.
    float ballx = ball_.position()[0];
    float bally = ball_.position()[1];
    float ballz = ball_.position()[2];
    float car_x = car_.position()[0];
    float car_y = car_.position()[1];
    float car_z =car_.position()[2];
    float ball_r = ball_.radius();
    float car_r = car_.collision_radius();
    int goalX = 10;
    int goalZ = 50;
    float tempx,tempy , tempz;
    ball_.set_position(ball_.position() + ball_.velocity() * timeStep);

// hit either side of the goal state
  if ((ballx >= (-goalX + ball_r))
    && ((ballx <= ( goalX - ball_r)))
    &&  (bally <= ( goalX - ball_r))
    && ((ballz <= (-goalZ + ball_r))
    ||  (ballz >= ( goalZ - ball_r)))) {
          // reset ball position and car position when it hit the goal state
          ball_.Reset();
          car_.Reset();
    }
    //ball collision with wall/wloor/ceiling
    if (ballx - ball_r < -40)
    { //ball collision wall
        ball_.set_position(Point3(-40 + ball_r, bally, ballz));
         tempx = -ball_.velocity()[0] * 0.8;
       tempy = ball_.velocity()[1] * 0.8;
         tempz = ball_.velocity()[2] * 0.8;
        ball_.set_velocity(Vector3(tempx, tempy, tempz));
    }

    if (ballx + ball_r > 40)
    { //ball collision wall
        ball_.set_position(Point3(40 - ball_r, bally, ballz));
        tempx = -ball_.velocity()[0] * 0.8;
        tempy = ball_.velocity()[1] * 0.8;
        tempz = ball_.velocity()[2] * 0.8;
        ball_.set_velocity(Vector3(tempx,tempy,tempz));
    }

    if (bally - ball_r < 0)
    { // collision wall
        ball_.set_position(Point3(ballx, 0 + ball_r, ballz));
        tempx = ball_.velocity()[0] * 0.8;
        tempy = -ball_.velocity()[1] * 0.8;
        tempz = ball_.velocity()[2] * 0.8;
        ball_.set_velocity(Vector3(tempx,tempy,tempz));
    }

    if (bally - ball_r > 35)
    { // collision wall
        ball_.set_position(Point3(ballx, 35 - ball_r, ballz));
        tempx = ball_.velocity()[0] * 0.8;
        tempy = -ball_.velocity()[1] * 0.8;
        tempz = ball_.velocity()[2] * 0.8;
        ball_.set_velocity(Vector3(tempx,tempy,tempz));
    }

    if (ballz - ball_r < -50)
    { //collision floor
        ball_.set_position(Point3(ballx, bally, -50 + ball_r));
        tempx = ball_.velocity()[0] * 0.8;
         tempy = ball_.velocity()[1] * 0.8;
        tempz = -ball_.velocity()[2] * 0.8;
        ball_.set_velocity(Vector3(tempx,tempy,tempz));
    }
    //collision ceiling
    if (ballz + ball_r > 50)
    {
        ball_.set_position(Point3(ballx, bally, 50 - ball_r));
        tempx = ball_.velocity()[0] * 0.8;
        tempy = ball_.velocity()[1] * 0.8;
        tempz = -ball_.velocity()[2] * 0.8;
        ball_.set_velocity(Vector3(tempx,tempy,tempz));
    }

    if (bally - ball_r > 0)
        ball_.set_velocity(Vector3(ball_.velocity()[0], ball_.velocity()[1] - (22 * timeStep), ball_.velocity()[2]));

    //Car Driving
    car_.set_position(car_.position() + car_.velocity() * timeStep);
    car_.set_dir(timeStep, joystick_direction()[0]);
    car_.set_speed(timeStep, joystick_direction()[1]);
    //car collision wall
    if (car_x - car_r < -40)
    {
        tempx = -40 + car_r;
        tempy = car_y;
        tempz = car_z;
        car_.set_position(Point3(tempx,tempy,tempz));
        car_.set_speed(0);
    }

    if (car_z - car_r < -50)
    {
        tempx = car_x;
        tempy = car_y;
        tempz = -50+car_r;
        car_.set_position(Point3(tempx,tempy,tempz));
        car_.set_speed(0);
    }

    if (car_x + car_r > 40)
    {
        tempx = 40 - car_r;
        tempy = car_y;
        tempz = car_z;
        car_.set_position(Point3(tempx,tempy,tempz));
        car_.set_speed(0);
    }

    if (car_z + car_r > 50)
    {
        tempx = car_x;
        tempy = car_y;
        tempz = 50-car_r;
        car_.set_position(Point3(tempx,tempy,tempz));
        car_.set_speed(0);
    }

    //collision with car
    Vector3 len = car_.position() - ball_.position();  //length between the car and the ball
    if (len.Length() <= car_r + ball_r)
    {
        Vector3 ball_direction = ball_.velocity().ToUnit();  //set the direction
        ball_.set_position(ball_.position() + 1.05*(ball_.radius() - (len.Length() - car_r)) * -ball_direction);  //set the position of the ball
        Vector3 relative = ball_.velocity() - car_.velocity();   //assign the velocity
        Vector3 new_relative = relative - 1.95 * relative.Dot(len.ToUnit()) * len.ToUnit();
        ball_.set_velocity(car_.velocity() + new_relative);  // set the velocity
    }
}


void CarSoccer::InitOpenGL() {
    // Set up the camera in a good position to see the entire field
    projMatrix_ = Matrix4::Perspective(60, aspect_ratio(), 1, 1000);
    modelMatrix_ = Matrix4::LookAt(Point3(0,60,70), Point3(0,0,10), Vector3(0,1,0));

    // Set a background color for the screen
    glClearColor(0.8,0.8,0.8, 1);

    // Load some image files we'll use
    fieldTex_.InitFromFile(Platform::FindFile("pitch.png", searchPath_));
    crowdTex_.InitFromFile(Platform::FindFile("crowd.png", searchPath_));
    srand(time(NULL));
    int x = rand()%9;
    int y = rand()%9;
    int z = rand()%9;
    ball_.set_velocity(Vector3(x, y, z));
}


void CarSoccer::DrawUsingOpenGL() {
    // Draw the crowd as a fullscreen background image
    quickShapes_.DrawFullscreenTexture(Color(1,1,1), crowdTex_);

    // Draw the field with the field texture on it.
    Color col(16.0/255.0, 46.0/255.0, 9.0/255.0);
    Matrix4 M = Matrix4::Translation(Vector3(0,-0.201,0)) * Matrix4::Scale(Vector3(50, 1, 60));
    quickShapes_.DrawSquare(modelMatrix_ * M, viewMatrix_, projMatrix_, col);
    M = Matrix4::Translation(Vector3(0,-0.2,0)) * Matrix4::Scale(Vector3(40, 1, 50));
    quickShapes_.DrawSquare(modelMatrix_ * M, viewMatrix_, projMatrix_, Color(1,1,1), fieldTex_);

    // Draw the car
    Color carcol(0.8, 0.2, 0.2);
    Matrix4 Mcar =
        Matrix4::Translation(car_.position() - Point3(0,0,0)) *
        Matrix4::Scale(car_.size()) *
        Matrix4::Scale(Vector3(0.5,0.5,0.5))*Matrix4::RotationY(car_.dir());
    quickShapes_.DrawCube(modelMatrix_ * Mcar, viewMatrix_, projMatrix_, carcol);


    // Draw the ball
    Color ballcol(1,1,1);
    Matrix4 Mball =
        Matrix4::Translation(ball_.position() - Point3(0,0,0)) *
        Matrix4::Scale(Vector3(ball_.radius(), ball_.radius(), ball_.radius()));
    quickShapes_.DrawSphere(modelMatrix_ * Mball, viewMatrix_, projMatrix_, ballcol);


    // Draw the ball's shadow -- this is a bit of a hack, scaling Y by zero
    // flattens the sphere into a pancake, which we then draw just a bit
    // above the ground plane.
    Color shadowcol(0.2,0.4,0.15);
    Matrix4 Mshadow =
        Matrix4::Translation(Vector3(ball_.position()[0], -0.1, ball_.position()[2])) *
        Matrix4::Scale(Vector3(ball_.radius(), 0, ball_.radius())) *
        Matrix4::RotationX(90);
    quickShapes_.DrawSphere(modelMatrix_ * Mshadow, viewMatrix_, projMatrix_, shadowcol);


    // You should add drawing the goals and the boundary of the playing area
    // using quickShapes_.DrawLines()
    Color line(1, 1, 1);  //set the color for the line
    std::vector<Point3> Right;

    Right.push_back(Point3(40, 0, -50));
    Right.push_back(Point3(40, 35, -50));
    Right.push_back(Point3(40, 35, 50));
    Right.push_back(Point3(40, 0, 50));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, line, Right, QuickShapes::LinesType::LINE_LOOP, 0.1);

    std::vector<Point3> Down;
    Down.push_back(Point3(-40, 0, 50));
    Down.push_back(Point3(-40, 35, 50));
    Down.push_back(Point3(40, 35, 50));
    Down.push_back(Point3(40, 0, 50));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, line, Down, QuickShapes::LinesType::LINE_LOOP, 0.1);

    std::vector<Point3> Left;
    Left.push_back(Point3(-40, 0, 50));
    Left.push_back(Point3(-40, 35, 50));
    Left.push_back(Point3(-40, 35, -50));
    Left.push_back(Point3(-40, 0, -50));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, line, Left, QuickShapes::LinesType::LINE_LOOP, 0.1);

    std::vector<Point3> Upper;
    Upper.push_back(Point3(-40, 35, -50));
    Upper.push_back(Point3(-40, 0, -50));
    Upper.push_back(Point3(40, 0, -50));
    Upper.push_back(Point3(40, 35, -50));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, line, Upper, QuickShapes::LinesType::LINE_LOOP, 0.1);


    std::vector<Point3> outcome;   //goals state

    int i =-10;
    int  j=0;
    int  k = -10;
    int h =0;
    while(i<=10){
        outcome.push_back(Point3(i, 0, 50));
        outcome.push_back(Point3(i, 10, 50));
        i+=1;
    }
    while (j<=10){
        outcome.push_back(Point3(-10, j, 50));
        outcome.push_back(Point3(10, j, 50));
        j+=1;
    }
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, Color(0.8, 0.8, 0.2), outcome, quickShapes_.LINES, 0.1);

    //another case of goal 2
    std::vector<Point3> outcome2;
    while(k<=10){
        outcome2.push_back(Point3(k, 0, -50));
        outcome2.push_back(Point3(k, 10, -50));
        k+=1;
    }

    while(h<=10){
        outcome2.push_back(Point3(-10, h, -50));
        outcome2.push_back(Point3(10, h, -50));
        h+=1;
    }
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, Color(0.2, 0.8, 0.8), outcome2, quickShapes_.LINES, 0.1);
}
