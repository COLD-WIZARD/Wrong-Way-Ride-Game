//Libreria Para Pantallas LCD
#include <LiquidCrystal.h>

//Constantes De Los Botones
#define NONE    0
#define RIGHT   1
#define UP      2
#define DOWN    3
#define LEFT    4
#define SELECT  5

//Constantes De Jugador Y Enemigos
#define PLAYER '}'
#define ENEMY  '{'
#define ENEMIES 4

//Variables De Estado
int PlayerRow     = 0;
int PlayerColumn  = 0;
int EnemySpeed    = 250;
int Enemies[ENEMIES][2];

//Pines Del Modulo LCD
/*
Pin A0 = Botones
Pin 4 = DB4
Pin 5 = DB5
Pin 6 = DB6
Pin 7 = DB7
Pin 8 = RS
Pin 9 = Enable
Pin 10 = Luz De Fondo
*/
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

//Inicializa La LCD, Le Pasa Como Parametros La Cantidad De Caracteres Y Lineas Que Soporta La LCD (16 Caracteres, 2 Lineas)
void setup()
{
  lcd.begin(16, 2);
  Serial.begin(9600);
}

void loop()
{
  //Menu De Presentacion
  while(ReadButtons() != SELECT)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" WRONG WAY RIDE");
    lcd.setCursor(0, 1);
    lcd.print("EDGAR & ROOSVELT");
    Wait();
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  PULSE SELECT");
    lcd.setCursor(0, 1);
    lcd.print("  PARA INICIAR  ");
    Wait();
  }
}

//Realiza Una Pausa De 1.5 Segundos, Y Escanea Los Botones Cada 0.1 Segundo Para Ver Si Se Pulsa El Boton Select
void Wait()
{
  for(int i = 0; i < 15; i++)
  {
    delay(100);
    if(ReadButtons() == SELECT)
    {
      //Inicia El Juego
      StartGame();
      break;
    }
  }
}

//Lee Y Devuelve El Valor Del Boton Que Se Presiono
int ReadButtons()
{
  int Pressed = analogRead(0);
  
  if(Pressed < 50)
  {  return RIGHT;  }
  else if(Pressed < 250)
  {  return UP;  }
  else if(Pressed < 450)
  {  return DOWN;  }
  else if(Pressed < 650)
  {  return LEFT;  }
  else if(Pressed < 850)
  {  return SELECT;  }
  
  return NONE;
}

//Dibuja Los Caracteres En La Pantalla
void Draw()
{
  lcd.clear();
  
  for(int i = 0; i < ENEMIES; i++)
  {
    //Detecta Choque De Jugador Y Enemigo, Si Lo Encuentra, Detiene El Juego
    if(Enemies[i][0] == PlayerColumn && Enemies[i][1] == PlayerRow)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("JUEGO TERMINADO!");
      lcd.setCursor(0, 1);
      lcd.print("  PULSE RESET!");
      while(true);
    }
    
    if(Enemies[i][0] < 16)
    {
      //Dibuja Al Enemigo
      lcd.setCursor(Enemies[i][0], Enemies[i][1]);
      lcd.print(ENEMY);
    }
  }
  
  //Dibuja Al Jugador
  lcd.setCursor(PlayerColumn, PlayerRow);
  lcd.print(PLAYER);
}

void GenerateEnemies()
{
  for(int i = 0; i < ENEMIES; i++)
  {
    Enemies[i][1] = random(2);
    Enemies[i][0] = (15 + (i * (i + 1)) / 2);
  }
}

void MoveEnemies()
{
  for(int i = 0; i < ENEMIES; i++)
  {
    if(Enemies[i][0] - 1 >= 0)
    {
      Enemies[i][0] = Enemies[i][0] - 1;
    }
    else
    {
      Enemies[i][0] = (15 + (i * (i + 1)) / 2);
      Enemies[i][1] = random(2);
    }
  }
}

//Inicio Del Juego
void StartGame()
{
  GenerateEnemies();
  
  int mov = 0;
  PlayerColumn = 0;
  PlayerRow = 1;
  
  while(true)
  {
    Draw();
    
    //Lee Los Botones Y Ejecuta La Accion Correspondiente
    switch(ReadButtons())
    {
      //Pausa El Juego
      case SELECT:
      {
        break;
      }
      //Si El Jugador No Esta En La Posicion X = 0, Desplaza El Caracter Un Espacio A La Izquierda
      case LEFT:
      {
        PlayerColumn = (PlayerColumn > 0)? (PlayerColumn - 1) : PlayerColumn;
        break;
      }
      //Si El Jugador No Esta En La Posicion Y = 1, Lo Desplaza A Esa Posicion
      case DOWN:
      {
        PlayerRow = 1;
        break;
      }
      //Si El Jugador No Esta En La Posicion Y = 0, Lo Desplaza A Esa Posicion
      case UP:
      {
        PlayerRow = 0;
        break;
      }
      //Si El Jugador No Esta En La Posicion X = 8, Desplaza El Caracter Un Espacio A La Derecha
      case RIGHT:
      {
        PlayerColumn = (PlayerColumn < 8)? (PlayerColumn + 1) : PlayerColumn;
        break;
      }
      //No Hace Nada
      case NONE:
      {
        break;
      }
    }
    
    delay(50);
    mov += 50;
    
    if(mov >= EnemySpeed)
    {
      mov = 0;
      MoveEnemies();
    }
  }
}
