# Phong vs Lambert: Visualizing the Phong Illumination Model in OpenGL 🎨

A two-color rotating demo — Graphics and Visualization

💡 Introduction
When we see objects around us, light interacts with their surfaces in fascinating ways.
Some materials look matte (like paper or rubber), while others appear shiny (like metal or glass)

In computer graphics, we recreate this behavior using illumination models.
In this post, we’ll explore and visualize two famous ones:

Lambert Model → simulates matte surfaces using only diffuse reflection.
Phong Model → adds ambient and specular reflection, producing realistic shiny highlights.
We’ll use OpenGL (GLUT) to show both models side by side — a blue Lambert sphere and a red Phong sphere — slowly rotating with dynamic lighting

🧠 Understanding the Phong Illumination Model
The Phong model combines three lighting components:

Press enter or click to view image in full size

Formula:

Where:

Ia,Id,Is → ambient, diffuse, and specular light intensities
ka,kd,ks → material reflection coefficients
N → surface normal
L → light direction
R → reflected light direction
V → view direction
n → shininess factor
🧩 OpenGL Implementation
We’ll use GLUT to create a simple rotating scene with two spheres:

Left (Blue): Lambert model — Diffuse only
Right (Red): Phong model — Ambient + Diffuse + Specular
Here’s the complete C code:

```c
// phong_vs_lambert_colors.c
// Lab 08: Phong Illumination Model
// Left Sphere: Lambert (Diffuse only, Blue)
// Right Sphere: Phong (Ambient + Diffuse + Specular, Red)
// Rotating demo with color contrast
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
GLfloat angle = 0.0f; // rotation angle
// --- Light properties ---
GLfloat light_ambient[]  = {0.15f, 0.15f, 0.15f, 1.0f};
GLfloat light_diffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat light_position[] = {2.0f, 2.0f, 2.0f, 1.0f};
// --- Lambert Sphere (Diffuse only, Blue) ---
GLfloat lambert_ambient[]       = {0.05f, 0.05f, 0.15f, 1.0f};
GLfloat lambert_diffuse_base[]  = {0.0f, 0.0f, 1.0f, 1.0f};
GLfloat lambert_specular[]      = {0.0f, 0.0f, 0.0f, 1.0f};
float lambert_diffuse_coef      = 1.0f;
// --- Phong Sphere (Red + Shiny) ---
GLfloat phong_ambient[]   = {0.15f, 0.05f, 0.05f, 1.0f};
GLfloat phong_diffuse[]   = {1.0f, 0.0f, 0.0f, 1.0f};
GLfloat phong_specular[]  = {1.0f, 1.0f, 1.0f, 1.0f};
float phong_shininess     = 32.0f;
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
void applyPhongMaterial() {
    if (phong_shininess < 0.0f) phong_shininess = 0.0f;
    if (phong_shininess > 128.0f) phong_shininess = 128.0f;
    glMaterialfv(GL_FRONT, GL_AMBIENT, phong_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, phong_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, phong_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, phong_shininess);
}
void initLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    GLfloat global_ambient[] = {0.05f, 0.05f, 0.05f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
}
void drawSceneSpheres() {
    // Left Sphere: Lambert (Blue)
    glPushMatrix();
        glTranslatef(-1.6f, 0.0f, 0.0f);
        applyLambertMaterial();
        glutSolidSphere(1.0, 48, 48);
    glPopMatrix();
    // Right Sphere: Phong (Red)
    glPushMatrix();
        glTranslatef(1.6f, 0.0f, 0.0f);
        applyPhongMaterial();
        glutSolidSphere(1.0, 48, 48);
    glPopMatrix();
}
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 6.0,  0.0, 0.0, 0.0,  0.0, 1.0, 0.0);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    GLfloat light_pos[] = {
        2.0f * cos(angle * M_PI / 180.0f),
        2.0f,
        2.0f * sin(angle * M_PI / 180.0f),
        1.0f
    };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    drawSceneSpheres();
    glutSwapBuffers();
}
void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLdouble)w / (GLdouble)h, 1.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
}
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case '1': lambert_diffuse_coef += 0.1f; break;
        case '2': lambert_diffuse_coef -= 0.1f; break;
        case '3': phong_shininess += 8.0f; break;
        case '4': phong_shininess -= 8.0f; break;
        case 'r': phong_shininess = 32.0f; lambert_diffuse_coef = 1.0f; break;
        case 27: exit(0); break;
    }
    glutPostRedisplay();
}
void timer(int value) {
    angle += 0.5f; // rotation speed
    if (angle > 360.0f) angle -= 360.0f;
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Phong vs Lambert - Two Colors & Rotation");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    initLighting();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}
```

⚙️ How to Run
🐧 On Linux or macOS
gcc phong_vs_lambert_colors.c -lGL -lGLU -lglut -lm -o phong_vs_lambert_colors
./phong_vs_lambert_colors
🪟 On Windows (MinGW)

gcc phong_vs_lambert_colors.c -lfreeglut -lopengl32 -lglu32 -o phong_vs_lambert_colors.exe
phong_vs_lambert_colors.exe
🎮 Controls
1 — Increase Lambert diffuse (left sphere brighter)

2 — Decrease Lambert diffuse

3 — Increase Phong shininess (sharper highlight)

4 — Decrease Phong shininess (duller highlight)

r — Reset parameters

ESC — Exit program

🎥 Demo Description
The left blue sphere (Lambert) remains matte — no matter where the light moves, it only reacts diffusely.
The right red sphere (Phong) has a moving specular highlight that becomes sharper when pressing 3 and softer with 4

As the scene rotates, you can clearly see how specular reflection depends on both the light and the viewer’s position — the essence of Phong shading

[Demo video](https://youtu.be/jmH5D4PZo9M)

🧩 Key Observations
The Lambert sphere only reacts to the angle between surface and light — no highlights.
The Phong sphere introduces realistic reflections — the specular highlight moves as the view rotates.
Adjusting shininess changes how concentrated the highlight is:
High shininess → small, bright highlight (metal-like).
Low shininess → broad, soft highlight (plastic or matte paint).
🏁 Conclusion
This simple OpenGL program perfectly demonstrates how the Phong Illumination Model extends Lambert’s by adding ambient and specular reflections.

Even today, Phong remains the conceptual foundation of modern shaders used in games, simulations, and 3D graphics.
