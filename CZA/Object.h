#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

//for 3ds class
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <io.h>
//end for 3ds class

#define DELTABOX 0.0f
//#include "ObjectManager.h"
#include "CZAIL.h"
#include "Motion.h"

using namespace std;

class Object{
      public:
        Object();
        Object(Vector3 Position, Vector3 Size);
        void SetPosition(Vector3 Position);
        void SetSize(Vector3 Size);
        void Rotate(float angle_deg);
        void Rotx(bool true_flase);
        void Roty(bool true_flase);
        void Rotz(bool true_flase);
        float GetCurrentx(){return _cx;}
        float GetCurrenty(){return _cy;}
        float GetCurrentz(){return _cz;}
        float SetCurrentx(float num){ PrevPos.x = _cx; _cx = num; Pos.x = num;}
        float SetCurrenty(float num){ PrevPos.y = _cy; _cy = num; Pos.y = num;}
        float SetCurrentz(float num){ PrevPos.z = _cz; _cz = num; Pos.z = num;}

        void SetScale(float Scale)
        {
             iScale = Scale;
        }
        Vector3 GetPosition(){
            return Vector3(Pos.x, Pos.y, Pos.z);
        }
        void GetSize();
        void Refresh();
        //void DrawCube();
        virtual void Render(){
                //DrawCube();
        }
        void DrawBoundingBox();
        Vector3 GetMin();
        Vector3 GetMax();
        int ManagerID;
         float velocityX() {
			return speed * cos(angle);
		}

		float velocityZ() {
			return speed * sin(angle);
		}
		float angle;
		float dangle;
		float radius() {
			return radius0;
		}
		float radius0; //The approximate radius of the guy
        bool ismoveable;

        float randomFloat() {

    	return (float)rand() / ((float)RAND_MAX + 1);
        }

        bool UseMotionPath;
        MotionPath *path;

        void SetupMotionPath(int numofpoints){
            path = new MotionPath(numofpoints);
            path->pathpos = 0;
        }

        bool isActive;
        bool isCenterDefined;//based off center being 0,0,0
        float otwidth(){
            return twidth;
        }

        float otlength(){
            return tlength;
        }

        virtual void AfterCollisionDetected(Object* collisionObject);
        Vector3 Normal;

        void setActive(bool active){
            isActive = active;
        }

        bool Active(){
            return isActive;
        }


      protected:
        Vector3 Pos, Siz, min, max, PrevPos;
        float iScale;
        void SetCollisionBox();
        void Setup();
        void Setup(Vector3 Position, Vector3 Size,int Scale = 1 );
        float _rot;
        bool _rotx;
        bool _roty;
        bool _rotz;
        float _cx;
        float _cy;
        float _cz;

        void DrawBoundingBoxNoTransitions();



        float rx, ry, rz;


        float speed;

        //for terrain
        float terrainScale;
        float twidth;
        float tlength;
        float modellen;
        //MD2Model* model;
		//Terrain* terrain;
		int pathstatus;
		void SetMotionPosition(){
		    //UseMotionPath = true;

		    if (path->pointscount() > 1){
		        if (pathstatus == -1){//-1 not initialized.
		            //initialze
		            Vector3 *temp = new Vector3();
		            Vector3 *dest = new Vector3();
		            path->gettwo(path->pathpos,temp,dest);
		            path->temp = temp;

		            double mx = temp->x - dest->x;
		            double mz = temp->z - dest->z;

		            path->xco = mx / 100;
		            path->zco = mz / 100;

		            //if (temp->x > dest->x)
                       path->xco = path->xco * -1;

		            //if (temp->z > dest->z)
                       path->zco = path->zco * -1;

                    SetPosition(*temp);
		            pathstatus = 0;

		        }else if (pathstatus == 0) { //start

                    Vector3 *temp = new Vector3();
		            Vector3 *dest = new Vector3();
                    path->gettwo(path->pathpos,temp,dest);

                    float dex = dest->vec[0];
                    float dez = dest->vec[2];

                    temp = path->temp;

                    float tx = temp->vec[0] + path->xco;
                    float ty = temp->vec[1];
                    float tz = temp->vec[2] + path->zco;

                    temp->set(tx,ty, tz);

                    SetPosition(*temp);

                    path->temp = temp;

                    bool proceed = false;

                    if (path->xco >= 0){

                        if (tx >= dest->vec[0])
                            proceed = true;

                    }else{

                        if (tx <= dest->vec[0])
                            proceed = true;

                    }

                    if (proceed){
                        if (path->zco >= 0){

                            if (tz >= dest->vec[2])
                                pathstatus = 1;

                        }else{

                            if (tz <= dest->vec[2])
                                pathstatus = 1;

                        }
                    }


                    // go until reach the end then set status to 1 then switch to next number then switch back to 0 and it keeps going;

		        }else if (pathstatus == 1){
		            path->pathpos += 1;
		            if (path->pathpos >= path->pointscount())
                        path->pathpos = 0;

                    pathstatus = -1;
		        }
		    }


		}



};

class CustomObject: public Object{

      void Render(){
      }

};

class Cube: public Object{
  public:
      Cube(){
         Setup();
      }
      Cube(Vector3 Position, Vector3 Size){
         Setup(Position, Size);
      }

      void Render(){
               GLfloat x,y,z;

                x = Siz.vec[0] / 2;
                y = Siz.vec[1] / 2;
                z = Siz.vec[2] / 2;


                glPushMatrix();
                glColor3f(0.0f,1.0f,0.0f);	// Color Blue
                glTranslatef(Pos.vec[0],Pos.vec[1],Pos.vec[2]);// Move Right
                glScalef(iScale, iScale, iScale);
                glBegin(GL_QUADS);		// Draw The Cube Using quads

                    glVertex3f( x, y,-z);	// Top Right Of The Quad (Top)
                    glVertex3f(-x, y,-z);	// Top Left Of The Quad (Top)
                    glVertex3f(-x, y, z);	// Bottom Left Of The Quad (Top)
                    glVertex3f( x, y, z);	// Bottom Right Of The Quad (Top)
                    glVertex3f( x,-y, z);	// Top Right Of The Quad (Bottom)
                    glVertex3f(-x,-y, z);	// Top Left Of The Quad (Bottom)
                    glVertex3f(-x,-y,-z);	// Bottom Left Of The Quad (Bottom)
                    glVertex3f( x,-y,-z);	// Bottom Right Of The Quad (Bottom)
                    glVertex3f( x, y, z);	// Top Right Of The Quad (Front)
                    glVertex3f(-x, y, z);	// Top Left Of The Quad (Front)
                    glVertex3f(-x,-y, z);	// Bottom Left Of The Quad (Front)
                    glVertex3f( x,-y, z);	// Bottom Right Of The Quad (Front)
                    glVertex3f( x,-y,-z);	// Top Right Of The Quad (Back)
                    glVertex3f(-x,-y,-z);	// Top Left Of The Quad (Back)
                    glVertex3f(-x, y,-z);	// Bottom Left Of The Quad (Back)
                    glVertex3f( x, y,-z);	// Bottom Right Of The Quad (Back)
                    glVertex3f(-x, y, z);	// Top Right Of The Quad (Left)
                    glVertex3f(-x, y,-z);	// Top Left Of The Quad (Left)
                    glVertex3f(-x,-y,-z);	// Bottom Left Of The Quad (Left)
                    glVertex3f(-x,-y, z);	// Bottom Right Of The Quad (Left)
                    glVertex3f( x, y,-z);	// Top Right Of The Quad (Right)
                    glVertex3f( x, y, z);	// Top Left Of The Quad (Right)
                    glVertex3f( x,-y, z);	// Bottom Left Of The Quad (Right)
                    glVertex3f( x,-y,-z);	// Bottom Right Of The Quad (Right)

              glEnd();			// End Drawing The Cube
              glPopMatrix();
      }

};

class Square: public Object{
  public:
      Square(){
         Setup();
      }
      Square(Vector3 Position, Vector3 Size){
         Setup(Position, Size);
      }

      void Render(){
               GLfloat x,y,z;

                x = Siz.vec[0] / 2;
                y = Siz.vec[1] / 2;
                z = Siz.vec[2];

                glPushMatrix();
                glTranslatef(Pos.vec[0],Pos.vec[1],Pos.vec[2]);// Move Right
                glScalef(iScale, iScale, iScale);
                glBegin(GL_QUADS);				// Draw A Quad
                	glVertex3f(-x, y, 0.0f);		// Top Left
                	glVertex3f( x, y, 0.0f);		// Top Right
                	glVertex3f( x,-y, 0.0f);		// Bottom Right
                	glVertex3f(-x,-y, 0.0f);		// Bottom Left
                glEnd();
                glPopMatrix();
      }

};

class Ball: public Object{
  public:
      Ball(){
         Setup();
      }
      Ball(Vector3 Position, Vector3 Size){
         Setup(Position, Size);
      }

      void Render(){
           GLfloat x,y,z;

            x = Siz.vec[0] / 2;
            y = Siz.vec[1] / 2;
            z = Siz.vec[2];

            glPushMatrix();
            glTranslatef(Pos.vec[0],Pos.vec[1],Pos.vec[2]);// Move Right
            glScalef(iScale, iScale, iScale);
            glutSolidSphere(x,16, 16);
            glPopMatrix();
      }

};

class Grid: public Object{
  public:
      Grid(){
         Setup();
      }
      Grid(Vector3 Position, Vector3 Size){
         Setup(Position, Size);
      }

      void Render(){
               glPushMatrix();
            	for(float i = -500; i <= 500; i += 5)
            	{
            		glBegin(GL_LINES);
            			glColor3ub(150, 190, 150);
            			glVertex3f(-500, 0, i);
            			glVertex3f(500, 0, i);
            			glVertex3f(i, 0,-500);
            			glVertex3f(i, 0, 500);
            		glEnd();
            	}
        	   glPopMatrix();
      }

};



Object::Object(){


      _rot = 0;
      _rotx = false;
      _roty = false;
      _rotz = false;
      rx = 0;
      ry = 0;
      rz = 0;
      twidth = 1;
      tlength = 1;
      ismoveable = false;
      UseMotionPath = false;
      pathstatus = -1;
      isActive = true;
      isCenterDefined = false;
      Normal = Vector3(0,1,0);
     Setup();

}

Object::Object(Vector3 Position, Vector3 Size){


     _rot = 0;
     _rotx = false;
      _roty = false;
      _rotz = false;
            rx = 0;
      ry = 0;
      rz = 0;
      twidth = 1;
      tlength = 1;
      ismoveable = false;
      UseMotionPath = false;
      pathstatus = -1;
        isActive = true;
        isCenterDefined = false;
         Normal = Vector3(0,1,0);
     Setup(Position,Size);
}

void Object::Setup(){
      ManagerID = 9999999;
      iScale = 1;
       Siz = Vector3(2,2,2);
       Pos = Vector3(0,1,0);
       isActive = true;
       isCenterDefined = false;
       SetCollisionBox();
}

void Object::Setup(Vector3 Position, Vector3 Size,int Scale ){
      ManagerID = 9999999;
      iScale = Scale;
       Siz = Size;
       Pos = Position;
       isActive = true;
       isCenterDefined = false;
       SetCollisionBox();
}

void Object::Refresh(){
SetCollisionBox();
}

void Object::SetCollisionBox(){
       GLfloat minx, miny, minz;
       GLfloat maxx, maxy, maxz;

       GLfloat x,y,z;

       x = Siz.vec[0] / 2;
       y = Siz.vec[1] / 2;
       z = Siz.vec[2] / 2;

       minx = -x + Pos.vec[0] - DELTABOX;
       maxx = x + Pos.vec[0] + DELTABOX;
       miny = -x + Pos.vec[1] - DELTABOX;
       maxy = x + Pos.vec[1] + DELTABOX;
       minz = -x + Pos.vec[2] - DELTABOX;
       maxz = x + Pos.vec[2] + DELTABOX;

       min = Vector3(minx, miny, minz);
       max = Vector3(maxx, maxy, maxz);

       if (ManagerID == 9999999){
             //add to manager
             //ManagerID = ObjectManager.size();
             //ObjectManager.push_back(this);
       }else{
             //replace whats in manager using id
             //ObjectManager(ManagerID) = this;
       }
}

void Object::SetPosition(Vector3 Position){

       Pos = Position;
       _cx = Position.x;
       _cy = Position.y;
       _cz = Position.z;
       SetCollisionBox();

}

void Object::SetSize(Vector3 Size){

       Siz = Size;
       SetCollisionBox();

}

void Object::AfterCollisionDetected(Object* collisionObject){
}

Vector3 Object::GetMin(){
        return min;
}

Vector3 Object::GetMax(){
        return max;
}

void Object::Rotx(bool true_flase){
        _rotx = true_flase;
}

void Object::Roty(bool true_flase){
        _roty = true_flase;
}

void Object::Rotz(bool true_flase){
        _rotz = true_flase;
}

void Object::Rotate(float angle_deg){
     _rot = angle_deg;
}

void Object::DrawBoundingBox()
{

    glPushMatrix();
    /*glTranslatef(terrainScale * (twidth/ -2), 0 ,terrainScale * (tlength/ -2));
    glTranslatef(GetCurrentx() ,GetCurrenty(),GetCurrentz());
    //glTranslatef(GetCurrentx() * terrainScale,GetCurrenty(),GetCurrentz()* terrainScale);
        glScalef(iScale, iScale, iScale);
        glRotatef(_rot, _rotx, _roty, _rotz);*/
        glTranslatef(terrainScale * (twidth/ -2), 0 ,terrainScale * (tlength/ -2));
        glTranslatef(Pos.vec[0],Pos.vec[1],Pos.vec[2]);// Move Right
        glScalef(iScale, iScale, iScale);
        glRotatef(_rot, _rotx, _roty, _rotz);
    	//for(int i = 0; i < 5; i++)
    	//{
        	glColor3f(0,0,1.0f);

        	glBegin(GL_LINE_LOOP);
        	glVertex3f( min.vec[0], min.vec[1], min.vec[2] );
        	glVertex3f( max.vec[0], min.vec[1], min.vec[2] );
        	glVertex3f( max.vec[0], min.vec[1], max.vec[2] );
        	glVertex3f( min.vec[0], min.vec[1], max.vec[2] );
        	glEnd();

        	glBegin(GL_LINE_LOOP);
        	glVertex3f( min.vec[0], max.vec[1], min.vec[2] );
        	glVertex3f( max.vec[0], max.vec[1], min.vec[2] );
        	glVertex3f( max.vec[0], max.vec[1], max.vec[2] );
        	glVertex3f( min.vec[0], max.vec[1], max.vec[2] );
        	glEnd();

        	glBegin(GL_LINES);
        	glVertex3f( min.vec[0], min.vec[1], min.vec[2] );
        	glVertex3f( min.vec[0], max.vec[1], min.vec[2] );

        	glVertex3f( max.vec[0], min.vec[1], min.vec[2] );
        	glVertex3f( max.vec[0], max.vec[1], min.vec[2] );

        	glVertex3f( max.vec[0], min.vec[1], max.vec[2] );
        	glVertex3f( max.vec[0], max.vec[1], max.vec[2] );

        	glVertex3f( min.vec[0], min.vec[1], max.vec[2] );
        	glVertex3f( min.vec[0], max.vec[1], max.vec[2] );
        	glEnd();
    	//}
   	glPopMatrix();
}

void Object::DrawBoundingBoxNoTransitions()
{


        	glColor3f(0,0,1.0f);

        	glBegin(GL_LINE_LOOP);
        	glVertex3f( min.vec[0], min.vec[1], min.vec[2] );
        	glVertex3f( max.vec[0], min.vec[1], min.vec[2] );
        	glVertex3f( max.vec[0], min.vec[1], max.vec[2] );
        	glVertex3f( min.vec[0], min.vec[1], max.vec[2] );
        	glEnd();

        	glBegin(GL_LINE_LOOP);
        	glVertex3f( min.vec[0], max.vec[1], min.vec[2] );
        	glVertex3f( max.vec[0], max.vec[1], min.vec[2] );
        	glVertex3f( max.vec[0], max.vec[1], max.vec[2] );
        	glVertex3f( min.vec[0], max.vec[1], max.vec[2] );
        	glEnd();

        	glBegin(GL_LINES);
        	glVertex3f( min.vec[0], min.vec[1], min.vec[2] );
        	glVertex3f( min.vec[0], max.vec[1], min.vec[2] );

        	glVertex3f( max.vec[0], min.vec[1], min.vec[2] );
        	glVertex3f( max.vec[0], max.vec[1], min.vec[2] );

        	glVertex3f( max.vec[0], min.vec[1], max.vec[2] );
        	glVertex3f( max.vec[0], max.vec[1], max.vec[2] );

        	glVertex3f( min.vec[0], min.vec[1], max.vec[2] );
        	glVertex3f( min.vec[0], max.vec[1], max.vec[2] );
        	glEnd();

}


class Loader3ds: public Object{

        #define MAX_VERTICES 8000 // Max number of vertices (for each object)
        #define MAX_POLYGONS 8000 // Max number of polygons (for each object)

        // Our vertex type
        typedef struct{
            float x,y,z;
        }vertex_type;

        // The polygon (triangle), 3 numbers that aim 3 vertices
        typedef struct{
            int a,b,c;
        }polygon_type;

        // The mapcoord type, 2 texture coordinates for each vertex
        typedef struct{
            float u,v;
        }mapcoord_type;

        // The object type
        typedef struct {
        	char name[20];

        	int vertices_qty;
            int polygons_qty;

            vertex_type vertex[MAX_VERTICES];
            polygon_type polygon[MAX_POLYGONS];
            mapcoord_type mapcoord[MAX_VERTICES];
            int id_texture;
        } obj_type, *obj_type_ptr;

        char Load3DSprivte (obj_type_ptr p_object, char *p_filename)
        {
        	int i; //Index variable

        	FILE *l_file; //File pointer

        	unsigned short l_chunk_id; //Chunk identifier
        	unsigned int l_chunk_lenght; //Chunk lenght

        	unsigned char l_char; //Char variable
        	unsigned short l_qty; //Number of elements in each chunk

        	unsigned short l_face_flags; //Flag that stores some face information

        	if ((l_file=fopen (p_filename, "rb"))== NULL) return 0; //Open the file

        	while (ftell (l_file) < filelength (fileno (l_file))) //Loop to scan the whole file
        	{
        		//getche(); //Insert this command for debug (to wait for keypress for each chuck reading)

        		fread (&l_chunk_id, 2, 1, l_file); //Read the chunk header
        		printf("ChunkID: %x\n",l_chunk_id);
        		fread (&l_chunk_lenght, 4, 1, l_file); //Read the lenght of the chunk
        		printf("ChunkLenght: %x\n",l_chunk_lenght);

        		switch (l_chunk_id)
                {
        			//----------------- MAIN3DS -----------------
        			// Description: Main chunk, contains all the other chunks
        			// Chunk ID: 4d4d
        			// Chunk Lenght: 0 + sub chunks
        			//-------------------------------------------
        			case 0x4d4d:
        			break;

        			//----------------- EDIT3DS -----------------
        			// Description: 3D Editor chunk, objects layout info
        			// Chunk ID: 3d3d (hex)
        			// Chunk Lenght: 0 + sub chunks
        			//-------------------------------------------
        			case 0x3d3d:
        			break;

        			//--------------- EDIT_OBJECT ---------------
        			// Description: Object block, info for each object
        			// Chunk ID: 4000 (hex)
        			// Chunk Lenght: len(object name) + sub chunks
        			//-------------------------------------------
        			case 0x4000:
        				i=0;
        				do
        				{
        					fread (&l_char, 1, 1, l_file);
                            p_object->name[i]=l_char;
        					i++;
                        }while(l_char != '\0' && i<20);
        			break;

        			//--------------- OBJ_TRIMESH ---------------
        			// Description: Triangular mesh, contains chunks for 3d mesh info
        			// Chunk ID: 4100 (hex)
        			// Chunk Lenght: 0 + sub chunks
        			//-------------------------------------------
        			case 0x4100:
        			break;

        			//--------------- TRI_VERTEXL ---------------
        			// Description: Vertices list
        			// Chunk ID: 4110 (hex)
        			// Chunk Lenght: 1 x unsigned short (number of vertices)
        			//             + 3 x float (vertex coordinates) x (number of vertices)
        			//             + sub chunks
        			//-------------------------------------------
        			case 0x4110:
        				fread (&l_qty, sizeof (unsigned short), 1, l_file);
                        p_object->vertices_qty = l_qty;
                        printf("Number of vertices: %d\n",l_qty);
                        for (i=0; i<l_qty; i++)
                        {
        					fread (&p_object->vertex[i].x, sizeof(float), 1, l_file);
         					printf("Vertices list x: %f\n",p_object->vertex[i].x);
                            fread (&p_object->vertex[i].y, sizeof(float), 1, l_file);
         					printf("Vertices list y: %f\n",p_object->vertex[i].y);
        					fread (&p_object->vertex[i].z, sizeof(float), 1, l_file);
         					printf("Vertices list z: %f\n",p_object->vertex[i].z);
        				}
        				break;

        			//--------------- TRI_FACEL1 ----------------
        			// Description: Polygons (faces) list
        			// Chunk ID: 4120 (hex)
        			// Chunk Lenght: 1 x unsigned short (number of polygons)
        			//             + 3 x unsigned short (polygon points) x (number of polygons)
        			//             + sub chunks
        			//-------------------------------------------
        			case 0x4120:
        				fread (&l_qty, sizeof (unsigned short), 1, l_file);
                        p_object->polygons_qty = l_qty;
                        printf("Number of polygons: %d\n",l_qty);
                        for (i=0; i<l_qty; i++)
                        {
        					fread (&p_object->polygon[i].a, sizeof (unsigned short), 1, l_file);
        					printf("Polygon point a: %d\n",p_object->polygon[i].a);
        					fread (&p_object->polygon[i].b, sizeof (unsigned short), 1, l_file);
        					printf("Polygon point b: %d\n",p_object->polygon[i].b);
        					fread (&p_object->polygon[i].c, sizeof (unsigned short), 1, l_file);
        					printf("Polygon point c: %d\n",p_object->polygon[i].c);
        					fread (&l_face_flags, sizeof (unsigned short), 1, l_file);
        					printf("Face flags: %x\n",l_face_flags);
        				}
                        break;

        			//------------- TRI_MAPPINGCOORS ------------
        			// Description: Vertices list
        			// Chunk ID: 4140 (hex)
        			// Chunk Lenght: 1 x unsigned short (number of mapping points)
        			//             + 2 x float (mapping coordinates) x (number of mapping points)
        			//             + sub chunks
        			//-------------------------------------------
        			case 0x4140:
        				fread (&l_qty, sizeof (unsigned short), 1, l_file);
        				for (i=0; i<l_qty; i++)
        				{
        					fread (&p_object->mapcoord[i].u, sizeof (float), 1, l_file);
        					printf("Mapping list u: %f\n",p_object->mapcoord[i].u);
                            fread (&p_object->mapcoord[i].v, sizeof (float), 1, l_file);
        					printf("Mapping list v: %f\n",p_object->mapcoord[i].v);
        				}
                        break;

        			//----------- Skip unknow chunks ------------
        			//We need to skip all the chunks that currently we don't use
        			//We use the chunk lenght information to set the file pointer
        			//to the same level next chunk
        			//-------------------------------------------
        			default:
        				 fseek(l_file, l_chunk_lenght-6, SEEK_CUR);
                }
        	}
        	fclose (l_file); // Closes the file stream

        	float tx, ty, tz;
        	tx = p_object->vertex[0].x /2;
        	ty = p_object->vertex[0].y /2;
        	tz = p_object->vertex[0].z /2;

            Siz = Vector3(tx, ty ,tz);


        	return (1); // Returns ok
        }

      public:

         Loader3ds(){
            Setup();
            filling = 1;
            iScale = 0.1f;
         }

         Loader3ds(char *p_filename){

            Load3DSprivte (&object_3ds, p_filename);
            filling = 1;
            iScale = 0.1f;
            //Setup();// need to look at this for setup
            //Setup(Position, Size); //this also
         }
         Loader3ds(char *model_filename, char * texture_filename){

            Load3DSprivte (&object_3ds, model_filename);
            object_3ds.id_texture=CZAIL::BuildTexture(texture_filename, true);
            filling = 1;
            iScale = 0.1f;
            //Setup();// need to look at this for setup
            //Setup(Position, Size); //this also
         }

        char Load3DS(char *p_filename){
             Load3DSprivte (&object_3ds, p_filename);
        }

        void LoadTexture(char * filename){
             object_3ds.id_texture=CZAIL::BuildTexture(filename, true);
        }

        int filling; //0=OFF 1=ON

        obj_type object_3ds;

        void Render(){

             glPushMatrix();
                glTranslatef(Pos.vec[0],Pos.vec[1],Pos.vec[2]);
                glColor3f(1,1,1);
                glScalef(iScale,iScale,iScale);
                //glScalef(0.1f,0.1f,0.1f);
                glRotatef(270, 1,0,0);
                glBindTexture(GL_TEXTURE_2D, object_3ds.id_texture); // We set the active texture

                glBegin(GL_TRIANGLES); // glBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
                for (int l_index=0;l_index<object_3ds.polygons_qty;l_index++)
                {
                    //----------------- FIRST VERTEX -----------------
                    // Texture coordinates of the first vertex
                    glTexCoord2f( object_3ds.mapcoord[ object_3ds.polygon[l_index].a ].u,
                                  object_3ds.mapcoord[ object_3ds.polygon[l_index].a ].v);
                    // Coordinates of the first vertex
                    glVertex3f( object_3ds.vertex[ object_3ds.polygon[l_index].a ].x,
                                object_3ds.vertex[ object_3ds.polygon[l_index].a ].y,
                                object_3ds.vertex[ object_3ds.polygon[l_index].a ].z); //Vertex definition

                    //----------------- SECOND VERTEX -----------------
                    // Texture coordinates of the second vertex
                    glTexCoord2f( object_3ds.mapcoord[ object_3ds.polygon[l_index].b ].u,
                                  object_3ds.mapcoord[ object_3ds.polygon[l_index].b ].v);
                    // Coordinates of the second vertex
                    glVertex3f( object_3ds.vertex[ object_3ds.polygon[l_index].b ].x,
                                object_3ds.vertex[ object_3ds.polygon[l_index].b ].y,
                                object_3ds.vertex[ object_3ds.polygon[l_index].b ].z);

                    //----------------- THIRD VERTEX -----------------
                    // Texture coordinates of the third vertex
                    glTexCoord2f( object_3ds.mapcoord[ object_3ds.polygon[l_index].c ].u,
                                  object_3ds.mapcoord[ object_3ds.polygon[l_index].c ].v);
                    // Coordinates of the Third vertex
                    glVertex3f( object_3ds.vertex[ object_3ds.polygon[l_index].c ].x,
                                object_3ds.vertex[ object_3ds.polygon[l_index].c ].y,
                                object_3ds.vertex[ object_3ds.polygon[l_index].c ].z);
                }
                glEnd();
            glPopMatrix();

        }




};

#endif
