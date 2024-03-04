#include <lab_m1/Tema2/Componenta.h>
#include <math.h>

using namespace componenta;
using namespace std;

Componenta::Componenta()
    {

    }

Componenta::Componenta(Mesh* mesh)
    {
        this->mesh = mesh;
    }

    Componenta::Componenta(float x, float y)
    {
        this->x = x;
        this->y = y;
        this->z = 0;
    }

    Componenta::~Componenta() {
        // Destructor implementation
    }

    float Componenta::getX() const {
        return x;
    }

    void Componenta::setX(float x) {
        this->x = x;
    }

    float Componenta::getY() const {
        return y;
    }

    void Componenta::setY(float y) {
        this->y = y;
    }

    float Componenta::getZ() const {
        return z;
    }

    void Componenta::setZ(float z) {
        this->z = z;
    }

    float Componenta::getInitialX() {
        return initial_x;
    }

    void Componenta::setInitialX(float x) {
        this->initial_x = x;
    }


    float Componenta::getInitialY() {
        return initial_y;
    }

    void Componenta::setInitialY(float y) {
        this->initial_y = y;
    }

    float Componenta::getInitialZ() {
        return initial_z;
    }

    void Componenta::setInitialZ(float z) {
        this->initial_z = z;
    }
    
    float Componenta::getScaleX() const {
        return scale_x;
    }

    void Componenta::setScaleX(float scale_x) {
        this->scale_x = scale_x;
    }


    float Componenta::getScaleY() const {
        return scale_y;
    }

    void Componenta::setScaleY(float scale_y) {
        this->scale_y = scale_y;
    }


    float Componenta::getScaleZ() const {
        return scale_z;
    }

    void Componenta::setScaleZ(float scale_z) {
        this->scale_z = scale_z;
    }

    float Componenta::getRotateOY() {
        return rotateOY;
    }
    void Componenta::setRotateOY(float rotateOY) {
        this->rotateOY = rotateOY;
    }

    float Componenta::getRotateOZ() {
        return rotateOZ;
    }
    void Componenta::setRotateOZ(float rotateOZ) {
        this->rotateOZ = rotateOZ;
    }

    time_t Componenta::getStartTime() {
        return start_time;
    }
    void Componenta::setStartTime(time_t start_time) {
        this->start_time = start_time;
    }

    float Componenta::getYLaunchAngle() {
        return y_launch_angle;
    }
    void Componenta::setYLaunchAngle(float YLaunchAngle) {
        this->y_launch_angle = YLaunchAngle;
    }

    float Componenta::getInitialSpeed() {
        return initial_speed;
    }
    void Componenta::setInitialSpeed(float initial_speed) {
        this->initial_speed = initial_speed;
    }

    Mesh* Componenta::getMesh() const {
        return mesh;
    }

    void Componenta::setMesh(Mesh* mesh) {
        this->mesh = mesh;
    }

    glm::vec3 Componenta::getVelocity() const{
        return initial_velocity;
    }

    void Componenta::setVelocity(glm::vec3 new_velocity) {
        this->initial_velocity = new_velocity;
    }

    float Componenta::getXAngle() const {
        return x_angle;
    }

    void Componenta::setXAngle(float x_angle) {
        this->x_angle = x_angle;
    }


    float Componenta::getZAngle() const {
        return z_angle;
    }

    void Componenta::setZAngle(float z_angle) {
        this->z_angle = z_angle;
    }

    int Componenta::BuildingCollision(vector<Componenta*> buildings) {
        for (auto& building : buildings)
        {
            float x_check1 = building->getX() - building->getScaleX();
            float x_check2 = building->getX() + building->getScaleX();

            float y_check1 = building->getY() - building->getScaleY();
            float y_check2 = building->getY() + building->getScaleY();

            float z_check1 = building->getZ() - building->getScaleZ();
            float z_check2 = building->getZ() + building->getScaleZ();

            float x_aux = max(x_check1, min(x, x_check2));
            float y_aux = max(y_check1, min(y, y_check2));
            float z_aux = max(z_check1, min(z, z_check2));

            // this is the same as isPointInsideSphere
            float distance = sqrt(
                (x_aux - x) * (x_aux - x) +
                (y_aux - y) * (y_aux - y) +
                (z_aux - z) * (z_aux - z));

            if (distance < 0.1f)
            {
               // printf("este");
                return 1;
            }
        }
        return 0;
    }