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



#ifndef MD2_MODEL_H_INCLUDED
#define MD2_MODEL_H_INCLUDED



#include <GL/glut.h>
#include <time.h>
#include "Vector3.h"
#include "CZAIL.h"
#include "Object.h"

bool FileExists( const char* FileName )
{
    FILE* fp = NULL;

    //will not work if you do not have read permissions

    //to the file, but if you don't have read, it

    //may as well not exist to begin with.

    fp = fopen( FileName, "rb" );
    if( fp != NULL )
    {
        fclose( fp );
        return true;
    }

    return false;
}

struct MD2Vertex {
	Vector3 pos;
	Vector3 normal;
};

struct MD2Frame {
	char name[16];
	MD2Vertex* vertices;
};

struct MD2TexCoord {
	float texCoordX;
	float texCoordY;
};

struct MD2Triangle {
	int vertices[3];  //The indices of the vertices in this triangle
	int texCoords[3]; //The indices of the texture coordinates of the triangle
};

class MD2Model: public Object {
	private:
		MD2Frame* frames;
		int numFrames;
		MD2TexCoord* texCoords;
		MD2Triangle* triangles;
		int numTriangles;
		GLuint textureId;

		int startFrame; //The first frame of the current animation
		int endFrame;   //The last frame of the current animation

		MD2Model();

    		float animTime;
    		float radius0;
    		float speed;
    		float timeUntilNextStep;
    		float MODEL_STEP_TIME;

	public:
		~MD2Model();

		//Switches to the given animation
		void setAnimation(const char* name);
		/* Draws the state of the animated model at the specified time in the
		 * animation.  A time of i, integer i, indicates the beginning of the
		 * animation, and a time of i + 0.5 indicates halfway through the
		 * animation.
		 */
		void draw(float time);
		void draw();

		//Loads an MD2Model from the specified file.  Returns NULL if there was
		//an error loading it.
		static MD2Model* load(const char* filename);

		void advance(float dt) {
			//Adjust animTime
			animTime += 0.45f * dt * speed / radius0;
			if (animTime > -100000000 && animTime < 1000000000) {
				animTime -= (int)animTime;
				if (animTime < 0) {
					animTime += 1;
				}
			}
			else {
				animTime = 0;
			}

			//Call step() the appropriate number of times
			while (dt > 0) {
				if (timeUntilNextStep < dt) {
					dt -= timeUntilNextStep;

					timeUntilNextStep = MODEL_STEP_TIME;
				}
				else {
					timeUntilNextStep -= dt;
					dt = 0;
				}
			}
		}
};


#endif
