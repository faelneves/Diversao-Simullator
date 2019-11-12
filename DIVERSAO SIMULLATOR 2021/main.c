#include "libraries.h"

// gcc main.c glm.c glmimg.c glm_util.c -lglut -lGLU -lGLEW -lGL -lSOIL -lSDL -lSDL_mixer -lm

#define TRUE 1
#define FALSE 0
#define AumentoAngulo 1

GLMmodel *poste = NULL, *elevador = NULL, *arvore = NULL, *base = NULL, *aro = NULL, *cadeiras = NULL;

int i = 0, j = 0;

typedef struct{
    GLdouble x, y, z;
}PONTO;


typedef struct{
	GLfloat ambiente[4];
	GLfloat difusa[4];
	GLfloat especular[4];
	GLfloat posicao[4];
    GLfloat emissiva[4];
    GLfloat brilho[1];
}LUZ, COR, MATERIAL;

int vira = 0, camType = 1, luz = 0, dia = 0; //flags "booleanas"


PONTO cam, focoCamera, camAux, escala, trasla;

LUZ sol;

MATERIAL chao;

float anguloRoda = 0;

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
        cam.x = camAux.x + 15*sin(anguloTranslacao);
        cam.z = camAux.z + 15*cos(anguloTranslacao);
        focoCamera.x = camAux.x;
        focoCamera.y = camAux.y;
        focoCamera.z = camAux.z;

    }
}

void criaObjeto(GLMmodel* objeto, PONTO ptTransled,PONTO ptScaled){
    glTranslated(ptTransled.x, ptTransled.y, ptTransled.z);
    glScaled(ptScaled.x, ptScaled.y, ptScaled.z);
    if(!objeto)
        exit(0);
    glColor3f(1.0f, 1.0f, 1.0f);
    glmDraw(objeto, GLM_SMOOTH | GLM_COLOR | GLM_TEXTURE);
}


void desenhaPlano(){
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    setMaterial(chao);
    glBindTexture(GL_TEXTURE_2D, texturaPiso);
 	
 	// Desenha um cubo no qual a textura é aplicada
	glBegin ( GL_QUADS );
		// Face frontal
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1000, -1.0f,  1000);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1000, -1.0f,  1000);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1000,  1.0f,  1000);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1000,  1.0f,  1000);
		// Face posterior
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1000, -1.0f, -1000);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1000,  1.0f, -1000);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1000,  1.0f, -1000);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1000, -1.0f, -1000);
		// Face superior
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1000,  1.0f, -1000);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1000,  1.0f,  1000);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1000,  1.0f,  1000);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1000,  1.0f, -1000);
		// Face inferior
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1000, -1.0f, -1000);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1000, -1.0f, -1000);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1000, -1.0f,  1000);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1000, -1.0f,  1000);
		// Face lateral direita
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1000, -1.0f, -1000);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1000,  1.0f, -1000);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1000,  1.0f,  1000);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1000, -1.0f,  1000);
		// Face lateral esquerda
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1000, -1.0f, -1000);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1000, -1.0f,  1000);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1000,  1.0f,  1000);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1000,  1.0f, -1000);
	glEnd();
    glDisable(GL_TEXTURE_2D);
}

void desenhaRoda(PONTO ptTransled, PONTO ptScaled){
    
    glPushMatrix();
    glTranslatef(ptTransled.x, ptTransled.y, ptTransled.z);
    glScaled(ptScaled.x, ptScaled.y, ptScaled.z);
    glmDraw(base, GLM_SMOOTH | GLM_TEXTURE | GLM_COLOR);
        glPushMatrix();
        glTranslatef(ptTransled.x, ptTransled.y-0.2,ptTransled.z);
        glRotatef(anguloRoda,0,0,1);
        glmDraw(aro, GLM_SMOOTH | GLM_TEXTURE | GLM_COLOR);
        glRotatef(-anguloRoda,0,0,1);
            int t;
            for(t=0;t<8;t++)
            {
                glPushMatrix();
                glScalef(0.3,0.3,0.25);
                glTranslatef(-2.5*sin(anguloRoda*M_PI/180+t*M_PI/4),+2.5*cos(anguloRoda*M_PI/180+t*M_PI/4),0);
                glmDraw(cadeiras, GLM_SMOOTH | GLM_TEXTURE | GLM_COLOR);
                glPopMatrix();
            }
        glPopMatrix();
    glPopMatrix();
}



void desenhaCena(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
        gluLookAt(cam.x, cam.y, cam.z, focoCamera.x, focoCamera.y, focoCamera.z, 0, 1, 0);
        glLightfv(GL_LIGHT0, GL_POSITION, sol.posicao);

        if(luz) glEnable(GL_LIGHTING);
        
        desenhaPlano();
        
        //ELEVADOR
        glPushMatrix();
        	trasla.x = -500;
        	trasla.y = 500;
        	trasla.z = 500;

            escala.x = 500;
        	escala.y = 500;
        	escala.z = 500;


            if (!elevador){
			    elevador = glmReadOBJ("objetos/elevador.obj");
			    if (!elevador) exit(0);
			    glmUnitize(elevador);
			    glmFacetNormals(elevador);
			    glmVertexNormals(elevador, 90.0f, GL_TRUE);
		    }
        	criaObjeto(elevador,trasla,escala);
        glPopMatrix();

        

        //ARVORES
    	escala.x = 100;
    	escala.y = 100;
    	escala.z = 100;
		
		trasla.x = -150;
		trasla.y = 100;
		trasla.z = -800;
        for (int i = 0; i < 6; ++i)
        {
        	trasla.z += i*40;

	        glPushMatrix();
	            if (!arvore){
				    arvore = glmReadOBJ("objetos/lowpolytree.obj");
				    if (!arvore) exit(0);
				    glmUnitize(arvore);
				    glmFacetNormals(arvore);
				    glmVertexNormals(arvore, 90.0f, GL_TRUE);
			    }
	        	criaObjeto(arvore,trasla,escala);
	        glPopMatrix();
        }
		
		trasla.x = 150;
		trasla.z = -800;
        for (int i = 0; i < 6; ++i)
        {
        	trasla.z += i*40;
	        glPushMatrix();
	        	criaObjeto(arvore,trasla,escala);
	        glPopMatrix();
        }


        //POSTE
    	trasla.x = 150;
    	trasla.z = 1000;

        if (!poste){
		    poste = glmReadOBJ("objetos/STLamp.obj");
		    if (!poste) exit(0);
		    glmUnitize(poste);
		    glmFacetNormals(poste);
		    glmVertexNormals(poste, 90.0f, GL_TRUE);
	    }

    	for (int i = 0; i < 6; ++i){
    		glPushMatrix();
        	trasla.z -= 145;
        	criaObjeto(poste,trasla,escala);
    		glPopMatrix();
        }

        trasla.x = -150;
    	trasla.z = 1000;
     	for (int i = 0; i < 6; ++i){
    		glPushMatrix();
        	trasla.z -= 145;
        	criaObjeto(poste,trasla,escala);
    		glPopMatrix();
        }

    	//RODA
        	trasla.x = 0;
        	trasla.y = 1.2;
        	trasla.z = 0;

        	escala.x = 300;
        	escala.y = 300;
        	escala.z = 300;

            if(!base){
	            base = glmReadOBJ("objetos/base.obj");
	            if(!base)
	                exit(0);
	            glmUnitize(base);
	            glmFacetNormals(base);
	            glmVertexNormals(base, 90.0f, GL_TRUE);
	        }

            if(!aro){
	            aro = glmReadOBJ("objetos/aro.obj");
	            if(!aro)
	                exit(0);
	            glmUnitize(aro);
	            glmFacetNormals(aro);
	            glmVertexNormals(aro, 90.0f, GL_TRUE);
	        }

            if(!cadeiras){
	            cadeiras = glmReadOBJ("objetos/cadeiras.obj");
	            if(!cadeiras)
	                exit(0);
	            glmUnitize(cadeiras);
	            glmFacetNormals(cadeiras);
	            glmVertexNormals(cadeiras, 90.0f, GL_TRUE);
	        }

        	desenhaRoda(trasla,escala);

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
    

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    glShadeModel (GL_SMOOTH);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
}

void atualiza(int periodo){
    configLuz();
    anguloRoda += AumentoAngulo;
    if(anguloRoda>360)
        anguloRoda-=360;

	glutTimerFunc(25, atualiza, 0);	
	glutPostRedisplay();
}

void teclado(unsigned char key, int x, int y){
    switch(key){
        case 27: // ESC
            exit(0);
            break;
        case 'a':
            if(camType == 1){
	            anguloRotacao += 2;
	            anguloTranslacao += M_PI/90;
	            atualizaCam();
        	}
            break;
        case 'd':
            if(camType == 1){
		        anguloTranslacao -= M_PI/90;
		        anguloRotacao -= 2;
		        atualizaCam();
		    }
            break;
       case 'w':
            if(camType == 1){
	            camAux.x -= 4*sin(anguloTranslacao);
	            camAux.z -= 4*cos(anguloTranslacao);
	            atualizaCam();
	        }
            break;
        case 's':
            if(camType == 1){
	            camAux.x += 2*sin(anguloTranslacao);
	            camAux.z += 2*cos(anguloTranslacao);
	            atualizaCam();
	        }
            break;
        case 'l':
        case 'L':
        	luz = !luz; 
        	break;
       	case '1':
            camType = 1;
            cam.x = 0;
		    cam.y = 250; //altura camera
		    cam.z = 10;
		    camAux.x = cam.x;
		    camAux.y = 250;
		    camAux.z = 140;
            break;
        case '2':
        	camType = 2;
            cam.x = -750;
		    cam.y = 120; //altura camera
		    cam.z = 900;
		    focoCamera.x = 850;
		    focoCamera.y = 750;
		    focoCamera.z = -350;
        	break;
        case '3':
        	camType = 2;
            cam.x = -60;
		    cam.y = 20; //altura camera
		    cam.z = -385;
		    focoCamera.x = -57;
		    focoCamera.y = 35;
		    focoCamera.z = -370;
        	break;
    }
}

void inicializa(){
	glClearColor(135.0f/255.0f,206.0f/255.0f,235.0f/255.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    cam.x = 0;
    cam.y = 250; //altura camera
    cam.z = 10;
    camAux.x = cam.x;
    camAux.y = 250;
    camAux.z = 140;

    //CHAO
    for(i = 0; i < 3; i++){
    	chao.ambiente[i] = 0;
    	chao.difusa[i] = .5;
    	chao.especular[i] = .0;
    	chao.emissiva[i] = .0;	
    } 
    chao.ambiente[1] = .2;
    chao.difusa[1] = .6;
    chao.especular[1] = .1;
    chao.ambiente[3] = chao.difusa[3] = chao.especular[3] = chao.emissiva[3] = 1;
    chao.brilho[0] = 0;


    texturaPiso = SOIL_load_OGL_texture("images/chao.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
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
    glutIdleFunc(desenhaCena);
    glutTimerFunc(25, atualiza, 0);
    glutKeyboardFunc(teclado);
    glewInit();
    inicializa();
    glutMainLoop();
    return 0;
}