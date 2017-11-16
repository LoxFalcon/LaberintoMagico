#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <vector>
using namespace std;

int dr[] = {1, 0, -1, 0};
int dc[] = {0, 1, 0, -1};

GLfloat posObjeto = -5.0f;
GLfloat anguloCamaraY = 0.0f;
GLfloat anguloCamaraX = 0.0f;
GLfloat camX,camY,camZ;
GLfloat deltay = 4;

int xx[20],yy[20];
int fin=0;
float initX, initZ; //(initX,initZ) esquina superior izquierda de todo el mapa
int posX, posZ; //(posX,posZ) posición en la matriz actual del jugador
float xActual,zActual;
float rotx=0,roty=0;
int tamCubo=20,tamSphere=tamCubo/2;
const int altoMapa=8,largoMapa=7;
// 0 = espacio
// 1 = pared
// 2 = jugador
// 3 = destino
int mapa[altoMapa][largoMapa]={   1,1,1,1,1,1,1,
						 1,0,0,0,3,1,1,
						 1,0,0,0,0,1,1,
						 1,0,0,0,0,1,1,
						 1,2,0,0,0,1,1,
						 1,1,1,1,1,1,1,
						 1,1,1,1,1,1,1,
						 1,1,1,1,1,1,1,};

int cubos[altoMapa][largoMapa][2];

//Esta función carga de la matriz inicial las coordenadas de todos los cubos
//con el fin de optimizar el proceso de dibujo
void parseCubos(){
    //(initX,initZ) es la esquina superior izquierda de todo el mapa
    initX = -(tamCubo*largoMapa)/2.0;
    initZ = -(tamCubo*altoMapa)/2.0;
    //cubos = new int[altoMapa][largoMapa][2]; // 2 coordenadas por cubo (esquina superior izquierda)
    for(int i=0;i<altoMapa;i++){
        for(int j=0;j<largoMapa;j++){
            if(mapa[i][j]==1 ||  mapa[i][j] == 3){
                cubos[i][j][0] = initX+(tamCubo*j);
                cubos[i][j][1] = initZ+(tamCubo*i);
            }else if(mapa[i][j]==2){
                posZ=i;
                posX=j;
                xActual = initX+(tamCubo*j);
                zActual = initZ+(tamCubo*i);
            }
        }
    }

}
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //int maxi = (altoMapa>largoMapa) ? altoMapa : largoMapa;
    gluPerspective(90.0,width/height,10,300);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

bool esValido(int X, int Z) {
    return X>0 && X<largoMapa && Z>0 && Z<altoMapa;
}
void crearCamino(int a,int b, int pos){
	//printf("%d %d\n",a,b);
	if (!esValido(a, b)) {
            return;
        }
        if (mapa[a][b] == 3) {
            xx[pos]=b;
            yy[pos]=a;
            xx[pos+1]=-1;
            yy[pos+1]=-1;
            fin =1;
            return;
        }
        if (mapa[a][b] != 0) {
            return ;
        }
        mapa[a][b] = 1;
        for (int d = 0; d < 4; d++) {
			xx[pos]=b;
            yy[pos]=a;
            crearCamino(a + dr[d], b + dc[d],pos+1);
            if(fin) return;
        }
        return ;
}


void dibujarMapa(){
	//float x,y;

	for(int i=0;i<altoMapa;i++){
        for(int j=0;j<largoMapa;j++){
            switch(mapa[i][j]){
                case 1:
                    glPushMatrix();
                    glColor3f(0,1,0);
                    glTranslatef(cubos[i][j][0],0,cubos[i][j][1]);
                    glutSolidCube(tamCubo);
                    glPopMatrix();
                    glPushMatrix();
                    glColor3f(0,0,0);
                    glTranslatef(cubos[i][j][0],0,cubos[i][j][1]);
                    glutWireCube(tamCubo);
                    glPopMatrix();
                break;

                case 3:
                    glPushMatrix();
                    glColor3f(0,0,1);
                    glTranslatef(cubos[i][j][0],0,cubos[i][j][1]);
                    glutSolidCube(tamCubo);
                    glPopMatrix();
                    glPushMatrix();
                    glColor3f(0,0,0);
                    glTranslatef(cubos[i][j][0],0,cubos[i][j][1]);
                    glutWireCube(tamCubo);
                    glPopMatrix();
                break;
            }
        }
    }
        glPushMatrix();
		glColor3f(1,0,0);
		glTranslatef(xActual,0,zActual);
		glRotatef(rotx,1,0,0);
		glRotatef(roty,0,1,0);
		glutWireSphere(tamSphere,tamSphere,tamSphere);
		glPopMatrix();
		glPopMatrix();
}


// función que muestra por pantalla la escena.
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(camX,camY,camZ,0,0,0,camX,camY+1,camZ);
	dibujarMapa();
    glutSwapBuffers();
}
void init() {
    glClearColor(0,0,0,0);
    glEnable(GL_DEPTH_TEST);
    camX = 0.0f,camY = 60.0f, camZ=120.0f;
}

// función que permite interactuar con la escena mediante el teclado
void keyboard(unsigned char key, int x, int y){
    float t;
    switch(key) {
        case 'w':
        case 'W': //Arriba
            printf("1!");
            if(esValido(posX,posZ-1) && mapa[posZ-1][posX]!=1){
                    printf("2!");
                for (int i = 0; i < tamCubo; i++)
                {
                    zActual-=1;
                    rotx+=4.5;
                    display();
                }
                posZ--;
            }
            break;
        case 's':
        case 'S': //Abajo
            if(esValido(posX,posZ+1) && mapa[posZ+1][posX]!=1){
                for (int i = 0; i < tamCubo; i++)
                {
                    zActual+=1;
                    rotx-=4.5;
                    display();
                }
                posZ++;
            }
            break;
        case 'a':
        case 'A': //Izquierda
           if(esValido(posX-1,posZ) && mapa[posZ][posX-1]!=1){
                for (int i = 0; i < tamCubo; i++)
                {
                    xActual-=1;
                    roty-=4.5;
                    display();
                }
                posX--;
            }
            break;
        case 'd':
        case 'D': //Derecha
        if(esValido(posX+1,posZ) && mapa[posZ][posX+1]!=1){
			for (int i = 0; i < tamCubo; i++)
			{
                xActual+=1;
                roty+=4.5;
                display();
			}
			posX++;
		}
        break;
// ROTAR CAMARA LA IZQUIERDA
    case '1':
        t=camX;
		camX=t*cos(deltay*3.1416/180)-camZ*sin(deltay*3.1416/180);
		camZ=camZ*cos(deltay*3.1416/180)+t*sin(deltay*3.1416/180);
		display();
        break;
// ROTAR CAMARA LA DERECHA
    case '2':
        t=camX;
		camX=t*cos(-deltay*3.1416/180)-camZ*sin(-deltay*3.1416/180);
		camZ=camZ*cos(-deltay*3.1416/180)+t*sin(-deltay*3.1416/180);
		display();
        break;
// ROTAR CAMARA HACIA ARRIBA
    case '3':
        //anguloCamaraX++;
        camY+=1;
        //printf("Angulo de rotacion de la camara en torno al eje X: %i \n",((int)anguloCamaraX % 360));
        display();
        break;
// ROTAR CAMARA HACIA ABAJO
    case '4':
        //anguloCamaraX--;
        camY-=1;
        //printf("Angulo de rotacion de la camara en torno al eje X: %i \n",((int)anguloCamaraX % 360));
        display();
        break;
    case 27:
        exit(0);
        break;
    }
}

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Laberinto Magico");
    parseCubos();
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
