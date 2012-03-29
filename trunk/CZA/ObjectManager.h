#ifndef OBJECTMANAGER_H_INCLUDED
#define OBJECTMANAGER_H_INCLUDED

#include <vector>
#include <map>
#include "Object.h"

#define EPSILON 1.0e-8

#define ZERO EPSILON

using namespace std;

class ObjectManager{

    public:


        void AddToManager(Object &Obj){
             int ID = ObjManager.size();
             if (Obj.ManagerID == 9999999){
                   Obj.ManagerID = ID;
                   ObjManager.push_back(&Obj);
             }else{
                   int OID = Obj.ManagerID;
                   ObjManager[OID] = &Obj;
             }

        }

         bool Inside_BoundingBox(int box, Vector3 point)
        {
               Vector3 lmax = ObjManager.at(box)->GetMax();
               Vector3 lmin = ObjManager.at(box)->GetMin();


        	if( lmax.vec[0] < point.vec[0] )
        		return false;
        	if( lmin.vec[0] > point.vec[0] )
        		return false;
        	if( lmax.vec[2] < point.vec[2] )
        		return false;
        	if( lmin.vec[2] > point.vec[2] )
        		return false;

        	return true;
        }

         int Is_Collision(Vector3 point)
        {
            int maxbox = ObjManager.size();

        	for (int i = 0; i < maxbox; i++)
        	{
        		if (Inside_BoundingBox(i,point) == 1) return i;
        	}

        	return -1;
        }

          //Adjusts the angle at which this guy is walking in response to a
		//collision with the specified guy
		void bounceOff(Object* otherGuy, Object* obj1) {
			float vx = obj1->velocityX();
			float vz = obj1->velocityZ();

			float dx = otherGuy->GetCurrentx() - obj1->GetCurrentx();
			float dz = otherGuy->GetCurrentz() - obj1->GetCurrentz();
			float m = sqrt(dx * dx + dz * dz);
			dx /= m;
			dz /= m;

			float dotProduct = vx * dx + vz * dz;
			vx -= 2 * dotProduct * dx;
			vz -= 2 * dotProduct * dz;

			if (vx != 0 || vz != 0) {
				obj1->angle = atan2(vz, vx);
			}
		}

		bool testCollision(Object* obj1, Object* obj2) {

             float obj1x, obj1z, obj2x, obj2z;

            /* if (obj1->isCenterDefined){
                obj1x = obj1->GetCurrentx() + (obj1->otwidth()/2);
                obj1z = obj1->GetCurrentz() + (obj1->otlength()/2);
             }
             else{*/
                obj1x = obj1->GetCurrentx();
                obj1z = obj1->GetCurrentz();
             /*}*/

             /*if (obj2->isCenterDefined){
                obj2x = obj2->GetCurrentx() + (obj2->otwidth()/2);
                obj2z = obj2->GetCurrentz() + (obj2->otlength()/2);
             }
             else{*/
                obj2x = obj2->GetCurrentx();
                obj2z = obj2->GetCurrentz();
             /*}*/



            //Get Previous Position Points for both objects.

            float xp1,lx1, lz1, zp1,xp2, lx2, lz2,zp2;
            lx1 = sin(obj1->dangle/100);
            lz1 = -cos(obj1->dangle/100);
            lx2 = sin(obj2->dangle)/100;
            lz2 = -cos(obj2->dangle/100);

            xp1 = obj1x - (-1)*lx1*1;
            zp1 = obj1z - (-1)*lz1*1;
            xp2 = obj2x - (-1)*lx2*1;
            zp2 = obj2z - (-1)*lz2*1;
            //x = x - direction*(lx)*1;
        	//z = z - direction*(lz)*1;

        	Vector3 x1(xp1, 0, zp1);
        	Vector3 x2(xp2, 0, zp2);
        	Vector3 objp1(obj1x, 0, obj1z);
        	Vector3 objp2(obj2x, 0, obj2z);

        	Vector3 output = findIntersection(objp1, x1, objp2, x2);

        	if (output == Vector3(-999,-999,-999))
                int i =0;
            else
                return true;


             /////////////End New Position Points////////////


             float dx = obj1x - obj2x;
             float dz = obj1z - obj2z;

             float r = obj1->radius()*2.0f + obj2->radius()*2.0f;
             if ((dx * dx) + (dz * dz) < r * r) {
                float vx = obj1->velocityX() - obj2->velocityX();
                float vz = obj1->velocityZ() - obj2->velocityZ();
                return vx * dx + vz * dz < 0;
             }
             else {
                  return false;
             }
             /*float dx = obj1->GetCurrentx() - obj2->GetCurrentx();


             float dz = obj1->GetCurrentz() - obj2->GetCurrentz();
             float r = obj1->radius() + obj2->radius();
             if (dx * dx + dz * dz < r * r) {
                float vx = obj1->velocityX() - obj2->velocityX();
                float vz = obj1->velocityZ() - obj2->velocityZ();
                return vx * dx + vz * dz < 0;
             }
             else {
                  return false;
             }*/
          }

            //Plane works. I can use it with walls.
          bool testCollisionPlaneIntersection(Object* obj1, Object* obj2) {

             float obj1x, obj1z, obj2x, obj2z;

             if (obj1->isCenterDefined){
                obj1x = obj1->GetCurrentx() + (obj1->otwidth()/2);
                obj1z = obj1->GetCurrentz() + (obj1->otlength()/2);
             }
             else{
                obj1x = obj1->GetCurrentx();
                obj1z = obj1->GetCurrentz();
             }

             if (obj2->isCenterDefined){
                obj2x = obj2->GetCurrentx() + (obj2->otwidth()/2);
                obj2z = obj2->GetCurrentz() + (obj2->otlength()/2);
             }
             else{
                obj2x = obj2->GetCurrentx();
                obj2z = obj2->GetCurrentz();
             }

             float dx = obj1x - obj2x;
             float dz = obj1z - obj2z;

             float o1lx, o1ly, o1lz;
             float o2lx, o2ly, o2lz;

             o1lx = sin(obj1->angle);
             o1lz = -cos(obj1->angle);
             o1ly = sin(obj1->angle);

             Vector3 o1DirectionVec(o1lx, o1ly, o1lz);

             o2lx = sin(obj1->angle);
             o2lz = -cos(obj1->angle);
             o2ly = sin(obj1->angle);

             Vector3 o2DirectionVec(o2lx, o2ly, o2lz);


             double dotproduct = o1DirectionVec.dot(obj2->Normal);

             if ((dotproduct<ZERO)&&(dotproduct>-ZERO))
                return false;

             double l2 = obj2->Normal.dot(obj2->GetPosition() - obj1->GetPosition());

             if (l2<-ZERO)
                return 0;

             return true;

          }

          void handleCollisions(int &numCollisions) {

               int managersize = ObjManager.size();
               bool visited[managersize][managersize];

               for (int i=0;i<managersize;i++)
                    for(int j=0;j<managersize;j++)
                        visited[i][j]=false;


               for(unsigned int i = 0; i < ObjManager.size(); i++) {

                   Object* g1 = ObjManager[i];

                   if (!g1->isActive)
                        continue;

                   for (unsigned int j = 0; j < ObjManager.size(); j++) {
                     if (i != j){

                        if (visited[i][j] || visited[j][i]){
                            continue;
                        }

                        visited[i][j] = true;
                        visited[j][i] = true;


                       Object* g2 = ObjManager[j];

                       if (!g2->isActive)
                          continue;

                       if (testCollision(g1, g2)) {

                           g1->AfterCollisionDetected(g2);
                           g2->AfterCollisionDetected(g1);

                           if (g1->ismoveable == true)
                              bounceOff(g2,g1);
                           if (g2->ismoveable == true)
                              bounceOff(g1,g2);


                           numCollisions++;
                       }
                     }
                   }

               }
        }

        void ClearOut(){ObjManager.clear();}

        private:
                vector<Object*> ObjManager;

                 // calculates intersection and checks for parallel lines.
         // also checks that the intersection point is actually on
         // the line segment p1-p2
         Vector3 findIntersection(Vector3 p1,Vector3 p2, Vector3 p3,Vector3 p4) {
               float xD1,yD1,xD2,yD2,xD3,yD3;
               float dot,deg,len1,len2;
               float segmentLen1,segmentLen2;
               float ua,ub,div;

               // calculate differences
               xD1=p2.x-p1.x;
               xD2=p4.x-p3.x;
               yD1=p2.z-p1.z;
               yD2=p4.z-p3.z;
               xD3=p1.x-p3.x;
               yD3=p1.z-p3.z;

               // calculate the lengths of the two lines
               len1=sqrt(xD1*xD1+yD1*yD1);
               len2=sqrt(xD2*xD2+yD2*yD2);

               // calculate angle between the two lines.
               dot=(xD1*xD2+yD1*yD2); // dot product
               deg=dot/(len1*len2);

               // if abs(angle)==1 then the lines are parallell,
               // so no intersection is possible
               if(abs(deg)==1) return Vector3(-999,-999,-999);

               // find intersection Pt between two lines
               Vector3 pt(0,0,0);
               div=yD2*xD1-xD2*yD1;
               ua=(xD2*yD3-yD2*xD3)/div;
               ub=(xD1*yD3-yD1*xD3)/div;
               pt.x=p1.x+ua*xD1;
               pt.z=p1.z+ua*yD1;

               // calculate the combined length of the two segments
               // between Pt-p1 and Pt-p2
               xD1=pt.x-p1.x;
               xD2=pt.x-p2.x;
               yD1=pt.z-p1.z;
               yD2=pt.z-p2.z;
               segmentLen1=sqrt(xD1*xD1+yD1*yD1)+sqrt(xD2*xD2+yD2*yD2);

               // calculate the combined length of the two segments
               // between Pt-p3 and Pt-p4
               xD1=pt.x-p3.x;
               xD2=pt.x-p4.x;
               yD1=pt.z-p3.z;
               yD2=pt.z-p4.z;
               segmentLen2=sqrt(xD1*xD1+yD1*yD1)+sqrt(xD2*xD2+yD2*yD2);

               // if the lengths of both sets of segments are the same as
               // the lenghts of the two lines the point is actually
               // on the line segment.

               // if the point isn’t on the line, return null
               if(abs(len1-segmentLen1)>0.01 || abs(len2-segmentLen2)>0.01)
                 return Vector3(-999,-999,-999);

               // return the valid intersection
               return pt;
         }


};

#endif
