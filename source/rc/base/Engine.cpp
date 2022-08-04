/**
 * @file Engine.cpp
 * @author Silmaen
 * @date 01/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#define INTERNAL
#include "Engine.h"
#include <GL/glut.h>

namespace rc::base {

/**
 * @brief Display call back
 */
static void display_cb(){
    Engine::get().display();
}
/**
 * @brief Input callback
 * @param key keyboard pressed
 * @param x First coordinate
 * @param y Second coordinate
 */
static void button_cb(uint8_t key, int32_t x, int32_t y){
    Engine::get().button(key, x, y);
}

void Engine::init(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width_, height);
    glutCreateWindow("RayCaster");
    glClearColor(0.3, 0.3, 0.3, 0);
    gluOrtho2D(0, width_, height, 0);
    glutDisplayFunc(display_cb);
    glutKeyboardFunc(button_cb);
}

void Engine::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(const auto& drawer: toRender)
        drawer();
    if (player != nullptr)
        player->draw();
    glutSwapBuffers();
}

void Engine::button(uint8_t key, int32_t x, int32_t y) {
    if (player==nullptr)
        return;
    if(key == 'q') {
        player->rotate(math::Angle{-5, math::Angle::Unit::Degree});
    }
    if(key == 'd') {
        player->rotate(math::Angle{5, math::Angle::Unit::Degree});
    }
    if(key == 'z') {
        player->walk(5);
    }
    if(key == 's') {
        player->walk(-5);
    }
    glutPostRedisplay();
}

void Engine::run() {
    running = true;
    glutMainLoop();
}

void Engine::registerRender(const std::function<void()>& func) {
    if (running)
        return;
    toRender.push_back(func);
}

void Engine::registerPlayer(const std::shared_ptr<Player>& player_) {
    if (running)
        return;
    player = player_;
}

void Engine::drawPoint(const math::Vector2<double>& location, double size, const graphics::Color& color) const {
    if (!running)
        return;
    setColor(color);
    glPointSize(size);
    glBegin(GL_POINTS);
    pushVertex(location);
    glEnd();
}
void Engine::drawLine(const graphics::Line2<double>& line, double width, const graphics::Color& color) const {
    if (!running)
        return;
    setColor(color);
    glLineWidth(width);
    glBegin(GL_LINES);
    pushVertex(line.getPoint(0));
    pushVertex(line.getPoint(1));
    glEnd();
}
void Engine::drawQuad(const graphics::Quad2<double>& quad, const graphics::Color& color) {
    if (!running)
        return;
    setColor(color);
    glBegin(GL_QUADS);
    pushVertex(quad.getPoint(0));
    pushVertex(quad.getPoint(1));
    pushVertex(quad.getPoint(2));
    pushVertex(quad.getPoint(3));
    glEnd();
}

void Engine::setColor(const graphics::Color& color) {
    glColor4ub(color.red(),color.green(),color.blue(),color.alpha());
}
void Engine::pushVertex(const math::Vector2<double>& vertex) {
    glVertex2d(vertex[0], vertex[1]);
}

}// namespace rc::base
