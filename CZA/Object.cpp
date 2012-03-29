class Object{
      public:
        Object();
        Object(Vector3 Position, int length, int width);
        void SetPosition(Vector3 Position);
        void SetSize(int length, int width);
        void GetPosition();
        void GetSize();
        
        void DrawSquare();
        void DrawCube();
        void DrawBall();
        void Draw_Grid();
        
      private:
        GLfloat len;
        GLfloat wid;
        Vector3 Pos;
        
};

Object::Object(){
       len = 2;
       wid = 2;
       Pos = Vector3(0,0,0);             
}

Object::Object(Vector3 Position, int length, int width){
       len = length;
       wid = width;
       Pos = Position;             
}

void Object::SetPosition(Vector3 Position){
      
       Pos = Position;
      
}

void Object::SetSize(int length, int width){
      
       len = length;
       wid = width;
      
}

void Object::DrawSquare(){
     
    GLfloat x,y,z;
    
    x = len / 2;
    y = wid / 2;
    
    glPushMatrix();
    glTranslatef(Pos.vec[0],Pos.vec[1],Pos.vec[2]);// Move Right
    glBegin(GL_QUADS);				// Draw A Quad
    	glVertex3f(-x, y, 0.0f);		// Top Left
    	glVertex3f( x, y, 0.0f);		// Top Right
    	glVertex3f( x,-y, 0.0f);		// Bottom Right
    	glVertex3f(-x,-y, 0.0f);		// Bottom Left
    glEnd();
    glPopMatrix();

}

void Object::Draw_Grid()
{															

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
}

void Object::DrawCube(){
     
    GLfloat x,y,z;
    
    x = len / 2;
    y = wid / 2;


    glPushMatrix();
    glColor3f(0.0f,1.0f,0.0f);	// Color Blue
    glTranslatef(Pos.vec[0],Pos.vec[1],Pos.vec[2]);// Move Right
    glBegin(GL_QUADS);		// Draw The Cube Using quads
        
        glVertex3f( x, y,-y);	// Top Right Of The Quad (Top)
        glVertex3f(-x, y,-y);	// Top Left Of The Quad (Top)
        glVertex3f(-x, y, y);	// Bottom Left Of The Quad (Top)
        glVertex3f( x, y, y);	// Bottom Right Of The Quad (Top)
        glVertex3f( x,-y, y);	// Top Right Of The Quad (Bottom)
        glVertex3f(-x,-y, y);	// Top Left Of The Quad (Bottom)
        glVertex3f(-x,-y,-y);	// Bottom Left Of The Quad (Bottom)
        glVertex3f( x,-y,-y);	// Bottom Right Of The Quad (Bottom)
        glVertex3f( x, y, y);	// Top Right Of The Quad (Front)
        glVertex3f(-x, y, y);	// Top Left Of The Quad (Front)
        glVertex3f(-x,-y, y);	// Bottom Left Of The Quad (Front)
        glVertex3f( x,-y, y);	// Bottom Right Of The Quad (Front)
        glVertex3f( x,-y,-y);	// Top Right Of The Quad (Back)
        glVertex3f(-x,-y,-y);	// Top Left Of The Quad (Back)
        glVertex3f(-x, y,-y);	// Bottom Left Of The Quad (Back)
        glVertex3f( x, y,-y);	// Bottom Right Of The Quad (Back)
        glVertex3f(-x, y, y);	// Top Right Of The Quad (Left)
        glVertex3f(-x, y,-y);	// Top Left Of The Quad (Left)
        glVertex3f(-x,-y,-y);	// Bottom Left Of The Quad (Left)
        glVertex3f(-x,-y, y);	// Bottom Right Of The Quad (Left)
        glVertex3f( x, y,-y);	// Top Right Of The Quad (Right)
        glVertex3f( x, y, y);	// Top Left Of The Quad (Right)
        glVertex3f( x,-y, y);	// Bottom Left Of The Quad (Right)
        glVertex3f( x,-y,-y);	// Bottom Right Of The Quad (Right)
        
  glEnd();			// End Drawing The Cube
  glPopMatrix();

     
}

void Object::DrawBall(){
     
    GLfloat x,y,z;
    
    x = len / 2;
    y = wid / 2;
    
    glPushMatrix();
    glTranslatef(Pos.vec[0],Pos.vec[1],Pos.vec[2]);// Move Right
    glutSolidSphere(x,16, 16);
    glPopMatrix();

}
