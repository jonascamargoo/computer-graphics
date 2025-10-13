#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// --- Variáveis Globais de Estado ---

// Controle da simulação
bool isPaused = false;
float planetAngle = 0.0f;
int orbitCount = 0;
float orbitSpeed = 0.01f;

float planetRotationAngle = 0.0f;
int rotationCount = 0;
float rotationSpeed = 0.05f;

// Controle do cenário
bool showStars = true;
bool isCircularOrbit = false;
const int NUM_STARS = 200;
std::vector<std::pair<float, float>> stars;
int starTimer = 0;

// Controle da visualização (Zoom)
float zoomFactor = 1.0f;

// Dimensões da órbita
const float ELLIPSE_RX = 150.0f;
const float ELLIPSE_RY = 80.0f;
const float CIRCLE_R = 120.0f;

// --- Funções Auxiliares de Desenho ---

void generateStars() {
    stars.clear();
    for (int i = 0; i < NUM_STARS; ++i) {
        float x = (rand() % 800 - 400);
        float y = (rand() % 800 - 400);
        stars.push_back({x, y});
    }
}

void drawCircle(float radius, int segments) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f); // Centro do círculo
    for (int i = 0; i <= segments; i++) {
        float angle = i * 2.0f * M_PI / segments;
        glVertex2f(cos(angle) * radius, sin(angle) * radius);
    }
    glEnd();
}

void drawText(float x, float y, const std::string& text) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

// --- Funções Principais da Simulação ---

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Configura a projeção para aplicar o zoom
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-250.0 * zoomFactor, 250.0 * zoomFactor, -250.0 * zoomFactor, 250.0 * zoomFactor);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Desenhar Estrelas
    if (showStars) {
        glColor3f(1.0f, 1.0f, 1.0f); // Estrelas brancas
        glPointSize(1.5f);
        glBegin(GL_POINTS);
        for (const auto& star : stars) {
            glVertex2f(star.first, star.second);
        }
        glEnd();
    }

    // Desenhar a Órbita
    glColor3f(1.0f, 1.0f, 1.0f); // Órbita branca
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; ++i) {
        float angle = i * 2.0f * M_PI / 100;
        float x, y;
        if (isCircularOrbit) {
            x = cos(angle) * CIRCLE_R;
            y = sin(angle) * CIRCLE_R;
        } else {
            x = cos(angle) * ELLIPSE_RX;
            y = sin(angle) * ELLIPSE_RY;
        }
        glVertex2f(x, y);
    }
    glEnd();
    
    // Desenhar o Sol
    glColor3f(1.0f, 1.0f, 0.0f);
    drawCircle(30.0f, 100);

    float planetX, planetY;
    if (isCircularOrbit) {
        planetX = cos(planetAngle) * CIRCLE_R;
        planetY = sin(planetAngle) * CIRCLE_R;
    } else {
        planetX = cos(planetAngle) * ELLIPSE_RX;
        planetY = sin(planetAngle) * ELLIPSE_RY;
    }

    glPushMatrix();
    glTranslatef(planetX, planetY, 0.0f); // Aplica translação ao planeta
    glRotatef(planetRotationAngle * 180.0 / M_PI, 0.0f, 0.0f, 1.0f); // Aplica rotação ao planeta

    // Desenha o Planeta
    glColor3f(0.0f, 0.0f, 1.0f); // Planeta azul
    drawCircle(10.0f, 50);

    // Desenha as Luas
    // Lua 1 (Polo Superior)
    glPushMatrix();
    glTranslatef(0.0f, 15.0f, 0.0f); // Posição relativa ao planeta
    glColor3f(0.8f, 0.8f, 0.8f); // Lua cinza
    drawCircle(3.0f, 30);
    glPopMatrix();

    // Lua 2 (Polo Inferior)
    glPushMatrix();
    glTranslatef(0.0f, -15.0f, 0.0f); // Posição relativa ao planeta
    glColor3f(0.8f, 0.8f, 0.8f); // Lua cinza
    drawCircle(3.0f, 30);
    glPopMatrix();

    glPopMatrix();

    // 5. Desenhar o Texto dos Contadores
    glColor3f(1.0f, 0.0f, 0.0f); // Texto vermelho
    std::string orbitText = "Translacao orbital: " + std::to_string(orbitCount);
    drawText(-240.0 * zoomFactor, -230.0 * zoomFactor, orbitText);

    std::string rotationText = "Rotacao: " + std::to_string(rotationCount);
    drawText(-240.0 * zoomFactor, -210.0 * zoomFactor, rotationText);

    // Exibe a mensagem de Pause
    if (isPaused) {
        glColor3f(1.0f, 1.0f, 1.0f);
        drawText(-30.0 * zoomFactor, 220.0 * zoomFactor, "*** Pause ***");
    }

    glutSwapBuffers(); // Troca os buffers para uma animação suave
}

void update(int value) {
    if (!isPaused) {
        // Atualiza o ângulo do planeta
        planetAngle += orbitSpeed;
        if (orbitSpeed > 0 && planetAngle > 2.0 * M_PI) {
            planetAngle -= 2.0 * M_PI;
            orbitCount++;
        } else if (orbitSpeed < 0 && planetAngle < -2.0 * M_PI) {
            planetAngle += 2.0 * M_PI;
            orbitCount--;
        }

        // Atualiza a rotação do planeta
        planetRotationAngle += rotationSpeed;
        if (rotationSpeed > 0 && planetRotationAngle > 2.0 * M_PI) {
            planetRotationAngle -= 2.0 * M_PI;
            rotationCount++;
        } else if (rotationSpeed < 0 && planetRotationAngle < -2.0 * M_PI) {
            planetRotationAngle += 2.0 * M_PI;
            rotationCount--;
        }

        // Atualiza as estrelas a cada 100 frames (aproximadamente)
        starTimer++;
        if (starTimer > 100) {
            generateStars();
            starTimer = 0;
        }
    }
    
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}


void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'p':
        case 'P':
            isPaused = !isPaused;
            break;
        case 'e':
        case 'E':
            showStars = !showStars;
            break;
        case 't':
        case 'T':
            isCircularOrbit = !isCircularOrbit;
            break;
        case 'i':
        case 'I':
            orbitSpeed = -orbitSpeed;
            rotationSpeed = -rotationSpeed;
            break;
        case 27:
            exit(0);
            break;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        if (button == GLUT_LEFT_BUTTON) {
            zoomFactor *= 0.9f;
        } else if (button == GLUT_RIGHT_BUTTON) {
            zoomFactor *= 1.1f;
        }
    }
    glutPostRedisplay();
}

void initialize() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Fundo preto
    generateStars(); // Gera a posição inicial das estrelas
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Exercicio Avaliativo 02 - Sistema Solar");

    initialize();

    // Registra as funções de callback
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}