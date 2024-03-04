# World of Tanks game

## IDE:
* Visual Studio 2022

## Framework folosit:
```
https://github.com/UPB-Graphics/gfx-framework
```

## Mod de folosire
* fisierele trebuie descarcate in src/lab_m1/<NUME_FISIER>, iar NUME_FISIER adaugat in lab_list.h, se ruleaza apoi folosind butonul RUN din Visual Studio

## Constructii tanc din forme mai complexe
Pentru realizarea tancului din forme mai complexe am creat cele 4 obiecte care formeaza tancul (corpul, turela, tunul și șenilele) in 
Blender, le-am dat export ca fisier de tip obj, si le-am salvat in assets/models/my_models. Aceste obiecte + proiectilele apartin clasei 
Componenta si au ca variabile principale:
```
 		float x;
        float y;
        float z;
        float scale_x;
        float scale_y;
        float scale_z;
        float rotateOY;
        float rotateOZ;
```

## Control tanc
### Deplasare tanc prin tastele WASD 
Am variabilele
```
In Tema2.h:
Tank* user_tank;
si in Tank.h:
float x_tank;
float y_tank;
float z_tank;
float rotation;
```
Si functiile principale:
```
  void CreateTank(Mesh *corp, Mesh* senile, Mesh *turela, Mesh *tun, float rotation, float timeout = 1);
  void PositionTank(float x, float y, float z, int initial);
  int MoveForward(float deltaTimeSeconds, vector<Componenta*> buildings);
  int MoveLeft(float deltaTimeSeconds, vector<Componenta*> buildings);
```
	Tancul utilizatorului este pozitionat inital la (0,0,0) si are o rotatie optima. Apoi daca se apasa W sau S se apeleaza MoveForward 
(se verifica coliziunile cu alte tancuri sau cu dimensiunile hartii), iar daca se apasa A sau D se apeleaza MoveLeft (dificultatea a stat 
in mutarea senilelor intr-o directie circulara). 

## Tras cu tunul
Folosite pentru monitorizarea timpului trecut de la ultima tragere:
```
clock_t start_time;
float timeout;
```
In Tank.cpp:
```
int CheckAddSphere(Mesh* mesh, float x = 0, float y = 0); // se verifica daca tancul e distrus si daca a trecut suficient timp de la 
aruncare, x si y reprez. unghiurile pe Ox si Oz de tragere, daca sunt 0 inseamna ca se trage pe directia in care e orientat tunul tancului utilizatorului
void AddSphere(Mesh* mesh, float x = 0, float y = 0); // creeaza sfera
void RemoveSphere(int index); // sterge sfera
```
## Dispariția proiectilului după un timp sau la coliziune cu un inamic
```
DetectSpheresCollisions();
```
Functia detecteaza trei cazuri de coliziune ale proiectilului care duce la disparitia acestuia: caz 1) intra in pamant, caz 2)
coliziune cu cladiri, caz 3) sferele userului vs tancurile inamicilor sau sferele inamicilor vs tancul userului

## Rotația camerei
Camera este implementata in Camera.h, fisier preluat din laborator la care i-am adaugat si functia:
```
void SetPosition(Tank *tank, float rotationAngle);
```
Functia muta camera in functie de coordonatele tancului, dar si in functie de variabilele:
```
	 float firstX;
     float firstY;
     float firstZ;
```
ce reprezinta coordonata unde a fost amplasata camera initial, si apoi un calcul prin care se determina pozitia noua + rotatia determinata 
de parametrul rotationAngle (pe Oy).

La apasarea butonului dreapta, camera se roteste third person in functia:
```
void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY);
```
	
## Rotație turelă tanc
Daca nu este apasat butonul dreapta al mouse-ului, atunci turela, impreuna cu tunul se vor misca pe axa Ox si axa Oy in functie de
miscarile mouseului: stanga-dreapta pe Oy, sus-jos pe Oz.
Salvez pozitia precedenta a mouse-ului cu:
```
// pentru rotatie turela
        float lastMouseX;
        float lastMouseZ;
```

## Atacul jucătorului de către inamici
Inamicii vor avea in permanenta turela indreptata spre tancul utilizatorului, acest lucru se realizeaza in functia din clasa Tema2:
```
void MoveEnemy(Tank* user_tank, Tank* tank, float deltaTimeSeconds);
```
Tancul tank isi va roti turela si tunul inspre user_tank folosind formula:
```
float angle = atan2(user_tank->getCorp()->getX() - tank->getX(), user_tank->getCorp()->getZ() - tank->getZ());
angle = angle * 180.0f / M_PI; // transform in grade
```
Iar cand tancul inamic este la o distanta maxima de cel al utilizatorului;
```
float distance = length(glm::vec3(tank->getX(), 0, tank->getZ()) -
		glm::vec3(user_tank->getX(), 0, user_tank->getZ()));
``` 
Se apeleaza: CheckAddSphere care face verificarile necesare si creeaza proiectilul daca este posibil.

## Mișcarea inamicilor în scenă pe o traiectorie aleatoare
Se folosesc:
```
 clock_t start_time_state;
 float timeout_state;
 char state;
 int nr_lives;
 float srand; // pentru randomizarea timpului in care sta in stari  
```
state poate fi: 'W', 'A', 'S', 'D'. Prima data se afla in 'W', apoi trece in urmatoarele stari avand timeout-uri random.

In clasa Tema2:
```
void MoveEnemy(Tank* user_tank, Tank* tank, float deltaTimeSeconds);
```
Se realizeaza trecerea dintr-o stare in alta daca nu a expirat timpul starii precedente si daca tancul nu a fost distrus.


## Sfârșitul jocului după cel puțin un minut 
In Tema2.cpp, in Init() sunt setate:
```
// cat timp dureaza jocul
        clock_t start_time;
        float timeout;
```
Apoi se fac verificari in Update().
In cazul în care timpul a expirat, în consolă se va afișa scorul. Scorul acumulat se consideră doar dacă timpul de joc a expirat. Dacă 
tancul jucătorului este distrus, atunci se afișează în consolă un mesaj corespunzător. 
Odată scurs timpul de joc, scena va ingheța; doar rotația turelei și a tunului vor mai fi posibile. 


## Desenare clădiri 
Acestea se deseneaza cu functia:
```
Mesh* CreateRectangle(const std::string&& name, const glm::vec3& color, const glm::vec3& size);
```
Care creeaza un paralelipiped folosind vectorul size.
Cladirile se salveaza in vectorul (de Componente) buildings.


## Coliziuni 
### Proiectil - inamic
```
int DetectSphereTankCollisions(Componenta* proiectil, Tank* tank);
```
cod preluat de pe https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection, sectiunea Sphere vs. AABB.
### Tanc - tanc
```
void DetectTanksCollisions();
```
Daca Distanta_Intre_Tancuri < Raza_Tanc_1 + Raza_Tanc_2, atunci acestea s-au ciocnit.
Si se suprapun cu o distanta egala cu |P| = ( Raza_Tanc_1 + Raza_Tanc_2 - Distanta_Intre_Tancuri )
pentru a evita suprapunerea lor, trebuie sa mutam tancurile în direcții opuse astfel încât distanța dintre acestea sa creasca cu |P|.
Directia in care trebuie sa le deplasam poate fi calculata astfel:
Dif = ( Centru_Tanc_2 - Centru_Tanc_1 )
P = |P| * glm::Normalize( Dif ) 
Centru_Tanc_1  += P * -0.5f;
Centru_Tanc_2  += P * 0.5f;

### Tanc - clădire
In functia din clasa Tank:
```
 int BuildingCollision(vector<Componenta*> buildings);
```
Primeste un vector de buildings si returneaza 1 daca este coliziunea, 0 astfel.

### Proiectil - clădire
In functia din clasa Componenta:
```
 int BuildingCollision(vector<Componenta*> buildings);
```

## Avarie inamici
###  Trimis către shader HP-ul tancului
Obiectele se pun pe scena prin functia:
```
  void RenderColorMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& objectColor, int nr_lives = -1);
```
Aici se trimit pozitia luminii, pozitia camerei, proprietatile de material (Kd, Ks, shininess, culoare obiect), matricile View, Model, Projection, numarul de vieti (reprezinta HP-ul tancului).

###  Deformare formă tanc în vertex shader în funcție de HP
In VertexShader.glsl, functia:
```
vec3 deformare(int nr_lives, vec3 coordonate_locale)
```
realizeaza deformarea formei tancului. Se aplica in acest mod:
```
vec3 new_pos = deformare(nr_lives, v_position);
    gl_Position = Projection * View * Model * vec4(new_pos, 1.0);
```
In FragmentShader.glsl:
```
vec3 deformare_culoare(int nr_lives, vec3 color)
```
Se aplica:
```
 vec3 new_color = deformare_culoare(nr_lives1, color);
    out_color = vec4(new_color, 1.f);
```

## Bonus: traiectorie parabola:
Pentru proiectile am urmatoarele variabile, setate cand se creeaza sfera:
```
        glm::vec3 initial_velocity;
        float x_angle;
        float z_angle;
        time_t start_time;
        float initial_speed;
        float y_launch_angle;
        float initial_x;
        float initial_y;
        float initial_z;
```
Y-ul e setat: initial_y + initial_speed * sin(y_launchAngle) * t - 0.5f * gravity * t * t

Asemanator ca la coliziunea proiectil - inamic.
