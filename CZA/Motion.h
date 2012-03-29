#ifndef MOTIONPATH_H_INCLUDED
#define MOTIONPATH_H_INCLUDED

class MotionPath
{
      private:
              // position
              float p;
              // vertices
              Vector3* vertices;
              int n;
      public:
             // constructor
             MotionPath(int numPoints);
             // destructor
             ~MotionPath() { delete vertices; };
             // set point
             void set(int u, float x, float y, float z);
             // get point
             void get(float u, Vector3* pos, Vector3* dest);
             void gettwo(float u, Vector3* pos, Vector3* dest);
             void getvert(float u, Vector3* pos);
             void get(float u, Vector3* pos);
             // draw as a line
             void draw();

             // current position
             // move
             void move(float d);
             // get now
             void getNow(Vector3* pos, Vector3* dest);
             // reset, return to beginning
             void reset();

             int pointscount(){
                 return n;
             }
             Vector3* temp;
             double xco;
             double yco;
             double zco;
             int pathpos;
};

MotionPath::MotionPath(int numPoints):
    vertices(new Vector3[numPoints]),
    n(numPoints),
    p(0.0)
{
}

// set a point on the path
void MotionPath::set(int u, float x, float y, float z)
{
    vertices[u].set(x, y, z);
}

// get point
void MotionPath::get(float u, Vector3* pos, Vector3* dest)
{
    // find the two points its between
    int v0 = (int)floor(u * n) % n;
    int v1 = (v0 + 1) % n;

    // return the point interpolated on that line
    float d = u*n - v0;
    Vector3 dir(vertices[v1]);
    dir = dir - vertices[v0];
    dir = dir * d ;
    dir = dir + vertices[v0];

    // return
    pos->set(dir.x, dir.y, dir.z);
    //dest->set(&vertices[v1]);
    if (dest != NULL) get(u+(0.8*(1.0/n)), dest);
}


void MotionPath::getvert(float u, Vector3* pos)
{

    Vector3 intpos(vertices[(int)u]);
    pos->set(intpos.x, intpos.y, intpos.z);

}

void MotionPath::gettwo(float u, Vector3* pos, Vector3* dest)
{

    getvert(u, pos);

    if (u+1 >= n)
        getvert(0,dest);
    else
        getvert(u+1,dest);

}


void MotionPath::get(float u, Vector3* pos)
{
    get(u, pos, NULL);
}

// reset
void MotionPath::reset()
{
    p = 0.0;
}

// move
void MotionPath::move(float d)
{
   p += d;
   if (p > 1.0) p -= 1.0;
}

// get now
void MotionPath::getNow(Vector3* pos, Vector3* dest)
{
     get(p, pos, dest);
}

// draw for testing purposes
void MotionPath::draw()
{

    glBegin(GL_LINE_STRIP);
    for (float u = 0; u < 1.0; u += 0.01) {
        Vector3 p, d;
        get(u, &p, &d);
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();
}

#endif
