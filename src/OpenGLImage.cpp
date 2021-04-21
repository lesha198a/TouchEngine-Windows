#include "stdafx.h"
#include "OpenGLImage.h"

OpenGLImage::OpenGLImage()
{
}


OpenGLImage::~OpenGLImage()
{
    if (myVAO)
    {
        glDeleteVertexArrays(1, &myVAO);
    }
    if (myVBO)
    {
        glDeleteBuffers(1, &myVBO);
    }
}

bool OpenGLImage::setup(GLint vertexAttribLocation, GLint textureAttribLocation)
{
    glGenVertexArrays(1, &myVAO);
    glGenBuffers(1, &myVBO);

    glBindVertexArray(myVAO);
    glBindBuffer(GL_ARRAY_BUFFER, myVBO);

    glEnableVertexAttribArray(vertexAttribLocation);
    glVertexAttribPointer(vertexAttribLocation, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);

    glEnableVertexAttribArray(textureAttribLocation);
    glVertexAttribPointer(textureAttribLocation, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid *)(2 * sizeof(GLfloat)));

    return true;
}

void OpenGLImage::position(float newx, float newy)
{
    if (newx != x || newy != y)
    {
        x = newx;
        y = newy;
        myDirty = true;
    }
}

void OpenGLImage::scale(float scaleX, float scaleY)
{
    if (scaleX != myScaleX || scaleY != myScaleY)
    {
        myScaleX = scaleX;
        myScaleY = scaleY;
        myDirty = true;
    }
}

void OpenGLImage::draw()
{
    if (myDirty)
    {
        glBindBuffer(GL_ARRAY_BUFFER, myVBO);

        float ratio = width == 0.0 ? 1.0 : height / width;

        GLfloat vertices[] = {
            (-1.0f * myScaleX) + x,   (-1.0f * myScaleY * ratio) + y,   0.0f,    myTexture.getFlipped() ? 1.0f : 0.0f,
            (-1.0f * myScaleX) + x,    (1.0f * myScaleY * ratio) + y,   0.0f,    myTexture.getFlipped() ? 0.0f : 1.0f,
            (1.0f * myScaleX) + x,   (-1.0f * myScaleY * ratio) + y,   1.0f,    myTexture.getFlipped() ? 1.0f : 0.0f,
            (1.0f * myScaleX) + x,    (1.0f * myScaleY * ratio) + y,   1.0f,    myTexture.getFlipped() ? 0.0f : 1.0f
        };

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        myDirty = false;
    }

    if (myTexture.isValid())
    {
        glBindTexture(GL_TEXTURE_2D, myTexture.getName());
        glBindVertexArray(myVAO);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void OpenGLImage::update(const OpenGLTexture & texture)
{
    if (myTexture.getWidth() != texture.getWidth() || myTexture.getHeight() != texture.getHeight() || myTexture.getFlipped() != texture.getFlipped())
    {
        width = texture.getWidth();
        height = texture.getHeight();
        myDirty = true;
    }
    myTexture = texture;
}
