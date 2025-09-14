#include <GL/glut.h>

void draw(void) {
    // Clear the viewport with the defined background color
    glClear(GL_COLOR_BUFFER_BIT);

    // Your image code starts here
    // Set the line width
    glLineWidth(2.0f);

    // Draw the X and Y axes
    glBegin(GL_LINES); // 
        glColor3f(1.0f, 1.0f, 0.0f); // Set the color to yellow 
        // Vertical line for the Y axis
        glVertex2i(0, 40);
        glVertex2i(0, -40); 
        // Horizontal line for the X axis
        glVertex2i(-40, 0);
        glVertex2i(40, 0);
    glEnd();

    // Draw the red square
    glBegin(GL_LINE_LOOP); // GL_LINE_LOOP draws a sequence of lines and closes the loop 
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2i(-20, 20); // Top-left vertex 
        glVertex2i(-20, -20); // Bottom-left vertex 
        glVertex2i(20, -20);  // Bottom-right vertex 
        glVertex2i(20, 20);   // Top-right vertex 
    glEnd();

    // Ensure the drawing commands are executed
    glFlush();
}

void initialize(void) {
    // Set the window background color (black)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Define the coordinate system to be used in the window (WCS)
    // The X and Y axes will range from -40 to +40
    gluOrtho2D(-40.0f, 40.0f, -40.0f, 40.0f); // 
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv); // Initialize GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Set the display mode (single buffer, RGB colors)
    
    // Set the window position and size on the screen (DCS)
    glutInitWindowPosition(100, 100); // 
    glutInitWindowSize(400, 400);     // 

    // Create the window with the specified title
    glutCreateWindow("Example 02 - Axes and Square");

    // Register the draw function to be called when the window needs to be redrawn
    glutDisplayFunc(draw);

    // Register the initialization function
    initialize();

    // Start the GLUT event processing loop
    glutMainLoop();

    return 0;
}