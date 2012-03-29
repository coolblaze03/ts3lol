#ifndef FOG_H_INCLUDED
#define FOG_H_INCLUDED

class fog{

 public:
    fog();
    ~fog();
    void Render();

private:
bool   gp;				// G Pressed? ( New )
GLuint filter;			// Which Filter To Use
GLuint fogMode[];       // Storage For Three Types Of Fog
GLuint fogfilter;	    // Which Fog To Use
GLfloat fogColor[4];	// Fog Color
void initfog();
bool isinit;

};

void fog::Render(){

    if (isinit == false)
        initfog();



}

void fog::initfog(){
    glFogi(GL_FOG_MODE, fogMode[fogfilter]);		// Fog Mode
    glFogfv(GL_FOG_COLOR, fogColor);			// Set Fog Color
    glFogf(GL_FOG_DENSITY, 0.02f);				// How Dense Will The Fog Be
    glHint(GL_FOG_HINT, GL_DONT_CARE);			// Fog Hint Value
    glFogf(GL_FOG_START, 1.0f);				// Fog Start Depth
    glFogf(GL_FOG_END, 5.0f);				// Fog End Depth
    glEnable(GL_FOG);					// Enables GL_FOG
    isinit = true;
}

fog::fog(){

    fogMode[0] =  GL_EXP;	// Storage For Three Types Of Fog
    fogMode[1] =  GL_EXP2 ;
    fogMode[2] =  GL_LINEAR;
    fogfilter = 1;					// Which Fog To Use
    fogColor[0] = 0.5f;		// Fog Color
    fogColor[1] = 0.5f;
    fogColor[2] = 0.5f;
    fogColor[3] = 1.0f;
    isinit = false;
}

fog::~fog(){
}

#endif
