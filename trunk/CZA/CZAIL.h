
#pragma once
#ifndef CZAIL_H
#define CZAIL_H


#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include "gl_image.c"

class ImgObject{
public:
       unsigned char * imgdata;
       int x;
       int y;
       int n;
       bool grayscale;
       bool invertimg;
       bool addalpha;
       bool gentexcoords;
	   bool buildMipMap;

};

class CZAIL{

public:

    static  unsigned char *GetTextureData(const char * filename,int &width, int &height,int &channels, bool invertimg = false, bool grayscale = false );
    static  ImgObject GetTextureDataBeta(const char * filename, bool invertimg = false, bool grayscale = false, bool gentexcoords = false, bool buildMipMap = false, bool addalpha = false );
    static GLuint ProcessTextureData(ImgObject imageobject);
    static GLuint BuildTexture(const char * filename, bool invertimg = false, bool grayscale = false, bool gentexcoords = false, bool buildMipMap = false, bool addalpha = false);
    static unsigned char * resample_image(	const unsigned char* const orig,int width, int height, int channels,int new_width, int new_height);
    static GLuint BuildColorTexture(unsigned char r, unsigned char g, unsigned char b);

private:
    static int up_scale_image(	const unsigned char* const orig,int width, int height, int channels,char* resampled,int resampled_width, int resampled_height);
    static int mipmap_image(const unsigned char* const orig,int width, int height, int channels,unsigned char* resampled,int block_size_x, int block_size_y);
    static void DisplayMsgBox(const char * str);



};

#endif

unsigned char * CZAIL::resample_image(	const unsigned char* const orig,int width, int height, int channels,int new_width, int new_height)
{

    unsigned char *resampled = (unsigned char*)malloc( channels*new_width*new_height );
    up_scale_image(orig, width, height, channels, (char*)resampled, new_width, new_height );

    //stbi_image_free( orig );
    //img = resampled;
    //x = new_width;
    //y = new_height;

    return resampled;

}

int CZAIL::up_scale_image(	const unsigned char* const orig,int width, int height, int channels,char* resampled,int resampled_width, int resampled_height)
{
	float dx, dy;
	int x, y, c;

    /* error(s) check	*/
    if ( 	(width < 1) || (height < 1) ||
            (resampled_width < 2) || (resampled_height < 2) ||
            (channels < 1) ||
            (NULL == orig) || (NULL == resampled) )
    {
        /*	signify badness	*/
        return 0;
    }
    /*
		for each given pixel in the new map, find the exact location
		from the original map which would contribute to this guy
	*/
    dx = (width - 1.0f) / (resampled_width - 1.0f);
    dy = (height - 1.0f) / (resampled_height - 1.0f);
    for ( y = 0; y < resampled_height; ++y )
    {
    	/* find the base y index and fractional offset from that	*/
    	float sampley = y * dy;
    	int inty = (int)sampley;
    	/*	if( inty < 0 ) { inty = 0; } else	*/
		if( inty > height - 2 ) { inty = height - 2; }
		sampley -= inty;
        for ( x = 0; x < resampled_width; ++x )
        {
			float samplex = x * dx;
			int intx = (int)samplex;
			int base_index;
			/* find the base x index and fractional offset from that	*/
			/*	if( intx < 0 ) { intx = 0; } else	*/
			if( intx > width - 2 ) { intx = width - 2; }
			samplex -= intx;
			/*	base index into the original image	*/
			base_index = (inty * width + intx) * channels;
            for ( c = 0; c < channels; ++c )
            {
            	/*	do the sampling	*/
				float value = 0.5f;
				value += orig[base_index]
							*(1.0f-samplex)*(1.0f-sampley);
				value += orig[base_index+channels]
							*(samplex)*(1.0f-sampley);
				value += orig[base_index+width*channels]
							*(1.0f-samplex)*(sampley);
				value += orig[base_index+width*channels+channels]
							*(samplex)*(sampley);
				/*	move to the next channel	*/
				++base_index;
            	/*	save the new value	*/
            	resampled[y*resampled_width*channels+x*channels+c] =
						(unsigned char)(value);
            }
        }
    }
    /*	done	*/
    return 1;
}

int CZAIL::mipmap_image(const unsigned char* const orig,int width, int height, int channels,unsigned char* resampled,int block_size_x, int block_size_y)
{
	int mip_width, mip_height;
	int i, j, c;

	/*	error check	*/
	if( (width < 1) || (height < 1) ||
		(channels < 1) || (orig == NULL) ||
		(resampled == NULL) ||
		(block_size_x < 1) || (block_size_y < 1) )
	{
		/*	nothing to do	*/
		return 0;
	}
	mip_width = width / block_size_x;
	mip_height = height / block_size_y;
	if( mip_width < 1 )
	{
		mip_width = 1;
	}
	if( mip_height < 1 )
	{
		mip_height = 1;
	}
	for( j = 0; j < mip_height; ++j )
	{
		for( i = 0; i < mip_width; ++i )
		{
			for( c = 0; c < channels; ++c )
			{
				const int index = (j*block_size_y)*width*channels + (i*block_size_x)*channels + c;
				int sum_value;
				int u,v;
				int u_block = block_size_x;
				int v_block = block_size_y;
				int block_area;
				/*	do a bit of checking so we don't over-run the boundaries
					(necessary for non-square textures!)	*/
				if( block_size_x * (i+1) > width )
				{
					u_block = width - i*block_size_y;
				}
				if( block_size_y * (j+1) > height )
				{
					v_block = height - j*block_size_y;
				}
				block_area = u_block*v_block;
				/*	for this pixel, see what the average
					of all the values in the block are.
					note: start the sum at the rounding value, not at 0	*/
				sum_value = block_area >> 1;
				for( v = 0; v < v_block; ++v )
				for( u = 0; u < u_block; ++u )
				{
					sum_value += orig[index + v*width*channels + u*channels];
				}
				resampled[j*mip_width*channels + i*channels + c] = sum_value / block_area;
			}
		}
	}
	return 1;
}

void CZAIL::DisplayMsgBox(const char * str){


           const char * str1 = str;

           MessageBox(NULL, str1, "Error", MB_OK);
}

unsigned char * CZAIL::GetTextureData(const char * filename,int &width, int &height, int &channels,  bool invertimg, bool grayscale ){
        char *ls = 0;

        ls = (char*)filename;//strcpy(ls, str.c_str());
        //ls = (char*)str.c_str();//strcpy(ls, str.c_str());

        char *temp = 0;

        temp = (char *)malloc((strlen(ls) + 1)
                              *sizeof(char));

        memset(temp,0,sizeof(temp));

        //strcat(temp, dir);
        strcat(temp, ls);


        int x,y,n;

        int comp = 0;

         if (grayscale == true)
           comp = 1;


     unsigned char *img = stbi_load(filename, &x, &y, &n, comp);

     width = x;
     height = y;
     channels = n;

     if (grayscale == true)
           n = 1;

     if (img == NULL)
        {

            char * holder;

            char * reason  = (char*)stbi_failure_reason();

            holder = (char *)malloc((strlen(reason) + 1)
                              *sizeof(char));

            memset(holder,0,sizeof(holder));

            strcat(holder, reason);

//write to log file

           DisplayMsgBox(holder);
           DisplayMsgBox(filename);
           //MessageBox(NULL, "error", "Error", MB_OK);


        }


        //invert image
        if (invertimg == true){
            int i, j;
    		for( j = 0; j*2 < y; ++j )
    		{
    			int index1 = j * x * n;
    			int index2 = (y - 1 - j) * x * n;
    			for( i = x * n; i > 0; --i )
    			{
    				unsigned char temp = img[index1];
    				img[index1] = img[index2];
    				img[index2] = temp;
    				++index1;
    				++index2;
    			}
    		}
        }
        return img;
}

 ImgObject CZAIL::GetTextureDataBeta(const char * filename, bool invertimg, bool grayscale, bool gentexcoords, bool buildMipMap , bool addalpha  ){
        char *ls = 0;

        ls = (char*)filename;//strcpy(ls, str.c_str());
        //ls = (char*)str.c_str();//strcpy(ls, str.c_str());

        char *temp = 0;

        temp = (char *)malloc((strlen(ls) + 1)
                              *sizeof(char));

        memset(temp,0,sizeof(temp));

        //strcat(temp, dir);
        strcat(temp, ls);


        int x,y,n;

        int comp = 0;

        if (grayscale == true)
           comp = 1;

     unsigned char *img = stbi_load(filename, &x, &y, &n, comp);

     if (grayscale == true)
           n = 1;

     if (img == NULL)
        {

            char * holder;

            char * reason  = (char*)stbi_failure_reason();

            holder = (char *)malloc((strlen(reason) + 1)
                              *sizeof(char));

            memset(holder,0,sizeof(holder));

            strcat(holder, reason);

//write to log file

           DisplayMsgBox(holder);
           DisplayMsgBox(filename);
           //MessageBox(NULL, "error", "Error", MB_OK);


        }


        //invert image
        if (invertimg == true){
            int i, j;
    		for( j = 0; j*2 < y; ++j )
    		{
    			int index1 = j * x * n;
    			int index2 = (y - 1 - j) * x * n;
    			for( i = x * n; i > 0; --i )
    			{
    				unsigned char temp = img[index1];
    				img[index1] = img[index2];
    				img[index2] = temp;
    				++index1;
    				++index2;
    			}
    		}
        }

        if (addalpha == true && n != 4){
        //TRANS STUFF
        int g_keyColor[3] = { 0, 0, 0 }; //Pure Black
        int oimgsize = x * y * 3;
        int newimgsize = x * y * 4;
        n = 4;

        unsigned char *pImage_RGBA = NULL;

        pImage_RGBA = new unsigned char[newimgsize];

        if (1==1){

        int i, j;

        for( i = 0, j = 0; i < oimgsize; i += 3, j += 4 )
        {
            // Does the current pixel match the selected color key?
            if( (img[i]   == g_keyColor[0] && img[i+1] == g_keyColor[1] && img[i+2] == g_keyColor[2]) || (img[i] == img[i+1] && img[i+1] == img[i+2]) )
            {
                pImage_RGBA[j+3] = 0;   // If so, set alpha to fully transparent.
            }
            else
            {
                pImage_RGBA[j+3] = 255; // If not, set alpha to fully opaque.
            }

            pImage_RGBA[j]   = img[i];
            pImage_RGBA[j+1] = img[i+1];
            pImage_RGBA[j+2] = img[i+2];
        }
        }//1==1
        //END TRANS STUFF
        img = pImage_RGBA;
        }


        //Make a Power of 2
        //int new_width = 1024;
		//int new_height = 1024;
		int new_width = 1;
		int new_height = 1;

	    while( new_width < x )
		{
			new_width *= 2;
		}
		while( new_height < y )
		{
			new_height *= 2;
		}


        if( (new_width != x) || (new_height != y) )
		{
			/*	yep, resize	*/
			unsigned char *resampled = (unsigned char*)malloc( n*new_width*new_height );
			up_scale_image(img, x, y, n, (char*)resampled, new_width, new_height );


			stbi_image_free( img );
			img = resampled;
			x = new_width;
			y = new_height;

		 }

        //if( (x > 128) || (y > 128) )
        if( (x > new_width) || (y > new_height) )
	    {
		    /*	I've already made it a power of two, so simply use the MIPmapping
		    	code to reduce its size to the allowable maximum.	*/
		    unsigned char *resampled;
		    int reduce_block_x = 1, reduce_block_y = 1;
		    int new_width, new_height;
		    if( x > 128 )
		    {
		    	reduce_block_x = x / 128;
		    }
		    if( y > 128 )
		    {
		    	reduce_block_y = y / 128;
		    }
		    new_width = x / reduce_block_x;
		    new_height = y / reduce_block_y;
		    resampled = (unsigned char*)malloc( n*new_width*new_height );
		    /*	perform the actual reduction	*/
		    mipmap_image(img, x, y, n, resampled, reduce_block_x, reduce_block_y );
		    /*	nuke the old guy, then point it at the new guy	*/
		    stbi_image_free( img );
		    img = resampled;
		    x = new_width;
		    y = new_height;
	    }

	    ImgObject myio;

	    myio.imgdata = img;
	    myio.x = x;
	    myio.y = y;
	    myio.n = n;
	    myio.grayscale = grayscale;
	    myio.invertimg = invertimg;
	    myio.addalpha = addalpha;
	    myio.gentexcoords = gentexcoords;
	    myio.buildMipMap = buildMipMap;

	    free(temp);
        return myio;
}


 GLuint CZAIL::ProcessTextureData(ImgObject imageobject){
 GLuint texture;
 glGenTextures(1, &texture);
 glBindTexture(GL_TEXTURE_2D, texture);

         if (imageobject.grayscale == false){

           if (imageobject.n == 4){

               if (imageobject.buildMipMap == true)
                     gluBuild2DMipmaps (GL_TEXTURE_2D, GL_RGBA, imageobject.x, imageobject.y, GL_RGBA, GL_UNSIGNED_BYTE,  imageobject.imgdata);
               else
                     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageobject.x, imageobject.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageobject.imgdata);
           }else{

               if (imageobject.buildMipMap == true)
                  gluBuild2DMipmaps (GL_TEXTURE_2D, GL_RGB, imageobject.x, imageobject.y, GL_RGB, GL_UNSIGNED_BYTE,  imageobject.imgdata);
               else
                  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageobject.x, imageobject.y, 0, GL_RGB, GL_UNSIGNED_BYTE, imageobject.imgdata);

           }

        }
        else{

           if (imageobject.buildMipMap == true)
               gluBuild2DMipmaps (GL_TEXTURE_2D, GL_LUMINANCE,imageobject.x, imageobject.y, GL_LUMINANCE, GL_UNSIGNED_BYTE,  imageobject.imgdata);
           else
               glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, imageobject.x, imageobject.y, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, imageobject.imgdata);
        }
        //GL_LUMINANCE

       // glTexParameteri( GL_RGB, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
       // glTexParameteri( GL_RGB, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

       glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
       glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
       glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
       glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

       if (imageobject.gentexcoords == true){
          GLfloat xequalzero[] = {1.0, 0.0, 0.0, 0.0};
          GLfloat slanted[] = {1.0, 1.0, 1.0, 0.0};
          GLfloat *currentCoeff;
          GLenum currentPlane;
          GLint currentGenMode;


          glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
          currentCoeff = xequalzero;
          currentGenMode = GL_OBJECT_LINEAR;
          currentPlane = GL_OBJECT_PLANE;
          glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, currentGenMode);
          glTexGenfv(GL_S, currentPlane, currentCoeff);

       }

        //free(temp);
        stbi_image_free( imageobject.imgdata );

        return texture;

}

 GLuint CZAIL::BuildTexture(const char * filename, bool invertimg , bool grayscale , bool gentexcoords , bool buildMipMap , bool addalpha )
{

    glColor3f(1,1,1);
                 GLuint texture;
          glGenTextures(1, &texture);

          //string str = filename;



        char *ls = 0;

        ls = (char*)filename;//strcpy(ls, str.c_str());
        //ls = (char*)str.c_str();//strcpy(ls, str.c_str());

        char *temp = 0;

        temp = (char *)malloc((strlen(ls) + 1)
                              *sizeof(char));

        memset(temp,0,sizeof(temp));

        //strcat(temp, dir);
        strcat(temp, ls);


        int x,y,n;

        int comp = 0;

        if (grayscale == true)
           comp = 1;

     unsigned char *img = stbi_load(filename, &x, &y, &n, comp);

     if (grayscale == true)
           n = 1;

     if (img == NULL)
        {

            char * holder;

            char * reason  = (char*)stbi_failure_reason();

            holder = (char *)malloc((strlen(reason) + 1)
                              *sizeof(char));

            memset(holder,0,sizeof(holder));

            strcat(holder, reason);

//write to log file

           DisplayMsgBox(holder);
           DisplayMsgBox(filename);
           //MessageBox(NULL, "error", "Error", MB_OK);


        }


        //invert image
        if (invertimg == true){
            int i, j;
    		for( j = 0; j*2 < y; ++j )
    		{
    			int index1 = j * x * n;
    			int index2 = (y - 1 - j) * x * n;
    			for( i = x * n; i > 0; --i )
    			{
    				unsigned char temp = img[index1];
    				img[index1] = img[index2];
    				img[index2] = temp;
    				++index1;
    				++index2;
    			}
    		}
        }

        if (addalpha == true && n != 4){
        //TRANS STUFF
        int g_keyColor[3] = { 0, 0, 0 }; //Pure Black
        int oimgsize = x * y * 3;
        int newimgsize = x * y * 4;
        n = 4;

        unsigned char *pImage_RGBA = NULL;

        pImage_RGBA = new unsigned char[newimgsize];

        if (1==1){

        int i, j;

        for( i = 0, j = 0; i < oimgsize; i += 3, j += 4 )
        {
            // Does the current pixel match the selected color key?
            if( (img[i]   == g_keyColor[0] && img[i+1] == g_keyColor[1] && img[i+2] == g_keyColor[2]) || (img[i] == img[i+1] && img[i+1] == img[i+2]) )
            {
                pImage_RGBA[j+3] = 0;   // If so, set alpha to fully transparent.
            }
            else
            {
                pImage_RGBA[j+3] = 255; // If not, set alpha to fully opaque.
            }

            pImage_RGBA[j]   = img[i];
            pImage_RGBA[j+1] = img[i+1];
            pImage_RGBA[j+2] = img[i+2];
        }
        }//1==1
        //END TRANS STUFF
        img = pImage_RGBA;
        }


        //Make a Power of 2
        //int new_width = 1024;
		//int new_height = 1024;
		int new_width = 1;
		int new_height = 1;

	    while( new_width < x )
		{
			new_width *= 2;
		}
		while( new_height < y )
		{
			new_height *= 2;
		}


        if( (new_width != x) || (new_height != y) )
		{
			/*	yep, resize	*/
			unsigned char *resampled = (unsigned char*)malloc( n*new_width*new_height );
			up_scale_image(img, x, y, n, (char*)resampled, new_width, new_height );


			stbi_image_free( img );
			img = resampled;
			x = new_width;
			y = new_height;

		 }

        //if( (x > 128) || (y > 128) )
        if( (x > new_width) || (y > new_height) )
	    {
		    /*	I've already made it a power of two, so simply use the MIPmapping
		    	code to reduce its size to the allowable maximum.	*/
		    unsigned char *resampled;
		    int reduce_block_x = 1, reduce_block_y = 1;
		    int new_width, new_height;
		    if( x > 128 )
		    {
		    	reduce_block_x = x / 128;
		    }
		    if( y > 128 )
		    {
		    	reduce_block_y = y / 128;
		    }
		    new_width = x / reduce_block_x;
		    new_height = y / reduce_block_y;
		    resampled = (unsigned char*)malloc( n*new_width*new_height );
		    /*	perform the actual reduction	*/
		    mipmap_image(img, x, y, n, resampled, reduce_block_x, reduce_block_y );
		    /*	nuke the old guy, then point it at the new guy	*/
		    stbi_image_free( img );
		    img = resampled;
		    x = new_width;
		    y = new_height;
	    }

        glBindTexture(GL_TEXTURE_2D, texture);

         if (grayscale == false){

           if (n == 4){

               if (buildMipMap == true)
                     gluBuild2DMipmaps (GL_TEXTURE_2D, GL_RGBA, x, y, GL_RGBA, GL_UNSIGNED_BYTE,  img);
               else
                     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
           }else{

               if (buildMipMap == true)
                  gluBuild2DMipmaps (GL_TEXTURE_2D, GL_RGB, x, y, GL_RGB, GL_UNSIGNED_BYTE,  img);
               else
                  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, img);

           }

        }
        else{

           if (buildMipMap == true)
               gluBuild2DMipmaps (GL_TEXTURE_2D, GL_LUMINANCE, x, y, GL_LUMINANCE, GL_UNSIGNED_BYTE,  img);
           else
               glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, x, y, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, img);
        }
        //GL_LUMINANCE

       // glTexParameteri( GL_RGB, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
       // glTexParameteri( GL_RGB, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

       glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
       glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
       glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
       glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

       if (gentexcoords == true){
          GLfloat xequalzero[] = {1.0, 0.0, 0.0, 0.0};
          GLfloat slanted[] = {1.0, 1.0, 1.0, 0.0};
          GLfloat *currentCoeff;
          GLenum currentPlane;
          GLint currentGenMode;


          glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
          currentCoeff = xequalzero;
          currentGenMode = GL_OBJECT_LINEAR;
          currentPlane = GL_OBJECT_PLANE;
          glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, currentGenMode);
          glTexGenfv(GL_S, currentPlane, currentCoeff);

       }

        free(temp);
        stbi_image_free( img );

        return texture;
}

GLuint CZAIL::BuildColorTexture(unsigned char r, unsigned char g, unsigned char b)
{
	unsigned char data[12];	// a 2x2 texture at 24 bits
	GLuint texture;

	// Store the data
	for(int i = 0; i < 12; i += 3)
	{
		data[i] = r;
		data[i+1] = g;
		data[i+2] = b;
	}

	// Generate the OpenGL texture id
	glGenTextures(1, &texture);

	// Bind this texture to its id
	glBindTexture(GL_TEXTURE_2D, texture);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Use mipmapping filter
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// Generate the texture
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 2, 2, GL_RGB, GL_UNSIGNED_BYTE, data);
	return texture;
}
//////////////////////////////////////////////////////////////////////////////
