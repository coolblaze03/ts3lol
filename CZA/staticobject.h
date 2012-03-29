#ifndef STATICOBJECT_H_INCLUDED
#define STATICOBJECT_H_INCLUDED
//Represents a non moving object
class StaticObject: public Guy  {
	private:
		GLuint staticDL;

	public:
		StaticObject(MD2Model* model1,Terrain* terrain1,float terrainScale1):Guy(model1, terrain1, terrainScale1) {

			//Guy::Guy(model1, terrain1, terrainScale1);

			init();
		}

		void init(){
             staticDL = glGenLists (1);
             glNewList(staticDL, GL_COMPILE);
                draw();
             glEndList();
        }

        void Render(){
           glCallList(staticDL);
        }

		void draw() {

			if (model == NULL) {
				return;
			}

            float scale = radius0 / 2.5f;//radius0/2.5f;

			glPushMatrix();
			//glTranslatef(terrainScale * (terrain->width()/ -2), 0 ,terrainScale * (terrain->length()/ -2));
			//glTranslatef(x2d, scale * 11.0f + y(), x2d);
			modellen = GetModelLen();

            glTranslatef(x0, y() - 1.0f, z0);

		    SetCurrentx(x0);SetCurrenty(y() - 1.0f);SetCurrentz(z0); //location
			glRotatef(90 - angle * 180 / PI, 0, 1, 0);
			glColor3f(1, 1, 1);
			glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
			glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
			glScalef(scale, scale, scale);
			model->draw(animTime);
			glPopMatrix();
		}

		float GetModelLen(){
           GLfloat minx, miny, minz;
           GLfloat maxx, maxy, maxz;

           GLfloat xx,yy,zz;

           xx = Siz.vec[0] / 2;
           yy = Siz.vec[1] / 2;
           zz = Siz.vec[2] / 2;

           minx = -xx + Pos.vec[0] - DELTABOX;
           maxx = xx + Pos.vec[0] + DELTABOX;
           miny = -xx + Pos.vec[1] - DELTABOX;
           maxy = xx + Pos.vec[1] + DELTABOX;
           minz = -xx + Pos.vec[2] - DELTABOX;
           maxz = xx + Pos.vec[2] + DELTABOX;

           return ((maxx - minx));

        }



		static vector<StaticObject*> makeObjects(int numObjects, MD2Model* model, Terrain* terrain) {
            float TERRAIN_WIDTH = 512.0f;
            vector<StaticObject*> Sobjects;
            for(int i = 0; i < numObjects; i++) {
                Sobjects.push_back(new StaticObject(model,
                                       terrain,
                                        (terrain->terrainScale())));
            }
            return Sobjects;
        }
};

#endif

struct ObjectPair {
	Guy* guy1;
	Guy* guy2;
};
