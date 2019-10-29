#include "libraries.h"

// gcc main.c glm.c glmimg.c glm_util.c -lglut -lGLU -lGLEW -lGL -lSOIL -lSDL -lSDL_mixer -lm

#define TRUE 1
#define FALSE 0
#define N_ARVORES 200
#define N_NUVENS 100

GLMmodel *m_arvore = NULL, *m_casa = NULL, *m_poste = NULL, *m_nuvens = NULL;

int i = 0, j = 0;

typedef struct{
    GLdouble x, y, z;
}POSICAO, NUVEM;

typedef struct{
	double scale[3];
	double x, z;
}ARVORE;

typedef struct{
	GLfloat ambiente[4];
	GLfloat difusa[4];
	GLfloat especular[4];
	GLfloat posicao[4];
    GLfloat emissiva[4];
    GLfloat brilho[1];
}LUZ, COR, MATERIAL;

int vira = 0, camType = 1, cam2Side = 1, luz = 1, fog = 1, dia = 0; //flags "booleanas"

PINHEIRO pinheiros[QTD_PINHEIROS];
NUVEM nuvens[QTD_NUVENS];

POSICAO cam, zeppelin, casa, poste, focoCamera;

LUZ sol, lampada;

MATERIAL plasticoDoZeppelin, madeira, chao, cabine, dome;

Mix_Music *jazzgostosinho;

GLuint texturaGrama, texturaHelice, texturaZeppelin, texturaDome;

GLfloat ambiente = 0.2;

double anguloRotacao = 360, anguloTranslacao = 2*M_PI, rotacaoHelice = 0, anguloCam3 = 2*M_PI;

void redimensiona(int largura, int altura){
    glViewport(0, 0, largura, altura);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, (float)(largura)/altura, 0.1, 30000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void setMaterial(MATERIAL m){
    glMaterialfv(GL_FRONT, GL_AMBIENT, m.ambiente);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, m.difusa);
    glMaterialfv(GL_FRONT, GL_SPECULAR, m.especular);
    glMaterialfv(GL_FRONT, GL_EMISSION, m.emissiva);
    glMaterialfv(GL_FRONT, GL_SHININESS, m.brilho);
}

void desenhaPlano(){
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    setMaterial(chao);
    glBindTexture(GL_TEXTURE_2D, texturaGrama);
 	    glBegin(GL_TRIANGLE_FAN);
            glColor3f(0.6, 0.6, 0.6);
            glTexCoord2f(0, 0);glVertex3f(0,0,0);
            glTexCoord2f(0, 1); glVertex3f(-10000, 0, -10000);
            glTexCoord2f(1, 1);glVertex3f(-10000,0,-5000);
            glTexCoord2f(1, 0);glVertex3f(-10000,0,0);
            glTexCoord2f(0, 1);glVertex3f(-10000,0,5000);
            glTexCoord2f(1, 1); glVertex3f(-10000, 0, 10000);
            glTexCoord2f(1, 0);glVertex3f(-5000,0,10000);
            glTexCoord2f(0, 1);glVertex3f(0,0,10000);
            glTexCoord2f(1, 1);glVertex3f(5000,0,10000);
            glTexCoord2f(1, 0); glVertex3f( 10000, 0,  10000);
            glTexCoord2f(0, 1);glVertex3f(10000,0,5000);
            glTexCoord2f(1, 1);glVertex3f(10000,0,0);
            glTexCoord2f(1, 0);glVertex3f(10000,0,-5000);
            glTexCoord2f(0, 1); glVertex3f( 10000, 0, -10000);
            glTexCoord2f(1, 1);glVertex3f(5000,0,-10000);
            glTexCoord2f(1, 0);glVertex3f(0,0,-10000);
            glTexCoord2f(0, 1);glVertex3f(-5000,0,-10000);
            glTexCoord2f(1, 1); glVertex3f(-10000, 0, -10000);
        glEnd();
    glDisable(GL_TEXTURE_2D);
}

int main(int argc, char *argv[]){

    glutInit(&argc, argv);
    
    srand(time(NULL));

    glutInitContextVersion(2, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("...");

    glutReshapeFunc(redimensiona);
    glutDisplayFunc(desenhaCena);
    glutSpecialFunc(SpecialInput);
    glutIdleFunc(desenhaCena);
    glutTimerFunc(25, atualiza, 0);
    glutKeyboardFunc(teclado);
    glewInit();
    setup();
    glutMainLoop();
    return 0;
}