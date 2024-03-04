#pragma once

#include <lab_m1/Tema2/Componenta.h>
#include <ctime>

using namespace componenta;
using namespace std;

namespace tank
{
    class Tank
    {
    public:

        Tank();
        ~Tank();

        void CreateTank(Mesh *corp, Mesh* senile, Mesh *turela, Mesh *tun, float rotation, float timeout = 1);
        Componenta* Tank::getCorp();
        Componenta* Tank::getSenile1();
        Componenta* Tank::getSenile2();
        Componenta* Tank::getTurela();
        Componenta* Tank::getTun();

        void Tank::setCorpPos(float x, float y, float z);
        void Tank::setSenile1Pos(float x, float y, float z);
        void Tank::setSenile2Pos(float x, float y, float z);
        void Tank::setTurelaPos(float x, float y, float z);
        void Tank::setTunPos(float x, float y, float z);


        float Tank::getRotation();
        void Tank::setRotation(float rotation);
        
        float getX() const;
        void setX(float x);

        float getY() const;
        void setY(float y);

        float getZ() const;
        void setZ(float z);

        clock_t getStartTime();
        void setStartTime(clock_t startTime);

        float getTimeout();
        void setTimeout(float timeout);

        clock_t getStartTimeState();
        void setStartTimeState(clock_t startTimeState);

        float getTimeoutState();
        void setTimeoutState(float timeoutState);

        char getState();
        void setState(char state);

        int getNrLives();
        void setNrLives(int nr_lives);

        void PositionTank(float x, float y, float z, int initial);
        glm::vec3 Tank::getPositionTank();

        void AddSphere(Mesh* mesh, float x = 0, float y = 0);
        int CheckAddSphere(Mesh* mesh, float x = 0, float y = 0);
        /* se verifica daca tancul e distrus si daca a trecut suficient timp de la
        aruncare, x si y reprez.unghiurile pe Ox si Oz de tragere,
        daca sunt 0 inseamna ca se trage pe directia in care e orientat
        tunul tancului utilizatorului
        */
        std::vector<Componenta*> GetTankSpheres();

        int BuildingCollision(vector<Componenta*> buildings);

        // se verifica coliziunile cu alte tancuri sau cu dimensiunile hartii
        int MoveForward(float deltaTimeSeconds, vector<Componenta*> buildings);
        int MoveLeft(float deltaTimeSeconds, vector<Componenta*> buildings);
        int ExitMap(float x, float z);
        void RemoveSphere(int index);
        bool isLessWithPrecision(double a, double b, double epsilon = 1e-9);
        glm::mat4 modelMatrix_tank;

    private:

    protected:
        glm::vec3 dark_green;
        glm::vec3 light_green;
        glm::vec3 gray;

        Componenta corp;
        Componenta senile1;
        Componenta senile2;
        Componenta turela;
        Componenta tun;

        float x_tank;
        float y_tank;
        float z_tank;
        float rotation;

        vector<Componenta *> proiectile;

        // pentru proiectile si pentru stari
        clock_t start_time;
        float timeout;

        clock_t start_time_state;
        float timeout_state;
        char state;
        int nr_lives;
        float srand; // pentru randomizarea timpului in care sta in stari
    };
}   // namespace tank

