#pragma once

#include "components/simple_scene.h"
#include <lab_m1/Tema2/Tank.h>
#include <lab_m1/Tema2/Componenta.h>
#include <lab_m1/Tema2/Camera.h>


using namespace tank;
using namespace componenta;
using namespace camera;

namespace tema2
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:

        Tema2();
        ~Tema2();

        void Init() override;
        void RenderColorMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& objectColor, int nr_lives = -1);

        static Shader* shader;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void RenderScene();
        void RenderBuildings();
        int BuildingCollision(Tank* tank);
        void RenderTank(Tank* tank, int i, float deltaTimeSeconds);
        void MoveEnemy(Tank* user_tank, Tank* tank, float deltaTimeSeconds);
        void RenderSpheres(float deltaTimeSeconds);
        Mesh* CreateRectangle(const std::string&& name, const glm::vec3& color, const glm::vec3& const);
        void DetectTanksCollisions();
        void DetectSpheresCollisions();
        int DetectSphereTankCollisions(Componenta* proiectil, Tank* tank);
        
    protected:
        glm::vec3 lightPosition;
        unsigned int materialShininess;
        float materialKd;
        float materialKs;

        Tank* user_tank;
        vector<Componenta*> buildings;
        vector <Tank*> enemies;

        // pentru rotatie third person camera
        float rotatedX;
        float rotatedY;

        // pentru rotatie turela
        float lastMouseX;
        float lastMouseZ;
        float srand;

        // cat timp dureaza jocul
        clock_t start_time;
        float timeout;

        glm::mat4 modelMatrix;
        GLenum polygonMode;

        glm::vec3 dark_green;
        glm::vec3 light_green;
        glm::vec3 gray;
        glm::vec3 red;
        glm::vec3 ground;
        glm::vec3 ground_enemies;
        glm::vec3 ground_user;
        glm::vec3 sky;
        glm::vec3 blue;

        Camera* camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;
        bool projectionType;

        GLfloat right;
        GLfloat left;
        GLfloat bottom;
        GLfloat top;
        GLfloat fov;
    };

    };
  // namespace m1

