#ifndef FONTCLASS_H_INCLUDED
#define FONTCLASS_H_INCLUDED
class FontClass{

      public:

        FontClass(){
               WINDOWWIDTH = 640;
               WINDOWHEIGHT = 480;

        }
        ~FontClass(){
               killFont();
        }

        void initFont(){
             fontTextureID = CZAIL::BuildTexture("font.bmp", true);
             buildFont();
        }


        void buildFont()										// Build Our Font Display List
        {
        	float	cx;											// Holds Our X Character Coord
        	float	cy;											// Holds Our Y Character Coord
        	int loop;

        	fontBaseIndex=glGenLists(256);								// Creating 256 Display Lists
        	glBindTexture(GL_TEXTURE_2D, fontTextureID);			// Select Our Font Texture
        	for (loop=0; loop<256; loop++)						// Loop Through All 256 Lists
        	{
        		cx=float(loop%16)/16.0f;						// X Position Of Current Character
        		cy=float(loop/16)/16.0f;						// Y Position Of Current Character

        		glNewList(fontBaseIndex+loop,GL_COMPILE);				// Start Building A List
        			glBegin(GL_QUADS);							// Use A Quad For Each Character
        				glTexCoord2f(cx,1-cy-0.0625f);			// Texture Coord (Bottom Left)
        				glVertex2i(0,0);						// Vertex Coord (Bottom Left)
        				glTexCoord2f(cx+0.0625f,1-cy-0.0625f);	// Texture Coord (Bottom Right)
        				glVertex2i(16,0);						// Vertex Coord (Bottom Right)
        				glTexCoord2f(cx+0.0625f,1-cy);			// Texture Coord (Top Right)
        				glVertex2i(16,16);						// Vertex Coord (Top Right)
        				glTexCoord2f(cx,1-cy);					// Texture Coord (Top Left)
        				glVertex2i(0,16);						// Vertex Coord (Top Left)
        			glEnd();									// Done Building Our Quad (Character)
        			glTranslated(10,0,0);						// Move To The Right Of The Character
        		glEndList();									// Done Building The Display List
        	}													// Loop Until All 256 Are Built
        }

        void killFont()									// Delete The Font From Memory
        {
        	glDeleteLists(fontBaseIndex,256);							// Delete All 256 Display Lists
        }


        /*void glPrint(GLint x, GLint y, char *string, int set, float r, float g, float b, float a )	// Where The Printing Happens
        {
            glPushMatrix();
            	float env_color[4];


            	env_color[0] = r; env_color[1] = g; env_color[2] = b; env_color[3] = 0.1;

            	if (set>1)
            	{
            		set=1;
            	}

            	glBindTexture(GL_TEXTURE_2D, fontTextureID);		// Select Our Font Texture

            	glColor4f( 1.0, 0.0, 0.0, 1.0 );

			//	enable2D();f
            	glTranslated(0,0,0);								// Position The Text (0,0 - Bottom Left)
            	glListBase(fontBaseIndex-32+(128*set));				// Choose The Font Set (0 or 1)
            	glCallLists(strlen(string),GL_BYTE,string);			// Write The Text To The Screen
               // disable2D();

           	glPopMatrix();

        }*/

         GLvoid glPrint(GLint x, GLint y, char *string, int set, float r, float g, float b, float a )	// Where The Printing Happens
        {

            glPushMatrix();

            glEnable(GL_BLEND);
           	glDepthFunc(GL_LEQUAL);
	        glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	        glShadeModel(GL_SMOOTH);

            glColor4f( r, g, b, a );
            	if (set>1)
            	{
            		set=1;
            	}
            	glBindTexture(GL_TEXTURE_2D, fontTextureID);			// Select Our Font Texture
            	glDisable(GL_DEPTH_TEST);							// Disables Depth Testing
            	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
            	glPushMatrix();										// Store The Projection Matrix
                	glLoadIdentity();									// Reset The Projection Matrix
                	glOrtho(0,640,0,480,-1,1);							// Set Up An Ortho Screen
                	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
                	glPushMatrix();										// Store The Modelview Matrix
                    	glLoadIdentity();									// Reset The Modelview Matrix
                    	glTranslated(x,y,0);								// Position The Text (0,0 - Bottom Left)
                    	glListBase(fontBaseIndex-32+(128*set));						// Choose The Font Set (0 or 1)
                    	glCallLists(strlen(string),GL_BYTE,string);			// Write The Text To The Screen
                    	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
                	glPopMatrix();										// Restore The Old Projection Matrix
                	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
            	glPopMatrix();										// Restore The Old Projection Matrix
            	glEnable(GL_DEPTH_TEST);
                glDisable(GL_BLEND);  							// Enables Depth Testing
        	glPopMatrix();
        }

        GLvoid glPrint(GLint x, GLint y, char *string, int set)	// Where The Printing Happens
        {

            glPushMatrix();
            	if (set>1)
            	{
            		set=1;
            	}
            	glBindTexture(GL_TEXTURE_2D, fontTextureID);			// Select Our Font Texture
            	glDisable(GL_DEPTH_TEST);							// Disables Depth Testing
            	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
            	glPushMatrix();										// Store The Projection Matrix
                	glLoadIdentity();									// Reset The Projection Matrix
                	glOrtho(0,640,0,480,-1,1);							// Set Up An Ortho Screen
                	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
                	glPushMatrix();										// Store The Modelview Matrix
                    	glLoadIdentity();									// Reset The Modelview Matrix
                    	glTranslated(x,y,0);								// Position The Text (0,0 - Bottom Left)
                    	glListBase(fontBaseIndex-32+(128*set));						// Choose The Font Set (0 or 1)
                    	glCallLists(strlen(string),GL_BYTE,string);			// Write The Text To The Screen
                    	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
                	glPopMatrix();										// Restore The Old Projection Matrix
                	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
            	glPopMatrix();										// Restore The Old Projection Matrix
            	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
        	glPopMatrix();
        }

        private:
                GLuint fontTextureID;
                GLuint fontBaseIndex;
                int WINDOWWIDTH ;
                int WINDOWHEIGHT;
                void enable2D() {

                   // Switch to projection mode
                   glMatrixMode(GL_PROJECTION);

                   // Save previous
                   glPushMatrix();
                   glLoadIdentity();
                   gluOrtho2D(0, WINDOWWIDTH, 0, WINDOWHEIGHT);

                   // Invert the y axis, down is positive
                   glScalef(1, -1, 1);

                   // Move the origin to the upper left corner
                   glTranslatef(0, -WINDOWHEIGHT, 0);
                   glMatrixMode(GL_MODELVIEW);

                   glPushMatrix();
                   glLoadIdentity();
                   glDisable(GL_DEPTH_TEST);

                }

                void disable2D() {

                   glEnable(GL_DEPTH_TEST);
                   glPopMatrix();

                   glMatrixMode(GL_PROJECTION);
                   glPopMatrix();
                   glMatrixMode(GL_MODELVIEW);

                }



};

#endif
