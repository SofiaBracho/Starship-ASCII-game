#include<iostream>
#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<stdlib.h>
#include<list>
#include<time.h>
#include<fstream>

using namespace std;

#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80

#define ARRIBA2 119
#define IZQUIERDA2 97
#define DERECHA2 100
#define ABAJO2 115

#define ESPACIO 32

int fin_del_juego=3;

void menu_inicio();
void juego();
void multijugador();
void creditos();

// AQUI INICIAN LAS BASES

void gotoxy(int x, int y){
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = x;
	dwPos .Y = y;
	
	SetConsoleCursorPosition(hCon, dwPos);
	
}

void ocultarCursor(){
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 50;
	cci.bVisible = false;
	
	
	SetConsoleCursorInfo(hCon,&cci);
}

void pintar_limites(){
	for(int i=2; i<79; i++){
		gotoxy(i,3);   printf("%c",205);
		gotoxy(i, 33); printf("%c",205);
	}
	
	for(int i=4; i<33; i++){
		gotoxy(2,i);   printf("%c",186);
		gotoxy(78,i);   printf("%c",186);
	}
	
	gotoxy(2,3);   printf("%c",201);
	gotoxy(2,33);  printf("%c",200);
	gotoxy(78,3);  printf("%c",187);
	gotoxy(78,33); printf("%c",188);
	
}


//     AQUI INICIAN LAS  NAVES

class NAVE{
	   int x,y;
	   int corazones;
	   int vidas;
	  
	public:
		
	NAVE(int _x, int _y, int _corazones,int _vidas): x(_x),y(_y), corazones(_corazones),vidas(_vidas){}
	void mover();
	void borrar();
	void pintar();
	void COR(){ corazones--; };
	void Vidas(int vid){ vidas=vid; }
	void Corazones(int cor){ corazones=cor; }
	void morir();
	void pintar_corazones();
	int X(){ return x; }
	int Y(){ return y; }
	
};


void NAVE::pintar(){
	gotoxy(x,y);   printf("  %c",65);
	gotoxy(x,y+1); printf(" %c%c%c",47,207,92);
	gotoxy(x,y+2); printf("%c%c %c%c",65,86,86,65);
	
}

void NAVE::borrar(){
	gotoxy(x,y);   printf("       ");
	gotoxy(x,y+1); printf("       ");
	gotoxy(x,y+2); printf("       ");
}

void NAVE::mover(){   //AQUI EMPIEZA MOVER
	
	if(kbhit()){
		char tecla = getch();
	
		borrar();
		
		if(tecla == IZQUIERDA && x>3){x--;}
		if(tecla == DERECHA && x+5 < 78){x++;}
		if(tecla == ARRIBA && y > 4){y--;}
		if(tecla == ABAJO && y+3 <33){y++;}
        
	}
	
	borrar();
	pintar();
	pintar_corazones();
}

//       	AQUI PINTAS LAS VIDAS

void NAVE::pintar_corazones(){
	
	
	gotoxy(50,0); printf("vidas %i",vidas);
	gotoxy(64,0); printf("salud");
	gotoxy(70,0); printf("      ");
	for(int i=0 ; i< corazones; i++){
		
		gotoxy(70+i,0); printf("%c",3);
    }
		
}

void NAVE::morir(){           //AQUI EMPIEZA MORIR
	if (corazones==0){
		
		borrar();
		gotoxy(x,y); printf  ("  **  ");
		gotoxy(x,y+1); printf(" **** ");
		gotoxy(x,y+2); printf("  **  ");
		Sleep(200);
		
		borrar();
		gotoxy(x,y); printf  ("* ** *");
		gotoxy(x,y+1); printf(" **** ");
		gotoxy(x,y+2); printf("* ** *");
		Sleep(200);
		borrar();
		
		if(vidas>0){
			vidas--;
			corazones=3;
		}
		else{
			corazones=0;
		}
		
		pintar_corazones();
		pintar();
		
		fin_del_juego=vidas;
		
	}
	
}


class AST{               //AQUI INICIAN LOS ASTEROIDES
	int x,y;
	bool b;
public:
	AST(int _x, int _y): x(_x),y(_y){ b=true; }
	void pintar();
	void mover(clock_t &t);
	void choque(class NAVE &N);
	int X() { return x;}
	int Y() { return y;}
	
};

void AST::pintar(){
	gotoxy(x,y); printf("*");
}

void AST::mover(clock_t &t){  
	
	if(b==true){
		t=clock();
		b = false;
	}
	else{
		t=clock()-t;
		
		if(t >= 33){
		
			gotoxy(x,y); printf(" ");
			y++;
			if (y> 32){
			x = rand()%71 + 4;
			y = 4;
			}
			pintar();
		
			b = true;
		
		}
	}
	
}

void AST::choque(class NAVE &N){
	if( x >= N.X() && x <= N.X()+4 && y >= N.Y() && y <= N.Y()+2 ){
		
		N.borrar();
		N.COR();
		N.pintar();
		N.pintar_corazones();
		
		x = rand()%71 + 4;
		y = 4;
		
	}
	
}

class BALA{
	int x,y;
public:
	BALA(int _x, int _y): x(_x),y(_y){}
	void mover();
	bool limite();
	int X(){ return x; };
	int Y(){ return y; };
	void cadenciaDisparo(bool &disparo);
};

void BALA::mover(){
	gotoxy(x,y); printf(" ");
	if(y > 4) y--;
	gotoxy(x,y); printf("%c",186);
}

bool BALA::limite(){
	
	if( y==4 ) return true;
	return false;
	
}

void menu_inicio(){
	
	int cont=1;
	int opcion;
	char tecla;
	
		gotoxy(32,14); printf(">");
	
	while(opcion!=5){
		
		ocultarCursor();
		
		gotoxy(34,14); printf("Nuevo  juego");
		gotoxy(37,16); printf("Tienda");
		gotoxy(36,18); printf("Opciones");
		gotoxy(36,20); printf("Creditos");
		gotoxy(37,22); printf("Cerrar");
		
		if(kbhit){
		
			tecla = getch();
			
			if(tecla==32){
				
				opcion = cont; //segun contador ejecutar la funcion que vaya
				
				if(opcion==1){ //ejecutar juego 
				
					juego();
					opcion=5;		
				}
				else if(opcion==2){ //iniciar modo multijugador
				
					
					
				}
				else if(opcion==3){ //mostrar menu de opciones
					
				}
				else if(opcion==4){ //mostrar los creditos
				  creditos();
					
				}
				else if(opcion==5){
					return;
				}
				
			}
			else if(tecla==ABAJO){
				
				gotoxy(32,14 + cont*2-2); printf(" ");
				
				if(cont<5){
					cont++;
				}
				else{
					cont=1;
				}
				
				gotoxy(32,14 + cont*2-2); printf(">");
				
			}
			else if(tecla==ARRIBA){
				
				gotoxy(32,14 + cont*2-2); printf(" ");
				
				if(cont>1){
					cont--;
				}
				else{
					cont=5;
				}
				
				gotoxy(32,14 + cont*2-2); printf(">");
				
			}
		
		}
	}
	
}

void juego(){ //FUNCION PRINCIPAL DEL JUEGO
	
	system("cls");
	ocultarCursor();
	pintar_limites();
	
	NAVE N(38,30,3,3);
	
	N.pintar();
	N.pintar_corazones();
	
	list<AST*> A;
	list<AST*>::iterator itA;
	
	clock_t t;
	
	for(int i=0;i<5;i++){
		
		A.push_back(new AST(rand()%75+3,rand()%5+4));
		
	}
	
	list<BALA*> B;
	list<BALA*>::iterator it;
	
	bool game_over = false;
	int puntos = 0;
	 
	while(!game_over){
		
		gotoxy(4,0); printf("Puntos: %d",puntos);
		
		if(kbhit()){
			
			char tecla = getch();
			if(tecla == ESPACIO){
				
				B.push_back( new BALA( N.X()+2, N.Y()-1 ) );
			}
			if(tecla == 'm'){
				N.Vidas(0); N.Corazones(0);
			}
		}
			
		
		
		for(it = B.begin(); it != B.end(); it++ ){
			
			(*it)->mover();
			if( (*it)->limite() ){
				gotoxy( (*it)->X(), (*it)->Y() ); printf(" ");
				delete(*it);
				it = B.erase(it);
			}
			
		}
		
		for(itA = A.begin(); itA != A.end(); itA++){
			
			(*itA)->mover(t);
			(*itA)->choque(N);
		}
		
		for(itA = A.begin(); itA != A.end(); itA++){
			for(it= B.begin(); it != B.end(); it++){
				if( (*itA)->X() == (*it)->X() && ( (*itA)->Y() +1 == (*it)->Y() || (*itA)->Y() == (*it)->Y()) ){
					
					gotoxy( (*it)->X(), (*it)->Y() ); printf(" ");
					delete(*it);
					it = B.erase(it);
					
					A.push_back( new AST(rand()%75 +3, 4) );
					gotoxy( (*itA)->X(), (*itA)->Y() ); printf(" ");
					
					delete(*itA);
					itA = A.erase(itA);
					
					puntos += 5;
				}
				
			}
			
		}
		
		N.morir();
		N.mover();
		
		Sleep(25);
		
		if(fin_del_juego<=0){
			
	   		char opcion_cerrar='p';
	   		
	   		while(opcion_cerrar!='x' && opcion_cerrar!='z' && opcion_cerrar!=27){
	   			
	   			system("cls");
	       		gotoxy(30,14); printf( "       GAME OVER");
	       		gotoxy(30,16); printf( "PRESIONE ESC PARA CERRAR ");
     	   		gotoxy(30,17); printf( "PRESIONE Z PARA IR AL MENU ");
		 		gotoxy(30,18); printf( "PRESIONE X PARA REINICIAR ");
		 	
		 		opcion_cerrar = getch();
		 	
			 		if(opcion_cerrar==27){
	 	      			game_over=true;
	 	      			system("cls");
					 }
					else if(opcion_cerrar=='x' || opcion_cerrar=='X' ){
     	    			N.Corazones(3);
     	    			N.Vidas(3);
     	    			puntos=0;
     	    			fin_del_juego=3;
     	    			
     	       			system("cls");
					}	
					else if(opcion_cerrar=='z' || opcion_cerrar=='Z'){
				
						fin_del_juego=3;
     	    			N.Corazones(3);
     	    			N.Vidas(3);
     	    			puntos=0;
     	       	
     	       			system("cls");
     	       	
						menu_inicio();
					}
					
	   		}
	   			
     	}
	    
	}
	
}



//        AQUI EMPIEZAN lOS CREDITOS

void creditos(){
	system("cls");
	ocultarCursor();
	int x=30,y=3;
	
	while(y<33){
		
		gotoxy(x+5,y); printf("Todo");
		
		if(y+4 < 33){
			gotoxy(x,y+4); printf("Abrahan Bracho");
		}
    	
    	
    	if(y+8 < 33){
    		gotoxy(x+1,y+8); printf("Sofia Bracho");
    	}
    	
    	
    	y++;
    	Sleep(200);
    	
    	system("cls");
    	
	}

}


//       AQUI EMPIEZA MAIN	

int main(){
	
	menu_inicio();
	
	
	return 0;
	
}
