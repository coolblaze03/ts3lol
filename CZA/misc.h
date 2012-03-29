#ifndef MISC_H_INCLUDED
#define MISC_H_INCLUDED

#define BUFSIZE 512
#define PI 3.1415926535f

class Misc
{
private:
        GLuint selectbuf[BUFSIZE];
public:

struct ObjectData{
               Vector3 bl, br, tr, tl;
               int name;
               double z;
        };

vector<ObjectData> Objects;
void processhits2(int hits, GLuint buff[], int pickx, int picky)
{
    int Do3D = 0;

    //do objects in player first

    for (int i = 0; i < Objects.size(); i++){
              ObjectData t1 = Objects[i];

              if (pickx >= t1.bl.x && picky >= t1.bl.y){
                 if (pickx <= t1.br.x && picky >= t1.br.y){
                    if (pickx <= t1.tr.x && picky <= t1.tr.y){
                       if (pickx >= t1.tl.x && picky <= t1.tl.y){

                          hits = 0;
                          Do3D = 0;

                          //if (t1.name >= (&cplayer)->SONGS && t1.name <=(&cplayer)->SONGS + 1000){
                          //      (&cplayer)->PlaySong(t1.name);
                          //}else if (t1.name == (&cplayer)->SCROLL_UP){
                          //	    (&cplayer)->playliststart--;
                          //}else if (t1.name == (&cplayer)->SCROLL_DOWN){
                          //      (&cplayer)->playliststart++;
                          //}else if (t1.name >= (&cplayer)->SONG_PROGRESS_BAR && t1.name <= (&cplayer)->SONG_PROGRESS_BAR + 1000){
                          //       unsigned int splace = t1.name - (&cplayer)->SONG_PROGRESS_BAR;
                          //       (&cplayer)->setPosition(splace);
                          //}else if (t1.name > 0 && t1.name < (&cplayer)->albumcount + 1){
                          //     if (t1.name - 1 == (&cplayer)->selected){
                                    //load playlist and display
                          //        (&cplayer)->loadplaylist(t1.name -1);
                          //        (&cplayer)->playliststart = 0;
                          //    }
                          //    (&cplayer)->selected = t1.name - 1;
                          //}else if (t1.name == (&cplayer)->PREV){
                          //      (&cplayer)->PrevSong();
                          //}else if (t1.name == (&cplayer)->NEXT){
                          //      (&cplayer)->NextSong();
                          //}else if (t1.name == (&cplayer)->PLAY){
                          //      (&cplayer)->PlayPause();
                          //}else if (t1.name == (&cplayer)->SHOW_HIDE_SEARCH){
                          //      (&cplayer)->toggleSearchBox();
                          //}else if (t1.name == (&cplayer)->SEARCH_BOX){
                          //      (&cplayer)->SearchInput = 1;
                          //}

                          break;

                       }else{
                             //no selection
                             Do3D = 1;
                       }
                    }else{
                          //no selection
                          Do3D = 1;
                    }
                 }else{
                       //no selection
                       Do3D = 1;
                 }
              }else{
                    //no selection
                    Do3D = 1;
              }

          }


    if (Do3D == 1){

    //searchactive = false;
    int lastid;
    unsigned int i, j;
    GLuint names, *ptr, minZ,*ptrNames, numberOfNames;

    GLuint bhits = buff[0];
    GLuint zval = buff[1];
    int temphit = buff[3];

    int pholder = 0;
    GLuint topmin = buff[1];

    for (int i = 0; i < hits; i++)
    {
        int ahits = buff[pholder];
        pholder++;
        GLuint pmin = buff[pholder];
        pholder++;
        GLuint pmax = buff[pholder];

        for (int j = 0; j < ahits; j++)
        {
            pholder++;
            if (pmin <= topmin)
            {
                topmin = pmin;
                temphit = buff[pholder];
            }
        }

        pholder++;
    }

    lastid = temphit;

    char test[10];

    itoa(lastid, test, 10);

    string str = test;

    const char * str1 = str.c_str();

    //MessageBox(NULL, str1, "Error", MB_OK);
    //if (lastid > 0 && lastid < (&cplayer)->albumcount + 1){

    //  if (lastid - 1 == (&cplayer)->selected){
       //load playlist and display
     //      (&cplayer)->loadplaylist(lastid -1);
     //      (&cplayer)->playliststart = 0;

     //  }
      //    (&cplayer)->selected = lastid - 1;

       //MessageBox(NULL, str1, "Error", MB_OK);
   // }else if (lastid >= (&cplayer)->SONGS && lastid <=(&cplayer)->SONGS + 1000){
    //      displayint(lastid);
    //}else if (lastid == 0){
    //
    //}
  }//end of if do3d


}

double calculateAngle(double y, double z)
{
    double radtheta, degtheta;

    radtheta = 2.0 * atan2 (y/2.0, z);
    degtheta = (180.0 * radtheta) / PI;
    return (degtheta);
}


void StartPicking(GLdouble x, GLdouble y, int &pickx, int &picky, int WINDOWHEIGHT)
{
    int viewport[4];



    //y = y + 5;

    glSelectBuffer(BUFSIZE, selectbuf);
    glRenderMode(GL_SELECT);
    glInitNames();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();glLoadIdentity();
//viewport[3] -
    glGetIntegerv(GL_VIEWPORT, viewport);
    GLdouble lsi = viewport[3] - y;

    /*if (x < (viewport[2]/2))
    gluPickMatrix(x - 30,  lsi, 1, 1, viewport);
    else
    gluPickMatrix(x + 30,  lsi, 1, 1, viewport);*/

    //gluOrtho2D(0.0, viewport[2], 0.0, viewport[3]);
      pickx = (GLint)x;
    picky = (GLint)lsi;
    gluPickMatrix(x,  lsi, 1, 1, viewport);

    double ratio = (viewport[2] + 0.0) / viewport[3];

    //Alway calculate the field of view angle.

    //float fov = atan((WINDOWWIDTH/2)/z)*2;

    //float fov = calculateAngle(WINDOWHEIGHT, Cam.z);

         float fovr = atan((float)WINDOWHEIGHT/(2.0f*1000.0f)) * 2;

         float fov = fovr * (180.0f/M_PI);
         //displayint((int)fov);



    gluPerspective(53.13, ratio, 0.0001, 1000);
    glMatrixMode(GL_MODELVIEW);


}

void StopPicking(int pickx, int picky, int& mode)
{
     int hits;

     glMatrixMode(GL_PROJECTION);
     glPopMatrix();
     glMatrixMode(GL_MODELVIEW);
     glFlush();

     // returning to normal rendering mode
     hits = glRenderMode(GL_RENDER);

     // if there are hits process them
     if (hits != 0)
         processhits2(hits, selectbuf, pickx, picky);


     mode = 0;

     //glutSwapBuffers();
}

void displayint(int val){

       char test[10];

    itoa(val, test, 10);

    string str = test;

    const char * str1 = str.c_str();

    MessageBox(NULL, str1, "Error", MB_OK);

}


static void enterortho(int WINDOWWIDTH, int WINDOWHEIGHT){

        /////////////////////////////////////////////////////////////////////////////////////////
        glPushMatrix();
        glDisable(GL_DEPTH_TEST);							// Disables Depth Testing
        glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
        glPushMatrix();										// Store The Projection Matrix
        glLoadIdentity();									// Reset The Projection Matrix
        glOrtho(0,WINDOWWIDTH,0,WINDOWHEIGHT,-1,1);							// Set Up An Ortho Screen
        glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
        glPushMatrix();										// Store The Modelview Matrix
        glLoadIdentity();									// Reset The Modelview Matrix
        // glTranslated(x,y,0);								// Position The Text (0,0 - Bo
        ////////////////////////////////////////////////////////////////////////////////////////
}
static void exitortho(){
        ///////////////////////////////////////////////////////////////////////////////////////////
        glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
            glPopMatrix();										// Restore The Old Projection Matrix
            glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
        glPopMatrix();										// Restore The Old Projection Matrix
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);  							// Enables Depth Testing
        glPopMatrix();
       ///////////////////////////////////////////////////////////////////////////////////////////
}


};

#endif
