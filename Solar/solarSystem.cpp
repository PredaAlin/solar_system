#include <GL/glut.h>
#include <math.h>
#include <SOIL2.h>

#define PI 3.1415926535898 




double planetOrbits[8] = {
    0.39 * 2,  // Mercury
    0.72 * 2,  // Venus
    1.0 * 2,   // Earth
    1.52 * 2,  // Mars
    2.20 * 2,  // Jupiter
    2.58 * 2,  // Saturn
    2.918 * 2, // Uranus
    3.007 * 2  // Neptune
};


double sunAngle = 0.0;
GLdouble angle[8] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }; // angles for planet rotation
double planetSizes[8] = {
    0.383 / 50, // Mercury
    0.949 / 50, // Venus
    1.0 / 50,   // Earth
    0.532 / 50, // Mars
    11.21 / 50, // Jupiter
    9.45 / 50,  // Saturn
    4.01 / 50,  // Uranus
    3.88 / 50   // Neptune
};



GLuint textureIDSun;

GLuint textureIDMercury;
GLuint textureIDVenus;
GLuint textureIDEarth;
GLuint textureIDMars;
GLuint textureIDJupiter;
GLuint textureIDSaturn;
GLuint textureIDUranus;
GLuint textureIDNeptune;

GLuint textureIDStarBlue;
GLuint textureIDStarGreen;

void loadTexture(const char* filename, GLuint& textureID) {
    // Load texture image
    int width, height;
    unsigned char* image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);

    // Generate texture ID
    glGenTextures(1, &textureID);

    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set texture image data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    // Free image data
    SOIL_free_image_data(image);
}

// Camera variables
float cameraAngleX = 0.0f;
float cameraAngleY = 0.0f;
float cameraDistance = 10.0f;


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Update camera position and direction
    gluLookAt(
        cameraDistance * sinf(cameraAngleX) * sinf(cameraAngleY),
        cameraDistance * cosf(cameraAngleY),
        cameraDistance * cosf(cameraAngleX) * sinf(cameraAngleY),
        0.0, 0.0, 0.0,  // Look at point
        0.0, cosf(cameraAngleY) > 0 ? 1.0 : -1.0, 0.0  // Up vector
    );

    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0); // Enable light #0

    // Set up sun light
    GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 }; // Position at the origin (where the sun is)
    GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 }; // White light
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);

    // Set up star lights (light #1 and #2)
    GLfloat blue_light[] = { 0.0, 0.0, 1.0, 1.0 }; // Blue light
    GLfloat green_light[] = { 0.0, 1.0, 0.0, 1.0 };
    GLfloat light_position1[] = { -5.0, 0.0, -5.0, 1.0 }; // Position for the first star
    GLfloat light_position2[] = { 5.0, 0.0, -5.0, 1.0 }; // Position for the second star

    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, blue_light);
    glLightfv(GL_LIGHT1, GL_SPECULAR, blue_light);

    glEnable(GL_LIGHT2);
    glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, green_light);
    glLightfv(GL_LIGHT2, GL_SPECULAR, blue_light);

    
    // Draw the sun
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureIDSun);
    glColor3f(1.0, 1.0, 1.0); // white color to not tint the texture

    glDisable(GL_LIGHTING);

    // Set material properties for the planets
    GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 }; // Diffuse reflection is white
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 }; // Specular reflection is white
    GLfloat mat_shininess[] = { 50.0 }; // Shininess parameter

    GLUquadricObj* quadricSun;
    quadricSun = gluNewQuadric();
    gluQuadricTexture(quadricSun, GL_TRUE);
    glPushMatrix(); // Save current transformation
    glRotated(sunAngle, 0.0, 1.0, 0.0); // Rotate the sun
    gluSphere(quadricSun, 109.0 / 200, 50, 50); // sun
    glPopMatrix(); // Restore transformation
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_LIGHTING);

    // Draw the planets
    GLuint textures[8] = { textureIDMercury, textureIDVenus, textureIDEarth, textureIDMars, textureIDJupiter, textureIDSaturn, textureIDUranus, textureIDNeptune };
    for (int i = 0; i < 8; i++) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glColor3f(1.0, 1.0, 1.0); // white color to not tint the texture

        // Set material properties for the planet
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);


        glPushMatrix();
        glRotated(angle[i], 0.0, 1.0, 0.0); // rotate the planet around the sun
        glTranslated(planetOrbits[i], 0.0, 0.0); // move the planet away from the sun
        GLUquadricObj* quadricPlanet;
        quadricPlanet = gluNewQuadric();
        gluQuadricTexture(quadricPlanet, GL_TRUE);
        gluSphere(quadricPlanet, planetSizes[i], 50, 50); // planet
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureIDStarBlue);
    glDisable(GL_LIGHTING); // Disable lighting for the stars

    glColor3f(1.0, 1.0, 1.0); // Blue color for the stars
    glPushMatrix();
    glTranslatef(light_position1[0], light_position1[1], light_position1[2]);
    GLUquadricObj* quadricStarBlue;
    quadricStarBlue = gluNewQuadric();
    gluQuadricTexture(quadricStarBlue, GL_TRUE);
    gluSphere(quadricStarBlue, 0.3, 20, 20); // First star
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureIDStarGreen);

    glColor3f(1.0, 1.0, 1.0);//green
    glPushMatrix();
    glTranslatef(light_position2[0], light_position2[1], light_position2[2]);
    GLUquadricObj* quadricStarGreen;
    quadricStarGreen = gluNewQuadric();
    gluQuadricTexture(quadricStarGreen, GL_TRUE);
    gluSphere(quadricStarGreen, 0.3, 20, 20); // Second star
    glPopMatrix();

    glEnable(GL_LIGHTING); // Re-enable lighting for other objects
    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers();
}

double zoom = 40.0;
int windowWidth = 1000;
int windowHeight = 1000;


void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, (float)w / (float)h, 1.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 10.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);
}
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
        cameraAngleY += 0.01f;
        break;
    case 's':
        cameraAngleY -= 0.01f;
        break;
    case 'a':
        cameraAngleX -= 0.01f;
        break;
    case 'd':
        cameraAngleX += 0.01f;
        break;
    case 'q':
        cameraDistance -= 0.1f;
        break;
    case 'e':
        cameraDistance += 0.1f;
        break;
    }
    glutPostRedisplay();
}


void idle() {
    sunAngle += 0.02; // speed of sun rotation
    if (sunAngle > 360.0) {
        sunAngle -= 360.0;
    }

    for (int i = 0; i < 8; i++) {
        angle[i] += 0.02 * (i + 1); // speed of planet rotation
        if (angle[i] > 360.0) {
            angle[i] -= 360.0;
        }
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("Solar System");
    glutKeyboardFunc(keyboard);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
 

   
    loadTexture("sun.jpg", textureIDSun);

    loadTexture("mercury.jpg", textureIDMercury);
    loadTexture("venus.jpg", textureIDVenus);
    loadTexture("earth.jpg", textureIDEarth);
    loadTexture("mars.jpg", textureIDMars);
    loadTexture("jupiter.jpg", textureIDJupiter);
    loadTexture("saturn.jpg", textureIDSaturn);
    loadTexture("uranus.jpg", textureIDUranus);
    loadTexture("neptune.jpg", textureIDNeptune);

    loadTexture("blue_star.jpg", textureIDStarBlue);
    loadTexture("green_star.jpg", textureIDStarGreen);


    glutMainLoop();
    return 0;
}

