#ifndef WATER_H_INCLUDED
#define WATER_H_INCLUDED

#include "noise.c"
#include "terrain.h"

class Water:public Object{
      private:
      //#define		RESOLUTION 128
      #define		RESOLUTION 1

         static GLuint	texture;

         Terrain* terrain;
         int	left_click;
         int	right_click;
         int	wire_frame;
         int	normals ;
         int	xold ;
         int	yold ;
         float	rotate_x ;
         float	rotate_y ;
         float	translate_z ;

         float	surface[6 * RESOLUTION * (RESOLUTION + 1)];
         float	normal[6 * RESOLUTION * (RESOLUTION + 1)];
         GLfloat texCoords[((6 * RESOLUTION * (RESOLUTION + 1))/3) * 2];


         float	z (const float x, const float y, const float t)
        {
          const float x2 = x - 3;
          const float y2 = y + 1;
          const float xx = x2 * x2;
          const float yy = y2 * y2;
          return ((2 * sinf (20 * sqrtf (xx + yy) - 4 * t) +
        	   Noise (10 * x, 10 * y, t, 0)) / 200);
        }



     public:

            Water(Terrain* terrain1){

                    terrain = terrain1;

                    glEnable( GL_TEXTURE_2D );
                	left_click = GLUT_UP;
                 	right_click = GLUT_UP;
                 	wire_frame = 0;
                 	normals = 0;
                 	xold = 0;
                 	yold = 0;
                 	rotate_x = 30;
                 	rotate_y = 15;
                 	translate_z = 4;
                 	//translate_z = 0;
                 	//iScale = 0.5f;

                 	surface[6 * RESOLUTION * (RESOLUTION + 1)];
                 	normal[6 * RESOLUTION * (RESOLUTION + 1)];
                    standingwater = true;
                 	InitNoise ();


            }

            /*void SetPosition(Vector3 Position){

                   float x0 = (Position.x/terrain->width_starting()) *((terrain->width()) - radius0) + radius0;
                   float z0 = (Position.z/terrain->length_starting()) *((terrain->width()) - radius0) + radius0;


                   Pos = Vector3(x0, Position.y, z0);
                   //Pos = Position;
                   _cx = x0;
                   _cy = Position.y;
                   _cz = z0;
                   SetCollisionBox();

            }*/

            bool standingwater;

            static void LoadTexture(char * alpha_filename, char * caustic_filename){
                 glPushMatrix();
                 unsigned char * total_texture = new unsigned char[4 * 256 * 256];
                      //unsigned char alpha_texture[256 * 256];
                      //unsigned char caustic_texture[3 * 256 * 256];
                      //unsigned char* total_texture;
                      unsigned char* alpha_texture;
                      unsigned char* caustic_texture;
                      unsigned int i;




                      //texture = BuildTexture("images\\reflection.jpg", true);

                      glGenTextures (1, &texture);
                      int sx, sy, schan;


                      alpha_texture = CZAIL::GetTextureData(alpha_filename,sx,sy,schan, true);
                      caustic_texture = CZAIL::GetTextureData(caustic_filename,sx,sy,schan, true);

                      for (i = 0; i < 256 * 256; i++)
                      {
                          total_texture[4 * i] = caustic_texture[3 * i];
                          total_texture[4 * i + 1] = caustic_texture[3 * i + 1];
                          total_texture[4 * i + 2] = caustic_texture[3 * i + 2];
                          total_texture[4 * i + 3] = alpha_texture[i];
                      }

                      glBindTexture (GL_TEXTURE_2D, texture);
                      gluBuild2DMipmaps (GL_TEXTURE_2D, GL_RGBA, 256, 256, GL_RGBA, GL_UNSIGNED_BYTE,  total_texture);
                      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
                      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
                      glEnable (GL_TEXTURE_GEN_S);
                      glEnable (GL_TEXTURE_GEN_T);
                      glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
                      glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
                      glDisable (GL_TEXTURE_GEN_S);
                      glDisable (GL_TEXTURE_GEN_T);
                      free(alpha_texture);
                      free(caustic_texture);
                      glPopMatrix();
            }

           void Render(){


                glPushMatrix();

                if (standingwater == false){
                      //glEnable (GL_DEPTH_TEST);
                      glEnable (GL_TEXTURE_GEN_S);
                      glEnable (GL_TEXTURE_GEN_T);
                      glEnable (GL_BLEND);
                      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
                      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

                      const float t = glutGet (GLUT_ELAPSED_TIME) / 1000.;
                      const float delta = 2. / RESOLUTION;
                      const unsigned int length = 2 * (RESOLUTION + 1);
                      const float xn = (RESOLUTION + 1) * delta + 1;
                      unsigned int i;
                      unsigned int j;
                      float x;
                      float y;
                      unsigned int indice;
                      unsigned int preindice;

                      /* Yes, I know, this is quite ugly... */
                      float v1x;
                      float v1y;
                      float v1z;

                      float v2x;
                      float v2y;
                      float v2z;

                      float v3x;
                      float v3y;
                      float v3z;

                      float vax;
                      float vay;
                      float vaz;

                      float vbx;
                      float vby;
                      float vbz;

                      float nx;
                      float ny;
                      float nz;

                      float l;
                      int sufsiz = sizeof(surface);
                      int texsiz = sizeof(texCoords);
                      int tcount = 0;

                      glBindTexture (GL_TEXTURE_2D, texture);

                    glTranslatef(Pos.vec[0],Pos.vec[1],Pos.vec[2]);
                    glScalef(iScale, iScale, iScale);

                     // glTranslatef (0, 0, -translate_z);
                      //glRotatef (rotate_y, 1, 0, 0);
                      //glRotatef (rotate_x, 0, 1, 0);

                      int looper = 0;

                      /* Vertices */
                      for (j = 0; j < RESOLUTION; j++)
                        {
                          y = (j + 1) * delta - 1;
                          for (i = 0; i <= RESOLUTION; i++)
                    	{
                    	  indice = 6 * (i + j * (RESOLUTION + 1));

                    	  x = i * delta - 1;
                    	  surface[indice + 3] = x;
                    	  surface[indice + 4] = z (x, y, t);
                    	  surface[indice + 5] = y;

                    	    float tx = (float)i / (float)(RESOLUTION);
                            float ty = (float)j / (float)(RESOLUTION);
                            float txb = (float)(i+1)/(float)(RESOLUTION);
                            float tyb = (float)(j+1)/(float)(RESOLUTION);


                            if (looper == 0){
                                texCoords[tcount] = tx;
                                tcount++;
                                texCoords[tcount] = ty;
                                tcount++;
                                looper++;
                            }else if (looper == 1){
                                texCoords[tcount] = tx;
                                tcount++;
                                texCoords[tcount] = tyb;
                                tcount++;
                                looper++;
                            }else if (looper == 2){
                                texCoords[tcount] = txb;
                                tcount++;
                                texCoords[tcount] = ty;
                                tcount++;
                                looper++;
                            }else if (looper == 3){
                                texCoords[tcount] = txb;
                                tcount++;
                                texCoords[tcount] = tyb;
                                tcount++;
                                looper = 0;
                            }

                    	  if (j != 0)
                    	    {
                    	      /* Values were computed during the previous loop */
                    	      preindice = 6 * (i + (j - 1) * (RESOLUTION + 1));
                    	      surface[indice] = surface[preindice + 3];
                    	      //texCoords[tcount] = surface[preindice + 3];
                    	      //tcount++;
                    	      surface[indice + 1] = surface[preindice + 4];
                    	      //texCoords[tcount] = surface[preindice + 4];
                    	      //tcount++;
                              surface[indice + 2] = surface[preindice + 5];
                    	    }
                    	  else
                    	    {
                    	      surface[indice] = x;
                    	      //texCoords[tcount] = x;
                    	      //tcount++;
                    	      surface[indice + 1] = z (x, -1, t);
                    	      //texCoords[tcount] = z (x, -1, t);
                    	      //tcount++;
                    	      surface[indice + 2] = -1;
                    	    }
                    	}
                        }

                      /* Normals */
                      for (j = 0; j < RESOLUTION; j++)
                        for (i = 0; i <= RESOLUTION; i++)
                          {
                    	indice = 6 * (i + j * (RESOLUTION + 1));

                    	v1x = surface[indice + 3];
                    	v1y = surface[indice + 4];
                    	v1z = surface[indice + 5];

                    	v2x = v1x;
                    	v2y = surface[indice + 1];
                    	v2z = surface[indice + 2];

                    	if (i < RESOLUTION)
                    	  {
                    	    v3x = surface[indice + 9];
                    	    v3y = surface[indice + 10];
                    	    v3z = v1z;
                    	  }
                    	else
                    	  {
                    	    v3x = xn;
                    	    v3y = z (xn, v1z, t);
                    	    v3z = v1z;
                    	  }

                    	vax =  v2x - v1x;
                    	vay =  v2y - v1y;
                    	vaz =  v2z - v1z;

                    	vbx = v3x - v1x;
                    	vby = v3y - v1y;
                    	vbz = v3z - v1z;

                    	nx = (vby * vaz) - (vbz * vay);
                    	ny = (vbz * vax) - (vbx * vaz);
                    	nz = (vbx * vay) - (vby * vax);

                    	l = sqrtf (nx * nx + ny * ny + nz * nz);
                    	if (l != 0)
                    	  {
                    	    l = 1 / l;
                    	    normal[indice + 3] = nx * l;
                    	    normal[indice + 4] = ny * l;
                    	    normal[indice + 5] = nz * l;
                    	  }
                    	else
                    	  {
                    	    normal[indice + 3] = 0;
                    	    normal[indice + 4] = 1;
                    	    normal[indice + 5] = 0;
                    	  }


                    	if (j != 0)
                    	  {
                    	    /* Values were computed during the previous loop */
                    	    preindice = 6 * (i + (j - 1) * (RESOLUTION + 1));
                    	    normal[indice] = normal[preindice + 3];
                    	    normal[indice + 1] = normal[preindice + 4];
                    	    normal[indice + 2] = normal[preindice + 5];
                    	  }
                    	else
                    	  {
                    /* 	    v1x = v1x; */
                    	    v1y = z (v1x, (j - 1) * delta - 1, t);
                    	    v1z = (j - 1) * delta - 1;

                    /* 	    v3x = v3x; */
                    	    v3y = z (v3x, v2z, t);
                    	    v3z = v2z;

                    	    vax = v1x - v2x;
                    	    vay = v1y - v2y;
                    	    vaz = v1z - v2z;

                    	    vbx = v3x - v2x;
                    	    vby = v3y - v2y;
                    	    vbz = v3z - v2z;

                    	    nx = (vby * vaz) - (vbz * vay);
                    	    ny = (vbz * vax) - (vbx * vaz);
                    	    nz = (vbx * vay) - (vby * vax);

                    	    l = sqrtf (nx * nx + ny * ny + nz * nz);
                    	    if (l != 0)
                    	      {
                    		l = 1 / l;
                    		normal[indice] = nx * l;
                    		normal[indice + 1] = ny * l;
                    		normal[indice + 2] = nz * l;
                    	      }
                    	    else
                    	      {
                    		normal[indice] = 0;
                    		normal[indice + 1] = 1;
                    		normal[indice + 2] = 0;
                    	      }
                    	  }
                          }


                      glTranslatef (0, 0.2, 0);

                      /* Render wireframe? */
                      if (wire_frame != 0)
                        glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

                      /* The water */
                      glEnable (GL_TEXTURE_2D);
                      glColor3f (1, 1, 1);
                      glEnableClientState (GL_NORMAL_ARRAY);
                      glEnableClientState (GL_VERTEX_ARRAY);
                      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                      glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
                      glNormalPointer (GL_FLOAT, 0, normal);
                      glVertexPointer (3, GL_FLOAT, 0, surface);

                      for (i = 0; i < RESOLUTION; i++)
                        glDrawArrays (GL_TRIANGLE_STRIP, i * length, length);



                      /* Draw normals? */
                      if (normals != 0)
                        {
                          //glDisable (GL_TEXTURE_2D);
                          glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                          glColor3f (1, 0, 0);
                          glBegin (GL_LINES);
                          for (j = 0; j < RESOLUTION; j++)
                    	for (i = 0; i <= RESOLUTION; i++)
                    	  {
                    	    indice = 6 * (i + j * (RESOLUTION + 1));
                    	    glVertex3fv (&(surface[indice]));
                    	    glVertex3f (surface[indice] + normal[indice] / 50,
                    			surface[indice + 1] + normal[indice + 1] / 50,
                    			surface[indice + 2] + normal[indice + 2] / 50);
                    	  }

                          glEnd ();
                        }
                        glEnable (GL_TEXTURE_2D);
                        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                        glDisable (GL_BLEND);
                        glTexCoordPointer(2, GL_FLOAT, 0, NULL);
                      glNormalPointer (GL_FLOAT, 0, NULL);
                      glVertexPointer (3, GL_FLOAT, 0, NULL);
                       glDisable (GL_TEXTURE_GEN_S);
                      glDisable (GL_TEXTURE_GEN_T);

                    }else{



                                                                  //glEnable (GL_DEPTH_TEST);
                                                                  glEnable (GL_TEXTURE_GEN_S);
                                                                  glEnable (GL_TEXTURE_GEN_T);
                                                                  glEnable (GL_BLEND);
                                                                  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                                                                  glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
                                                                  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

                                                                  const float t = glutGet (GLUT_ELAPSED_TIME) / 1000.;
                                                                  const float delta = 1. / RESOLUTION;
                                                                  const unsigned int length = 2 * (RESOLUTION + 1);
                                                                  const float xn = (RESOLUTION + 1) * delta + 1;
                                                                  unsigned int i;
                                                                  unsigned int j;
                                                                  float x;
                                                                  float y;
                                                                  unsigned int indice;
                                                                  unsigned int preindice;



                                                                  float l;
                                                                  int sufsiz = sizeof(surface);
                                                                  int texsiz = sizeof(texCoords);
                                                                  int tcount = 0;

                                                                  glBindTexture (GL_TEXTURE_2D, texture);

                                                                  //glTranslatef(Pos.vec[0],Pos.vec[1],Pos.vec[2]);
                                                                  glScalef(iScale, iScale, iScale);

                                                                 // glTranslatef (0, 0, -translate_z);
                                                                  //glRotatef (rotate_y, 1, 0, 0);
                                                                  //glRotatef (rotate_x, 0, 1, 0);

                                                                  int looper = 0;

                                                                  /* Vertices */
                                                                  for (j = 0; j < RESOLUTION; j++)
                                                                    {
                                                                      y = (j + 1) * delta - 1;
                                                                      for (i = 0; i <= RESOLUTION; i++)
                                                                    {
                                                                      indice = 6 * (i + j * (RESOLUTION + 1));

                                                                      x = i * delta - 1;
                                                                      surface[indice + 3] = x;
                                                                      surface[indice + 4] = z (x, y, t);
                                                                      surface[indice + 5] = y;

                                                                        float tx = (float)i / (float)(RESOLUTION);
                                                                        float ty = (float)j / (float)(RESOLUTION);
                                                                        float txb = (float)(i+1)/(float)(RESOLUTION);
                                                                        float tyb = (float)(j+1)/(float)(RESOLUTION);


                                                                        if (looper == 0){
                                                                            texCoords[tcount] = tx;
                                                                            tcount++;
                                                                            texCoords[tcount] = ty;
                                                                            tcount++;
                                                                            looper++;
                                                                        }else if (looper == 1){
                                                                            texCoords[tcount] = tx;
                                                                            tcount++;
                                                                            texCoords[tcount] = tyb;
                                                                            tcount++;
                                                                            looper++;
                                                                        }else if (looper == 2){
                                                                            texCoords[tcount] = txb;
                                                                            tcount++;
                                                                            texCoords[tcount] = ty;
                                                                            tcount++;
                                                                            looper++;
                                                                        }else if (looper == 3){
                                                                            texCoords[tcount] = txb;
                                                                            tcount++;
                                                                            texCoords[tcount] = tyb;
                                                                            tcount++;
                                                                            looper = 0;
                                                                        }

                                                                      if (j != 0)
                                                                        {
                                                                          /* Values were computed during the previous loop */
                                                                          preindice = 6 * (i + (j - 1) * (RESOLUTION + 1));
                                                                          surface[indice] = surface[preindice + 3];
                                                                          //texCoords[tcount] = surface[preindice + 3];
                                                                          //tcount++;
                                                                          surface[indice + 1] = surface[preindice + 4];
                                                                          //texCoords[tcount] = surface[preindice + 4];
                                                                          //tcount++;
                                                                          surface[indice + 2] = surface[preindice + 5];
                                                                        }
                                                                      else
                                                                        {
                                                                          surface[indice] = x;
                                                                          //texCoords[tcount] = x;
                                                                          //tcount++;
                                                                          surface[indice + 1] = z (x, -1, t);
                                                                          //texCoords[tcount] = z (x, -1, t);
                                                                          //tcount++;
                                                                          surface[indice + 2] = -1;
                                                                        }
                                                                    }
                                                                    }


  //MY Test
                                                                  /*surface[0] = -0.5f;
                                                                  surface[1] = 0;
                                                                  surface[2] = -0.5f;

                                                                  surface[3] = -0.5f;
                                                                  surface[4] = 0;
                                                                  surface[5] = 0.5;

                                                                  surface[6] = 0.5f;
                                                                  surface[7] = 0;
                                                                  surface[8] = -0.5f;

                                                                  surface[9] = 0.5f;
                                                                  surface[10] = 0;
                                                                  surface[11] = 0.5f;

                                                                  //MY Test
                                                                  surface[0] = -1;
                                                                  surface[1] = 0;
                                                                  surface[2] = -1;

                                                                  surface[3] = -1;
                                                                  surface[4] = 0;
                                                                  surface[5] = 1;

                                                                  surface[6] = 1;
                                                                  surface[7] = 0;
                                                                  surface[8] = -1;

                                                                  surface[9] = 1;
                                                                  surface[10] = 0;
                                                                  surface[11] = 1;

                                                                   //MY Test
                                                                  surface[0] = -1;
                                                                  surface[1] = 0;
                                                                  surface[2] = -1;

                                                                  surface[3] = -1;
                                                                  surface[4] = 0;
                                                                  surface[5] = 1;

                                                                  surface[6] = 1;
                                                                  surface[7] = 0;
                                                                  surface[8] = -1;

                                                                  surface[9] = 1;
                                                                  surface[10] = 0;
                                                                  surface[11] = 1;
*/
                                                                   // glTranslatef(Pos.vec[0],Pos.vec[1],Pos.vec[2]);
                                                                  surface[0] = Pos.vec[0] + 0;
                                                                  surface[1] = 0;
                                                                  surface[2] = Pos.vec[2] + 0;

                                                                  surface[3] = Pos.vec[0] + 0;
                                                                  surface[4] = 0;
                                                                  surface[5] = Pos.vec[2] + 1;

                                                                  surface[6] = Pos.vec[0] + 1;
                                                                  surface[7] = 0;
                                                                  surface[8] = Pos.vec[2] + 0;

                                                                  surface[9] = Pos.vec[0] + 1;
                                                                  surface[10] = 0;
                                                                  surface[11] = Pos.vec[2] + 1;

                                                                  for (int ac = 0; ac <= 11 ; ac++){
                                                                    if (ac!=1 && ac!=4 && ac!=7 && ac!=10 ){
                                                                        surface[ac] -= 1;
                                                                    }
                                                                  }


                                                                  glTranslatef (0, 0.2, 0);


                                                                  /* Render wireframe? */
                                                                  if (wire_frame != 0)
                                                                    glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

                                                                  /* The water */
                                                                  glEnable (GL_TEXTURE_2D);
                                                                  glColor3f (1, 1, 1);
                                                                  glEnableClientState (GL_NORMAL_ARRAY);
                                                                  glEnableClientState (GL_VERTEX_ARRAY);
                                                                  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                                                                  glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
                                                                  glNormalPointer (GL_FLOAT, 0, normal);
                                                                  glVertexPointer (3, GL_FLOAT, 0, surface);

                                                                  for (i = 0; i < RESOLUTION; i++)
                                                                    glDrawArrays (GL_TRIANGLE_STRIP, i * length, length);


                                                                    glEnable (GL_TEXTURE_2D);
                                                                    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                                                                    glDisable (GL_BLEND);
                                                                    glTexCoordPointer(2, GL_FLOAT, 0, NULL);
                                                                  glNormalPointer (GL_FLOAT, 0, NULL);
                                                                  glVertexPointer (3, GL_FLOAT, 0, NULL);
                                                                   glDisable (GL_TEXTURE_GEN_S);
                                                                  glDisable (GL_TEXTURE_GEN_T);

                    }

                    glPopMatrix();
           }

};

GLuint Water::texture;

#endif
