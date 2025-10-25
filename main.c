// phong_vs_lambert.c
// Lab Topic 4: The Phong Illumination Model
// Left sphere = Lambert model (diffuse only)
// Right sphere = Phong model (ambient + diffuse + specular)
//
// Controls:
//   '1' -> Increase Lambert diffuse coefficient
//   '2' -> Decrease Lambert diffuse coefficient
//   '3' -> Increase Phong shininess (sharper highlight)
//   '4' -> Decrease Phong shininess (duller highlight)
//   'r' -> Reset all parameters
//   'ESC' -> Exit program

#include <GL/glut.h>
#include <stdio.h>

//
// Light properties
//
GLfloat light_ambient[]  = {0.15f, 0.15f, 0.15f, 1.0f};
GLfloat light_diffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat light_position[] = {2.0f, 2.0f, 2.0f, 1.0f};

//
// Lambert sphere (Diffuse only)
//
GLfloat lambert_ambient[]       = {0.05f, 0.05f, 0.1f, 1.0f};
GLfloat lambert_diffuse_base[]  = {0.0f, 0.0f, 1.0f, 1.0f};  // Blue
GLfloat lambert_specular[]      = {0.0f, 0.0f, 0.0f, 1.0f};  // No specular
float lambert_diffuse_coef      = 1.0f;

//
// Phong sphere (Ambient + Diffuse + Specular)
//
GLfloat phong_ambient[]   = {0.05f, 0.05f, 0.05f, 1.0f};
GLfloat phong_diffuse[]   = {0.0f, 0.0f, 1.0f, 1.0f};
GLfloat phong_specular[]  = {1.0f, 1.0f, 1.0f, 1.0f};
float phong_shininess     = 32.0f;

//
// Apply Lambert material (diffuse only)
//
void applyLambertMaterial() {
    GLfloat diff[4] = {
        lambert_diffuse_base[0] * lambert_diffuse_coef,
        lambert_diffuse_base[1] * lambert_diffuse_coef,
        lambert_diffuse_base[2] * lambert_diffuse_coef,
        1.0f
    };
    glMaterialfv(GL_FRONT, GL_AMBIENT, lambert_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, lambert_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);
}

//
// Apply Phong material (ambient + diffuse + specular)
//
void applyPhongMaterial() {
    if (phong_shininess < 0.0f) phong_shininess = 0.0f;
    if (phong_shininess > 128.0f) phong_shininess = 128.0f;

    glMaterialfv(GL_FRONT, GL_AMBIENT, phong_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, phong_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, phong_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, phong_shininess);
}

//
// Initialize light settings
//
void initLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    // Global ambient light
    GLfloat global_ambient[] = {0.05f, 0.05f, 0.05f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
}

//
// Draw both spheres
//
void drawSceneSpheres() {
    // LEFT SPHERE - Lambert
    glPushMatrix();
        glTranslatef(-1.6f, 0.0f, 0.0f);
        applyLambertMaterial();
        glutSolidSphere(1.0, 48, 48);
    glPopMatrix();

    // RIGHT SPHERE - Phong
    glPushMatrix();
        glTranslatef(1.6f, 0.0f, 0.0f);
        applyPhongMaterial();
        glutSolidSphere(1.0, 48, 48);
    glPopMatrix();
}

//
// Display callback
//
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Camera
    gluLookAt(0.0, 0.0, 6.0,   // Eye position
              0.0, 0.0, 0.0,   // Look-at point
              0.0, 1.0, 0.0);  // Up vector

    // Light position (set AFTER camera)
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    drawSceneSpheres();

    glutSwapBuffers();
}

//
// Reshape callback
//
void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLdouble)w / (GLdouble)h, 1.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
}

//
// Keyboard controls
//
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case '1':
            lambert_diffuse_coef += 0.1f;
            if (lambert_diffuse_coef > 1.5f) lambert_diffuse_coef = 1.5f;
            printf("Lambert diffuse coefficient: %.2f\n", lambert_diffuse_coef);
            break;

        case '2':
            lambert_diffuse_coef -= 0.1f;
            if (lambert_diffuse_coef < 0.0f) lambert_diffuse_coef = 0.0f;
            printf("Lambert diffuse coefficient: %.2f\n", lambert_diffuse_coef);
            break;

        case '3':
            phong_shininess += 8.0f;
            if (phong_shininess > 128.0f) phong_shininess = 128.0f;
            printf("Phong shininess increased: %.1f (sharper highlight)\n", phong_shininess);
            break;

        case '4':
            phong_shininess -= 8.0f;
            if (phong_shininess < 0.0f) phong_shininess = 0.0f;
            printf("Phong shininess decreased: %.1f (duller highlight)\n", phong_shininess);
            break;

        case 'r':
        case 'R':
            lambert_diffuse_coef = 1.0f;
            phong_shininess = 32.0f;
            printf("Parameters reset to defaults.\n");
            break;

        case 27: // ESC
            exit(0);
            break;

        default:
            break;
    }
    glutPostRedisplay();
}

//
// Print controls
//
void printInstructions() {
    printf("\n=== Illumination Lab Controls ===\n");
    printf("1 : Increase Lambert diffuse coefficient (left sphere)\n");
    printf("2 : Decrease Lambert diffuse coefficient (left sphere)\n");
    printf("3 : Increase Phong shininess (right sphere)\n");
    printf("4 : Decrease Phong shininess (right sphere)\n");
    printf("r : Reset all parameters\n");
    printf("ESC : Exit\n");
    printf("\nLeft Sphere: Lambert Model (Diffuse only)\n");
    printf("Right Sphere: Phong Model (Ambient + Diffuse + Specular)\n");
    printf("=========================================\n\n");
}

//
// Main function
//
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Lab 08 - Phong Illumination Model");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    initLighting();
    printInstructions();

    glClearColor(0.12f, 0.12f, 0.12f, 1.0f);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
