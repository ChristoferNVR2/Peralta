#include <GL/glut.h>

// Variables to control the square's position and jumping state
float posX = 0.0f;
float posY = 5.0f;
float lowerLimit = 5.0f;
float jumpVelocity = 0.0f;
bool isJumping = false;
float gravity = -0.05f;
float rotationAngle = 0.0f; // Rotation angle during jump

bool keys[256] = { false };

void init() {
    glClearColor(0.8, 0.8, 0.8, 0.0); // Background color
}

void graphingAxes() {
    glBegin(GL_LINES);
        glColor3f(1, 0, 0); // Red color for the X-axis
        glVertex2f(-100, 0);
        glVertex2f(100, 0);
        glColor3f(1, 0, 0); // Green color for the Y-axis
        glVertex2f(0, -100);
        glVertex2f(0, 100);
    glEnd();
}

// Function to draw a square at the given position
void graphingCharacter(const float x, const float y) {
    glColor3f(0.0078f, 0.4078f, 0.4510f); // Blue color for the square
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

void resize(const int w, int h) {
    if (h == 0) {
        h = 1;  // Prevent division by zero (in case window height is 0)
    }

    const float aspectRatio = static_cast<float>(w) / static_cast<float>(h);

    // Set the viewport to cover the new window
    glViewport(0, 0, w, h);

    // Reset the coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Adjust the orthographic projection to maintain aspect ratio
    if (aspectRatio >= 1.0f) {
        // Wide window: Expand the horizontal limits
        glOrtho(-50.0 * aspectRatio, 50.0 * aspectRatio, -30.0, 70.0, -1.0, 1.0);
    } else {
        // Tall window: Expand the vertical limits
        glOrtho(-50.0, 50.0, -30.0 / aspectRatio, 70.0 / aspectRatio, -1.0, 1.0);
    }
}


// Function to handle keyboard key press events
void keyDown(const unsigned char key, int x, int y) {
    keys[key] = true;  // Mark the key as pressed

    // Only start a jump if the space key is pressed and the square is not already jumping
    if (key == 32 && !isJumping) {
        isJumping = true;
        jumpVelocity = 1.0f; // Initial upward velocity
    }
}

// Function to handle keyboard key release events
void keyUp(const unsigned char key, int x, int y) {
    keys[key] = false;  // Mark the key as released
}

// Function to update character movement based on key states
void handleMovement() {
    if (keys['a']) { // Move left if 'a' is pressed
        posX -= 1.0f;
    }
    if (keys['d']) { // Move right if 'd' is pressed
        posX += 1.0f;
    }
    if (keys['r']) { // Reset the square's position if 'r' is pressed
        posX = 0.0f;
        posY = 5.0f;
        isJumping = false;
        jumpVelocity = 0.0f;
        rotationAngle = 0.0f;
    }
}

void updateJump(int value) {
    if (isJumping) {
        posY += jumpVelocity;   // Apply the current vertical velocity
        jumpVelocity += gravity; // Apply gravity (downward force)

        // Check if 'd' is pressed for clockwise rotation
        if (keys['d']) {
            rotationAngle -= 5.0f; // Clockwise rotation (negative angle)
        } else if (keys['a']) {
            rotationAngle += 5.0f; // Counterclockwise rotation (positive angle)
        }

        // Check if the square has landed (at y = 0)
        if (posY <= lowerLimit) {
            posY = lowerLimit;      // Set square to ground level
            isJumping = false; // Stop jumping
            jumpVelocity = 0.0f;
            rotationAngle = 0.0f; // Reset rotation
        }
    }

    // Handle horizontal movement (even when jumping)
    handleMovement();

    // Re-call this function in 16 ms (approx. 60 frames per second)
    glutTimerFunc(16, updateJump, 0);
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 400);
    // glutInitWindowPosition(100, 200);
    glutCreateWindow("Geometry Dash Jump");

    init();

    glutDisplayFunc(graph);      // Display callback
    glutReshapeFunc(resize); // Window resize callback
    glutKeyboardFunc(keyDown);   // Register key down function
    glutKeyboardUpFunc(keyUp);   // Register key up function

    // Start the jump update timer
    // This function will be called only one time to start the jump
    glutTimerFunc(16, updateJump, 0); // Call updateJump every 16ms (60 FPS)

    glutMainLoop();

    return 0;
}
