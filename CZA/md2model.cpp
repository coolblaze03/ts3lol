/* Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/* File for "Putting It All Together" lesson of the OpenGL tutorial on
 * www.videotutorialsrock.com
 */



#include <fstream>
#include "md2model.h"

using namespace std;

namespace {
	//Normals used in the MD2 file format
	float NORMALS[486] =
		{-0.525731f,  0.000000f,  0.850651f,
		 -0.442863f,  0.238856f,  0.864188f,
		 -0.295242f,  0.000000f,  0.955423f,
		 -0.309017f,  0.500000f,  0.809017f,
		 -0.162460f,  0.262866f,  0.951056f,
		  0.000000f,  0.000000f,  1.000000f,
		  0.000000f,  0.850651f,  0.525731f,
		 -0.147621f,  0.716567f,  0.681718f,
		  0.147621f,  0.716567f,  0.681718f,
		  0.000000f,  0.525731f,  0.850651f,
		  0.309017f,  0.500000f,  0.809017f,
		  0.525731f,  0.000000f,  0.850651f,
		  0.295242f,  0.000000f,  0.955423f,
		  0.442863f,  0.238856f,  0.864188f,
		  0.162460f,  0.262866f,  0.951056f,
		 -0.681718f,  0.147621f,  0.716567f,
		 -0.809017f,  0.309017f,  0.500000f,
		 -0.587785f,  0.425325f,  0.688191f,
		 -0.850651f,  0.525731f,  0.000000f,
		 -0.864188f,  0.442863f,  0.238856f,
		 -0.716567f,  0.681718f,  0.147621f,
		 -0.688191f,  0.587785f,  0.425325f,
		 -0.500000f,  0.809017f,  0.309017f,
		 -0.238856f,  0.864188f,  0.442863f,
		 -0.425325f,  0.688191f,  0.587785f,
		 -0.716567f,  0.681718f, -0.147621f,
		 -0.500000f,  0.809017f, -0.309017f,
		 -0.525731f,  0.850651f,  0.000000f,
		  0.000000f,  0.850651f, -0.525731f,
		 -0.238856f,  0.864188f, -0.442863f,
		  0.000000f,  0.955423f, -0.295242f,
		 -0.262866f,  0.951056f, -0.162460f,
		  0.000000f,  1.000000f,  0.000000f,
		  0.000000f,  0.955423f,  0.295242f,
		 -0.262866f,  0.951056f,  0.162460f,
		  0.238856f,  0.864188f,  0.442863f,
		  0.262866f,  0.951056f,  0.162460f,
		  0.500000f,  0.809017f,  0.309017f,
		  0.238856f,  0.864188f, -0.442863f,
		  0.262866f,  0.951056f, -0.162460f,
		  0.500000f,  0.809017f, -0.309017f,
		  0.850651f,  0.525731f,  0.000000f,
		  0.716567f,  0.681718f,  0.147621f,
		  0.716567f,  0.681718f, -0.147621f,
		  0.525731f,  0.850651f,  0.000000f,
		  0.425325f,  0.688191f,  0.587785f,
		  0.864188f,  0.442863f,  0.238856f,
		  0.688191f,  0.587785f,  0.425325f,
		  0.809017f,  0.309017f,  0.500000f,
		  0.681718f,  0.147621f,  0.716567f,
		  0.587785f,  0.425325f,  0.688191f,
		  0.955423f,  0.295242f,  0.000000f,
		  1.000000f,  0.000000f,  0.000000f,
		  0.951056f,  0.162460f,  0.262866f,
		  0.850651f, -0.525731f,  0.000000f,
		  0.955423f, -0.295242f,  0.000000f,
		  0.864188f, -0.442863f,  0.238856f,
		  0.951056f, -0.162460f,  0.262866f,
		  0.809017f, -0.309017f,  0.500000f,
		  0.681718f, -0.147621f,  0.716567f,
		  0.850651f,  0.000000f,  0.525731f,
		  0.864188f,  0.442863f, -0.238856f,
		  0.809017f,  0.309017f, -0.500000f,
		  0.951056f,  0.162460f, -0.262866f,
		  0.525731f,  0.000000f, -0.850651f,
		  0.681718f,  0.147621f, -0.716567f,
		  0.681718f, -0.147621f, -0.716567f,
		  0.850651f,  0.000000f, -0.525731f,
		  0.809017f, -0.309017f, -0.500000f,
		  0.864188f, -0.442863f, -0.238856f,
		  0.951056f, -0.162460f, -0.262866f,
		  0.147621f,  0.716567f, -0.681718f,
		  0.309017f,  0.500000f, -0.809017f,
		  0.425325f,  0.688191f, -0.587785f,
		  0.442863f,  0.238856f, -0.864188f,
		  0.587785f,  0.425325f, -0.688191f,
		  0.688191f,  0.587785f, -0.425325f,
		 -0.147621f,  0.716567f, -0.681718f,
		 -0.309017f,  0.500000f, -0.809017f,
		  0.000000f,  0.525731f, -0.850651f,
		 -0.525731f,  0.000000f, -0.850651f,
		 -0.442863f,  0.238856f, -0.864188f,
		 -0.295242f,  0.000000f, -0.955423f,
		 -0.162460f,  0.262866f, -0.951056f,
		  0.000000f,  0.000000f, -1.000000f,
		  0.295242f,  0.000000f, -0.955423f,
		  0.162460f,  0.262866f, -0.951056f,
		 -0.442863f, -0.238856f, -0.864188f,
		 -0.309017f, -0.500000f, -0.809017f,
		 -0.162460f, -0.262866f, -0.951056f,
		  0.000000f, -0.850651f, -0.525731f,
		 -0.147621f, -0.716567f, -0.681718f,
		  0.147621f, -0.716567f, -0.681718f,
		  0.000000f, -0.525731f, -0.850651f,
		  0.309017f, -0.500000f, -0.809017f,
		  0.442863f, -0.238856f, -0.864188f,
		  0.162460f, -0.262866f, -0.951056f,
		  0.238856f, -0.864188f, -0.442863f,
		  0.500000f, -0.809017f, -0.309017f,
		  0.425325f, -0.688191f, -0.587785f,
		  0.716567f, -0.681718f, -0.147621f,
		  0.688191f, -0.587785f, -0.425325f,
		  0.587785f, -0.425325f, -0.688191f,
		  0.000000f, -0.955423f, -0.295242f,
		  0.000000f, -1.000000f,  0.000000f,
		  0.262866f, -0.951056f, -0.162460f,
		  0.000000f, -0.850651f,  0.525731f,
		  0.000000f, -0.955423f,  0.295242f,
		  0.238856f, -0.864188f,  0.442863f,
		  0.262866f, -0.951056f,  0.162460f,
		  0.500000f, -0.809017f,  0.309017f,
		  0.716567f, -0.681718f,  0.147621f,
		  0.525731f, -0.850651f,  0.000000f,
		 -0.238856f, -0.864188f, -0.442863f,
		 -0.500000f, -0.809017f, -0.309017f,
		 -0.262866f, -0.951056f, -0.162460f,
		 -0.850651f, -0.525731f,  0.000000f,
		 -0.716567f, -0.681718f, -0.147621f,
		 -0.716567f, -0.681718f,  0.147621f,
		 -0.525731f, -0.850651f,  0.000000f,
		 -0.500000f, -0.809017f,  0.309017f,
		 -0.238856f, -0.864188f,  0.442863f,
		 -0.262866f, -0.951056f,  0.162460f,
		 -0.864188f, -0.442863f,  0.238856f,
		 -0.809017f, -0.309017f,  0.500000f,
		 -0.688191f, -0.587785f,  0.425325f,
		 -0.681718f, -0.147621f,  0.716567f,
		 -0.442863f, -0.238856f,  0.864188f,
		 -0.587785f, -0.425325f,  0.688191f,
		 -0.309017f, -0.500000f,  0.809017f,
		 -0.147621f, -0.716567f,  0.681718f,
		 -0.425325f, -0.688191f,  0.587785f,
		 -0.162460f, -0.262866f,  0.951056f,
		  0.442863f, -0.238856f,  0.864188f,
		  0.162460f, -0.262866f,  0.951056f,
		  0.309017f, -0.500000f,  0.809017f,
		  0.147621f, -0.716567f,  0.681718f,
		  0.000000f, -0.525731f,  0.850651f,
		  0.425325f, -0.688191f,  0.587785f,
		  0.587785f, -0.425325f,  0.688191f,
		  0.688191f, -0.587785f,  0.425325f,
		 -0.955423f,  0.295242f,  0.000000f,
		 -0.951056f,  0.162460f,  0.262866f,
		 -1.000000f,  0.000000f,  0.000000f,
		 -0.850651f,  0.000000f,  0.525731f,
		 -0.955423f, -0.295242f,  0.000000f,
		 -0.951056f, -0.162460f,  0.262866f,
		 -0.864188f,  0.442863f, -0.238856f,
		 -0.951056f,  0.162460f, -0.262866f,
		 -0.809017f,  0.309017f, -0.500000f,
		 -0.864188f, -0.442863f, -0.238856f,
		 -0.951056f, -0.162460f, -0.262866f,
		 -0.809017f, -0.309017f, -0.500000f,
		 -0.681718f,  0.147621f, -0.716567f,
		 -0.681718f, -0.147621f, -0.716567f,
		 -0.850651f,  0.000000f, -0.525731f,
		 -0.688191f,  0.587785f, -0.425325f,
		 -0.587785f,  0.425325f, -0.688191f,
		 -0.425325f,  0.688191f, -0.587785f,
		 -0.425325f, -0.688191f, -0.587785f,
		 -0.587785f, -0.425325f, -0.688191f,
		 -0.688191f, -0.587785f, -0.425325f};

	//Returns whether the system is little-endian
	bool littleEndian() {
		//The short value 1 has bytes (1, 0) in little-endian and (0, 1) in
		//big-endian
		short s = 1;
		return (((char*)&s)[0]) == 1;
	}

	float randomFloat() {
    	return (float)rand() / ((float)RAND_MAX + 1);
    }

	//Converts a four-character array to an integer, using little-endian form
	int toInt(const char* bytes) {
		return (int)(((unsigned char)bytes[3] << 24) |
					 ((unsigned char)bytes[2] << 16) |
					 ((unsigned char)bytes[1] << 8) |
					 (unsigned char)bytes[0]);
	}

	//Converts a two-character array to a short, using little-endian form
	short toShort(const char* bytes) {
		return (short)(((unsigned char)bytes[1] << 8) |
					   (unsigned char)bytes[0]);
	}

	//Converts a two-character array to an unsigned short, using little-endian
	//form
	unsigned short toUShort(const char* bytes) {
		return (unsigned short)(((unsigned char)bytes[1] << 8) |
								(unsigned char)bytes[0]);
	}

	//Converts a four-character array to a float, using little-endian form
	float toFloat(const char* bytes) {
		float f;
		if (littleEndian()) {
			((char*)&f)[0] = bytes[0];
			((char*)&f)[1] = bytes[1];
			((char*)&f)[2] = bytes[2];
			((char*)&f)[3] = bytes[3];
		}
		else {
			((char*)&f)[0] = bytes[3];
			((char*)&f)[1] = bytes[2];
			((char*)&f)[2] = bytes[1];
			((char*)&f)[3] = bytes[0];
		}
		return f;
	}

	//Reads the next four bytes as an integer, using little-endian form
	int readInt(ifstream &input) {
		char buffer[4];
		input.read(buffer, 4);
		return toInt(buffer);
	}

	//Reads the next two bytes as a short, using little-endian form
	short readShort(ifstream &input) {
		char buffer[2];
		input.read(buffer, 2);
		return toShort(buffer);
	}

	//Reads the next two bytes as an unsigned short, using little-endian form
	unsigned short readUShort(ifstream &input) {
		char buffer[2];
		input.read(buffer, 2);
		return toUShort(buffer);
	}

	//Reads the next four bytes as a float, using little-endian form
	float readFloat(ifstream &input) {
		char buffer[4];
		input.read(buffer, 4);
		return toFloat(buffer);
	}

	//Calls readFloat three times and returns the results as a Vector3 object
	Vector3 readVector3(ifstream &input) {
		float x = readFloat(input);
		float y = readFloat(input);
		float z = readFloat(input);
		return Vector3(x, y, z);
	}

}

MD2Model::~MD2Model() {
	if (frames != NULL) {
		for(int i = 0; i < numFrames; i++) {
			delete[] frames[i].vertices;
		}
		delete[] frames;
	}

	if (texCoords != NULL) {
		delete[] texCoords;
	}
	if (triangles != NULL) {
		delete[] triangles;
	}
}

MD2Model::MD2Model() {
    srand ( time(NULL) );
	frames = NULL;
	texCoords = NULL;
	triangles = NULL;

	animTime = 0;
	timeUntilNextStep = 0;

	radius0 = 0.4f * randomFloat() + 0.25f;
	speed = 1.5f * randomFloat() + 2.0f;
	MODEL_STEP_TIME = 0.01f;
    //animTime;
    //radius0;
    //speed;
    //timeUntilNextStep;

}

typedef struct _pcxHeader
        {
          short id[2];
          short offset[2];
          short size[2];
        } pcxHeader;

        GLuint LoadPCX(const char *filename, bool invertimg = false)
        {

          GLuint texID;
          FILE *hTexFile = fopen( filename, "rb" );
          unsigned char * imageData;
          int bpp, height, width;
          /* check the file open command */
          if( hTexFile != NULL )
          {

            int imgWidth, imgHeight, texFileLen, imgBufferPtr, i;
            pcxHeader *pcxPtr;
            unsigned char *imgBuffer, *texBuffer, *pcxBufferPtr, *paletteBuffer;

            /* find length of file */
            fseek( hTexFile, 0, SEEK_END );
            texFileLen = ftell( hTexFile );
            fseek( hTexFile, 0, SEEK_SET );

            /* read in file */
            texBuffer = (unsigned char *)malloc( texFileLen+1 );
            fread( texBuffer, sizeof( char ), texFileLen, hTexFile );

            /* get the image dimensions */
            pcxPtr = (pcxHeader *)texBuffer;
            imgWidth = pcxPtr->size[0] - pcxPtr->offset[0] + 1;
            imgHeight = pcxPtr->size[1] - pcxPtr->offset[1] + 1;
             /* image starts at 128 from the beginning of the buffer */
            imgBuffer = (unsigned char *)malloc( imgWidth * imgHeight );
            imgBufferPtr = 0;
            pcxBufferPtr = &texBuffer[128];

            /* decode the pcx image */
            while( imgBufferPtr < (imgWidth * imgHeight) )
            {
              if( *pcxBufferPtr > 0xbf )
              {
                int repeat = *pcxBufferPtr++ & 0x3f;
                for( i=0; i<repeat; i++ )
                {
                  imgBuffer[imgBufferPtr++] = *pcxBufferPtr;
                }
              } else {
                imgBuffer[imgBufferPtr++] = *pcxBufferPtr;
              }
              pcxBufferPtr++;
            }

            /* read in the image palette */
            paletteBuffer = (unsigned char *)malloc( 768 );
            for( i=0; i<768; i++ )
              paletteBuffer[i] = texBuffer[ texFileLen-768+i ];

            width=imgWidth;
            height=imgHeight;

            /* now create the OpenGL texture */
            {
              int i, j;
              imageData = (unsigned char *)malloc( width * height * 3 );
              for (j = 0; j < imgHeight; j++)
              {
                for (i = 0; i < imgWidth; i++)
                {
                  imageData[3*(j * width + i)+0]
                    = paletteBuffer[ 3*imgBuffer[j*imgWidth+i]+0 ];
                  imageData[3*(j * width + i)+1]
                    = paletteBuffer[ 3*imgBuffer[j*imgWidth+i]+1 ];
                  imageData[3*(j * width + i)+2]
                    = paletteBuffer[ 3*imgBuffer[j*imgWidth+i]+2 ];
                }
              }
            }

            int n = 3;
           if (invertimg == true){
               for(int j = 0; j*2 < imgHeight; ++j )
         		{
            			int index1 = j * imgWidth * n;
            			int index2 = (imgHeight - 1 - j) * imgWidth * n;
            			for(int i = imgWidth * n; i > 0; --i )
            			{
            				unsigned char temp = imageData[index1];
            				imageData[index1] = imageData[index2];
            				imageData[index2] = temp;
            				++index1;
            				++index2;
            			}
        		}
            }
            /* cleanup */
            free( paletteBuffer );
            free( imgBuffer );
            glGenTextures(1, &texID);
            glBindTexture(GL_TEXTURE_2D, texID);
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glTexImage2D(GL_TEXTURE_2D,0,3,width,height,0,GL_RGB,
            GL_UNSIGNED_BYTE,imageData);
            bpp=24;


          } else {
            /* skip the texture setup functions */
            return 0;
          }

          return texID;
        }

//Loads the MD2 model
MD2Model* MD2Model::load(const char* filename) {
	ifstream input;
	input.open(filename, istream::binary);

	char buffer[64];
	input.read(buffer, 4); //Should be "IPD2", if this is an MD2 file
	if (buffer[0] != 'I' || buffer[1] != 'D' ||
		buffer[2] != 'P' || buffer[3] != '2') {
		return NULL;
	}
	if (readInt(input) != 8) { //The version number
		return NULL;
	}

	int textureWidth = readInt(input);   //The width of the textures
	int textureHeight = readInt(input);  //The height of the textures
	readInt(input);                      //The number of bytes per frame
	int numTextures = readInt(input);    //The number of textures
	if (numTextures != 1) {
		return NULL;
	}
	int numVertices = readInt(input);    //The number of vertices
	int numTexCoords = readInt(input);   //The number of texture coordinates
	int numTriangles = readInt(input);   //The number of triangles
	readInt(input);                      //The number of OpenGL commands
	int numFrames = readInt(input);      //The number of frames

	//Offsets (number of bytes after the beginning of the file to the beginning
	//of where certain data appear)
	int textureOffset = readInt(input);  //The offset to the textures
	int texCoordOffset = readInt(input); //The offset to the texture coordinates
	int triangleOffset = readInt(input); //The offset to the triangles
	int frameOffset = readInt(input);    //The offset to the frames
	readInt(input);                      //The offset to the OpenGL commands
	readInt(input);                      //The offset to the end of the file

	//Load the texture
	input.seekg(textureOffset, ios_base::beg);
	input.read(buffer, 64);
	if (strlen(buffer) < 5 || (strcmp(buffer + strlen(buffer) - 4, ".bmp") != 0 && strcmp(buffer + strlen(buffer) - 4, ".pcx") != 0 && strcmp(buffer + strlen(buffer) - 4, ".png") != 0 && strcmp(buffer + strlen(buffer) - 4, ".tga") != 0)) {

             return NULL;

	}
	 GLuint textureId;

	 //need to take the path string from filename and append it to buffer.
	 string path = filename;
	 size_t found;

	 found = path.rfind("\\");
	 if (found!=string::npos){
        //add to buffer
        path = path.substr(0, found);
        path += "\\";
        string buff = buffer;
        path += buff;

     }

     if (FileExists(path.c_str()) == true){
     }else{
           found = path.rfind(".");
           if (found!=string::npos){
                string ext = path.substr(found+ 1);
                string tp = path.substr(0, found);
                if (ext == "pcx"){
                        tp+= ".bmp";
                        if (FileExists(tp.c_str()) == true){
                            path = tp;
                            string buff = buffer;
                            found = buff.rfind(".");
                            char bmp[3] = {'b','m','p'};
                            int place = 0;
                             if (found!=string::npos){
                                for (int ctr = found + 1; ctr < strlen(buffer); ctr++){
                                    buffer[ctr] = bmp[place];
                                    place++;
                                }
                             }
                        }else{
                              tp = path.substr(0, found);
                              tp+= ".tga";
                              path = tp;
                                string buff = buffer;
                                found = buff.rfind(".");
                                char bmp[3] = {'t','g','a'};
                                int place = 0;
                                 if (found!=string::npos){
                                    for (int ctr = found + 1; ctr < strlen(buffer); ctr++){
                                        buffer[ctr] = bmp[place];
                                        place++;
                                    }
                                 }

                        }
                }

           }

     }


	//Image* image = loadBMP(buffer);
	if (strcmp(buffer + strlen(buffer) - 4, ".pcx") != 0){
	    textureId = CZAIL::BuildTexture(path.c_str(), true);//loadTexture(image);
    }else{
        textureId = LoadPCX(path.c_str(), true);//loadTexture(image);
    }
	//delete image;
	MD2Model* model = new MD2Model();
	model->textureId = textureId;

	//Load the texture coordinates
	input.seekg(texCoordOffset, ios_base::beg);
	model->texCoords = new MD2TexCoord[numTexCoords];
	for(int i = 0; i < numTexCoords; i++) {
		MD2TexCoord* texCoord = model->texCoords + i;
		texCoord->texCoordX = (float)readShort(input) / textureWidth;
		texCoord->texCoordY = 1 - (float)readShort(input) / textureHeight;
	}

	//Load the triangles
	input.seekg(triangleOffset, ios_base::beg);
	model->triangles = new MD2Triangle[numTriangles];
	model->numTriangles = numTriangles;
	for(int i = 0; i < numTriangles; i++) {
		MD2Triangle* triangle = model->triangles + i;
		for(int j = 0; j < 3; j++) {
			triangle->vertices[j] = readUShort(input);
		}
		for(int j = 0; j < 3; j++) {
			triangle->texCoords[j] = readUShort(input);
		}
	}

	//Load the frames
	input.seekg(frameOffset, ios_base::beg);
	model->frames = new MD2Frame[numFrames];
	model->numFrames = numFrames;
	for(int i = 0; i < numFrames; i++) {
		MD2Frame* frame = model->frames + i;
		frame->vertices = new MD2Vertex[numVertices];
		Vector3 scale = readVector3(input);
		Vector3 translation = readVector3(input);
		input.read(frame->name, 16);

		for(int j = 0; j < numVertices; j++) {
			MD2Vertex* vertex = frame->vertices + j;
			input.read(buffer, 3);
			Vector3 v((unsigned char)buffer[0],
					(unsigned char)buffer[1],
					(unsigned char)buffer[2]);
			vertex->pos = translation + Vector3(scale[0] * v[0],
											  scale[1] * v[1],
											  scale[2] * v[2]);
			input.read(buffer, 1);
			int normalIndex = (int)((unsigned char)buffer[0]);
			vertex->normal = Vector3(NORMALS[3 * normalIndex],
								   NORMALS[3 * normalIndex + 1],
								   NORMALS[3 * normalIndex + 2]);
		}
	}

	model->startFrame = 0;
	model->endFrame = numFrames - 1;
	return model;
}

void MD2Model::setAnimation(const char* name) {
	/* The names of frames normally begin with the name of the animation in
	 * which they are, e.g. "run", and are followed by a non-alphabetical
	 * character.  Normally, they indicate their frame number in the animation,
	 * e.g. "run_1", "run_2", etc.
	 */
	bool found = false;
	for(int i = 0; i < numFrames; i++) {
		MD2Frame* frame = frames + i;
		if (strlen(frame->name) > strlen(name) &&
			strncmp(frame->name, name, strlen(name)) == 0 &&
			!isalpha(frame->name[strlen(name)])) {
			if (!found) {
				found = true;
				startFrame = i;
			}
			else {
				endFrame = i;
			}
		}
		else if (found) {
			break;
		}
	}
}

void MD2Model::draw() {
     draw(animTime);
}

void MD2Model::draw(float time) {

    glPushMatrix();
    glColor3f(1,1,1);
    	if (time > -100000000 && time < 1000000000) {
    		time -= (int)time;
    		if (time < 0) {
    			time += 1;
    		}
    	}
    	else {
    		time = 0;
    	}

    	if (UseMotionPath)
            SetMotionPosition();

    	glTranslatef(Pos.vec[0],Pos.vec[1],Pos.vec[2]);// Move Right
        glScalef(iScale, iScale, iScale);
        glRotatef(_rot, _rotx, _roty, _rotz);

    	glEnable(GL_TEXTURE_2D);
    	glBindTexture(GL_TEXTURE_2D, textureId);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    	//Figure out the two frames between which we are interpolating
    	int frameIndex1 = (int)(time * (endFrame - startFrame + 1)) + startFrame;
    	if (frameIndex1 > endFrame) {
    		frameIndex1 = startFrame;
    	}

    	int frameIndex2;
    	if (frameIndex1 < endFrame) {
    		frameIndex2 = frameIndex1 + 1;
    	}
    	else {
    		frameIndex2 = startFrame;
    	}

    	MD2Frame* frame1 = frames + frameIndex1;
    	MD2Frame* frame2 = frames + frameIndex2;

    	//Figure out the fraction that we are between the two frames
    	float frac =
    		(time - (float)(frameIndex1 - startFrame) /
    		 (float)(endFrame - startFrame + 1)) * (endFrame - startFrame + 1);

    	//Draw the model as an interpolation between the two frames
    	glBegin(GL_TRIANGLES);
    	for(int i = 0; i < numTriangles; i++) {
    		MD2Triangle* triangle = triangles + i;
    		for(int j = 0; j < 3; j++) {
    			MD2Vertex* v1 = frame1->vertices + triangle->vertices[j];
    			MD2Vertex* v2 = frame2->vertices + triangle->vertices[j];
    			Vector3 pos = v1->pos * (1 - frac) + v2->pos * frac;
    			Vector3 normal = v1->normal * (1 - frac) + v2->normal * frac;
    			if (normal[0] == 0 && normal[1] == 0 && normal[2] == 0) {
    				normal = Vector3(0, 0, 1);
    			}
    			glNormal3f(normal[0], normal[1], normal[2]);

    			MD2TexCoord* texCoord = texCoords + triangle->texCoords[j];
    			glTexCoord2f(texCoord->texCoordX, texCoord->texCoordY);
    			glVertex3f(pos[0], pos[1], pos[2]);
    		}
    	}
    	glEnd();
	glPopMatrix();
}











