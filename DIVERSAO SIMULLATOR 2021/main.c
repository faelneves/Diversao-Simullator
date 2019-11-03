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

int vira = 0, camType = 1, cam2Side = 1, luz = 1, dia = 0; //flags "booleanas"


POSICAO cam, focoCamera;

LUZ sol, lampada;

MATERIAL plasticoDoZeppelin, madeira, chao, cabine, dome;

//Mix_Music *jazzgostosinho;

GLuint texturaPiso;

GLfloat ambiente = 0.2;

double anguloRotacao = 360, anguloTranslacao = 2*M_PI, anguloCam3 = 2*M_PI;

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

void atualizaCam(){
    if(camType == 1){
        focoCamera.x = cam.x;
        focoCamera.y = cam.y;
        //cam.x = cam.x + 15*sin(anguloTranslacao);
        //cam.z = cam.z + 15*cos(anguloTranslacao);
        focoCamera.z = cam.z;
    }
}

/*void criaObjeto(GLMmodel* objeto, coordenadas coordenada){
    if(!objeto)
        exit(0);
    glmScale(objeto, 200.0);
    glmUnitize(objeto);
    glmFacetNormals(objeto);
    glmVertexNormals(objeto, 90.0, 1);

    glPushMatrix();
    glTranslatef(coordenada.x, coordenada.y, coordenada.z);
    glScalef(1,1,1);
    glmDraw(objeto, GLM_SMOOTH | GLM_TEXTURE | GLM_COLOR);
    glPopMatrix();

}*/

void desenhaPlano(){
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    setMaterial(chao);
    glBindTexture(GL_TEXTURE_2D, texturaPiso);
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

void desenhaCena(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
        gluLookAt(cam.x, cam.y, cam.z, focoCamera.x, focoCamera.y, focoCamera.z, 0, 1, 0);
        glLightfv(GL_LIGHT0, GL_POSITION, sol.posicao);

        if(luz) glEnable(GL_LIGHTING);
        
        desenhaPlano();

       	if(luz) glDisable(GL_LIGHTING);
    	
    glPopMatrix();

    glutSwapBuffers();
}

void configLuz(){
	if(ambiente <= -0.3) dia = 1;
    if(ambiente >= 0.5) dia = 0;
    if(dia) ambiente += 0.0005;
    else ambiente -= 0.0005;
    for(i = 0; i < 3; i++) sol.ambiente[i] = ambiente;
    for(i = 0; i < 3; i++) sol.difusa[i] = 0.7;
    for(i = 0; i < 3; i++) sol.especular[i] = 1;
    sol.ambiente[3] = sol.difusa[3] = sol.especular[3] = 1;
    sol.posicao[0] = 0; sol.posicao[1] = 1; sol.posicao[2] = 0; sol.posicao[3] = 0;

    glLightfv(GL_LIGHT0, GL_AMBIENT, sol.ambiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sol.difusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, sol.especular);
    
    glLightfv(GL_LIGHT1, GL_AMBIENT, lampada.ambiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lampada.difusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lampada.especular);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05f);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    glShadeModel (GL_SMOOTH);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
}

void atualiza(int periodo){
    configLuz();
    //setupMaterial();
	glutTimerFunc(25, atualiza, 0);	
	glutPostRedisplay();
}

void inicializa(){
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    cam.x = 0;
    cam.y = 108;
    cam.z = 10;

    //CHAO
    for(i = 0; i < 3; i++) chao.ambiente[i] = 0;
    chao.ambiente[1] = .2;
    for(i = 0; i < 3; i++) chao.difusa[i] = .5;
    chao.difusa[1] = .6;
    for(i = 0; i < 3; i++) chao.especular[i] = .0;
    chao.especular[1] = .1;
    for(i = 0; i < 3; i++) chao.emissiva[i] = .0;
    chao.ambiente[3] = chao.difusa[3] = chao.especular[3] = chao.emissiva[3] = 1;
    chao.brilho[0] = 0;

    texturaPiso = SOIL_load_OGL_texture("chao.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (!texturaPiso)
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    configLuz();
    atualizaCam();
}

int main(int argc, char *argv[]){

    glutInit(&argc, argv);
    
    srand(time(NULL));

    glutInitContextVersion(2, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("DIVERSAO SIMULATOR");

    glutReshapeFunc(redimensiona);
    glutDisplayFunc(desenhaCena);
    //glutSpecialFunc(SpecialInput);
    glutIdleFunc(desenhaCena);
    glutTimerFunc(25, atualiza, 0);
    /*glutKeyboardFunc(teclado);*/
    glewInit();
    inicializa();
    glutMainLoop();
    return 0;
}