#include "mask.h"
#include "shared.h"
#include <GL/glut.h>
#include <memory.h>
#include <time.h>
#include <windows.h>

int FRAMERATE;
int FRAMETIME;
GLubyte **img;
GLubyte **bkg;
CMask *mask;
int g_width;
int g_height;
int SPEED;
bool PAUSE;

long long GetSysTimeMillis()
{
    FILETIME ft;
    LARGE_INTEGER li;
    GetSystemTimeAsFileTime(&ft);
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;
    return (li.QuadPart - 116444736000000000UL) / 10000;
}

void display()
{
    // glClear(GL_COLOR_BUFFER_BIT);
    glDrawPixels((GLsizei)g_width, (GLsizei)g_height, GL_LUMINANCE, GL_UNSIGNED_BYTE, img[0]);
    glutSwapBuffers();
}

void idle()
{
    static long long lastMillis = 0;
    long long currentMillis = GetSysTimeMillis();
    if (currentMillis - FRAMETIME < lastMillis)
        return;
    lastMillis = currentMillis;

    static int k = 0;
    k = (k + SPEED) % g_height;

    if (!PAUSE)
        if (!mask->GetMask())
            exit(0);

    for (int i = 0; i < g_height; ++i)
        for (Cell *p = mask->mask[i].cell; p; p = p->next)
            if (!p->mask)
                memcpy(&img[i][p->offset], &bkg[i][p->offset], sizeof(GLubyte) * p->length);
            else
                memcpy(&img[i][p->offset], &bkg[(i - k + g_height) % g_height][p->offset], sizeof(GLubyte) * p->length);
}

void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(FRAMETIME, OnTimer, 1);
}

void SpecialKeys(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        ++SPEED;
        break;
    case GLUT_KEY_DOWN:
        if (SPEED > 1)
            --SPEED;
        break;
    case GLUT_KEY_LEFT:
        if (FRAMERATE > 1)
        {
            --FRAMERATE;
            FRAMETIME = 1000 / FRAMERATE;
        }
        break;
    case GLUT_KEY_RIGHT:
        ++FRAMERATE;
        FRAMETIME = 1000 / FRAMERATE;
        break;
    default:
        break;
    }

    char szTitle[100];
    sprintf(szTitle, "noise [Scroll Speed : %d    Frame Rate : %d]", SPEED, FRAMERATE);
    glutSetWindowTitle(szTitle);
}

void NormalKeys(unsigned char key, int x, int y)
{
    if (key == ' ')
        PAUSE = !PAUSE;
    if (key == 27)
        exit(0);
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);

    PAUSE = FALSE;
    SPEED = 1;
    FRAMERATE = 63;
    FRAMETIME = 1000 / FRAMERATE;
    srand((unsigned int)time(0));
    for (int x = 0; x < g_width; ++x)
        for (int y = 0; y < g_height; ++y)
            img[y][x] = bkg[y][x] = rand() % 256;

    char szTitle[100];
    sprintf(szTitle, "noise [Scroll Speed : %d    Frame Rate : %d]", SPEED, FRAMERATE);
    glutSetWindowTitle(szTitle);

    // for (int i = 0; i < g_height; ++i)
    //     bkg[i][int(float(g_width) / float(g_height) * float(i))] = 255;
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
}

void malloc2D(GLubyte **&p, int width, int height)
{
    // Attention : addr(p) may not equal to addr(p[0]), e.g. addr(p)+8=addr(p[0])
    p = (GLubyte **)malloc(sizeof(GLubyte *) * height);
    p[0] = (GLubyte *)malloc(sizeof(GLubyte) * height * width);
    for (int i = 1; i < height; ++i)
        p[i] = p[i - 1] + width;
}

int main(int argc, char **argv)
{
    mask = NULL;
    img = NULL;
    bkg = NULL;
    char szFile[] = "noise.bin";
    mask = new CMask(szFile);
    mask->GetSize(g_width, g_height);
    if (!mask->GetMask())
        return -1;

    malloc2D(img, g_width, g_height);
    malloc2D(bkg, g_width, g_height);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(g_width, g_height);
    glutCreateWindow("noise");
    init();
    glutKeyboardFunc(NormalKeys);
    glutSpecialFunc(SpecialKeys);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutTimerFunc(FRAMETIME, OnTimer, 1);
    glutMainLoop();
    return 0;
}
