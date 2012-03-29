#ifndef CAMERA_H
#define CAMERA_H

class Camera{

      public:
                          
        float angle,ratio;
        float x,y,z;
        float lx,ly,lz;
        
        Camera();
        
        void orientMe(float ang);
        void moveMeFlat(int direction);
        void moveVert(int direction);
      
};

#endif
