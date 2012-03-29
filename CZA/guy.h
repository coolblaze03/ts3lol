#ifndef GUY_H_INCLUDED
#define GUY_H_INCLUDED

#include "Object.h"
#include <typeinfo>
#include "bullet.h"

//The amount by which the Guy class's step function advances the state of a guy
const float GUY_STEP_TIME = 0.01f;
//const float PI = 3.1415926535f;
const int NUM_GUYS = 100;

//Represents a guy
class Guy : public Object {
	protected:
		MD2Model* model;
		Terrain* terrain;
		//float terrainScale; //The scaling factor for the terrain
		float x0;
		float z0;
		float animTime; //The current position in the animation of the model


		//The angle at which the guy is currently walking, in radians.  An angle
		//of 0 indicates the positive x direction, while an angle of PI / 2
		//indicates the positive z direction.  The angle always lies between 0
		//and 2 * PI.

		//The amount of time until step() should next be called
		float timeUntilNextStep;
		bool isTurningLeft; //Whether the guy is currently turning left
		float timeUntilSwitchDir; //The amount of time until switching direction

		//Advances the state of the guy by GUY_STEP_TIME seconds (without
		//altering animTime)
		void step() {
			//Update the turning direction information
			timeUntilSwitchDir -= GUY_STEP_TIME;
			while (timeUntilSwitchDir <= 0) {
				timeUntilSwitchDir += 20 * model->randomFloat() + 15;
				isTurningLeft = !isTurningLeft;
			}

			//Update the position and angle
			//float maxX = terrainScale * (terrain->width() - 1) - radius0;
			//float maxZ = terrainScale * (terrain->length() - 1) - radius0;

			float maxX = (terrain->width() - 1) - radius0;
			float maxZ = (terrain->length() - 1) - radius0;

			x0 += velocityX() * GUY_STEP_TIME;
			z0 += velocityZ() * GUY_STEP_TIME;
			bool hitEdge = false;
			if (x0 < radius0) {
				x0 = radius0;
				hitEdge = true;
			}
			else if (x0 > maxX) {
				x0 = maxX;
				hitEdge = true;
			}

			if (z0 < radius0) {
				z0 = radius0;
				hitEdge = true;
			}
			else if (z0 > maxZ) {
				z0 = maxZ;
				hitEdge = true;
			}

			if (hitEdge) {
				//Turn more quickly if we've hit the edge
				if (isTurningLeft) {
					angle -= 0.5f * speed * GUY_STEP_TIME;
				}
				else {
					angle += 0.5f * speed * GUY_STEP_TIME;
				}
			}
			else if (isTurningLeft) {
				angle -= 0.05f * speed * GUY_STEP_TIME;
			}
			else {
				angle += 0.05f * speed * GUY_STEP_TIME;
			}

			while (angle > 2 * PI) {
				angle -= 2 * PI;
			}
			while (angle < 0) {
				angle += 2 * PI;
			}
		}
	public:
		Guy(MD2Model* model1,
			Terrain* terrain1,
			float terrainScale1) {
			model = model1;
			terrain = terrain1;
			terrainScale = terrainScale1;

			animTime = 0;
			timeUntilNextStep = 0;

			//Initialize certain fields to random values
			float ra = randomFloat();
			radius0 = 0.4f * ra + 0.25f;
			float wid = terrain->width();
			float lens = terrain->length();

			twidth = wid;
			tlength = lens;

			ra = randomFloat();

			//float xmove = (wid - 1) / 2;

			//x0 = ra *(terrainScale * (terrain->width() - 1) - radius0) + radius0;
			x0 = ra *((terrain->width() - 1) - radius0) + radius0;

			//x0 -= xmove;

			ra = randomFloat();

			//z0 = ra *(terrainScale * (terrain->length() - 1) - radius0) + radius0;
			z0 = ra *((terrain->length() - 1) - radius0) + radius0;

			speed = 1.5f * randomFloat() + 2.0f;
			isTurningLeft = randomFloat() < 0.5f;
			angle = 2 * PI * randomFloat();
			timeUntilSwitchDir = randomFloat() * (20 * randomFloat() + 15);
			Setup();SetPosition(Vector3(x0,y(), z0));Refresh();
			ismoveable = true;

		}



		void draw(float x2d, float y2d) {
			if (model == NULL) {
				return;
			}

			x0 = x2d;
			z0 = y2d;


			float scale = radius0 / 2.5f;

			glPushMatrix();
			//glTranslatef(terrainScale * (terrain->width()/ -2), 0 ,terrainScale * (terrain->length()/ -2));
			//glTranslatef(x2d, scale * 11.0f + y(), x2d);
			glTranslatef(x2d, y() - 1.5f, y2d);
		    SetCurrentx(x2d);SetCurrenty(y() - 1.5f);SetCurrentz(y2d); //location
			glRotatef(90 - angle * 180 / PI, 0, 1, 0);
			glColor3f(1, 1, 1);
			glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
			glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
			glScalef(scale, scale, scale);
			model->draw(animTime);
			glPopMatrix();
		}

		//Advances the state of the guy by the specified amount of time, by
		//calling step() the appropriate number of times and adjusting animTime
		void advance(float dt) {
			//Adjust animTime
			animTime += 0.45f * dt * speed / radius0;
			if (animTime > -100000000 && animTime < 1000000000) {
				animTime -= (int)animTime;
				if (animTime < 0) {
					animTime += 1;
				}
			}
			else {
				animTime = 0;
			}

			//Call step() the appropriate number of times
			while (dt > 0) {
				if (timeUntilNextStep < dt) {
					dt -= timeUntilNextStep;
					step();
					timeUntilNextStep = GUY_STEP_TIME;
				}
				else {
					timeUntilNextStep -= dt;
					dt = 0;
				}
			}
		}

		void draw() {
			if (model == NULL) {
				return;
			}



			float scale = radius0 / 2.5f;
			//iScale = scale;

			//float mynewy = scale * 11.0f + (y() + 1.0f);

            //float mynewy = 1.0f + (y() + 1.0f);
            float mynewy = y() + 2.0f;

			glPushMatrix();
			//glTranslatef(terrainScale * (terrain->width()/ -2), 0 ,terrainScale * (terrain->length()/ -2));
			glTranslatef(x0, mynewy, z0);
			SetCurrentx(x0);SetCurrenty(mynewy);SetCurrentz(z0);
			SetPosition(Vector3(x0, mynewy, z0));
			glRotatef(90 - angle * 180 / PI, 0, 1, 0);
			dangle = 90 - angle * 180 / PI;
			glColor3f(1, 1, 1);
			glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
			glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
			glScalef(scale, scale, scale);
			model->draw(animTime);
			SetCollisionBox();
			glPopMatrix();
		}

		void draw(Camera* Cam) {
			if (model == NULL) {
				return;
			}

			x0 = Cam->x + 22.0f;
			z0 = Cam->z + 22.0f;


			float scale = radius0 / 2.5f;

			glPushMatrix();
			//glTranslatef(terrainScale * (terrain->width()/ -2), 0 ,terrainScale * (terrain->length()/ -2));
			//glTranslatef(x2d, scale * 11.0f + y(), x2d);
			glTranslatef(x0, y() + 3.0f,z0);
			//RotationTest
			//glTranslatef(Cam->lx + x0,0,Cam->lz + z0);
			//End RT
		    SetCurrentx(x0);SetCurrenty(y() - 3.0f);SetCurrentz(z0); //location
			glRotatef(90 - angle * 180 / PI, 0, 1, 0);
			glColor3f(1, 1, 1);
			glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
			glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
			glScalef(scale, scale, scale);
			model->draw(animTime);
			glPopMatrix();
		}

		float x() {
			return x0;
		}

		float z() {
			return z0;
		}

		//Returns the current height of the guy on the terrain
		float y() {
			//return terrainScale *
			//	terrain->heightAt( x0 / terrainScale, z0 / terrainScale);
			return terrain->heightAt(x0, z0);
		}



		//Returns the approximate radius of the guy


		//Returns the angle at which the guy is currently walking, in radians.
		//An angle of 0 indicates the positive x direction, while an angle of
		//PI / 2 indicates the positive z direction.
		float walkAngle() {
			return angle;
		}

		static vector<Guy*> makeGuys(int numGuys, MD2Model* model, Terrain* terrain) {
        float TERRAIN_WIDTH = 512.0f;
        vector<Guy*> guys;
        for(int i = 0; i < numGuys; i++) {
            //guys.push_back(new Guy(model,
            //					   terrain,
            //					   TERRAIN_WIDTH / (terrain.width() - 1)));
            guys.push_back(new Guy(model,
                                   terrain,
                                   (terrain->terrainScale())));
        }
        return guys;
        }

        void AfterCollisionDetected(Object* collisionObject){

            //if bullet then active is false.

            Bullet* b = dynamic_cast<Bullet*>(collisionObject);

            const char * name = typeid(collisionObject).name();

            //if (typeid(collisionObject).name() == "Bullet"){
            if (b != NULL){
                isActive = false;
            }


        }



};

struct GuyPair {
	Guy* guy1;
	Guy* guy2;
};

#endif
