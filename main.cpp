//Add Clickable object to the objects collection of the Miscfunct


#include <GL/glut.h>
#include <GL/glfw.h>

#include <stdlib.h>
#include <math.h>
#include "CZA/Vector3.cpp"
#include "CZA/CZAIL.h"
#include <string.h>
#include "CZA/Motion.h"
#include "CZA/Camera.cpp"
#include "CZA/Camera2.cpp"
#include "CZA/SkyBox.h"
#include "CZA/ObjectManager.h"
#include "CZA/Water.h"
#include "CZA/Terrain.h"
#include "CZA/md2model.cpp"
#include "CZA/guy.h"
#include "CZA/staticobject.h"
#include "CZA/font.h"
#include "CZA/misc.h"
#include "CZA/grass.h"
#include "CZA/fog.h"
#include "CZA/TextruedSquare.h"
#include "CZA/bullet.h"
#include "CZA/FPSCamera.cpp"
#include "CZA/Model_3DS.cpp"
//#include "3ds.cpp"
//#include "loader.h"
//#include "3dsloader.cpp"
//#include "Object.h"
#include <vector>

#define BULLETMAX 30
#define WORLDSIZE 512

using namespace std;

  Vector3 position;
  Vector3 size;
  Vector3 g0vec(-999,-999,-999);
  float g0ang;


  //CLoad3DS g_Load3ds;										// This is 3DS class.  This should go in a good model class.
  //Model_3DS m;
  vector<Model_3DS> models3ds;

  SkyBox SkyBox1;
  Grid Grid1;
  Ball Ball1;
  Cube Cube1;
  ObjectManager OManage;
  Loader3ds spaceship, bus,maxtest;//, house;
  Water Lake;
  Water Lake2;
  Terrain Land1;
  FPSCamera Cam; //Camera for scene
  Misc Miscfunct;

  Bullet* bulletList[BULLETMAX];

  TexturedSquare Crosshair;


  MD2Model* _model;
  MD2Model* fighter;
  MD2Model* testmodel;
  MD2Model* van;
  MD2Model* sonic;
  MD2Model* Archv;

  vector<Guy*> _guys;
  vector<StaticObject*> _trees;
  Guy* CameraGuy;
  //vector<GrassObject*> _grass;
  fog myfog;

  GrassObject* grass;

  FontClass Font;
  int WINDOWWIDTH = 640;
  int WINDOWHEIGHT = 480;
  //C3dsLoader obj3dsLoader;

  int mode = 0;
  int mousex = 0;
  int mousey = 0;
  int pickx = 0;
  int picky = 0;
  char s[30];

  int grassmode = 0;

static int slices = 16;
static int stacks = 16;

GLuint landDL;
GLuint treeDL;

int frame = 0;
int timebase = glutGet(GLUT_ELAPSED_TIME);
int time1 = glutGet(GLUT_ELAPSED_TIME);


/* GLUT callback Handlers */
bool runonce = false;

static void
resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 10000.0);//change the last numer to stay on screen.

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
    runonce = false;
}

static void SetDefaultMousePosition(){
    glutWarpPointer(WINDOWWIDTH / 2, WINDOWHEIGHT / 2);
    Cam.ResetLast();
}

void processMouse(int button, int state, int x, int y) {


	//specialKey = glutGetModifiers();

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
        mousex = x;
        mousey = y;
        mode = 1;

        //Create Bullets
        for (int i = 0; i < BULLETMAX; i++){
            if (!bulletList[i]->Active()){

                bulletList[i]->SetPosition(Cam);
                bulletList[i]->setActive(true);

                break;
            }
        }
    }

}



int ztran = -6;
float lx = 0;
float ly = 1;

int ttl = -100;

string inttostring(int val){

       char test[10];

    itoa(val, test, 10);

    string str = test;

    const char * str1 = str.c_str();

    return str;//MessageBox(NULL, str1, "Error", MB_OK);

}

static int
handle_collisions(){
    int jki = 0;
    OManage.handleCollisions(jki);
    return jki;
}

static void
render_skybox(){
    glPushMatrix();
    glDisable(GL_DEPTH_TEST);
       SkyBox1.RenderSkyBox(Cam.GetPosition());
       //SkyBox1.RenderSkyBox();
    glEnable(GL_DEPTH_TEST);
    glColor3f(1,1,1);
}

static void
render_land(){
    (&Land1)->Render();
}

static void
render_guys(){
    glDepthFunc(GL_LEQUAL);
	for(unsigned int i = 0; i < _guys.size(); i++) {
	    if(!_guys[i]->Active())
            continue;

	    if (i==0){
	        glPushMatrix();
	        g0vec = Vector3(_guys[i]->GetCurrentx(),_guys[i]->GetCurrenty(),_guys[i]->GetCurrentz());
	        g0ang = 90 - _guys[i]->walkAngle() * 180 / PI; //rad to deg
            //Cam.SetPosition(_guys[i]->GetCurrentx(),_guys[i]->GetCurrenty(),_guys[i]->GetCurrentz());
            //Cam.orientMe(90 - _guys[i]->angle * 180 / PI);
            //Cam.orientMe(_guys[i]->angle);
            //_guys[i]->DrawBoundingBox();
            glPopMatrix();
	    }
		_guys[i]->draw();
		_guys[i]->Refresh();
		_guys[i]->advance(0.01);
		//_guys[i]->DrawBoundingBox();
	}
    glDepthFunc(GL_LESS);
}

static void
render_bullets(){
    glPushMatrix();

    for(unsigned int i = 0; i < BULLETMAX; i++) {
		if (bulletList[i]->Active()){
            bulletList[i]->Render();
            bulletList[i]->Update(1);
		}

	}

	glPopMatrix();
}

static void
render_trees(){
    glPushMatrix();

    for(unsigned int i = 0; i < _trees.size(); i++) {
		_trees[i]->Render();
	    //_trees[i]->DrawBoundingBox();
	}

	glPopMatrix();
}

static void
render_grass(){
    glPushMatrix();
    if (grassmode ==  1)
       grass->Render();
    glPopMatrix();
}

static void
render_testitems(){

    glPushMatrix();
      glDisable(GL_CULL_FACE);
       //Vector3 camloc = Cam.GetPosition();
       //glColor3f(1,1,0);
       //glTranslatef(0,4.7f,0);
       //CameraGuy->draw(&Cam);
       //CameraGuy->Refresh(); Bad Attempt at 3rd person Need a real 3rd Person Cam
	   //CameraGuy->advance(0.01);
	   //glColor3f(1,1,1);

	   //MotionPath Test
       //MP.draw();

       //Fighter
        glDepthFunc(GL_LEQUAL);
            fighter->draw();
            fighter->Refresh();
            fighter->advance(0.01);
            fighter->DrawBoundingBox();

            //van->draw();
           // van->Refresh();
            //van->advance(0.01);
            //van->DrawBoundingBox();

            sonic->draw();
            sonic->Refresh();
            sonic->advance(0.001);
            sonic->DrawBoundingBox();

            for (int i=0;i<models3ds.size();i++){

                models3ds.at(i).Render();

                //m.Render();
            }

            //Lake.Render();

                //spaceship.Render();
                //g_Load3ds.Render();

            //    house.Render();
        glDepthFunc(GL_LESS);

        //myfog.Render();
        Crosshair.Render(WINDOWWIDTH, WINDOWHEIGHT);
        glEnable(GL_CULL_FACE);
    glPopMatrix();



    //(&maxtest)->SetPosition(Vector3(0 , ly, 0));
    //(&maxtest)->Render();
    //(&maxtest)->DrawBoundingBox();
}

static void
render_world(){

    render_skybox();

    render_land();

    render_trees();

    render_guys();

    render_grass();

    render_testitems();

    render_bullets();
}

static void
renderOnscreenText(int collisions){

                     //Font.glPrint( 30, WINDOWHEIGHT - 50, "This is my Text test!!!", 1, 0.0, 1.0, 0.0, 0.5  );
    Font.glPrint( 30, WINDOWHEIGHT - 50, (char*)(s), 1, 0.0, 1.0, 0.0, 0.5  );
    Font.glPrint( 30, WINDOWHEIGHT - 75, (char*)(inttostring(collisions).c_str()), 1, 0.0, 1.0, 0.0, 0.5  );

    char clx[30];
    char clz[30];
    sprintf(clx,"lx:%4.2f",Cam.lx);
    sprintf(clz,"ly:%4.2f",Cam.lz);

    Font.glPrint( 30, WINDOWHEIGHT - 100, (char*)(clx), 1, 0.0, 1.0, 0.0, 0.5  );
    Font.glPrint( 30, WINDOWHEIGHT - 125, (char*)(clz), 1, 0.0, 1.0, 0.0, 0.5  );

    char cx[30];
    char cz[30];
    sprintf(cx,"x:%4.2f",Cam.x);
    sprintf(cz,"z:%4.2f",Cam.z);

    Font.glPrint( 30, WINDOWHEIGHT - 150, (char*)(cx), 1, 0.0, 1.0, 0.0, 0.5  );
    Font.glPrint( 30, WINDOWHEIGHT - 175, (char*)(cz), 1, 0.0, 1.0, 0.0, 0.5  );

    char cx1[30];
    char cz1[30];
    sprintf(cx1,"TSCALE:%4.2f",Land1.terrainScale());
    sprintf(cz1,"angle:%4.2f",Cam.angle*100.0f);

    Font.glPrint( 30, WINDOWHEIGHT - 200, (char*)(cx1), 1, 0.0, 1.0, 0.0, 0.5  );
    Font.glPrint( 30, WINDOWHEIGHT - 225, (char*)(cz1), 1, 0.0, 1.0, 0.0, 0.5  );

}

static void
startpicking(){
    if (mode == 1)
       Miscfunct.StartPicking(mousex,mousey, pickx,picky, WINDOWHEIGHT);
}

static void
stoppicking(){
    if (mode == 1)
       Miscfunct.StopPicking(pickx, picky, mode);
    else
       glutSwapBuffers();
}

static void
initdisplay_const(){
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (runonce == false){
    runonce = true;
    Cam.moveMeFlat(0);
    }else{
        if (Cam.mode == 1){//follow mode
            if (g0vec.x != -999){
                //Cam.SetPosition(g0vec.x - (Land1.width_starting()/2),g0vec.y,g0vec.z - (Land1.length_starting()/2));
                Cam.SetPosition(g0vec.x,g0vec.y,g0vec.z);
                //Cam.orientMe((180+g0ang)/100.0f);
            }
        }

    }
}

static void
reset_lx_ly(){
    if (lx >= 10)
       lx = 0;
    if (ly >= 10)
       ly = 0;
}

static void
update_lx_ly(){
    lx += 0.01f;
    ly += 0.01f;
}

double savedTime = 0;
double updateTimer = 0;
//const double updateInterval = 1.0/60.0;
const double updateInterval = 1000.0/65.0;


static bool framerate_correct(){

    /*if (time1 - timebase > 1000) {
		sprintf(s,"FPS:%4.2f",frame*1000.0/(time1-timebase));
		timebase = time1;
		frame = 0;
	}*/

    //double currentTime =  glfwGetTime();
    double currentTime = glutGet(GLUT_ELAPSED_TIME);
    double elapseTime = currentTime - savedTime;
    savedTime = currentTime;

    updateTimer += elapseTime;

    if(updateTimer >= updateInterval){

        //update(updateInterval);  //update game logic /keyframe animation

        updateTimer = 0;
        return true;

    }
    return false;
}

static void
display(void)
{


    if (framerate_correct()){

         frame++;
	time1=glutGet(GLUT_ELAPSED_TIME);
	if (time1 - timebase > 1000) {
		sprintf(s,"FPS:%4.2f",frame*1000.0/(time1-timebase));
		timebase = time1;
		frame = 0;
	}

        initdisplay_const();

        startpicking();

        reset_lx_ly();

        render_world();

        int jki = 0;
        jki = handle_collisions();

        renderOnscreenText(jki);

        glPopMatrix();

        update_lx_ly();

        stoppicking();

        glFlush();

    }




}

static void pressKey(int key, int x, int y) {

	switch (key) {
	    case GLUT_KEY_F1:
        //case 's' :
			Cam.SetPosition(55,0,55);
			break;
		case GLUT_KEY_LEFT:
        //case 's' :
			/*Cam.angle -= 0.01f;
			if (Cam.mode == 0){
                Cam.orientMe(Cam.angle);
			}else{
                Cam.CirclePoint(Cam.angle);
			}*/
			Cam.Strafe(-1);
			break;
        case GLUT_KEY_RIGHT:
		//case 'f' :
			/*Cam.angle +=0.01f;
			if (Cam.mode == 0){
                Cam.orientMe(Cam.angle);
			}else{
                Cam.CirclePoint(Cam.angle);
			}*/
			Cam.Strafe(1);
			break;
		case GLUT_KEY_UP:
		//case 'e' :
			Cam.moveMeFlat(1);break;
		case GLUT_KEY_DOWN:
		//case 'd' :
			Cam.moveMeFlat(-1);break;
		case 99 :
			Cam.moveVert(1);break;
		case 118 :
			Cam.moveVert(-1);break;

        case 27 :
        //case 'q':
            exit(0);
            break;
	}
	glutPostRedisplay();
}

static void releaseKey(int key, int x, int y) {

	switch (key) {
		//case GLUT_KEY_LEFT :
		//case GLUT_KEY_RIGHT :
		//	deltaAngle = 0.0f;break;
		//case GLUT_KEY_UP :
		//case GLUT_KEY_DOWN :
		//	deltaMove = 0;break;
	}
}


static void
key(unsigned char key, int x, int y)
{
    switch (key)
    {

        //case GLUT_KEY_LEFT:
        case 's' :
			/*Cam.angle -= 0.01f;
			if (Cam.mode == 0){
                Cam.orientMe(Cam.angle);
			}else{
                Cam.CirclePoint(Cam.angle);
			}*/
			Cam.Strafe(-1);
			break;
        //case GLUT_KEY_RIGHT:
		case 'f' :
			/*Cam.angle +=0.01f;

			if (Cam.mode == 0){
                Cam.orientMe(Cam.angle);
			}else{
                Cam.CirclePoint(Cam.angle);
			}*/

			Cam.Strafe(1);
			break;
		//case GLUT_KEY_UP:
		case 'e' :
			Cam.moveMeFlat(1);break;
		//case GLUT_KEY_DOWN:
		case 'd' :
			Cam.moveMeFlat(-1);break;
		case 'c' :
			Cam.moveVert(1);break;
		case 'v' :
			Cam.moveVert(-1);break;

        case 27 :
        case 'q':
            exit(0);
            break;
        case 'w':
             ztran += 1;
             break;
        case 'k':
             ztran -=1;
             break;
        case '+':
            slices++;
            stacks++;
            break;

        case '-':
            if (slices>3 && stacks>3)
            {
                slices--;
                stacks--;
            }
            break;
        case '1':
            if (Cam.mode == 1)
            Cam.mode = 0;
            else
            Cam.mode = 1;
            break;
        case '2':
            if (grassmode == 1)
            grassmode = 0;
            else
            grassmode = 1;
            break;
    }

    glutPostRedisplay();
}



static void
idle(void)
{
   /*  frame++;
	time1=glutGet(GLUT_ELAPSED_TIME);
	if (time1 - timebase > 1000) {
		sprintf(s,"FPS:%4.2f",frame*1000.0/(time1-timebase));
		timebase = time1;
		frame = 0;
	}*/
    glutPostRedisplay();
}

void mouseMove(int x, int y) {

    Cam.CursorChange(x,y);
    if (x >= (WINDOWWIDTH-10) || x <= (10+0))// || y >= (WINDOWHEIGHT - 10) ||y <= (0 + 10))
        SetDefaultMousePosition();
    //SetDefaultMousePosition();
}

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

//This needs to moved to the guy class.
const float TERRAIN_WIDTH = 512.0f;

struct mapobj{
    char let;
    int x;
    int y;
};

void init(){



    //testing scales
    //float ra = 1;//randomFloat();
    //float radius0 = 0.4f * ra + 0.25f;
    //float tscale = radius0 / 2.5;


    glEnable( GL_TEXTURE_2D );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.0F, 0.0F, ztran);
    //load spaceship 3ds model

    Font.initFont();

    //spaceship.Load3DS ("models\\Castle\\Castle.3ds");
    //spaceship.LoadTexture("models\\Castle\\castlewa.bmp");
    //spaceship.SetScale(5);




    //g_Load3ds.Import3DS("models\\Castle\\Castle.3ds");
    //g_Load3ds.init("models\\Castle\\\0");

    //g_Load3ds.Import3DS("models\\spaceship\\spaceship.3ds");
    //g_Load3ds.init("models\\spaceship\\");


    //CLoad3DS g_Load3ds;										// This is 3DS class.  This should go in a good model class.
    //t3DModel g_3DModel;



    //spaceship.Load3DS ("models\\spaceship\\spaceship.3ds");
    //spaceship.LoadTexture("models\\spaceship\\spaceshiptexture.bmp");

    /*house.Load3DS ("models\\house\\cabin.3DS");
    house.LoadTexture("models\\house\\house.jpg");*/

    //maxtest.Load3DS("models\\cube\\Camaro_2006.3DS");



    Lake.LoadTexture("images\\alpha.jpg","images\\reflection.jpg");
    Lake.SetPosition(Vector3(0,1,0));


    //Land1.LoadTextures("images\\heightmap512.bmp", "images\\green.bmp",10);






    //models3ds.push_back(m);
    //m.scale = 14;

    /*

       Model_3DS m;
    //m.Load("models\\hogwarts\\Hogwarts.3ds");
    m.Load("models\\deathstar\\Death_Star.3ds");
    //m.Load("models\\Castle\\Castle.3DS");
    m.lit = false;
    m.pos.x = Land1.width() / 2;
    m.pos.y += 0.1f;
    m.pos.z = Land1.length() / 2;
    m.scale = .2;

    */


    //Load the model GUY
	_model = MD2Model::load("models\\misc\\blockybalboa.md2");
	if (_model != NULL) {
		_model->setAnimation("run");
	}

	fighter = MD2Model::load("models\\Fighter\\fighter.md2");
	if (fighter != NULL) {
		fighter->setAnimation("run");
		fighter->Rotate(-90);
        fighter->Rotx(true);
        fighter->SetScale(0.05f);

        fighter->SetPosition(Vector3(0,1.5f,0));
		fighter->UseMotionPath = true;
		fighter->SetupMotionPath(5);
		fighter->path->set(0,0,15.0f,0);
		fighter->path->set(1,-20,15.0f,-20);
		fighter->path->set(2,20,15.0f,-20);
		fighter->path->set(3,20,15.0f,20);
		fighter->path->set(4,-20,15.0f,20);
		//OManage.AddToManager(*fighter);
	}

	testmodel = MD2Model::load("models\\banana\\banana.md2");
	if (testmodel != NULL) {
		testmodel->setAnimation("run");
		testmodel->Rotate(-90);
        testmodel->Roty(true);
        testmodel->SetPosition(Vector3(-5,0,0)); //puts tree on surface instead of under
	}

	sonic = MD2Model::load("models\\sonic\\ClassicSonic\\PLAY.md2");
	if (sonic != NULL) {
		sonic->setAnimation("run");
		sonic->Rotate(-90);
        sonic->Rotx(true);
        sonic->SetScale(0.05f);
        sonic->SetPosition(Vector3(15,1,10)); //puts tree on surface instead of under
	}

	van = MD2Model::load("models\\van\\van.md2");
	if (van != NULL) {
		van->setAnimation("run");
		van->Rotate(-90);
        van->Rotx(true);
        van->SetScale(0.05f);
        van->SetPosition(Vector3(-15,1,10)); //puts tree on surface instead of under
	}

	Archv = MD2Model::load("models\\Archvile\\Archvile.md2");
	if (Archv != NULL) {
		Archv->setAnimation("run");
		Archv->Rotate(-90);
        Archv->Rotx(true);
        Archv->SetScale(0.05f);
        Archv->SetPosition(Vector3(-15,1,10)); //puts tree on surface instead of under
	}


    vector<mapobj> mapobjs;
	//Load in file for processing.
    int cmx = 0;
    int cmy = 0;

    int cmwid = 0;
    int cmlen = 0;

    string line;
    ifstream myfile ("Map3.txt");
    if (myfile.is_open())
    {
       while ( myfile.good() )
       {
         getline (myfile,line);

         for (int i = 0;i<line.size();i++){
            if (cmx == 0 && cmy == 0){
                cmwid = line.size();
                break;
            }
            char c = line.at(i);

            if (c != ' ' && c != 'X'){

                mapobj maptemp;
                maptemp.let = c;
                maptemp.x = cmx;
                maptemp.y = cmy;

                mapobjs.push_back(maptemp);


            }

            cmx += 1;

         }
         //cout << line << endl;
         cmx = 0;
         cmy += 1;
       }
       cmlen = cmy;
       myfile.close();
    }


    Land1 = Terrain(cmlen,cmwid);
    //Land1 = Terrain(WORLDSIZE,WORLDSIZE);
    Cam = FPSCamera(&Land1); //Camera for scene

    Land1.LoadTextures("images\\heightmap.bmp", "images\\green.bmp",0);

     Cam.setUseTerrainHeight(false);

    for (int i = 0; i < mapobjs.size();i++){

        mapobj m1 = mapobjs.at(i);

        if (m1.let == 'T'){
            _trees.push_back(StaticObject::makeObject(0, testmodel, &Land1, m1.x, m1.y));
        }

    }



        Model_3DS m;
    //m.Load("models\\hogwarts\\Hogwarts.3ds");
    //m.Load("models\\deathstar\\Death_Star.3ds");
    //m.Load("models\\Castle\\Castle.3DS");
    m.Load("models\\peachtree\\ganyedes_s Peach tree.3ds");
    m.lit = false;
    m.pos.x = Land1.width() / 2;
    m.pos.y += 0.1f;
    m.pos.z = Land1.length() / 2;
    m.scale = 14.0f;


    CameraGuy = new Guy(_model, &Land1, TERRAIN_WIDTH / (Land1.width() - 1));

	//Make the guys
	_guys = Guy::makeGuys(0, _model, &Land1); //Create the guys

	//create banana trees objects
	//_trees = StaticObject::makeObjects(0, testmodel, &Land1); //Create the trees

    grass = new GrassObject(&Land1, (Land1.terrainScale()));

    grass->LoadGrassTexture("images\\grassPackjjjj.jpg","images\\grassmap60.jpg","images\\grassPackmask.jpg",true);

    //bus.Load3DS ("models\\cars\\masterchief2.3ds");

    //obj3dsLoader.Init_3ds("models\\spaceship\\spaceship.3ds");

    //Init Bullets
     for (int i=0 ; i < BULLETMAX; i++){
        bulletList[i] = new Bullet(&Land1);
    }

    //add to manager
    for(unsigned int i = 0; i < _trees.size(); i++) {
		OManage.AddToManager(*_trees[i]);
	}

    //add to manager
	for(unsigned int i = 0; i < _guys.size(); i++) {
		OManage.AddToManager(*_guys[i]);
	}

	for(unsigned int i = 0; i < BULLETMAX; i++) {
		OManage.AddToManager(*bulletList[i]);
	}


	Crosshair.LoadTexture("images\\Crosshair.tga");

	OManage.AddToManager(Cam);


    //OManage.AddToManager(Ball1);
    //OManage.AddToManager(Ball1);
    //OManage.AddToManager(Cube1);
    //OManage.AddToManager(spaceship);
    //OManage.AddToManager(bus);
    //OManage.Is_Collision(Vector3(0,0,0));

     SkyBox1.LoadSkyBoxTexture("skybox\\out_up.bmp",SKYUP, true, false);
     SkyBox1.LoadSkyBoxTexture("skybox\\out_down.bmp",SKYDOWN, true, false);
     SkyBox1.LoadSkyBoxTexture("skybox\\out_east.bmp",SKYRIGHT, true, false);
     SkyBox1.LoadSkyBoxTexture("skybox\\out_west.bmp",SKYLEFT, true, false);
     SkyBox1.LoadSkyBoxTexture("skybox\\out_north.bmp",SKYBACK, true, false);
     SkyBox1.LoadSkyBoxTexture("skybox\\out_south.bmp",SKYFRONT, true, false);

     /*SkyBox1.LoadSkyBoxTexture("skybox\\side1.png",SKYUP, true, false);
     SkyBox1.LoadSkyBoxTexture("skybox\\side6.png",SKYDOWN, true, false);
     SkyBox1.LoadSkyBoxTexture("skybox\\side5.png",SKYRIGHT, true, false);
     SkyBox1.LoadSkyBoxTexture("skybox\\side3.png",SKYLEFT, true, false);
     SkyBox1.LoadSkyBoxTexture("skybox\\side4.png",SKYBACK, true, false);
     SkyBox1.LoadSkyBoxTexture("skybox\\side2.png",SKYFRONT, true, false);
     */

     //(&Lake)->SetPosition(Vector3(0 , -140.0f, 0));
     //(&Lake)->SetScale(500);




      //Display List Test
      //landDL = glGenLists (1);
      //glNewList(landDL, GL_COMPILE);
      //(&Land1)->Render();
      //glEndList();


      /*treeDL = glGenLists (_trees.size());

      for(unsigned int i = 0; i < _trees.size(); i++) {
        glNewList(treeDL + i, GL_COMPILE);
		    _trees[i]->draw();
		glEndList();
      }*/


SetDefaultMousePosition();




}

/* Program entry point */

int
main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("CZA Framework");

    init();

    glutSetCursor(GLUT_CURSOR_NONE);


    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutMouseFunc(processMouse);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    //glutIgnoreKeyRepeat(1);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);


    glClearColor(0,0,0,0);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glutPassiveMotionFunc(mouseMove);

    /*glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);*/

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glutMainLoop();

    return EXIT_SUCCESS;
}

