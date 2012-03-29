#pragma once
#ifndef TEXTUREDSQUARE_H_INCLUDED
#define TEXTUREDSQUARE_H_INCLUDED

#include "CZAIL.h"
#include "misc.h"


class TexturedSquare: Object{


public:

  void LoadTexture(const char * filename, bool inverttex = false, bool grayscale = false);

  void Render(int WINDOWWIDTH, int WINDOWHEIGHT);



private:

GLint tex;



};

#endif


  void TexturedSquare::LoadTexture(const char * filename, bool inverttex, bool grayscale){

      tex = CZAIL::BuildTexture(filename, inverttex, grayscale);
  }

  void TexturedSquare::Render(int WINDOWWIDTH, int WINDOWHEIGHT){

    glPushMatrix();
      Misc::enterortho(WINDOWWIDTH, WINDOWHEIGHT);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        glBindTexture(GL_TEXTURE_2D, (GLuint)tex);

        glBegin(GL_POLYGON);
            glTexCoord2f(0.0, 1.0);glVertex3f(300,220,-1);
            glTexCoord2f(1.0, 1.0);glVertex3f(340,220,-1);
            glTexCoord2f(1.0, 0.0);glVertex3f(340,260,-1);
            glTexCoord2f(0.0, 0.0);glVertex3f(300,260,-1);
        glEnd();

        glDisable(GL_BLEND);

      Misc::exitortho();
    glPopMatrix();

  }
