#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>


enum DrawingMode {
    MODE_PAIRS,
    MODE_STRIP
};
struct Point {
    int x, y;
};

DrawingMode currentMode = MODE_PAIRS;  

std::vector<Point> points; 
bool showAxes = true;      
float pointSize = 5.0f; 
float lineWidth = 2.0f;


void draw(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    glLineWidth(lineWidth);
    if (showAxes) {
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_LINES);
            glVertex2i(0, 40);
            glVertex2i(0, -40);
            glVertex2i(-40, 0);
            glVertex2i(40, 0);
        glEnd();
    }

    glColor3f(1.0f, 0.0f, 0.0f);
    glPointSize(pointSize);
    glBegin(GL_POINTS);
        for (const Point& p : points) {
            glVertex2i(p.x, p.y);
        }
    glEnd();

    if (points.size() >= 2) {
        glColor3f(0.0f, 0.0f, 1.0f);
        glLineWidth(lineWidth);

        if (currentMode == MODE_PAIRS) {
            glBegin(GL_LINES);
                for (size_t i = 0; i < points.size() - 1; i += 2) {
                    glVertex2i(points[i].x, points[i].y);  
                    glVertex2i(points[i+1].x, points[i+1].y); 
                }
            glEnd();
        } else {
            glBegin(GL_LINE_STRIP);
                for (const Point& p : points) {
                    glVertex2i(p.x, p.y);
                }
            glEnd();
        }
    }
    
    glFlush();
}


void initialize(void) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    gluOrtho2D(-40.0f, 40.0f, -40.0f, 40.0f); 
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int world_x = round(-40.0f + (x / 400.0f) * 80.0f);
        int world_y = round(40.0f - (y / 400.0f) * 80.0f);
        Point newPoint = {world_x, world_y};

        std::cout << "X: " << newPoint.x << "\tY: " << newPoint.y << std::endl;

        bool shouldCalculateDistance = false;
        if (currentMode == MODE_PAIRS && points.size() % 2 != 0) {
            shouldCalculateDistance = true;
        } else if (currentMode == MODE_STRIP && !points.empty()) {
            shouldCalculateDistance = true;
        }

        if (shouldCalculateDistance) {
            Point p1 = points.back();
            float distancia = sqrt(pow(newPoint.x - p1.x, 2) + pow(newPoint.y - p1.y, 2));
            std::cout << "Distancia entre os pontos (" << p1.x << "," << p1.y
                      << ") e (" << newPoint.x << "," << newPoint.y << ") = "
                      << std::fixed << std::setprecision(4) << distancia << std::endl;
        }

        points.push_back(newPoint);
        glutPostRedisplay();
    }
}


void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'L':
        case 'l':
            points.clear();
            std::cout << "Tela limpa!" << std::endl;
            break;
        
        case 'E':
        case 'e':
            showAxes = !showAxes;
            break;
            
        case '+':
            pointSize += 1.0f;
            lineWidth += 1.0f;
            std::cout << "Espessura aumentada. Ponto: " << pointSize << ", Linha: " << lineWidth << std::endl;
            break;

        case '-':
            if (pointSize > 1.0f) pointSize -= 1.0f;
            if (lineWidth > 1.0f) lineWidth -= 1.0f;
            std::cout << "Espessura diminuida. Ponto: " << pointSize << ", Linha: " << lineWidth << std::endl;
            break;

        case 'Z':
        case 'z':
            if (!points.empty()) {
                points.pop_back();
                std::cout << "Ultima acao desfeita." << std::endl;
            }
            break;
        case 'R':
        case 'r':
            glClear(GL_COLOR_BUFFER_BIT);
            if (currentMode == MODE_PAIRS) {
                currentMode = MODE_STRIP;
                std::cout << "Modo de desenho alterado para: Linha Continua" << std::endl;
            } else {
                currentMode = MODE_PAIRS;
                std::cout << "Modo de desenho alterado para: Pares de Pontos" << std::endl;
            }
            break;
    }
    glutPostRedisplay();
}

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