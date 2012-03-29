#pragma once
#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include "CZAIL.h"
#include "misc.h"
#include "Object.h"
#include "FPSCamera.cpp"

class Bullet:public Object{

private:
float xangle, yangle, lx,ly, lz;

public:

    Terrain* terrain;




    Bullet(Terrain* terrain){
        this->terrain = terrain;
        Setup();
        isActive = false;
        isCenterDefined = true;
        twidth = terrain->width_starting();
        tlength = terrain->length_starting();
    }

    void SetPosition(FPSCamera cam){
        Object::SetPosition(cam.GetPosition());

        xangle = cam.xAngle();
        yangle = cam.yAngle();

        angle = xangle - (1.57f);
        dangle = angle*100.0f;
        speed = 1;

        lx = cos(xangle - (1.57f));
        ly = sin(yangle);
        lz = sin(xangle - (1.57f));
        iScale = 0.1f;
        radius0 = 1.0f;

        Update(1.5f);

    }

    void Update(float step = 1){

        float newx = GetCurrentx() - (step * lx);
        float newy = GetCurrenty() - (step * ly);
        float newz = GetCurrentz() - (step * lz);

        Vector3 newPosition(newx, newy, newz);

        Object::SetPosition(newPosition);


        if (Pos.x > 0 && Pos.x < (terrain->width_starting())  && Pos.z > 0  && Pos.z < (terrain->length_starting()) && Pos.y > (terrain->heightAt(Pos.x,Pos.z)) && Pos.y < (200)){
            //Do Nothing

        }else{
            isActive = false;
        }


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

    void AfterCollisionDetected(Object* collisionObject){
        isActive = false;
    }

};

#endif
