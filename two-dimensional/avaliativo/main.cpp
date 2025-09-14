#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

struct Point {
    int x, y;
};

std::vector<Point> points;
bool showAxes = true;      

void draw(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    // Eixos de Coordenadas (já usam glVertex2i ou 2f, mas garantimos 2i)
    if (showAxes) {
        glColor3f(1.0f, 1.0f, 0.0f); // Cor amarela
        glBegin(GL_LINES);
            glVertex2i(0, 40);
            glVertex2i(0, -40);
            glVertex2i(-40, 0);
            glVertex2i(40, 0);
        glEnd();
    }

    // Desenho de Pontos
    glColor3f(1.0f, 0.0f, 0.0f);
    glPointSize(5.0f);
    glBegin(GL_POINTS);
        for (const auto& p : points) {
            glVertex2i(p.x, p.y);
        }
    glEnd();

    // Desenho de Linhas
    if (points.size() >= 2) {
        glColor3f(0.0f, 0.0f, 1.0f);
        glBegin(GL_LINES);
            for (size_t i = 0; i < points.size() - 1; i += 2) {
                glVertex2i(points[i].x, points[i].y);  
                glVertex2i(points[i+1].x, points[i+1].y); 
            }
        glEnd();
    }
    
    glFlush();
}

// Função de inicialização
void initialize(void) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Fundo branco
    gluOrtho2D(-40.0f, 40.0f, -40.0f, 40.0f); 
}

// Callback para eventos de mouse
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Converte e ARREDONDA para o inteiro mais próximo
        int world_x = round(-40.0f + (x / 400.0f) * 80.0f);
        int world_y = round(40.0f - (y / 400.0f) * 80.0f);

        // Cria o novo ponto
        Point newPoint = {world_x, world_y};

        // Lógica de impressão simplificada para inteiros
        if (points.size() % 2 == 0) {
            // Primeiro clique do par
            std::cout << "X: " << newPoint.x
                      << "\tY: " << newPoint.y << std::endl;
        } else {
            // Segundo clique do par
            Point p1 = points.back();
            std::cout << "X: " << newPoint.x
                      << "\tY: " << newPoint.y << std::endl;
            
            float distancia = sqrt(pow(newPoint.x - p1.x, 2) + pow(newPoint.y - p1.y, 2));

            // A impressão da distância continua com float para precisão
            std::cout << "Distancia entre os pontos (" << p1.x << "," << p1.y
                      << ") e (" << newPoint.x << "," << newPoint.y << ") = "
                      << std::fixed << std::setprecision(4) << distancia << std::endl;
        }

        points.push_back(newPoint);
        glutPostRedisplay();
    }
}

// Callback para eventos de teclado
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        // [Requisito 0.6] Limpeza da Tela
        case 'L':
        case 'l':
            points.clear(); // Limpa o vetor de pontos
            std::cout << "Tela limpa!" << std::endl;
            break;
        
        // [Requisito 0.6] Ocultar/Exibir Eixos
        case 'E':
        case 'e':
            showAxes = !showAxes; // Inverte o valor do booleano
            break;
    }
    glutPostRedisplay(); // Pede para redesenhar a tela após qualquer tecla
}

// Função principal
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Editor 2D Basico");

    initialize();

    glutDisplayFunc(draw);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}