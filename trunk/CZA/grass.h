#ifndef GRASS_H_INCLUDED
#define GRASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//Represents a non moving object
class GrassObject: public Object  {
	private:
		//MD2Model* model;
		Terrain* terrain;
		float terrainScale; //The scaling factor for the terrain
		float x0;
		float z0;
		float animTime; //The current position in the animation of the model

		float timeUntilNextStep;
		bool isTurningLeft; //Whether the guy is currently turning left
		float timeUntilSwitchDir; //The amount of time until switching direction
		GLuint staticDL;
		GLuint grasstex;
		GLuint grassmasktex;
		int w; //Width
		int l; //Length
		float** hs; //GrassPoints
		void setpars(int w2, int l2) {
			w = w2;
			l = l2;
			hs = new float*[l2];
			for(int i = 0; i < l2; i++) {
				hs[i] = new float[w2];
			}

        }

        int width() {
			return w;
		}

		int length() {
			return l;
		}

	public:
		GrassObject(Terrain* terrain1,
			float terrainScale1) {
			//model = model1;
			terrain = terrain1;
			terrainScale = terrainScale1;

			animTime = 0;
			timeUntilNextStep = 0;

			//Initialize certain fields to random values
			float ra = randomFloat();
			radius0 = 0.4f * ra + 0.25f;
			float wid = terrain->width();
			float lens = terrain->length();

			ra = randomFloat();

			//float xmove = (wid - 1) / 2;

			x0 = ra *
				(terrainScale * (terrain->width() - 1) - radius0) + radius0;

			//x0 -= xmove;

			ra = randomFloat();

			z0 = ra *
				(terrainScale * (terrain->length() - 1) - radius0) + radius0;

			speed = 1.5f * randomFloat() + 2.0f;
			isTurningLeft = randomFloat() < 0.5f;
			angle = 2 * PI * randomFloat();
			timeUntilSwitchDir = randomFloat() * (20 * randomFloat() + 15);

			Setup();SetPosition(Vector3(x0,y(), z0));Refresh();
		}

		void init(){
             srand ( time(NULL) );
             staticDL = glGenLists (1);
             glNewList(staticDL, GL_COMPILE);

             for(int z = 0; z < this->length() - 1; z++) {
            		for(int x = 0; x < this->width(); x+= 2) {
                         if (getHeight(x,z) > 0.1f)
                             draw(x+1,z+1);//add 1 4 grid placemnt
                    }
              }

             glEndList();
        }

        void Render(){
           glCallList(staticDL);
        }

		void draw(float x2d, float y2d) {

            x0 = x2d;
            z0 = y2d;

			float scale = radius0 / 2.5f;

			float mynewy = scale * 11.0f + (y());

			glPushMatrix();
			glTranslatef(terrainScale * (terrain->width()/ -2), 0 ,terrainScale * (terrain->length()/ -2));
			//glTranslatef(x2d, scale * 11.0f + y(), x2d);
			glTranslatef(terrainScale *x2d, mynewy,terrainScale * y2d);
		    SetCurrentx(x2d);SetCurrenty(mynewy);SetCurrentz(y2d); //location
			glRotatef(90 - angle * 180 / PI, 0, 1, 0);
			glColor3f(1, 1, 1);
			glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
			glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
			glScalef(scale, scale, scale);
			drawGrass();
			glPopMatrix();
		}

		void draw() {



			float scale = radius0 / 2.5f;

			glPushMatrix();
			glTranslatef(terrainScale * (terrain->width()/ -2), 0 ,terrainScale * (terrain->length()/ -2));
			//glTranslatef(x2d, scale * 11.0f + y(), x2d);
			glTranslatef(x0, y() - 1.5f, z0);
		    SetCurrentx(x0);SetCurrenty(y() - 1.5f);SetCurrentz(z0); //location
			glRotatef(90 - angle * 180 / PI, 0, 1, 0);
			glColor3f(1, 1, 1);
			glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
			glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
			glScalef(scale, scale, scale);
		    drawGrass();
			glPopMatrix();


		}

		void drawGrass(){

             //RAND #
             int randomrotation = (rand() % 360);
             glPushMatrix();
             //END RAND

             //Random Rotation
             glRotatef(randomrotation,1,0,0);
             //End Random Rotation

             glDisable(GL_CULL_FACE);

              glAlphaFunc( GL_GREATER, 0.5 );
              glEnable( GL_ALPHA_TEST );


             glColor4f(1,1,1, 1);
             //End Test

             glBindTexture(GL_TEXTURE_2D, grasstex);
             glScalef(5,5,5);
             for (float i = 0; i < 3;i++){

                float wlen = 256.0f / 1024.0f;
        		float cx = float(i*256.0f)/1024.0f;						// X Position Of Current Character
        		//float cy = float(loop/16)/16.0f;						// Y Position Of Current Character

        		glPushMatrix();
                    glRotatef(i*60,1,0,0);
        			glBegin(GL_QUADS);							// Use A Quad For Each Character
        				glTexCoord2f(cx+wlen,1.0);		// Texture Coord (Bottom Left)
        				glVertex2i(0 - 1,0 - 1);						// Vertex Coord (Bottom Left)
        				glTexCoord2f(cx+wlen,0.0);	// Texture Coord (Bottom Right)
        				glVertex2i(1,0 - 1);						// Vertex Coord (Bottom Right)
        				glTexCoord2f(cx,0.0);			// Texture Coord (Top Right)
        				glVertex2i(1,1);						// Vertex Coord (Top Right)
        				glTexCoord2f(cx,1.0);						// Texture Coord (Top Left)
        				glVertex2i(0 - 1,1);						// Vertex Coord (Top Left)
        			glEnd();
                glPopMatrix();    									// Done Building Our Quad (Character)
        			//glTranslated(10,0,0);						// Move To The Right Of The Character
        	    }

             glBindTexture(GL_TEXTURE_2D, (GLuint)NULL);

              glEnable(GL_CULL_FACE);
              glDisable(GL_BLEND);
              glDisable( GL_ALPHA_TEST );
              glDepthFunc(GL_LESS);
              glPopMatrix();
        }

        //Sets the height at (x, z) to y
		void setHeight(int x, int z, float y) {
			hs[z][x] = y;
		}

		//Returns the height at (x, z)
		float getHeight(int x, int z) {
			return hs[z][x];
		}

        void LoadGrassTexture(const char * filename,const char * heightfilename, const char * grassmaskfilename , bool inverttex = false, bool grayscale = false){
             int w, h,channels;
             grasstex = CZAIL::BuildTexture(filename, inverttex, grayscale,false,false,true);//since bmp load alpha channel to it
             grassmasktex = CZAIL::BuildTexture(grassmaskfilename, inverttex, grayscale);
             unsigned char * image = CZAIL::GetTextureData(heightfilename, w, h,channels, true);

            setpars(w, h);
        	for(int y = 0; y < h; y++) {
        		for(int x = 0; x < w; x++) {
        			unsigned char color = (unsigned char)image[3 * (y * w + x)];
        			float h = 1 * ((color / 255.0f) );//- 0.5f);
        			this->setHeight(x, y, h);
        		}
        	}

             init();
        }

		float x() {
			return x0;
		}

		float z() {
			return z0;
		}

		//Returns the current height of the guy on the terrain
		float y() {
              float temp =
				terrain->heightAt( x0 , z0);

			return temp;
		}

		//Returns the approximate radius of the guy
		float radius() {
			return radius0;
		}

		//Returns the angle at which the guy is currently walking, in radians.
		//An angle of 0 indicates the positive x direction, while an angle of
		//PI / 2 indicates the positive z direction.
		float walkAngle() {
			return angle;
		}

		vector<GrassObject*> makeGrass(int numObjects, Terrain* terrain) {
		    float TERRAIN_WIDTH = 512.0f;
            vector<GrassObject*> Gobjects;
            for(int i = 0; i < numObjects; i++) {
                //guys.push_back(new Guy(model,
                //					   terrain,
                //					   TERRAIN_WIDTH / (Land1.width() - 1)));
                Gobjects.push_back(new GrassObject(terrain,
                                        (terrain->terrainScale())));
            }
            return Gobjects;
        }



};

#endif
