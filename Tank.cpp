#include <lab_m1/Tema2/Tema2.h>
#include <lab_m1/Tema2/Tank.h>
#include <lab_m1/Tema2/Componenta.h>
#include <lab_m1/Tema2/Camera.h>
#include <time.h>

using namespace tank; 
using namespace std; 
using namespace tema2;
using namespace componenta;
using namespace camera;


Tank::Tank() {
    dark_green = glm::vec3(0, 0.2, 0);
    light_green = glm::vec3(0, 0.4, 0);
    gray = glm::vec3(0, 0.5, 0.5);
}

Tank::~Tank() {
   
}

void Tank::CreateTank(Mesh* corp, Mesh* senile, Mesh* turela, Mesh* tun, float rotation, float timeout) {
	this->corp.setMesh(corp);
	this->senile1.setMesh(senile);
	this->senile2.setMesh(senile);
	this->turela.setMesh(turela);
    this->tun.setMesh(tun);
	this->rotation = rotation;
	
	this->nr_lives = 3;

	// pentru proiectile
	start_time = NULL;
	this->timeout = timeout;
}

void Tank::PositionTank(float x, float y, float z, int initial) {
	x_tank = x;
	y_tank = y;
	z_tank = z;

	corp.setScaleX(0.2f);
	corp.setScaleY(0.2f);
	corp.setScaleZ(0.2f);
	corp.setX(x);
	corp.setY(y);
	corp.setZ(z);
	corp.setRotateOY(0.0f);


	turela.setScaleX(0.3f);
	turela.setScaleY(0.3f);
	turela.setScaleZ(0.3f);
	turela.setX(x - 0.03f);
	turela.setY(y + 0.15f);
	turela.setZ(z);
	if (initial == 0) {
		turela.setRotateOY(15.0f);
	}
	
	tun.setScaleX(0.1f);
	tun.setScaleY(0.1f);
	tun.setScaleZ(0.1f);
	tun.setX(x - 0.03f);
	tun.setY(y + 0.20f);
	tun.setZ(z);
	if (initial == 0) {
		tun.setRotateOY(105.0f);
		tun.setRotateOZ(70.0f);
	}

	senile1.setScaleX(0.3f);
	senile1.setScaleY(0.3f);
	senile1.setScaleZ(0.3f);
	senile1.setX(x + 0.3f);
	senile1.setY(y);
	senile1.setZ(z);
	senile1.setRotateOY(-90.0f);

	senile2.setScaleX(0.3f);
	senile2.setScaleY(0.3f);
	senile2.setScaleZ(0.3f);
	senile2.setX(x - 0.35f);
	senile2.setY(y);
	senile2.setZ(z);
	senile2.setRotateOY(90.0f);

	float distance1 = 0.3f;
	float distance2 = 0.35f;

	float angle = -getRotation();
	x = getCorp()->getX() + distance1 * cos(angle);
	y = 0;
	z = getCorp()->getZ() + distance1 * sin(angle);
	setSenile1Pos(x, y, z);

	angle = -getRotation();
	x = getCorp()->getX() - distance2 * cos(angle);
	y = 0;
	z = getCorp()->getZ() - distance2 * sin(angle);
	setSenile2Pos(x, y, z);
}

glm::vec3 Tank::getPositionTank() {
	return glm::vec3(x_tank, y_tank, z_tank);
}

Componenta* Tank::getCorp() {
    return &corp;
}
Componenta* Tank::getSenile1() {
    return &senile1;
}

Componenta* Tank::getSenile2() {
	return &senile2;
}

Componenta* Tank::getTurela() {
    return &turela;
}
Componenta* Tank::getTun() {
    return &tun;
}
float Tank::getRotation() {
	return rotation;
}
void Tank::setRotation(float rotation) {
	this->rotation = rotation;
}

float Tank::getX() const {
	return x_tank;
}

void Tank::setX(float x) {
	x_tank = x;
}

float Tank::getY() const {
	return y_tank;
}

void Tank::setY(float y) {
	y_tank = y;
}

float Tank::getZ() const {
	return z_tank;
}

void Tank::setZ(float z) {
	z_tank = z;
}

clock_t Tank::getStartTime() {
	return start_time;
}
void Tank::setStartTime(clock_t startTime) {
	this->start_time = startTime;
}

float Tank::getTimeout() {
	return timeout;
}
void Tank::setTimeout(float timeout) {
	this->timeout = timeout;
}

clock_t Tank::getStartTimeState() {
	return start_time_state;
}
void Tank::setStartTimeState(clock_t startTimeState) {
	this->start_time_state = startTimeState;
}

float Tank::getTimeoutState() {
	return timeout_state;
}

void Tank::setTimeoutState(float timeout_state) {
	this->timeout_state = timeout_state;
}

char Tank::getState() {
	return state;
}

void Tank::setState(char state) {
	this->state = state;
}

void Tank::setCorpPos(float x, float y, float z) {
	corp.setX(x);
	corp.setY(y);
	corp.setZ(z);
}
void Tank::setSenile1Pos(float x, float y, float z) {
	senile1.setX(x);
	senile1.setY(y);
	senile1.setZ(z);
}
void Tank::setSenile2Pos(float x, float y, float z)
{
	senile2.setX(x);
	senile2.setY(y);
	senile2.setZ(z);
}
void Tank::setTurelaPos(float x, float y, float z)
{
	turela.setX(x);
	turela.setY(y);
	turela.setZ(z);
}
void Tank::setTunPos(float x, float y, float z)
{
	tun.setX(x);
	tun.setY(y);
	tun.setZ(z);
}

int Tank::CheckAddSphere(Mesh* mesh, float x, float y) {
	if (nr_lives == 0) {
		return 0;
	}

	if (getStartTime() == NULL) {
		clock_t start = clock();
		setStartTime(start);
		AddSphere(mesh, x, y);
		return 1;
	}

	else if (double(clock() - getStartTime()) / CLOCKS_PER_SEC > getTimeout()) {
		clock_t start = clock();
		setStartTime(start);
		AddSphere(mesh, x, y);
		return 1;
	}
	return 0;
}

void Tank::AddSphere(Mesh * mesh, float x_angle, float y_angle) {
	Componenta *sfera = new Componenta();
	sfera->setMesh(mesh);

	float rotationAngle;
	if (x_angle == 0 && y_angle == 0)
	{
		// userul
		rotationAngle = rotation + glm::radians(turela.getRotateOY() + 180);
		glm::vec3 direction = glm::vec3(sin(rotationAngle), 0, cos(rotationAngle));
		
		sfera->setX(tun.getX() + sin(rotationAngle) * 0.30f);
		sfera->setInitialX(tun.getX() + sin(rotationAngle) * 0.30f);
		
		sfera->setY(0.00556 * tun.getRotateOZ() + 0.20); //0.20f pentru 0, 0.45f pentru 45
		sfera->setInitialY(0.00556 * tun.getRotateOZ() + 0.20);

		sfera->setZ(tun.getZ() + cos(rotationAngle) * 0.30f);
		sfera->setInitialZ(tun.getZ() + cos(rotationAngle) * 0.30f);
		

		sfera->setXAngle(sin(rotationAngle));
		sfera->setZAngle(cos(rotationAngle));

		float initial_speed = 3;
		sfera->setInitialSpeed(initial_speed);
		time_t start_time = clock();
		sfera->setStartTime(start_time);
		sfera->setVelocity(direction);
		sfera->setYLaunchAngle(glm::radians(tun.getRotateOZ()));
	}
	else {
		rotationAngle = atan2(x_angle, y_angle);
		glm::vec3 direction = glm::vec3(sin(rotationAngle), 0, cos(rotationAngle));
		
		sfera->setX(tun.getX() + sin(rotationAngle) * 0.30f);
		sfera->setZ(tun.getZ() + cos(rotationAngle) * 0.30f);

		sfera->setY(tun.getY());
		sfera->setVelocity(direction);
	}
	
	sfera->setScaleX(0.1f);
	sfera->setScaleY(0.1f);
	sfera->setScaleZ(0.1f);


	proiectile.push_back(sfera);
}

vector<Componenta *> Tank::GetTankSpheres() {

	return proiectile;
}
void Tank::RemoveSphere(int index) {
	Componenta* sphere = proiectile[index];
	std::swap(proiectile[index], proiectile.back());
	proiectile.pop_back();

	delete sphere;
}

int Tank::getNrLives() {
	return nr_lives;
}
void Tank::setNrLives(int nr_lives) {
	this->nr_lives = nr_lives;
}

bool Tank::isLessWithPrecision(double a, double b, double epsilon) {
	return a < b || std::abs(a - b) < epsilon;
}

int Tank::ExitMap(float x, float z) {
	// 0 daca nu se iese din harta, 1 daca se iese
	// colt stanga jos (-10, -5)
	// colt dreapta jos (10, -5)
	// colt stanga sus  (-20, 25)
	// colt dreapta sus  (10, 25)
	
	double epsilon = 1e-9;
	if ((isLessWithPrecision(-10, x) == 1 && isLessWithPrecision(x, 10) == 1) &&
		(isLessWithPrecision(-5, z) == 1 && isLessWithPrecision(z, 25) == 1))
	{
		return 0;
	}
	else return 1;
}

int Tank::BuildingCollision(vector<Componenta*> buildings) {
	for (auto& building : buildings)
	{
		float x_check1 = building->getX() - building->getScaleX();
		float x_check2 = building->getX() + building->getScaleX();
		
		float z_check1 = building->getZ() - building->getScaleZ();
		float z_check2 = building->getZ() + building->getScaleZ();

		float x_tank1 = getX() + 0.35f;
		float x_tank2 = getX() - 0.35f;
		
		float z_tank1 = getZ() + 0.35f;
		float z_tank2 = getZ() - 0.35f;

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

int Tank::MoveForward(float deltaTimeSeconds, vector<Componenta*> buildings) {
	float x_aux = getX();
	float z_aux = getZ();

	glm::vec3 forwardVector = glm::vec3(sin(getRotation()), 0.0f, cos(getRotation()));
	glm::vec3 movement = deltaTimeSeconds * forwardVector;

	float new_move_x = movement.x + getX();
	float new_move_z = movement.z + getZ();
	
	if (ExitMap(new_move_x, new_move_z) == 1)
	{
		return 0;
	}

	setX(new_move_x);
	setZ(new_move_z);

	if (BuildingCollision(buildings) == 1)
	{
		setX(x_aux);
		setZ(z_aux);
		return 0;
	}
	setX(x_aux);
	setZ(z_aux);

	PositionTank(new_move_x, movement.y + getY(),
		new_move_z, 1);
	return 1;
}

int Tank::MoveLeft(float deltaTimeSeconds, vector<Componenta*> buildings) {
	float distance1 = 0.30f;
	float distance2 = 0.35f;

	float previous_rot = getRotation();

	setRotation(getRotation() + deltaTimeSeconds);
	float x, y, z;
	float angle = -getRotation();
	x = getCorp()->getX() + distance1 * cos(angle);
	y = 0;
	z = getCorp()->getZ() + distance1 * sin(angle);
	setSenile1Pos(x, y, z);

	angle = -getRotation();
	x = getCorp()->getX() - distance2 * cos(angle);
	y = 0;
	z = getCorp()->getZ() - distance2 * sin(angle);
	setSenile2Pos(x, y, z);
	return 1;
}



