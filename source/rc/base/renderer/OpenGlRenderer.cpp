/**
 * @file OpenGlRenderer.cpp
 * @author Silmaen
 * @date 05/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "OpenGlRenderer.h"
#include <GL/glut.h>


namespace rc::base::renderer {
// hat trick

static OpenGLRenderer* globalPtr = nullptr;///< Global pointer to the actual renderer...
/**
 * @brief Display function used to callback in GLUT
 */
static void displayFunction() {
    globalPtr->display_cb();
}
/**
 * @brief Keyboard input function for glut callback
 * @param key The key pressed
 * @param x Location X
 * @param y Location Y
 */
static void buttonFunction(uint8_t key, int32_t x, int32_t y) {
    globalPtr->button_cb(key, x, y);
}

void OpenGLRenderer::Init() {
    int param    = 1;
    char toto1[] = "Raycaster";
    char* toto[] = {&toto1[0]};
    glutInit(&param, toto);
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
    glutKeyboardFunc(buttonFunction);
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
void OpenGLRenderer::setButtonCallback(const std::function<void(uint8_t key, int32_t x, int32_t y)>& func) {
    btn = func;
}

void OpenGLRenderer::setColor(const graphics::Color& color) {
    glColor4ub(color.red(), color.green(), color.blue(), color.alpha());
}
void OpenGLRenderer::pushVertex(const math::Vector2<double>& vertex) {
    glVertex2d(vertex[0], vertex[1]);
}

void OpenGLRenderer::drawPoint(const math::Vector2<double>& location, double size, const graphics::Color& color) const {
    if (status != Status::Running)
        return;
    setColor(color);
    glPointSize(static_cast<GLfloat>(size));
    glBegin(GL_POINTS);
    pushVertex(location);
    glEnd();
}
void OpenGLRenderer::drawLine(const graphics::Line2<double>& line, double width, const graphics::Color& color) const {
    if (status != Status::Running)
        return;
    setColor(color);
    glLineWidth(static_cast<GLfloat>(width));
    glBegin(GL_LINES);
    pushVertex(line.getPoint(0));
    pushVertex(line.getPoint(1));
    glEnd();
}
void OpenGLRenderer::drawQuad(const graphics::Quad2<double>& quad, const graphics::Color& color) const {
    if (status != Status::Running)
        return;
    setColor(color);
    glBegin(GL_QUADS);
    pushVertex(quad.getPoint(0));
    pushVertex(quad.getPoint(1));
    pushVertex(quad.getPoint(2));
    pushVertex(quad.getPoint(3));
    glEnd();
}

void OpenGLRenderer::display_cb() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (mainDraw)
        mainDraw();
    glutSwapBuffers();
}
void OpenGLRenderer::button_cb(uint8_t key, int32_t x, int32_t y) {
    if (btn)
        btn(key, x, y);
}

}// namespace rc::base::renderer