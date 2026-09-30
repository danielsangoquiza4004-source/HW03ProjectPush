#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include <list>
using namespace std;


#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80
// podemos hacer la llamada a estas definiciones dentro de la función principal main y  cuando las llamemos se estará hablando de un número basados en el código ASCII

void caracteres(int x, int y){
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;	
	SetConsoleCursorPosition(hCon, dwPos); 
}
void OcultarCursor(){ // funcion para que el cursor no titile
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 50; // controla el tamaño del cursor 
	cci.bVisible = FALSE; // controla la visibilidad
	
	SetConsoleCursorInfo(hCon,&cci);
}

void pintar_limites(){ //función límites

	for(int i=2 ; i<78; i++){
		caracteres(i,3);printf("%c",205);
		caracteres(i,33);printf("%c",205);
	}
	
	for (int i =4; i<33 ; i++){
		caracteres(2,i); printf("%c",186);
		caracteres(77,i); printf("%c",186);
	}
	caracteres(2,3);printf("%c",201);
	caracteres(2,33);printf("%c",200);
	caracteres(77,3);printf("%c",187);
	caracteres(77,33);printf("%c",188);
};
class NAVE{ //atributos de la nave
	int x,y;
	int corazones;
	int vidas;
public: // permite que todo lo que se escriba abajo, sea accesible para todo el programa 
	NAVE(int _x, int _y, int _corazones, int _vidas): x(_x),y(_y),corazones(_corazones), vidas (_vidas) {}
	int X() { return x; }
	int Y() { return y; }
	int VID(){ return vidas;}
	void COR() {corazones--;}
	void pintar();
	void borrar();
	void mover();
	void pintar_corazones();
	void morir();
};



void NAVE::pintar(){ // este operador :: nos permite acceder a los métodos de la clase 
	caracteres(x,y); printf("  %c",30);
	caracteres(x,y+1); printf(" %c%c%c",40,207,41);
	caracteres(x,y+2); printf("%c%c %c%c",30,190,190,30);
}
   
void NAVE::borrar(){
	caracteres(x,y); printf("        ");
	caracteres(x,y+1);printf("        ");
	caracteres(x,y+2);printf("        ");
		
}

void NAVE::mover(){
	if(kbhit( )){
			char tecla = getch();
			borrar();
			if(tecla == IZQUIERDA &&x>3) x--; //método propio de la clase 
			if(tecla == DERECHA &&x+6<77) x++;
			if(tecla == ARRIBA &&y>4) y--;
			if(tecla == ABAJO &&y+3<33) y++;
			if (tecla == 'e') corazones--;
			//tenemos variables de tipo char y le igualamos a un número entero, hace referencia al codigo ASCII
			pintar();
			pintar_corazones();
}

}
void NAVE::pintar_corazones(){
	caracteres(50,2); printf("VIDAS %d", vidas);
	caracteres(64,2); printf("Salud");
	caracteres(70,2); printf("      ");
	for(int i=0 ; i < corazones; i++){
		
		caracteres (70+i,2); printf("%c",3);
	}
}

void NAVE::morir(){
	if(corazones == 0){
		borrar();
		caracteres(x,y); printf("   **   ");
		caracteres(x,y+1); printf("  ****  ");
		caracteres(x,y+2); printf("  **  ");
		
		Sleep(200); //da tienmpo para que suceda 
		
		borrar();
		caracteres(x,y); printf(" * ** * ");
		caracteres(x,y+1); printf("  ****  ");
		caracteres(x,y+2); printf(" * ** *");
		Sleep (200);
		
		borrar();
		vidas--;
		corazones = 3;
		pintar_corazones();
		pintar();
	}
}

class AST {
	int x,y;
public:
	AST(int _x, int _y):x(_x),y(_y){}
	void pintar ();
	void mover ();
	void choque(class NAVE &A);
	int X() { return x; }
	int Y() {return y; }
};

void AST::pintar(){
	caracteres(x,y); printf("%c",184);
}
void AST::mover(){
	caracteres(x,y); printf(" ");
	y++;
	if (y>32){
		x = rand()%71 + 4; // funcion da un número al azar entre 0 y 71
		y = 4;
		
	}
	pintar();
}

void AST::choque(class NAVE &A){
	if (x >= A.X() && x < A.X() +6 && y >= A.Y() && y <= A.Y()+2)
	{
		A.COR();
		A.borrar();
		A.pintar();
		A.pintar_corazones();
		x = rand()%71 + 4; 
		y = 4;
		
	}
}

class BALA{
	int x,y;
public:
	BALA(int _x, int _y): x(_x), y(_y){}
	int X() { return x; }
	int Y() { return y;}
	void mover();
	bool fuera();
};

void BALA::mover(){
	caracteres(x,y); printf(" ");
	y--;
	caracteres(x,y); printf("*");
	
}

bool BALA::fuera(){
	if(y==4) return true;
	return false;
}
int main(){

	OcultarCursor();
	pintar_limites();
	NAVE A(37,30,3,3);
	A.pintar(); // lo podemos llamar fuera de la calse dembido a que es pública 
	A.pintar_corazones();

	
	list<AST*> N;
	list<AST*>::iterator itN;
	for(int i=0; i<5; i++){
		N.push_back(new AST(rand()%75 + 3, rand()%5 +4));
	}
	
	list<BALA*>  B;
	list<BALA*>::iterator it;
	
	bool game_over = false;
	int puntos = 0;
	while (!game_over){
		
		caracteres(4,2); printf ("Puntos %d", puntos );
		
		if(kbhit())//kbhit detecta una tecla 
		{
			
			char tecla = getch();
			if(tecla == 'a')
			B.push_back(new BALA(A.X() +2 , A.Y() -1));
			
		}
		for (it = B.begin(); it !=B.end() ; it++)
		{
			(*it)->mover();
			if ((*it)->fuera()){
					caracteres((*it)->X(), (*it)->Y()); printf(" ");
					delete (*it);
					it = B.erase(it);
				
			}
			
		}
		
		for(itN = N.begin(); itN != N.end(); itN++){
			
			(*itN)->mover();
			(*itN)->choque(A);
		}
		for(itN = N.begin(); itN != N.end(); itN++){ //recorre los asteroides
			
			for(it = B.begin(); it !=B.end(); it++){  // recorre las balas 
				if((*itN)->X() == (*it)->X() && ( (*itN)->Y()+1 == (*it)->Y() ||  (*itN)->Y() == (*it)->Y()  ))
				{
					caracteres ((*it)->X(), (*it)->Y()); printf(" ");
					
					delete(*it);
					it=B.erase(it);
					
					N.push_back(new AST(rand()%74 + 3, 4));
					caracteres((*itN)->X(), (*itN)->Y()); printf(" ");
				
					delete (*itN);
					itN = N.erase(itN);
					
					puntos+=5;
				}
				
		}
	}
		if(A.VID() == 0) game_over = true;
		A.morir();
		A.mover();
	 	Sleep (30);
		}
	return 0;
	
}
	

