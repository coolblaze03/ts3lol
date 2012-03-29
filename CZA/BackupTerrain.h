class Terrain: public Object{
        #define MAP_X	32				         // size of map along x-axis
        #define MAP_Z	32				         // size of map along z-axis
        #define MAP_SCALE	20.0f		         // the scale of the terrain map
        
      public:
             
             Terrain(){
                Setup();
             }
             
             void Render(){
                  glPushMatrix();
                  glEnable(GL_TEXTURE_2D);
						      // counter clock-wise polygons are out
                       glTranslatef(Pos.vec[0],Pos.vec[1],Pos.vec[2]);// Move Right
                       glScalef(iScale, iScale, iScale);
                       glBindTexture(GL_TEXTURE_2D, groundtex);

                    	// we are going to loop through all of our terrain's data points,
                    	// but we only want to draw one triangle strip for each set along the x-axis.
                    	for (int z = 0; z < MAP_Z-1; z++)
                    	{
                    		glBegin(GL_TRIANGLE_STRIP);
                    		for (int x = 0; x < MAP_X-1; x++)
                    		{
                    			// for each vertex, we calculate the grayscale shade color, 
                    			// we set the texture coordinate, and we draw the vertex.
                    			/*
                    			   the vertices are drawn in this order:
                    
                    			   0  ---> 1
                    			          /
                    				    /
                    			     |/
                    			   2  ---> 3
                    			*/
                    		
                    			// draw vertex 0
                    			glColor3f(terrain[x][z][1]/255.0f, terrain[x][z][1]/255.0f, terrain[x][z][1]/255.0f);
                    			glTexCoord2f(0.0f, 0.0f);
                    			glVertex3f(terrain[x][z][0], terrain[x][z][1], terrain[x][z][2]);
                    
                    			// draw vertex 1
                    			glTexCoord2f(1.0f, 0.0f);
                    			glColor3f(terrain[x+1][z][1]/255.0f, terrain[x+1][z][1]/255.0f, terrain[x+1][z][1]/255.0f);
                    			glVertex3f(terrain[x+1][z][0], terrain[x+1][z][1], terrain[x+1][z][2]);
                    
                    			// draw vertex 2
                    			glTexCoord2f(0.0f, 1.0f);
                    			glColor3f(terrain[x][z+1][1]/255.0f, terrain[x][z+1][1]/255.0f, terrain[x][z+1][1]/255.0f);
                    			glVertex3f(terrain[x][z+1][0], terrain[x][z+1][1], terrain[x][z+1][2]);
                    
                    			// draw vertex 3
                    			glColor3f(terrain[x+1][z+1][1]/255.0f, terrain[x+1][z+1][1]/255.0f, terrain[x+1][z+1][1]/255.0f);
                    			glTexCoord2f(1.0f, 1.0f);
                    			glVertex3f(terrain[x+1][z+1][0], terrain[x+1][z+1][1], terrain[x+1][z+1][2]);
                    		}
                    		glEnd();
                    	}
                    	// enable blending
                    	glEnable(GL_BLEND);
                    
                    	// enable read-only depth buffer
                    	glDepthMask(GL_FALSE);
                    
                    	// set the blend function to what we use for transparency
                    	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                    
                    	// set back to normal depth buffer mode (writable)
                    	glDepthMask(GL_TRUE);
                    
                    	// disable blending
                    	glDisable(GL_BLEND);
                    	 glBindTexture(GL_TEXTURE_2D, (GLuint)NULL);
                  glPopMatrix();
             }
             
             void LoadTextures(char * heightmap, char * ground_texture){
                  
                  glEnable(GL_TEXTURE_2D);
                  
                  heightData = GetTextureData(heightmap, true);
                  
                  unsigned char * gdata;
                  
                  gdata = GetTextureData(ground_texture, true);
                  
                  glGenTextures(1, &groundtex);                  
	              glBindTexture(GL_TEXTURE_2D, groundtex);       
            	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            	  //make 128 dynamic by returing the actual size from gettexturedata
            	  gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, 128, 128, GL_RGB, GL_UNSIGNED_BYTE, gdata);
                  
                  InitializeTerrain();
             }
      
      private:
              GLuint groundtex;
              unsigned char* heightData;
              float terrain[MAP_X][MAP_Z][3];		// heightfield terrain data (0-255); 256x256
              void InitializeTerrain()
                {
                	// loop through all of the heightfield points, calculating
                	// the coordinates for each point
                	for (int z = 0; z < MAP_Z; z++)
                	{
                		for (int x = 0; x < MAP_X; x++)
                		{
                			terrain[x][z][0] = float(x)*MAP_SCALE;				
                			terrain[x][z][1] = (float)heightData[(z*MAP_Z+x)*3];
                			terrain[x][z][2] = -float(z)*MAP_SCALE;
                		}
                	}
                }
              
};
