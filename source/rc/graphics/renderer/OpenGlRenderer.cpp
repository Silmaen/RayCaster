/**
 * @file OpenGlRenderer.cpp
 * @author Silmaen
 * @date 05/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "OpenGlRenderer.h"
#include <GL/freeglut.h>


namespace rc::graphics::renderer {


namespace gl {
static bool initialized = false;///< If glut already initialized
void init() {
    if (initialized) return;
    int param    = 1;
    char toto1[] = "Raycaster";
    char* toto[] = {&toto1[0]};
    glutInit(&param, toto);
    initialized = true;
}
}// namespace gl


// hat trick
static OpenGLRenderer* globalPtr = nullptr;///< Global pointer to the actual renderer...
/**
 * @brief Display function used to callback in GLUT
 */
static void displayFunction() {
    globalPtr->display_cb();
}

OpenGLRenderer::~OpenGLRenderer() {
    globalPtr = nullptr;
}

void OpenGLRenderer::Init() {
    gl::init();
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(settingInternal.ScreenResolution[0], settingInternal.ScreenResolution[1]);
    glutCreateWindow("RayCaster");
    glClearColor(static_cast<GLclampf>(settingInternal.Background.redf()),
                 static_cast<GLclampf>(settingInternal.Background.greenf()),
                 static_cast<GLclampf>(settingInternal.Background.bluef()),
                 static_cast<GLclampf>(settingInternal.Background.alphaf()));
    gluOrtho2D(0, settingInternal.ScreenResolution[0], settingInternal.ScreenResolution[1], 0);
    globalPtr = this;
    glutDisplayFunc(displayFunction);
    status = Status::Ready;
}

void OpenGLRenderer::run() {
    status = Status::Running;
    glutMainLoop();
}

void OpenGLRenderer::update() {
    glutPostRedisplay();
}

void OpenGLRenderer::setDrawingCallback(const std::function<void()>& func) {
    mainDraw = func;
}

void OpenGLRenderer::setColor(const graphics::Color& color) {
    glColor4ub(color.red(), color.green(), color.blue(), color.alpha());
}
void OpenGLRenderer::pushVertex(const math::geometry::Vectf& vertex) {
    glVertex2d(vertex[0], vertex[1]);
}

void OpenGLRenderer::drawPoint(const math::geometry::Vectf& location, double size, const graphics::Color& color) const {
    if (status != Status::Running)
        return;
    setColor(color);
    glPointSize(static_cast<GLfloat>(size));
    glBegin(GL_POINTS);
    pushVertex(location);
    glEnd();
}

void OpenGLRenderer::drawLine(const math::geometry::Line2<double>& line, double width, const graphics::Color& color) const {
    if (status != Status::Running)
        return;
    setColor(color);
    glLineWidth(static_cast<GLfloat>(width));
    glBegin(GL_LINES);
    pushVertex(line.getPoint(0));
    pushVertex(line.getPoint(1));
    glEnd();
}

void OpenGLRenderer::drawTextureVerticalLine(double lineX, double lineY, double lineLength, const image::Texture& tex, double texX, const math::geometry::Box2& drawBox, bool shade) const {
    if (status != Status::Running)
        return;
    // Coordinate are input in the layout's frame: conversion into Scree coordinates
    lineX += drawBox.left();
    lineY += drawBox.top();
    // check vertical is in the layout
    if (lineX<drawBox.left() ||lineX> drawBox.right())
        return;
    double textureIncrement = tex.height() / lineLength;
    auto cols = tex.getPixelColumn(static_cast<uint16_t>(texX));
    uint16_t beginCoord = static_cast<uint16_t>(std::max((int16_t)lineY, (int16_t)drawBox.top()));
    uint16_t endCoord = lineY + lineLength > drawBox.bottom() ? drawBox.bottom() : lineY + lineLength;
    uint16_t length = endCoord - beginCoord;
    uint16_t beginTex = (uint16_t)std::max((int16_t)0,(int16_t)(-lineY*textureIncrement));
    cols+= beginTex;
    glPointSize(static_cast<GLfloat>(1));
    glBegin(GL_POINTS);



    for (uint16_t pixel = 0; pixel < length; ++pixel) {
        //if (!drawBox.isIn({static_cast<int32_t>(lineX), static_cast<int32_t>(lineY + pixel)}))
        //    continue;
        if (shade)
            setColor((*(cols + pixel * textureIncrement)).darker());
        else
            setColor(*(cols + pixel * textureIncrement));
        glVertex2d(lineX, beginCoord + pixel);
    }
    glEnd();
}

void OpenGLRenderer::drawQuad(const math::geometry::Quad2<double>& quad, const graphics::Color& color) const {
    if (status != Status::Running)
        return;
    setColor(color);
    glBegin(GL_QUADS);
    pushVertex(quad[0]);
    pushVertex(quad[1]);
    pushVertex(quad[2]);
    pushVertex(quad[3]);
    glEnd();
}

void OpenGLRenderer::drawText(const std::string& text, const math::geometry::Vectf& location, const graphics::Color& color) const {
    setColor(color);
    glRasterPos2d(location[0], location[1]);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, reinterpret_cast<const unsigned char*>(text.c_str()));
}

void OpenGLRenderer::display_cb() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (mainDraw)
        mainDraw();
    glutSwapBuffers();
}


}// namespace rc::graphics::renderer
