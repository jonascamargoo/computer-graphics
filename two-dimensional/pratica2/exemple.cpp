#include <GL/glut.h>
#include <iostream>
#include <vector>

// Estrutura para armazenar as informações de cada ponto
struct Point {
    float x, y;     // Coordenadas no SRU (Sistema de Referência do Universo)
    float r, g, b;  // Cor (RGB)
};

// --- Variáveis Globais ---
std::vector<Point> points; // Vetor para armazenar os pontos
float current_r = 0.0f;    // Cor atual (inicialmente preta)
float current_g = 0.0f;
float current_b = 0.0f;

// Função que desenha tudo na tela
void draw(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    // --- Eixos X e Y (Amarelo) ---
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
        // Eixo Y (vertical)
        glVertex2i(0, 40);
        glVertex2i(0, -40);
        // Eixo X (horizontal)
        glVertex2i(-40, 0);
        glVertex2i(40, 0);
    glEnd();

    // --- Pontos clicados pelo usuário ---
    glPointSize(5.0f);
    glBegin(GL_POINTS);
        for (const auto& p : points) {
            glColor3f(p.r, p.g, p.b);
            glVertex2f(p.x, p.y);
        }
    glEnd();
    
    glFlush();
}

// Função de inicialização
void initialize(void) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Fundo branco
    gluOrtho2D(-40.0f, 40.0f, -40.0f, 40.0f); 
}

// Callback for mouse events
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float world_x, world_y;
        
        // Converte coordenadas da tela (SRT) para o universo (SRU)
        world_x = -40.0f + (x / 400.0f) * 80.0f;
        world_y = 40.0f - (y / 400.0f) * 80.0f;

        // --- ADICIONE ESTA LINHA ---
        // Imprime as coordenadas no console
        std::cout << "Tela (SRT): (" << x << ", " << y << ")"
                  << "\t-> Universo (SRU): (" << world_x << ", " << world_y << ")" << std::endl;

        // Cria e armazena o novo ponto
        Point newPoint;
        newPoint.x = world_x;
        newPoint.y = world_y;
        newPoint.r = current_r;
        newPoint.g = current_g;
        newPoint.b = current_b;
        points.push_back(newPoint);

        glutPostRedisplay(); // Pede para a tela ser redesenhada
    }
}

// Callback para eventos de teclado
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'R': case 'r': current_r = 1.0f; current_g = 0.0f; current_b = 0.0f; break; // Vermelho
        case 'G': case 'g': current_r = 0.0f; current_g = 1.0f; current_b = 0.0f; break; // Verde
        case 'B': case 'b': current_r = 0.0f; current_g = 0.0f; current_b = 1.0f; break; // Azul
        case 'S': case 's': current_r = 0.0f; current_g = 0.0f; current_b = 0.0f; break; // Preto
    }
}

// Função principal
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Solucao - Pratica 02");

    initialize();

    glutDisplayFunc(draw);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}