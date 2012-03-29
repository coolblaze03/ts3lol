#include "noise.c"

class Water:public Object{
      private:
            GLuint texture;
            float points[45][45][3];    // The Array For The Points On The Grid Of Our "Wave"
            int wiggle_count;		// Counter Used To Control How Fast Flag Waves
            
            GLfloat	xrot;				// X Rotation ( NEW )
            GLfloat	yrot;				// Y Rotation ( NEW )
            GLfloat	zrot;				// Z Rotation ( NEW )
            GLfloat hold;				// Temporarily Holds A Floating Point Value
        
      public:
             
           Water(){
                   
               
                
                wiggle_count = 0;
                
            	for(int x=0; x<45; x++)
            	{
            		for(int y=0; y<45; y++)
            		{
            			points[x][y][0]=float((x/5.0f)-4.5f);
            			points[x][y][1]=float((y/5.0f)-4.5f);
            			points[x][y][2]=float(sin((((x/5.0f)*40.0f)/360.0f)*3.141592654*2.0f));
            		}
            	}
                   
           }
           
           void LoadTexture(char * filename){
                 texture = BuildTexture(filename, true);
           }
               
           void Render(){
                
                glPushMatrix();
                glColor3f(1,1,1);
                //glPolygonMode( GL_BACK, GL_FILL );					// Back Face Is Solid
            	//glPolygonMode( GL_FRONT, GL_LINE );					// Front Face Is Made Of Lines
                
                int x, y;
                float float_x, float_y, float_xb, float_yb;
                glBindTexture(GL_TEXTURE_2D, texture);

            	glBegin(GL_QUADS);
            	for( x = 0; x < 44; x++ )
            	{
            		for( y = 0; y < 44; y++ )
            		{
            			float_x = float(x)/44.0f;
            			float_y = float(y)/44.0f;
            			float_xb = float(x+1)/44.0f;
            			float_yb = float(y+1)/44.0f;
            
            			glTexCoord2f( float_x, float_y);
            			glVertex3f( points[x][y][0], points[x][y][1], points[x][y][2] );
            
            			glTexCoord2f( float_x, float_yb );
            			glVertex3f( points[x][y+1][0], points[x][y+1][1], points[x][y+1][2] );
            
            			glTexCoord2f( float_xb, float_yb );
            			glVertex3f( points[x+1][y+1][0], points[x+1][y+1][1], points[x+1][y+1][2] );
            
            			glTexCoord2f( float_xb, float_y );
            			glVertex3f( points[x+1][y][0], points[x+1][y][1], points[x+1][y][2] );
            		}
            	}
            	glEnd();
            
            	if( wiggle_count == 2 )
            	{
            		for( y = 0; y < 45; y++ )
            		{
            			hold=points[0][y][2];
            			for( x = 0; x < 44; x++)
            			{
            				points[x][y][2] = points[x+1][y][2];
            			}
            			points[44][y][2]=hold;
            		}
            		wiggle_count = 0;
            	}
            
            	wiggle_count++;
            
            	xrot+=0.3f;
            	yrot+=0.2f;
            	zrot+=0.4f;
                     
                glPopMatrix();
           
           }
               
};
