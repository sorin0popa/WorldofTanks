#include <lab_m1/Tema2/Tema2.h>
#include <lab_m1/Tema2/Tank.h>
#include <lab_m1/Tema2/transform3D.h>
#include <lab_m1/Tema2/Componenta.h>
#include <lab_m1/Tema2/Camera.h>

#include <lab_m1/Tema2/components/transform.h>
#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include <glm/glm.hpp>

using namespace tank; 
using namespace std; 
using namespace tema2;
using namespace componenta;
using namespace camera;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}

Mesh* Tema2::CreateRectangle(const std::string&& name, const glm::vec3& color, const glm::vec3& size) {
	Mesh* rectangle = new Mesh(name);
	// pentru dreptunghiul din stanga (scena de joc)
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(-size.x, -size.y, -size.z), gray),
			VertexFormat(glm::vec3(size.x, -size.y, -size.z), gray),
			VertexFormat(glm::vec3(size.x, -size.y, size.z), gray),
			VertexFormat(glm::vec3(-size.x, -size.y, size.z), gray),
			VertexFormat(glm::vec3(-size.x, size.y, -size.z), gray),
			VertexFormat(glm::vec3(size.x, size.y, -size.z), gray),
			VertexFormat(glm::vec3(size.x, size.y, size.z), gray),
			VertexFormat(glm::vec3(-size.x, size.y, size.z), gray),
		};

		vector<unsigned int> indices =
		{
		0, 1, 2,
		2, 3, 0,
		
		4, 5, 6,
		6, 7, 4,

		0, 4, 5,
		5, 1, 0,
		
		2, 6, 7,
		7, 3, 2,
		
		0, 3, 7,
		7, 4, 0,

		1, 2, 6,
		6, 5, 1
		};
		rectangle->InitFromData(vertices, indices);
	}
	return rectangle;
}


void Tema2::RenderColorMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, int nr_lives)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// Render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Set shader uniforms for light & material properties
	// TODO(student): Set light position uniform
	GLint location_light_position = glGetUniformLocation(shader->program, "light_position");
	glUniform3fv(location_light_position, 1, glm::value_ptr(lightPosition));


	// glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
	glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
	// TODO(student): Set eye position (camera position) uniform
	GLint location_eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3fv(location_eye_position, 1, glm::value_ptr(eyePosition));

	// TODO(student): Set material property uniforms (shininess, kd, ks, object color)
	GLint location_material = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(location_material, materialShininess);

	GLint location_kd = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(location_kd, materialKd);

	GLint location_ks = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(location_ks, materialKs);

	GLint location_object_color = glGetUniformLocation(shader->program, "object_color");
	glUniform3fv(location_object_color, 1, glm::value_ptr(color));

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	// glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	// glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	int location_nr_lives = glGetUniformLocation(shader->program, "nr_lives");
	glUniform1i(location_nr_lives, nr_lives);

	// Draw the object
	mesh->Render();
	//glBindVertexArray(mesh->GetBuffers()->m_VAO);
	//glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void Tema2::Init()
{
	polygonMode = GL_FILL;
	
	renderCameraTarget = false;
	projectionType = true;

	right = 10.f;
	left = .01f;
	bottom = .01f;
	top = 10.f;
	fov = 40.f;

	rotatedX = 0.0f;
	rotatedY = 0.0f;
	lastMouseX = 0.0f;
	lastMouseZ = 0.0f;

	start_time = clock();
	timeout = 90;
	srand = time(NULL);

	camera = new Camera();
	camera->Set(glm::vec3(0, 0.5f, -1.8f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, Z_NEAR, Z_FAR);

	dark_green = glm::vec3(0, 0.2f, 0);
	light_green = glm::vec3(0, 0.4f, 0);
	gray = glm::vec3(0.4f, 0.4f, 0.4f);
	red = glm::vec3(0.4, 0, 0);
	ground = glm::vec3(0.2f, 0.1f, 0);
	sky = glm::vec3(0, 0.8f, 0.8f);
	blue = glm::vec3(0, 0, 0.4f);

	Mesh* mesh = new Mesh("turela");
	mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "my_models/"), "turela.obj");
	meshes[mesh->GetMeshID()] = mesh;

	mesh = new Mesh("tun");
	mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "my_models/"), "tun.obj");
	meshes[mesh->GetMeshID()] = mesh;

	mesh = new Mesh("senile");
	mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "my_models/"), "senile_colorat_v1.obj");
	meshes[mesh->GetMeshID()] = mesh;

	mesh = new Mesh("corp");
	mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "my_models/"), "corp.obj");
	meshes[mesh->GetMeshID()] = mesh;

	mesh = new Mesh("proiectil");
	mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives/"), "sphere.obj");
	meshes[mesh->GetMeshID()] = mesh;


	mesh = new Mesh("plane");
	mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
	meshes[mesh->GetMeshID()] = mesh;

	user_tank = new Tank();
	user_tank->CreateTank(meshes["corp"], meshes["senile"], meshes["turela"], meshes["tun"], 0.0f);
	user_tank->PositionTank(0.0f, 0.0f, 0.0f, 0);

	for (int i = 0; i < 10; i++) {
		enemies.push_back(new Tank());
		enemies.at(i)->CreateTank(meshes["corp"], meshes["senile"], meshes["turela"], meshes["tun"], 0.0f, 3);
		enemies.at(i)->PositionTank(i - 2, 0.0f, 20, 0);
		
		enemies.at(i)->setState('W');
		enemies.at(i)->setStartTimeState(clock());
		enemies.at(i)->setTimeoutState(5);
		
		float angle = atan2(user_tank->getCorp()->getX() - enemies.at(i)->getX(),
			user_tank->getCorp()->getZ() - enemies.at(i)->getZ());
		angle = angle * 180.0f / M_PI; // transform in grade

		enemies.at(i)->getTun()->setRotateOY(angle - 90);
		enemies.at(i)->getTurela()->setRotateOY(angle - 90);
	}
	// in stanga tanc = z+, x+
	// in dreapta tanc = z+, x-

	meshes["cladire1"] = CreateRectangle("cladire1", gray, glm::vec3(0.5,0.5,0.5));
	Componenta* building1 = new Componenta(meshes["cladire1"]);
	building1->setX(5);
	building1->setY(0.5f);
	building1->setZ(5);
	building1->setScaleX(0.5f);
	building1->setScaleY(0.5f);
	building1->setScaleZ(0.5f);
	buildings.push_back(building1);

	meshes["cladire2"] = CreateRectangle("cladire2", gray, glm::vec3(1, 0.5f, 1));
	Componenta* building2 = new Componenta(meshes["cladire2"]);
	building2->setX(-5);
	building2->setY(0.5f);
	building2->setZ(5);
	building2->setScaleX(1);
	building2->setScaleY(0.5f);
	building2->setScaleZ(1);
	buildings.push_back(building2);

	meshes["cladire3"] = CreateRectangle("cladire3", gray, glm::vec3(1, 1.5f, 1));
	Componenta* building3 = new Componenta(meshes["cladire3"]);
	building3->setX(-2);
	building3->setY(1);
	building3->setZ(6);
	building3->setScaleX(1);
	building3->setScaleY(1.5f);
	building3->setScaleZ(1);
	buildings.push_back(building3);

	meshes["cladire4"] = CreateRectangle("cladire4", gray, glm::vec3(1.5f, 1, 1.5f));
	Componenta* building4 = new Componenta(meshes["cladire4"]);
	building4->setX(3);
	building4->setY(1);
	building4->setZ(7);
	building4->setScaleX(1.5f);
	building4->setScaleY(1);
	building4->setScaleZ(1.5f);
	buildings.push_back(building4);

	meshes["cladire5"] = CreateRectangle("cladire5", gray, glm::vec3(1, 1, 1));
	Componenta* building5 = new Componenta(meshes["cladire5"]);
	building5->setX(-5);
	building5->setY(1);
	building5->setZ(15);
	building5->setScaleX(1);
	building5->setScaleY(1);
	building5->setScaleZ(1);
	buildings.push_back(building5);

	Componenta* building6 = new Componenta(meshes["cladire4"]);
	building6->setX(4);
	building6->setY(1);
	building6->setZ(15);
	building6->setScaleX(1.5f);
	building6->setScaleY(1);
	building6->setScaleZ(1.5f);
	buildings.push_back(building6);


	meshes["cladire7"] = CreateRectangle("cladire7", gray, glm::vec3(2, 1.5f, 2));
	Componenta* building7 = new Componenta(meshes["cladire7"]);
	building7->setX(-5);
	building7->setY(1);
	building7->setZ(15);
	building7->setScaleX(2);
	building7->setScaleY(1.5f);
	building7->setScaleZ(2);
	buildings.push_back(building7);

	meshes["cladire8"] = CreateRectangle("cladire8", gray, glm::vec3(2, 1.5f, 2));
	Componenta* building8 = new Componenta(meshes["cladire8"]);
	building8->setX(-5);
	building8->setY(1);
	building8->setZ(1);
	building8->setScaleX(2);
	building8->setScaleY(1.5f);
	building8->setScaleZ(2);
	buildings.push_back(building8);

	meshes["cladire9"] = CreateRectangle("cladire9", gray, glm::vec3(2, 1.5f, 2));
	Componenta* building9 = new Componenta(meshes["cladire9"]);
	building9->setX(5);
	building9->setY(1);
	building9->setZ(0);
	building9->setScaleX(2);
	building9->setScaleY(1.5f);
	building9->setScaleZ(2);
	buildings.push_back(building9);

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader* shader = new Shader("TemaShader");
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	
	// Light & material properties
	{
		lightPosition = glm::vec3(1, 1, 1);
		materialShininess = 100;
		materialKd = 8;
		materialKs = 8;
	}
}

void Tema2::FrameStart()
{
	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int Tema2::DetectSphereTankCollisions(Componenta *proiectil, Tank *tank) {
	
	if (proiectil->getY() - 0.35f > 0)
		return 0;
	
	float x_tank1 = tank->getX() - 0.45f;
	float y_tank1 = tank->getY() - 0.45f;
	float z_tank1 = tank->getZ() - 0.45f;
	float x_tank2 = tank->getX() + 0.45f;
	float y_tank2 = tank->getY() + 0.45f;
	float z_tank2 = tank->getZ() + 0.45f;

	float x = proiectil->getX();
	float y = proiectil->getY();
	float z = proiectil->getZ();

	float x_aux = max(x_tank1, min(x, x_tank2));
	float y_aux = max(y_tank1, min(y, y_tank2));
	float z_aux = max(z_tank1, min(z, z_tank2));

		// this is the same as isPointInsideSphere
		float distance = sqrt(
			(x_aux - x) * (x_aux - x) +
			(y_aux - y) * (y_aux - y) +
			(z_aux - z) * (z_aux - z));

		if (distance < 0.1f)
		{
			return 1;
		}
	return 0;
}

void Tema2::DetectSpheresCollisions() {
	for (int i = 0; i < user_tank->GetTankSpheres().size(); i++) {
		Componenta* proiectil = user_tank->GetTankSpheres().at(i);

		// caz 1, intra in pamant
		if (proiectil->getY() < 0)
		{
			user_tank->RemoveSphere(i);
			continue;
		}

		// caz 2, coliziune cu cladiri
		if (proiectil->BuildingCollision(buildings) == 1) {
			user_tank->RemoveSphere(i);
		}

		// caz 3, sferele userului vs tancurile inamicilor
		// trec prin sfere userului
	 for (int i = 0; i < user_tank->GetTankSpheres().size(); i++)
	{
		Componenta* proiectil = user_tank->GetTankSpheres().at(i);
		for (int j = 0; j < enemies.size(); j++) // trec prin tancurile inamicilor
		{
			Tank* tank = enemies.at(j);
			if (DetectSphereTankCollisions(proiectil, tank) == 1)
			{
				if (tank->getNrLives() > 0) {
					tank->setNrLives(tank->getNrLives() - 1);
				}
				user_tank->RemoveSphere(i);
			}
		}
	}
	}
	

	// sferele inamicilor
	for (int i = 0; i < enemies.size(); i++) // trec prin tancurile inamicilor
	{
		Tank* tank = enemies.at(i);
		for (int j = 0; j < tank->GetTankSpheres().size(); j++) 
		{
			// trec prin sferele inamicilor
			Componenta* proiectil = tank->GetTankSpheres().at(j);
			// caz 1, intra in pamant
			if (proiectil->getY() < 0)
			{
				tank->RemoveSphere(j);
				continue;
			}

			// caz 2, cladire
			if (proiectil->BuildingCollision(buildings) == 1) {
				tank->RemoveSphere(j);
				continue;
			}

			// caz 3, sferele inamicilor vs tancul userului
			if (DetectSphereTankCollisions(proiectil, user_tank) == 1)
			{
				if (user_tank->getNrLives() > 0) {
					user_tank->setNrLives(user_tank->getNrLives() - 1);
					if (user_tank->getNrLives() == 0)
					{
						printf("Ai pierdut\n");
					}
				}
				tank->RemoveSphere(j);
			}
		}
	}
}



void Tema2::DetectTanksCollisions() {
	for (auto& enemy : enemies)
	{
		float x_enemy = enemy->getX();
		float z_enemy = enemy->getZ();

		float x_user = user_tank->getX();
		float z_user = user_tank->getZ();

		// distanta intre centrele tancurilor
		float distance = std::sqrt((x_enemy - x_user) * (x_enemy - x_user) + (z_enemy - z_user) * (z_enemy - z_user));
		if (distance < 0.7f)
		{
			glm::vec3 dif =
				glm::vec3(enemy->getX() - user_tank->getX(),
					0,
					enemy->getZ() - user_tank->getZ());

			float overlap = 0.7f - glm::length(dif);
			glm::vec3 displacement = glm::normalize(dif) * overlap;

			enemy->PositionTank(enemy->getX() + displacement.x * (+0.5f), 0,
				enemy->getZ() + displacement.z * (+0.5f), 1);

			user_tank->PositionTank(user_tank->getX() + displacement.x * (-0.5f),
				0, user_tank->getZ() + displacement.z * (-0.5f), 1);
		
			if (BuildingCollision(enemy) == 1 ||
				enemy->ExitMap(enemy->getX() + displacement.x * (+0.5f),
					enemy->getZ() + displacement.z * (+0.5f)) == 1) {
				// il aduc pe 1 in pozitie initiala si il mut pe user mai departe
				enemy->PositionTank(enemy->getX() + displacement.x * (-0.5f), 0,
					enemy->getZ() + displacement.z * (-0.5f), 1);
				user_tank->PositionTank(user_tank->getX() + displacement.x * (-1), 0,
					user_tank->getZ() + displacement.z * (-1), 1);
			}
			else if (BuildingCollision(user_tank) == 1 ||
				user_tank->ExitMap(user_tank->getX() + displacement.x * (-0.5f),
					user_tank->getZ() + displacement.z * (-0.5f)) == 1) {
				// il aduc pe user in pozitie initiala si il mut pe 1 mai departe
				enemy->PositionTank(enemy->getX() + displacement.x * (+1), 0,
					enemy->getZ() + displacement.z * (+1), 1);
				user_tank->PositionTank(user_tank->getX() + displacement.x * (+0.5f), 0,
					user_tank->getZ() + displacement.z * (+0.5f), 1);
			}
		}
	}

		// si intre inamici:
		for (auto& enemy1 : enemies)
		{
			for (auto& enemy2 : enemies)
				if (enemy1 != enemy2)
				{

					float x_enemy1 = enemy1->getX();
					float z_enemy1 = enemy1->getZ();

					float x_enemy2 = enemy2->getX();
					float z_enemy2 = enemy2->getZ();

					// distanta intre centrele tancurilor
					float distance = std::sqrt((x_enemy1 - x_enemy2) * (x_enemy1 - x_enemy2) + (z_enemy1 - z_enemy2) * (z_enemy1 - z_enemy2));
					if (distance < 0.7f)
					{
						glm::vec3 dif =
							glm::vec3(enemy1->getX() - enemy2->getX(),
								0,
								enemy1->getZ() - enemy2->getZ());
						
						float overlap = 0.7f - glm::length(dif);
						glm::vec3 displacement = glm::normalize(dif) * overlap;

						enemy1->PositionTank(enemy1->getX() + displacement.x * (+0.5f), 0,
							enemy1->getZ() + displacement.z * (+0.5f), 1);

						enemy2->PositionTank(enemy2->getX() + displacement.x * (-0.5f),
							0, enemy2->getZ() + displacement.z * (-0.5f), 1);
						
						if (BuildingCollision(enemy1) == 1 || 
							enemy1->ExitMap(enemy1->getX() + displacement.x * (+0.5f),
								enemy1->getZ() + displacement.z * (+0.5f)) == 1) {
							// il aduc pe 1 in pozitie initiala si il mut pe 2 mai departe
							enemy1->PositionTank(enemy1->getX() + displacement.x * (-0.5f), 0,
								enemy1->getZ() + displacement.z * (-0.5f), 1);
							enemy2->PositionTank(enemy1->getX() + displacement.x * (-1), 0,
								enemy1->getZ() + displacement.z * (-1), 1);
						}
						else if (BuildingCollision(enemy2) == 1 || 
							enemy2->ExitMap(enemy2->getX() + displacement.x * (-0.5f),
								enemy2->getZ() + displacement.z * (-0.5f)) == 1) {
							// il aduc pe 2 in pozitie initiala si il mut pe 1 mai departe
							enemy1->PositionTank(enemy1->getX() + displacement.x * (+1), 0,
								enemy1->getZ() + displacement.z * (+1), 1);
							enemy2->PositionTank(enemy1->getX() + displacement.x * (+0.5f), 0,
								enemy1->getZ() + displacement.z * (+0.5f), 1);
						}
						

					}

				}
		}
}

void Tema2::RenderScene() {
	// Render ground
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= transform3D::Translate(0, -0.1, 0);
		modelMatrix *= transform3D::Scale(10, 25, 25);
		RenderColorMesh(meshes["plane"], shaders["TemaShader"], modelMatrix, ground);
	}

	// Render sky
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= transform3D::Translate(0, 100, 0);
		modelMatrix *= transform3D::RotateOX(glm::radians(180.0f));
		modelMatrix *= transform3D::Scale(10, 25, 25);
		RenderColorMesh(meshes["plane"], shaders["TemaShader"], modelMatrix, sky);
	}

	// Render lateral_sky
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= transform3D::Translate(100, 0, 0);
		modelMatrix *= transform3D::RotateOZ(glm::radians(270.0f));
		modelMatrix *= transform3D::Scale(10, 25, 25);
		RenderColorMesh(meshes["plane"], shaders["TemaShader"], modelMatrix, sky);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= transform3D::Translate(-100, 0, 0);
		modelMatrix *= transform3D::RotateOZ(glm::radians(90.0f));
		modelMatrix *= transform3D::Scale(10, 25, 25);
		RenderColorMesh(meshes["plane"], shaders["TemaShader"], modelMatrix, sky);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= transform3D::Translate(0, 0, 100);
		modelMatrix *= transform3D::RotateOX(glm::radians(270.0f));
		modelMatrix *= transform3D::Scale(10, 25, 25);
		RenderColorMesh(meshes["plane"], shaders["TemaShader"], modelMatrix, sky);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= transform3D::Translate(0, 0, -100);
		modelMatrix *= transform3D::RotateOX(glm::radians(270.0f));
		modelMatrix *= transform3D::Scale(10, 25, 25);
		RenderColorMesh(meshes["plane"], shaders["TemaShader"], modelMatrix, sky);
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= transform3D::Translate(0, 70, 70);
		modelMatrix *= transform3D::Scale(25, 25, 25);
		RenderColorMesh(meshes["proiectil"], shaders["TemaShader"], modelMatrix, red);
	}



	// Render ground_enemies
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= transform3D::Translate(0, -0.09, 20);
		modelMatrix *= transform3D::Scale(0.1, 0.1, 0.1);
		RenderColorMesh(meshes["plane"], shaders["TemaShader"], modelMatrix, red);
	}

	// Render ground_user
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= transform3D::Translate(0, -0.09, -2);
		modelMatrix *= transform3D::Scale(0.1, 0.1, 0.1);
		RenderColorMesh(meshes["plane"], shaders["TemaShader"], modelMatrix, dark_green);
	}
}

void Tema2::MoveEnemy(Tank* user_tank, Tank* tank, float deltaTimeSeconds) {
	// unghi folosit pentru a roti catre tancul userului
	float angle = atan2(user_tank->getCorp()->getX() - tank->getX(),
		user_tank->getCorp()->getZ() - tank->getZ());
	angle = angle * 180.0f / M_PI; // transform in grade

	if (double (clock() - tank->getStartTimeState()) / CLOCKS_PER_SEC > tank->getTimeoutState())
	{
		if (tank->getState() == 'W') {
			tank->setState('A');
			tank->setStartTimeState(clock());
			tank->setTimeoutState(rand() % 5 + 0.5f); // minim 0.5f, maxim 5
		}
		else if (tank->getState() == 'A') {
			tank->setState('S');
			tank->setStartTimeState(clock());
			tank->setTimeoutState(rand() % 5 + 0.5f);
		}
		else if (tank->getState() == 'S') {
			tank->setState('D');
			tank->setStartTimeState(clock());
			tank->setTimeoutState(rand() % 5 + 0.5f);
		}
		else if (tank->getState() == 'D') {
			tank->setState('W');
			tank->setStartTimeState(clock());
			tank->setTimeoutState(rand() % 5 + 0.5f);
		}
	}
	if (tank->getState() == 'W')
	{
		if (tank->getNrLives() > 0) {
			tank->MoveForward(-deltaTimeSeconds, buildings);
		}
		tank->getTun()->setRotateOY(angle - 90);
		tank->getTurela()->setRotateOY(angle - 90);
	}
	else if (tank->getState() == 'A')
	{
		if (tank->getNrLives() > 0) {
			tank->MoveLeft(deltaTimeSeconds, buildings);
		}
	}
	else if (tank->getState() == 'S')
	{
		if (tank->getNrLives() > 0) {
			tank->MoveForward(deltaTimeSeconds, buildings);
		}
		tank->getTun()->setRotateOY(angle - 90);
		tank->getTurela()->setRotateOY(angle - 90);
	}
	else if (tank->getState() == 'D')
	{
		if (tank->getNrLives() > 0) {
			tank->MoveLeft(-deltaTimeSeconds, buildings);
		}
	}

	float distance = length(glm::vec3(tank->getX(), 0, tank->getZ()) -
		glm::vec3(user_tank->getX(), 0, user_tank->getZ()));

	if (distance < 3.0f)
	{
		tank->CheckAddSphere(meshes["proiectil"], user_tank->getCorp()->getX() - tank->getX(),
			user_tank->getCorp()->getZ() - tank->getZ());
	}
}

void Tema2::RenderTank(Tank* tank, int i, float deltaTimeSeconds) {
	if (i == 0) { 
		// user tank
		float r = tank->getRotation();

		modelMatrix = glm::mat4(1);
		modelMatrix *= transform3D::Translate(tank->getCorp()->getX(),
			tank->getCorp()->getY(), tank->getCorp()->getZ());
		modelMatrix *= transform3D::Scale(tank->getCorp()->getScaleX(), tank->getCorp()->getScaleY(), tank->getCorp()->getScaleZ());
		modelMatrix *= transform3D::RotateOY(r + glm::radians(tank->getCorp()->getRotateOY()));
		RenderColorMesh(tank->getCorp()->getMesh(), shaders["TemaShader"], modelMatrix, dark_green, tank->getNrLives());

		modelMatrix = glm::mat4(1);
		modelMatrix *= transform3D::Translate(tank->getTurela()->getX(),
			tank->getTurela()->getY(), tank->getTurela()->getZ());
		modelMatrix *= transform3D::Scale(tank->getTurela()->getScaleX(), tank->getTurela()->getScaleY(), tank->getTurela()->getScaleZ());
		modelMatrix *= transform3D::RotateOY(r + glm::radians(tank->getTurela()->getRotateOY()));
		RenderColorMesh(tank->getTurela()->getMesh(), shaders["TemaShader"], modelMatrix, light_green, tank->getNrLives());

		modelMatrix = glm::mat4(1);
		modelMatrix *= transform3D::Translate(tank->getTun()->getX(),
			tank->getTun()->getY(), tank->getTun()->getZ());
		modelMatrix *= transform3D::Scale(tank->getTun()->getScaleX(), tank->getTun()->getScaleY(), tank->getTun()->getScaleZ());
		modelMatrix *= transform3D::RotateOY(r + glm::radians(tank->getTun()->getRotateOY()));
		modelMatrix *= transform3D::RotateOZ(glm::radians(tank->getTun()->getRotateOZ()));
		RenderColorMesh(tank->getTun()->getMesh(), shaders["TemaShader"], modelMatrix, gray, tank->getNrLives());

		modelMatrix = glm::mat4(1);
		modelMatrix *= transform3D::Translate(tank->getSenile1()->getX(),
			tank->getSenile1()->getY(), tank->getSenile1()->getZ());
		modelMatrix *= transform3D::RotateOY(r + glm::radians(tank->getSenile1()->getRotateOY()));
		modelMatrix *= transform3D::Scale(tank->getSenile1()->getScaleX(), tank->getSenile1()->getScaleY(), tank->getSenile1()->getScaleZ());
		RenderColorMesh(tank->getSenile1()->getMesh(), shaders["TemaShader"], modelMatrix, gray, tank->getNrLives());

		modelMatrix = glm::mat4(1);
		modelMatrix *= transform3D::Translate(tank->getSenile2()->getX(),
			tank->getSenile2()->getY(), tank->getSenile2()->getZ());
		modelMatrix *= transform3D::Scale(tank->getSenile2()->getScaleX(), tank->getSenile2()->getScaleY(), tank->getSenile2()->getScaleZ());
		modelMatrix *= transform3D::RotateOY(r + glm::radians(tank->getSenile2()->getRotateOY()));
		RenderColorMesh(tank->getSenile2()->getMesh(), shaders["TemaShader"], modelMatrix, gray, tank->getNrLives());
	}
	else { // inamicii
		if (user_tank->getNrLives() > 0)
		{
			MoveEnemy(user_tank, tank, deltaTimeSeconds);
		}
		float r = tank->getRotation();
		modelMatrix = glm::mat4(1);
		modelMatrix *= transform3D::Translate(tank->getCorp()->getX(),
			tank->getCorp()->getY(), tank->getCorp()->getZ());
		modelMatrix *= transform3D::Scale(tank->getCorp()->getScaleX(), tank->getCorp()->getScaleY(), tank->getCorp()->getScaleZ());
		modelMatrix *= transform3D::RotateOY(r + glm::radians(tank->getCorp()->getRotateOY()));
		RenderColorMesh(tank->getCorp()->getMesh(), shaders["TemaShader"], modelMatrix, red, tank->getNrLives());

		modelMatrix = glm::mat4(1);
		modelMatrix *= transform3D::Translate(tank->getTurela()->getX(),
			tank->getTurela()->getY(), tank->getTurela()->getZ());
		modelMatrix *= transform3D::Scale(tank->getTurela()->getScaleX(), tank->getTurela()->getScaleY(), tank->getTurela()->getScaleZ());
		modelMatrix *= transform3D::RotateOY(glm::radians(tank->getTurela()->getRotateOY()));
		RenderColorMesh(tank->getTurela()->getMesh(), shaders["TemaShader"], modelMatrix, blue, tank->getNrLives());

		modelMatrix = glm::mat4(1);
		modelMatrix *= transform3D::Translate(tank->getTun()->getX(),
			tank->getTun()->getY(), tank->getTun()->getZ());
		modelMatrix *= transform3D::Scale(tank->getTun()->getScaleX(), tank->getTun()->getScaleY(), tank->getTun()->getScaleZ());
		modelMatrix *= transform3D::RotateOY(glm::radians(tank->getTun()->getRotateOY()));
		RenderColorMesh(tank->getTun()->getMesh(), shaders["TemaShader"], modelMatrix, gray, tank->getNrLives());

		modelMatrix = glm::mat4(1);
		modelMatrix *= transform3D::Translate(tank->getSenile1()->getX(),
			tank->getSenile1()->getY(), tank->getSenile1()->getZ());
		modelMatrix *= transform3D::RotateOY(r + glm::radians(tank->getSenile1()->getRotateOY()));
		modelMatrix *= transform3D::Scale(tank->getSenile1()->getScaleX(), tank->getSenile1()->getScaleY(), tank->getSenile1()->getScaleZ());
		RenderColorMesh(tank->getSenile1()->getMesh(), shaders["TemaShader"], modelMatrix, gray, tank->getNrLives());

		modelMatrix = glm::mat4(1);
		modelMatrix *= transform3D::Translate(tank->getSenile2()->getX(),
			tank->getSenile2()->getY(), tank->getSenile2()->getZ());
		modelMatrix *= transform3D::Scale(tank->getSenile2()->getScaleX(), tank->getSenile2()->getScaleY(), tank->getSenile2()->getScaleZ());
		modelMatrix *= transform3D::RotateOY(r + glm::radians(tank->getSenile2()->getRotateOY()));
		RenderColorMesh(tank->getSenile2()->getMesh(), shaders["TemaShader"], modelMatrix, gray, tank->getNrLives());
	}
}


void Tema2::RenderSpheres(float deltaTimeSeconds) {
	for (int i = 0; i < user_tank->GetTankSpheres().size(); i++)
	{
		Componenta *projectile = user_tank->GetTankSpheres().at(i);

		modelMatrix = glm::mat4(1);
		glm::vec3 direction = projectile->getVelocity() * (deltaTimeSeconds * 5);
		projectile->setY(projectile->getY() - deltaTimeSeconds * 0.1f);
		float t = double(clock() - projectile->getStartTime()) / CLOCKS_PER_SEC / 10;
		float x = projectile->getInitialX() + (sin(projectile->getYLaunchAngle()) + 0.8) * projectile->getXAngle() * projectile->getInitialSpeed() * t * 3;
		float y = projectile->getInitialY() + projectile->getInitialSpeed()
			* sin(projectile->getYLaunchAngle())
			* t - 0.5f * t * t * 9.8f;
		float z = projectile->getInitialZ() + (sin(projectile->getYLaunchAngle()) + 0.8) * projectile->getZAngle() * projectile->getInitialSpeed() * t * 3;

		projectile->setX(x);
		projectile->setY(y);
		projectile->setZ(z);

		modelMatrix *= transform3D::Translate(x,
			y, z);
		modelMatrix *= transform3D::Scale(projectile->getScaleX(), projectile->getScaleY(), projectile->getScaleZ());

	RenderColorMesh(meshes["proiectil"], shaders["TemaShader"], modelMatrix, glm::vec3(0,0,0));
	}
	// si pentru sferele inamicilor:
	for (int j = 0; j < enemies.size(); j++) {
		Tank* enemy = enemies.at(j);
		for (int i = 0; i < enemy->GetTankSpheres().size(); i++)
		{
			Componenta* projectile = enemy->GetTankSpheres().at(i);
		
			modelMatrix = glm::mat4(1);
			glm::vec3 direction = projectile->getVelocity() * (deltaTimeSeconds * 5);
			projectile->setX(projectile->getX() + (float)direction.x);
			projectile->setY(projectile->getY() - deltaTimeSeconds * 0.1f);
			projectile->setZ(projectile->getZ() + (float)direction.z);

			modelMatrix *= transform3D::Translate(projectile->getX(),
				projectile->getY(), projectile->getZ());
			modelMatrix *= transform3D::Scale(projectile->getScaleX(), projectile->getScaleY(), projectile->getScaleZ());

			RenderColorMesh(meshes["proiectil"], shaders["TemaShader"], modelMatrix, glm::vec3(0, 0, 0));
		}
	}
}

void Tema2::RenderBuildings() {
	for (auto& building : buildings) {
		modelMatrix = glm::mat4(1);
		modelMatrix *= transform3D::Translate(building->getX(),
			building->getY(), building->getZ());
		RenderColorMesh(building->getMesh(), shaders["TemaShader"], modelMatrix, gray);
	}
}

void Tema2::Update(float deltaTimeSeconds)
{
	glLineWidth(3);
	glPointSize(5);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, window->props.resolution.x, window->props.resolution.y);

	if (double(clock() - start_time) / CLOCKS_PER_SEC > timeout && user_tank -> getNrLives() != 0)
	{
		printf("Ai castigat\n");
		int score = 0;
		for (int i = 0; i < enemies.size(); i++) {
			if (enemies.at(i)->getNrLives() == 0)
			{
				score++;
			}
		}
		printf("Ai distrus %d tancuri\n", score);
		user_tank->setNrLives(0);
	}


	// Sets the screen area where to draw
	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
	glEnable(GL_DEPTH_TEST); // debug

	RenderScene();
	DetectTanksCollisions();
	DetectSpheresCollisions();
	RenderTank(user_tank, 0, deltaTimeSeconds);

	RenderBuildings();

	for (auto& tank : enemies)
	{
		RenderTank(tank, 1, deltaTimeSeconds);
	}
	RenderSpheres(deltaTimeSeconds);

	// DrawCoordinateSystem();

}

void Tema2::FrameEnd()
{
	// DrawCoordinateSystem();
	// DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

int Tema2 :: BuildingCollision(Tank *tank) {
	for (auto& building : Tema2 :: buildings)
	{
		float x_check1 = building->getX() - building->getScaleX();
		float x_check2 = building->getX() + building->getScaleX();

		float z_check1 = building->getZ() - building->getScaleZ();
		float z_check2 = building->getZ() + building->getScaleZ();
		
		float x_tank1 = tank->getX() + 0.35f;
		float z_tank1 = tank->getZ() + 0.35f;
		float x_tank2 = tank->getX() - 0.35f;
		float z_tank2 = tank->getZ() - 0.35f;

		if (x_check1 < x_tank1 && x_check2 > x_tank1 && z_check1 < z_tank1 && z_check2 > z_tank1) {
			return 1;
		}

		if (x_check1 < x_tank2 && x_check2 > x_tank2 && z_check1 < z_tank2 && z_check2 > z_tank2) {
			return 1;
		}

		if (x_check1 < x_tank1 && x_check2 > x_tank1 && z_check1 < z_tank2 && z_check2 > z_tank2) {
			return 1;
		}

		if (x_check1 < x_tank2 && x_check2 > x_tank2 && z_check1 < z_tank1 && z_check2 > z_tank1) {
			return 1;
		}
	}
	return 0;
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{

	float cameraSpeed = 2.0f;

	if (window->KeyHold(GLFW_KEY_W) && user_tank->getNrLives() > 0) {

		int exit_code = user_tank->MoveForward(deltaTime, buildings);

		if (exit_code == 1) {
			camera->MoveForward(deltaTime);
			camera->SetPosition(user_tank, 0.0);
		}
	}

	if (window->KeyHold(GLFW_KEY_A) && user_tank->getNrLives() > 0)
	{  
		float previous_rot = user_tank->getRotation();
		user_tank->MoveLeft(deltaTime, buildings);

		float rotation = user_tank->getRotation() - previous_rot;
		camera->SetPosition(user_tank, rotation);
	}

	if (window->KeyHold(GLFW_KEY_S) && user_tank->getNrLives() > 0)
	{
		int exit_code = user_tank->MoveForward(-deltaTime, buildings);

		if (exit_code == 1) {
			camera->MoveForward(-deltaTime);
			camera->SetPosition(user_tank, 0);
		}
	}

	if (window->KeyHold(GLFW_KEY_D) && user_tank->getNrLives() > 0)
	{
		float previous_rot = user_tank->getRotation();
		user_tank->MoveLeft(-deltaTime, buildings);

		float rotation = user_tank->getRotation() - previous_rot;
		camera->SetPosition(user_tank, rotation);
	}
}

void Tema2::OnKeyPress(int key, int mods)
{
	// Add key press event
	if (key == GLFW_KEY_SPACE)
	{
		switch (polygonMode)
		{
		case GL_POINT:
			polygonMode = GL_FILL;
			break;
		case GL_LINE:
			polygonMode = GL_POINT;
			break;
		default:
			polygonMode = GL_LINE;
			break;
		}
	}
}


void Tema2::OnKeyRelease(int key, int mods)
{
	// Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// Add mouse move event
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{

		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

	    renderCameraTarget = true;
		// TODO(student): Rotate the camera in third-person mode around
		// OX and OY using `deltaX` and `deltaY`. Use the sensitivity
		// variables for setting up the rotation speed.
		camera->RotateThirdPerson_OX(-sensivityOX * deltaY);
		camera->RotateThirdPerson_OY(-sensivityOY * deltaX);

		rotatedX += -sensivityOX * deltaY;
		rotatedY += -sensivityOY * deltaX;
		
	}
	else {
		// printf("aici\n");
		float sensivity = 0.3f;

		deltaX = mouseX - lastMouseX;	
		

		user_tank->getTurela()->setRotateOY(user_tank->getTurela()->getRotateOY() - deltaX * sensivity);
		user_tank->getTun()->setRotateOY(user_tank->getTun()->getRotateOY() - deltaX * sensivity);

		lastMouseX = mouseX;

		sensivity = 0.1f;

		deltaY = mouseY - lastMouseZ;

		user_tank->getTun()->setRotateOZ(user_tank->getTun()->getRotateOZ() - deltaY * sensivity);

		lastMouseZ = mouseY;
	}
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// Add mouse button press event
	if (button == 1) {
		user_tank->CheckAddSphere(meshes["proiectil"]);
	
	}

}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// Add mouse button release event
	if (button == 2) {
		camera->RotateThirdPerson_OX(-rotatedX);
		camera->RotateThirdPerson_OY(-rotatedY);

		rotatedX = 0;
		rotatedY = 0;
	}
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}


