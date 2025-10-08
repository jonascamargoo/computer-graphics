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
const float ORBIT_SPEED = 0.01f;

// Controle do cenário
bool showStars = true;
bool isCircularOrbit = false;
const int NUM_STARS = 200;
std::vector<std::pair<float, float>> stars;
int starTimer = 0; // Temporizador para atualizar as estrelas

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

    // 1. Desenhar Estrelas
    if (showStars) {
        glColor3f(1.0f, 1.0f, 1.0f); // Estrelas brancas
        glPointSize(1.5f);
        glBegin(GL_POINTS);
        for (const auto& star : stars) {
            glVertex2f(star.first, star.second);
        }
        glEnd();
    }

    // 2. Desenhar a Órbita
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
    
    // 3. Desenhar o Sol
    glColor3f(1.0f, 1.0f, 0.0f); // Sol amarelo
    drawCircle(30.0f, 100);

    // 4. Calcular e Desenhar o Planeta
    float planetX, planetY;
    if (isCircularOrbit) {
        planetX = cos(planetAngle) * CIRCLE_R;
        planetY = sin(planetAngle) * CIRCLE_R;
    } else {
        planetX = cos(planetAngle) * ELLIPSE_RX;
        planetY = sin(planetAngle) * ELLIPSE_RY;
    }

    glPushMatrix(); // Salva a matriz de transformação atual [cite: 1969]
    glTranslatef(planetX, planetY, 0.0f); // Aplica translação ao planeta
    glColor3f(0.0f, 0.0f, 1.0f); // Planeta azul
    drawCircle(10.0f, 50);
    glPopMatrix(); // Restaura a matriz de transformação [cite: 1970]

    // 5. Desenhar o Texto do Contador
    glColor3f(1.0f, 0.0f, 0.0f); // Texto vermelho
    std::string text = "Translacao orbital: " + std::to_string(orbitCount);
    // Posiciona o texto no canto inferior esquerdo da área de visualização
    drawText(-240.0 * zoomFactor, -230.0 * zoomFactor, text);

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
        planetAngle += ORBIT_SPEED;
        if (planetAngle > 2.0 * M_PI) {
            planetAngle -= 2.0 * M_PI;
            orbitCount++;
        }

        // Atualiza as estrelas a cada 100 frames (aproximadamente)
        starTimer++;
        if (starTimer > 100) {
            generateStars();
            starTimer = 0;
        }
    }
    
    glutPostRedisplay(); // Solicita o redesenho da tela
    glutTimerFunc(16, update, 0); // Agenda a próxima atualização (aprox. 60 FPS)
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
        case 27: // Tecla ESC para sair
            exit(0);
            break;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        if (button == GLUT_LEFT_BUTTON) { // Zoom In
            zoomFactor *= 0.9f;
        } else if (button == GLUT_RIGHT_BUTTON) { // Zoom Out
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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // Habilita double buffering [cite: 1380]
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Exercicio Avaliativo 02 - Sistema Solar");

    initialize();

    // Registra as funções de callback
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutTimerFunc(16, update, 0); // Inicia o loop de animação

    glutMainLoop();
    return 0;
}