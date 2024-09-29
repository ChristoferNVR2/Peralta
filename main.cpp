#include <GL/glut.h>
#include <cmath> // For trigonometric functions

// Variables to control the square's position and jumping state
float posX = 0.0f;
float posY = 0.0f;
float jumpVelocity = 0.0f;
bool isJumping = false;
float gravity = -0.05f;
float rotationAngle = 0.0f; // Rotation angle during jump

void init() {
    glClearColor(0.8, 0.8, 0.8, 0.0); // Background color
}

void graphingAxes() {
    glBegin(GL_LINES);
        glColor3f(1, 0, 0); // Red color for the X-axis
        glVertex2f(-50, 0);
        glVertex2f(50, 0);
        glColor3f(0, 1, 0); // Green color for the Y-axis
        glVertex2f(0, -50);
        glVertex2f(0, 50);
    glEnd();
}

// Function to draw a square at the given position
void graphingCharacter(const float x, const float y) {
    glColor3f(0, 0, 1); // Blue color for the square
    glBegin(GL_QUADS);
        glVertex2f(x - 5, y - 5); // Bottom-left corner
        glVertex2f(x + 5, y - 5); // Bottom-right corner
        glVertex2f(x + 5, y + 5); // Top-right corner
        glVertex2f(x - 5, y + 5); // Top-left corner
    glEnd();
}

void graph() {
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    graphingAxes(); // Draw the X and Y axes

    // Translate to the new position of the square and rotate if jumping
    glTranslatef(posX, posY, 0);

    if (isJumping) {
        glRotatef(rotationAngle, 0, 0, 1); // Rotate around the Z-axis (spin during jump)
    }

    graphingCharacter(0, 0); // Draw the square at the translated position

    glFlush(); // Render everything
}

void resize(const int w, const int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-50, 50, -50, 50, -1, 1); // Define the orthographic projection
}

// Function to handle keyboard input for movement and jumping
void keyboardOperation(const unsigned char key, int x, int y) {
    switch (key) {
        case 32: // Start jumping when the space bar is pressed
            if (!isJumping) { // Only allow jump if not already jumping
                isJumping = true;
                jumpVelocity = 1.0f; // Initial upward velocity
            }
            break;
        case 'a': posX -= 1.0f; // Move left
                  break;
        case 'd': posX += 1.0f; // Move right
                  break;
        case 'w': posY += 1.0f; // Move up
                  break;
        case 's': posY -= 1.0f; // Move down
                  break;
        default:
            break;
    }

    glutPostRedisplay(); // Request a redraw after movement
}

void updateJump(int value) {
    if (isJumping) {
        posY += jumpVelocity;   // Apply the current vertical velocity
        jumpVelocity += gravity; // Apply gravity (downward force)

        // Rotate the square as it jumps
        rotationAngle += 5.0f;

        // Check if the square has landed (at y = 0)
        if (posY <= 0.0f) {
            posY = 0.0f;      // Set square to ground level
            isJumping = false; // Stop jumping
            jumpVelocity = 0.0f;
            rotationAngle = 0.0f; // Reset rotation
        }
    }

    // Re-call this function in 16 ms (approx. 60 frames per second)
    glutTimerFunc(16, updateJump, 0);
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 400);
    glutInitWindowPosition(100, 200);
    glutCreateWindow("Geometry Dash Jump");

    init();

    glutDisplayFunc(graph);      // Display callback
    glutReshapeFunc(resize); // Window resize callback
    glutKeyboardFunc(keyboardOperation); // Keyboard input callback

    // Start the jump update timer
    glutTimerFunc(16, updateJump, 0); // Call updateJump every 16ms (60 FPS)

    glutMainLoop();

    return 0;
}
