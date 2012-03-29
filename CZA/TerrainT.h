#ifndef TERRAINT_H_INCLUDED
#define TERRAINT_H_INCLUDED

#include <math.h>
#include "Object.h"

class TerrainT: public Object {
	private:
        float TERRAIN_WIDTH;
		int w; //Width
		int l; //Length
		int swidth; //staring width and below is length. because of set pars it will change according to height map. this is to keep track.
		int slength;
		float** hs; //Heights
		Vector3** normals;
		bool computedNormals; //Whether normals is up-to-date
		GLuint landDL;
		void setpars(int w2, int l2) {
            w = w2;
			l = l2;

			hs = new float*[l];
			for(int i = 0; i < l; i++) {
				hs[i] = new float[w];
			}

			normals = new Vector3*[l];
			for(int i = 0; i < l; i++) {
				normals[i] = new Vector3[w];
			}

			computedNormals = false;
        }
        GLuint groundtex;
	public:
           TerrainT() {
             Setup();
             TERRAIN_WIDTH = 50;
             swidth = 50;
             slength = 50;
                     }

		TerrainT(int w2, int l2) {
                    TERRAIN_WIDTH = w2;
			w = w2;
			l = l2;

			swidth = w2;
			slength = l2;

			hs = new float*[l];
			for(int i = 0; i < l; i++) {
				hs[i] = new float[w];
			}

			normals = new Vector3*[l];
			for(int i = 0; i < l; i++) {
				normals[i] = new Vector3[w];
			}

			computedNormals = false;
		}

		~TerrainT() {
			for(int i = 0; i < l; i++) {
				delete[] hs[i];
			}
			delete[] hs;

			for(int i = 0; i < l; i++) {
				delete[] normals[i];
			}
			delete[] normals;
		}



		int width() {
			return w;
		}

		int width_starting(){
		    return swidth;
		}

		int length() {
			return l;
		}

		int length_starting(){
            return slength;
		}
		float terrainScale(){
              return (TERRAIN_WIDTH / (width() - 1));
        }
		//Sets the height at (x, z) to y
		void setHeight(int x, int z, float y) {
			hs[z][x] = y;
			computedNormals = false;
		}

		//Returns the height at (x, z)
		float getHeight(int x, int z) {
			return hs[z][x];
		}

		//Computes the normals, if they haven't been computed yet
		void computeNormals() {
			if (computedNormals) {
				return;
			}

			//Compute the rough version of the normals
			Vector3** normals2 = new Vector3*[l];
			for(int i = 0; i < l; i++) {
				normals2[i] = new Vector3[w];
			}

			for(int z = 0; z < l; z++) {
				for(int x = 0; x < w; x++) {
					Vector3 sum(0.0f, 0.0f, 0.0f);

					Vector3 out;
					if (z > 0) {
						out = Vector3(0.0f, hs[z - 1][x] - hs[z][x], -1.0f);
					}
					Vector3 in;
					if (z < l - 1) {
						in = Vector3(0.0f, hs[z + 1][x] - hs[z][x], 1.0f);
					}
					Vector3 left;
					if (x > 0) {
						left = Vector3(-1.0f, hs[z][x - 1] - hs[z][x], 0.0f);
					}
					Vector3 right;
					if (x < w - 1) {
						right = Vector3(1.0f, hs[z][x + 1] - hs[z][x], 0.0f);
					}

					if (x > 0 && z > 0) {
						out.cross(left).normalize();
						sum += out;
					}
					if (x > 0 && z < l - 1) {
						left.cross(in).normalize();
						sum += left;
					}
					if (x < w - 1 && z < l - 1) {
						in.cross(right).normalize();
						sum += in;
					}
					if (x < w - 1 && z > 0) {
						right.cross(out).normalize();
						sum += right;
					}

					normals2[z][x] = sum;
				}
			}

			//Smooth out the normals
			const float FALLOUT_RATIO = 0.5f;
			for(int z = 0; z < l; z++) {
				for(int x = 0; x < w; x++) {
					Vector3 sum = normals2[z][x];

					if (x > 0) {
						sum += normals2[z][x - 1] * FALLOUT_RATIO;
					}
					if (x < w - 1) {
						sum += normals2[z][x + 1] * FALLOUT_RATIO;
					}
					if (z > 0) {
						sum += normals2[z - 1][x] * FALLOUT_RATIO;
					}
					if (z < l - 1) {
						sum += normals2[z + 1][x] * FALLOUT_RATIO;
					}

					if (sum.magnitude() == 0) {
						sum = Vector3(0.0f, 1.0f, 0.0f);
					}
					normals[z][x] = sum;
				}
			}

			for(int i = 0; i < l; i++) {
				delete[] normals2[i];
			}
			delete[] normals2;

			computedNormals = true;
		}

		//Returns the normal at (x, z)
		Vector3 getNormal(int x, int z) {
			if (!computedNormals) {
				computeNormals();
			}
			return normals[z][x];
		}

		void LoadTextures(char * heightmap, char * ground_texture, float height){
            int w, h, channels;
            unsigned char * image = CZAIL::GetTextureData(heightmap, w, h,channels, true);
            //image = CZAIL::resample_image(image, w, h, channels, width_starting(), length_starting());
            groundtex = CZAIL::BuildTexture(ground_texture, true, false, false, true);

        	setpars(w, h);
        	for(int y = 0; y < h; y++) {
        		for(int x = 0; x < w; x++) {
        			unsigned char color = (unsigned char)image[3 * (y * w + x)];
        			float h = height * ((color / 255.0f) - 0.5f);
        			this->setHeight(x, y, h);
        		}
        	}

        	delete image;
        	this->computeNormals();
        	init();

        }

        //Returns the approximate height of the terrain at the specified (x, z) position
        float heightAt(float x, float z) {
        	//Make (x, z) lie within the bounds of the this
        	if (x < 0) {
        		x = 0;
        	}
        	else if (x > this->width() - 1) {
        		x = this->width() - 1;
        	}
        	if (z < 0) {
        		z = 0;
        	}
        	else if (z > this->length() - 1) {
        		z = this->length() - 1;
        	}

        	//Compute the grid cell in which (x, z) lies and how close we are to the
        	//left and outward edges
        	int leftX = (int)x;
        	if (leftX == this->width() - 1) {
        		leftX--;
        	}
        	float fracX = x - leftX;

        	int outZ = (int)z;
        	if (outZ == this->width() - 1) {
        		outZ--;
        	}
        	float fracZ = z - outZ;

        	//Compute the four heights for the grid cell
        	float h11 = this->getHeight(leftX, outZ);
        	float h12 = this->getHeight(leftX, outZ + 1);
        	float h21 = this->getHeight(leftX + 1, outZ);
        	float h22 = this->getHeight(leftX + 1, outZ + 1);

        	//Take a weighted average of the four heights
        	return (1 - fracX) * ((1 - fracZ) * h11 + fracZ * h12) +
        		fracX * ((1 - fracZ) * h21 + fracZ * h22);
        }

        void init(){
               landDL = glGenLists (1);
               glNewList(landDL, GL_COMPILE);
                   drawTerrain();
               glEndList();
        }

        void drawTerrain(){
             //const float TERRAIN_WIDTH = 50.0f;
             glPushMatrix();
             //glEnable(GL_TEXTURE_GEN_S);


                //glTranslatef(0.0f, 0.0f, -10.0f);

                /*glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
            	glRotatef(-_angle, 0.0f, 1.0f, 0.0f);

            	GLfloat ambientColor[] = {0.4f, 0.4f, 0.4f, 1.0f};
            	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

            	GLfloat lightColor0[] = {0.6f, 0.6f, 0.6f, 1.0f};
            	GLfloat lightPos0[] = {-0.5f, 0.8f, 0.1f, 0.0f};
            	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
            	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);*/
           	    glTranslatef(Pos.vec[0],Pos.vec[1],Pos.vec[2]);
                glColor3f(1,1,1);
                glScalef(iScale,iScale,iScale);

            	//float scale = 5.0f / fmax(this->width() - 1, this->length() - 1);
            	float scale = TERRAIN_WIDTH / (this->width() - 1);
            	glScalef(scale, scale, scale);
            	glTranslatef(-(float)(this->width() - 1) / 2,
            				 0.0f,
            				 -(float)(this->length() - 1) / 2);

            	glColor3f(1,1,1);
          	    glBindTexture (GL_TEXTURE_2D, groundtex);
          	    float alt = 0;

            	for(int z = 0; z < this->length() - 1; z++) {
            		//Makes OpenGL draw a triangle at every three consecutive vertices
            		glBegin(GL_TRIANGLE_STRIP);
            		alt = 0;
            		for(int x = 0; x < this->width(); x+= 2) {

                        /* float tx = (float)alt / (float)(w - 1);
                        float ty = (float)z / (float)(l - 1);
                        float txb = (float)(alt+1)/(float)(w - 1);
                        float tyb = (float)(z+1)/(float)(l - 1);*/

            			Vector3 normal = this->getNormal(x, z);
            			 float holder = this->getHeight(x, z);

                        glNormal3f(normal[0], normal[1], normal[2]);

                        //glTexCoord2f(tx,ty);
                        glTexCoord2f(0.0,1.0);
            			glVertex3f(x, holder, z);

            			float holder1 = this->getHeight(x, z + 1);
                        normal = this->getNormal(x, z + 1);
            			glNormal3f(normal[0], normal[1], normal[2]);

            			//glTexCoord2f(tx,tyb);
            			glTexCoord2f(1.0,1.0);
            			glVertex3f(x, holder1, z + 1);

            			 normal = this->getNormal(x + 1, z);
            			 float holder2 = this->getHeight(x + 1, z);

                        glNormal3f(normal[0], normal[1], normal[2]);

                        //glTexCoord2f(txb,tyb);
                        glTexCoord2f(1.0,0.0);
            			glVertex3f(x + 1, holder2, z);

            			float holder3 = this->getHeight(x + 1, z + 1);
                        normal = this->getNormal(x + 1, z + 1);
            			glNormal3f(normal[0], normal[1], normal[2]);

            			//glTexCoord2f(txb,ty);
            			glTexCoord2f(0.0,0.0);
            			glVertex3f(x + 1, holder3, z + 1);

            			alt++;

            		}
            		glEnd();
            	}
            	//glDisable(GL_TEXTURE_GEN_S);

             glPopMatrix();
        }

        void Render(){
             glDepthFunc(GL_ALWAYS);
             glCallList(landDL);
             glDepthFunc(GL_LESS);
        }
};

#endif
