#include <Keypad.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <Wire.h>
#include "RTClib.h"

const byte filas = 4;
const byte columnas = 4;
char teclas[filas][columnas] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
///byte pinesfilas[filas]={7,6,5,4};
//byte pinescolumnas[columnas]={3,2,1,0};
byte pinesfilas[filas] = {22, 24, 26, 28};
byte pinescolumnas[columnas] = {30, 32, 34, 36};

LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
RTC_DS1307 rtc;
Keypad teclado = Keypad(makeKeymap(teclas), pinesfilas, pinescolumnas, filas, columnas);


char dias[7][12] = {"Domingo" , "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"};
int rele1 = 1;
int rele2 = 2;
char tecla;
int Aenchu;
int Adia;
int i;
int alarma[4];
int horasini, minutosini;
int horasfin, minutosfin;
int dir;
int flag1=201;
int flag2=202;
int dact;
int hora, minuto;

void setup()
{
  lcd.begin(20, 4);
  lcd.clear();
  if (! rtc.begin())
  {
    lcd.print("No se encontro RTC");
    while (1);
  }


  if (! rtc.isrunning())
  {
    lcd.print("El RTC no está corriendo");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  pinMode(rele1, OUTPUT);
  pinMode(rele2, OUTPUT);

  digitalWrite(rele1,!(EEPROM.read(flag1)));
  digitalWrite(rele2,!(EEPROM.read(flag2)));

}

void loop()
{
  inicio();
  checar();
  char tec = teclado.getKey();
  if (tec != NO_KEY)
  {
    switch (tec)
    {
      case 'A':
        poner();
        break;

      case 'B':
        borrar();
        break;

      case 'C':
        ver();
        break;

      case 'D':
        Btodo();
        break;
    }
  }



}

void inicio()
{
  DateTime reloj = rtc.now();
  lcd.setCursor(2, 0);
  lcd.print(dias[reloj.dayOfTheWeek()]);
  lcd.print(" ");
  lcd.print(reloj.day());
  lcd.print('/');
  lcd.print(reloj.month());
  lcd.print('/');
  lcd.print(reloj.year());
  lcd.setCursor(5, 1);
  //lcd.print("Hora:");
  lcd.print(reloj.hour());
  lcd.print(':');
  if (reloj.minute() < 10)
  {
    lcd.print("0");
  }
  lcd.print(reloj.minute());
  lcd.print(':');
  if (reloj.second() < 10)
  {
    lcd.print("0");
  }
  lcd.print(reloj.second());

  lcd.setCursor(0, 2);
  lcd.print("ENCHUFE 1: ");
  if (EEPROM.read(flag1) == 0)
  {
    lcd.print("OFF");
    digitalWrite(rele1,HIGH);
  }
  else
  {
    lcd.print("ON ");
    digitalWrite(rele1,LOW);
  }
  lcd.setCursor(0, 3);
  lcd.print("ENCHUFE 2: ");
  if (EEPROM.read(flag2) == 0)
  {
    lcd.print("OFF");
    digitalWrite(rele2,HIGH);
  }
  else
  {
    lcd.print("ON ");
    digitalWrite(rele2,LOW);
  }


}
void checar()
{
  DateTime check = rtc.now();
  dact = check.dayOfTheWeek();
  direccion(1, dact);
  hora = EEPROM.read(dir + 2);
  minuto = EEPROM.read(dir + 3);
  /*lcd.setCursor(18, 2);
    lcd.print(dir);
    lcd.setCursor(0, 2);
    lcd.print(hora);
    lcd.print(minuto);
  */
  if (hora == check.hour() && minuto == check.minute())
  {

    EEPROM.write(flag1,1);
    digitalWrite(rele1, LOW);
    //lcd.setCursor(17, 2);
    //lcd.print(flag1);
  }
  direccion(2, dact);
  hora = EEPROM.read(dir + 2);
  minuto = EEPROM.read(dir + 3);
  /*lcd.setCursor(18, 3);
    lcd.print(dir);

    lcd.setCursor(0, 3);
    lcd.print(hora);
    lcd.print(minuto);
  */
  if (hora == check.hour() && minuto == check.minute())
  {
    EEPROM.write(flag2,1);
    digitalWrite(rele2, LOW);
    //lcd.setCursor(17, 3);
    //lcd.print(flag2);
  }

  direccion(1, dact);
  hora = EEPROM.read(dir + 4);
  minuto = EEPROM.read(dir + 5);

  if (hora == check.hour() && minuto == check.minute())
  {
    EEPROM.write(flag1,0);
    digitalWrite(rele1, HIGH);
  }
  
  direccion(2, dact);
  hora = EEPROM.read(dir + 4);
  minuto = EEPROM.read(dir + 5);

  if (hora == check.hour() && minuto == check.minute())
  {
    EEPROM.write(flag2,0);
    digitalWrite(rele1, HIGH);
  }

  

}

void poner()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Seleccione enchufe");
  tecla = teclado.getKey();
  do
  {
    tecla = teclado.getKey();
  } while (tecla != '1' && tecla != '2');
  Aenchu = tecla - 48;
  lcd.clear();
  lcd.print(Aenchu);
  lcd.setCursor(2, 0);
  lcd.print("Seleccione el dia:");
  lcd.setCursor(0, 1);
  lcd.print("0:Dom | 1:Lun | 2:Ma");
  lcd.setCursor(0, 2);
  lcd.print("3:Mie | 4:Jue | 5:Vi");
  lcd.setCursor(0, 3);
  lcd.print("6:Sab");
  tecla = NO_KEY;
  tecla = teclado.getKey();
  do
  {
    tecla = teclado.getKey();
  } while (tecla != '1' && tecla != '2' && tecla != '3' && tecla != '4' && tecla != '5' && tecla != '6' && tecla != '0');
  Adia = tecla - 48;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enchufe: ");
  lcd.print(Aenchu);
  lcd.setCursor(0, 1);
  lcd.print("Dia: ");
  lcd.print(dias[Adia]);
  lcd.setCursor(0, 2);
  lcd.print("Hora Inicio: ");
  lcd.setCursor(13, 2);

  ingresar();
  horasini = ((alarma[0] * 10) + alarma[1]);
  minutosini = ((alarma[2] * 10) + alarma[3]);

  lcd.setCursor(0, 3);
  lcd.print("Hora Fin: ");
  lcd.setCursor(13, 3);

  ingresar();
  horasfin = ((alarma[0] * 10) + alarma[1]);
  minutosfin = ((alarma[2] * 10) + alarma[3]);

  direccion(Aenchu, Adia);
  EEPROM.write(dir, Aenchu);
  EEPROM.write((dir + 1), Adia);
  EEPROM.write((dir + 2), horasini);
  EEPROM.write((dir + 3), minutosini);
  EEPROM.write((dir + 4), horasfin);
  EEPROM.write((dir + 5), minutosfin);


  lcd.clear();
  /*  lcd.setCursor(7,2);
    lcd.print(horas);
    lcd.print(":");
    lcd.print(minutos);
  */

}

void ingresar()
{
  tecla = NO_KEY;
  tecla = teclado.getKey();
  do
  {
    tecla = teclado.getKey();
  } while (tecla != '1' && tecla != '2' && tecla != '0');
  alarma[0] = tecla - 48;
  lcd.print(alarma[0]);

  tecla = NO_KEY;
  if (alarma[0] == 2)
  {
    tecla = teclado.getKey();
    do
    {
      tecla = teclado.getKey();
    } while (tecla != '1' && tecla != '2' && tecla != '3' && tecla != '4' && tecla != '0');
  }
  else
  {
    tecla = teclado.getKey();
    do
    {
      tecla = teclado.getKey();
    } while (tecla != '1' && tecla != '2' && tecla != '3' && tecla != '4' && tecla != '5' && tecla != '6' && tecla != '7' && tecla != '8' && tecla != '9' && tecla != '0');
  }
  alarma[1] = tecla - 48;
  lcd.print(alarma[1]);
  lcd.print(":");

  tecla = NO_KEY;
  tecla = teclado.getKey();
  do
  {
    tecla = teclado.getKey();
  } while (tecla != '1' && tecla != '2' && tecla != '3' && tecla != '4' && tecla != '5' && tecla != '0');
  alarma[2] = tecla - 48;
  lcd.print(alarma[2]);

  tecla = NO_KEY;
  tecla = teclado.getKey();
  do
  {
    tecla = teclado.getKey();
  } while (tecla != '1' && tecla != '2' && tecla != '3' && tecla != '4' && tecla != '5' && tecla != '6' && tecla != '7' && tecla != '8' && tecla != '9' && tecla != '0');
  alarma[3] = tecla - 48;
  lcd.print(alarma[3]);
}

void direccion(int enchufe, int diaAla)
{
  dir = 0;
  switch (enchufe)
  {
    case 1:
      dir = 0;
      break;

    case 2:
      dir = 8;
      break;

  }
  switch (diaAla)
  {
    case 1:
      dir += 16;
      break;

    case 2:
      dir += 32;
      break;

    case 3:
      dir += 48;
      break;

    case 4:
      dir += 64;
      break;

    case 5:
      dir += 80;
      break;

    case 6:
      dir += 96;
      break;
  }
}

void borrar()
{
  int Benchu, Bdia;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("BORRAR:");
  lcd.setCursor(0, 1);
  lcd.print("Ingese datos:");
  lcd.setCursor(0, 2);
  lcd.print("Enchufe :");
  tecla = NO_KEY;
  tecla = teclado.getKey();
  do
  {
    tecla = teclado.getKey();
  } while (tecla != '1' && tecla != '2');
  Benchu = tecla - 48;
  lcd.print(Benchu);

  lcd.setCursor(0, 3);
  lcd.print("Dia: ");
  tecla = NO_KEY;
  tecla = teclado.getKey();
  do
  {
    tecla = teclado.getKey();
  } while (tecla != '1' && tecla != '2' && tecla != '3' && tecla != '4' && tecla != '5' && tecla != '6'  && tecla != '0');
  Bdia = tecla - 48;
  lcd.print(Bdia);

  direccion(Benchu, Bdia);
  EEPROM.write(dir, 255);
  EEPROM.write((dir + 1), 255);
  EEPROM.write((dir + 2), 255);
  EEPROM.write((dir + 3), 255);
  EEPROM.write((dir + 4), 255);
  EEPROM.write((dir + 5), 255);

  lcd.clear();
  lcd.setCursor(8, 1);
  lcd.print("ALARMA");
  lcd.setCursor(8, 2);
  lcd.print("BORRADA");
  if(EEPROM.read(dir)==1)
    {
      EEPROM.write(flag1,0);
    }
     else
    {
      EEPROM.write(flag2,0);
    }
  delay(2000);
  lcd.clear();
}

void ver()
{
  int Venchu, Vdia;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Buscar:");
  lcd.setCursor(0, 1);
  lcd.print("Ingese datos:");
  lcd.setCursor(0, 2);
  lcd.print("Enchufe :");
  tecla = NO_KEY;
  tecla = teclado.getKey();
  do
  {
    tecla = teclado.getKey();
  } while (tecla != '1' && tecla != '2');
  Venchu = tecla - 48;
  lcd.print(Venchu);

  lcd.setCursor(0, 3);
  lcd.print("Dia: ");
  tecla = NO_KEY;
  tecla = teclado.getKey();
  do
  {
    tecla = teclado.getKey();
  } while (tecla != '1' && tecla != '2' && tecla != '3' && tecla != '4' && tecla != '5' && tecla != '6'  && tecla != '0');
  Vdia = tecla - 48;
  lcd.print(Vdia);

  direccion(Venchu, Vdia);
  if (EEPROM.read(dir) == 1 || EEPROM.read(dir) == 2)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enchufe: ");
    lcd.print(EEPROM.read(dir));
    lcd.setCursor(0, 1);
    lcd.print("Dia: ");
    lcd.print(dias[EEPROM.read(dir + 1)]);
    lcd.setCursor(0, 2);
    lcd.print("Hora Inicio: ");
    lcd.setCursor(13, 2);
    lcd.print(EEPROM.read(dir + 2));
    lcd.print(":");
    lcd.print(EEPROM.read(dir + 3));
    lcd.setCursor(0, 3);
    lcd.print("Hora Fin: ");
    lcd.setCursor(13, 3);
    lcd.print(EEPROM.read(dir + 4));
    lcd.print(":");
    lcd.print(EEPROM.read(dir + 5));

    
  }
  else
  {
    lcd.clear();
    lcd.clear();
    lcd.setCursor(8, 1);
    lcd.print("ALARMA");
    lcd.setCursor(10, 2);
    lcd.print("NO");
    lcd.setCursor(6, 3);
    lcd.print("ENCONTRADA");
  }
  delay(3000);
  lcd.clear();
  

}
void Btodo()
{
  for (int i = 0; i < 200; i++)
  {
    EEPROM.write(i, 255);
  }
  lcd.clear();
  lcd.setCursor(8, 1);
  lcd.print("ALARMAS");
  lcd.setCursor(8, 2);
  lcd.print("BORRADAS");
  delay(2000);
  lcd.clear();
  EEPROM.write(flag1,0);
  EEPROM.write(flag2,0);
}

