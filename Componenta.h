#pragma once
#include "components/simple_scene.h"
#include <glm/glm.hpp >

using namespace std;
namespace componenta 
{
    class Componenta
    {
    public:
        Componenta();
        Componenta(Mesh* mesh);
        Componenta(float x, float y);
        ~Componenta();

        float getX() const;
        void setX(float x);

        float getY() const;
        void setY(float y);

        float getZ() const;
        void setZ(float z);

        float getInitialX();
        void setInitialX(float initial_x);

        float getInitialY();
        void setInitialY(float initial_y);

        float getInitialZ();
        void setInitialZ(float initial_z);

        float getXAngle() const;
        void setXAngle(float x_angle);

        float getZAngle() const;
        void setZAngle(float y_angle);

        float getScaleX() const;
        void setScaleX(float scale_x);

        float getScaleY() const;
        void setScaleY(float scale_y);

        float getScaleZ() const;
        void setScaleZ(float scale_z);

        float getRotateOY();
        void setRotateOY(float rotateOY);

        float getRotateOZ();
        void setRotateOZ(float rotateOZ);

        Mesh* getMesh() const;
        void setMesh(Mesh*);

        glm::vec3 getVelocity() const;
        void setVelocity(glm::vec3 new_velocity);
        int BuildingCollision(vector<Componenta*> buildings);

        // pentru proiectile

        float getTimeOfFlight();
        void setTimeOfFlight(float TimeOfFlight);

        float getYLaunchAngle();
        void setYLaunchAngle(float YLaunchAngle);

        float getInitialSpeed();
        void setInitialSpeed(float InitialSpeed);

        time_t getStartTime();
        void setStartTime(time_t start_time);
 
    private:
        float x;
        float y;
        float z;
        

        float scale_x;
        float scale_y;
        float scale_z;
        float rotateOY;
        float rotateOZ;

        Mesh* mesh;

        // pentru proiectil
        glm::vec3 initial_velocity;
        float initial_speed;
        float y_launch_angle;
        time_t start_time;
        float x_angle;
        float z_angle;
        float initial_x;
        float initial_y;
        float initial_z;
    };
}