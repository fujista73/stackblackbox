/*

   Autor: Daniel ST
   eMail: fujista73@gmail.com
   Licencia: Indicar siempre al autor.

   StackBlackBox
   Version 9.41 * 16/09/2019 *

   Posibles direcciones I2C para pantallas 2004 0x27 y 0x3F

   SDA    Pin A4
   SCL    Pin A5

*/

/****** MODIFICAR ESTAS VARIABLES ANTES DE SUBIR A ARDUINO CON LOS VALORES CORRECTOS ***********************************/

#define SBB_V4        1       // Si es la version 4, control por hardware de M0, M1 y M2

/***********************************************************************************************************************/

#define TEST          1       // Desabilita entradas de nivel alto si no tienen resistencia pull-down
#define HAS_RELE      0       // Indica si se usan reles (1) para la salida de la camara u optoacopladores (0)

/***********************************************************************************************************************/

#define vEEPROM 111

#define J_DY  0       // Entrada analogica Y de joystick
#define J_DX  1       // Entrada analogica X de joystick
#define J_BTN 2       // Boton de Joystick - usa la entrada digital 2. Deja las otras libres.

#define M0  3         // Salida M0 para control del driver A8825 o A4988
#define M1  4         // Salida M1 para control del driver A8825 o A4988
#define M2  9         // Salida M2 para control del driver A8825 o A4988

#define DIR 5         // Direccion para el motor paso a paso
#define STEP 6        // Pasos para el motor paso a paso
#define EN  13        // Salida para habilitar el driver del motor PaP

#define CH7 7         // Salida para enfocar                - Se activan las dos a la vez
#define CH8 8         // Salida para disparar la camara     - Se activan las dos a la vez

#define IN1 10        // Entrada 1 para disparar la camara por sensor - Nivel bajo
#define IN2 11        // Entrada 2 para disparar la camara por sensor - NIVEL ALTO
#define FC 12         // Entrada para final de carrera - NIVEL ALTO

#include "EEPROM.h"
#include "LiquidCrystal_I2C.h"

// *********************************************************************
// LCDML display settings
// *********************************************************************

LiquidCrystal_I2C lcd0x27(0x27, 20, 4);   // Pantalla LCD i2c con direccion 0x27 de 20 caracteres x 4 lineas
LiquidCrystal_I2C lcd0x3F(0x3F, 20, 4);   // Pantalla LCD i2c con direccion 0x3F de 20 caracteres x 4 lineas


typedef struct struct_U {       // Defino una estructura con el modo de usuario
  unsigned int pausa, dPaso, nFotos, delayPaP, aRotacion, pvuelta;
  byte tEstab, pulsosPaso, fCiclo, lead, autoEN, precision;
  boolean sentido, tipoMov, volver, unidades;
};

struct_U modoU[4];              // Defino un array con los tres modos de usuario disponibles

char linea1[21], linea2[21], linea3[21], linea4[21];

unsigned pausa,                 // Tiempo de espera entre foto y foto, en decimas de segundo
         dPaso,                      // Distancia que avanza la camara despues de cada foto, en centesimas de milimetro
         fotosHechas,                // Numero de fotos que lleva hechas
         delayPaP,                   // Tiempo de pausa para los pulsos del motor en microsegundos
         nFotos,                     // Numero de fotos a hacer. Entero para controlar < 0
         nFotosM1, nFotosM2,         // Para guardar el num. de fotos de cada modo, M1 = normal programadas, M2 = calculadas
         totalPasos,                 // Num. total de pasos que se dan en un ciclo
         aRotacion,                  // Angulo de rotacion para movimientos rotatorios, valor * 1.8
         pvuelta;                    // Numero de pasos del motor (antes solo 200 o 400)

int cuantosPasos;               // Pasos a mover en el modo de rango, puede ser positivo o negativo

byte  modoMenu,                 // Modo actual del menu
      tEstab,                   // Tiempo para estabilizar la camara despues de mover el motor, de 1 a 99 segundos
      pulsosPaso,               // Es el indice al array con el numero de pulsos por paso
      fCiclo,                   // Num de fotos en cada ciclo de disparos
      lead,                     // Paso del husillo en pasos de 0.5mm -> 2 = 1mm, 4 = 2mm, 6 = 3mm ....
      tiempoLCD,                // Tiempo en segundos que permanece activo el LCD. 0 = no se apaga nunca
      autoEN,                   // Si se pone a 1, solo habilita el motor cuando lo va a usar, a 0 siempre esta habilitado
      precision,                // Precision del movimiento, 1 = x1, 2 = x2, 3 = x4 (la mitad de micropasos al motor = la mitad del movimiento indicado)
      modoCiclo,                // 0=normal, 1=no fotos, 2=no motor
      pulsosPaP[8] = {1, 2, 4, 8, 16, 32, 64, 128};

boolean funcionando,            // Esta haciendo fotos
        menuIn,                 // 0=Esta en modo de seleccion del menu, 1=Esta en modo de cambio de parametros
        moviendoeje,            // 1 = moviendo eje, no esperar para refrescar
        sentido,                // Sentido del movimiento. 0 = INI->FIN / 1 = FIN->INI
        tipoMov,                // Tipo de movimiento para el eje: 0 = lineal, 1 = rotativo
        bmas, bmasmas,
        bmenos, bmenosmenos,
        bup, bupup,
        bdown, bdowndown,
        bjoy, volver,
        iniCarrera, finCarrera,
        unidades;               // 0 = micras, 1 = milimetros

float unPaso, disPaso, grados;

unsigned long Antes, Ahora,     // Para controlar el tiempo encendido de la pantalla
         pEntera, pDecimal,
         pEnteraT, pDecimalT;

void setup()
{

  lcd0x27.begin();
  lcd0x3F.begin();

  ponPantallaInicio();

  delay(1200);

  // Si en la posicion 1 de la memoria EEPROM hay un valor especificado, entiendo que el programa se ha ejecutado antes y lee los valores a las variables

  if (EEPROM.read(1) == vEEPROM) {
    leeSetup();
    leeValoresU(1);            // Recupera de EEPROM los valores U almacenados
    leeValoresU(2);
    leeValoresU(3);
    leeValoresU(4);
  }
  else {
    valoresDefecto();         // Carga los valores por defecto para el modo activo
    valoresDefectoU();        // Carga los valores por defecto para los modos de usuario
    EEPROM.write(1, vEEPROM); // Indica que ya se ha ejecutado el programa
    guardaSetup();            // Guarda en EEPROM los valores actuales del modo activo
    escribeValoresU(1);       // Guardo los valores por defecto de los modos de usuario
    escribeValoresU(2);
    escribeValoresU(3);
    escribeValoresU(4);
  }

  // Inicializa las variables

  moviendoeje = menuIn = funcionando = iniCarrera = finCarrera = cuantosPasos = modoCiclo = 0;

  modoMenu = 1;

  pinMode(J_BTN, INPUT_PULLUP);

  if (SBB_V4 == 1) {    // Si es la version v4 activamos las salidas M0, M1 y M2. Version V4 solamente con Joystick.

    pinMode(M0, OUTPUT);
    pinMode(M1, OUTPUT);
    pinMode(M2, OUTPUT);

    ajustaM012();       // Ajusta los micropasos del motor

  }

  pinMode(FC, INPUT);           // Entrada 1 para finales de carrera

  pinMode(IN2, INPUT);          // Entrada 2 de disparo directo por nivel alto
  pinMode(IN1, INPUT_PULLUP);   // Entrada 1 de disparo directo de camara por nivel bajo

  pinMode(CH8, OUTPUT);         // Rele 1 - Disparo
  pinMode(CH7, OUTPUT);         // Rele 2 - Enfoque

  pinMode(DIR, OUTPUT);         // Motor paso a paso - Direccion
  pinMode(STEP, OUTPUT);        // Motor paso a paso - Pasos
  pinMode(EN, OUTPUT);          // Enable del driver PaP

  // Los reles se activan por nivel bajo, los optoacopladores por nivel alto

  digitalWrite(CH8, HAS_RELE);      // Pongo un 1 para "desactivar" el rele de disparo de camara.
  digitalWrite(CH7, HAS_RELE);      // Pongo un 1 para "desactivar" el rele de enfoque de camara.

  if (autoEN) digitalWrite(EN, 1);  // Por defecto desactivo el driver PaP, solo se activa para mover el motor

  calculaUnPaso();
  //calculaCuantosPasos();

  muestraMenu();                    // Muestra las opciones del menu
  ponCursor();                      // Pon el cursor correcto

  Antes = millis();   // Guardo el tiempo actual para la funcion "reposo LCD"

}


void loop()
{
  boolean opcion;

  leeEntradasMenu();    // Siempre lee las entradas del menu

  if (iniCarrera) {                             // Si debe ir al inicio del carril ...

    if (TEST == 0 && !digitalRead(FC) && digitalRead(J_BTN)) mueveMotor(20, sentido);   // Si no se ha activado el final de carrera mueve el motor
    else detieneCarril();

  }
  else if (finCarrera) {                        // Si debe ir al fin del carril ...

    if (TEST == 0 && !digitalRead(FC) && digitalRead(J_BTN)) mueveMotor(20, !sentido);   // Si no se ha activado el final de carrera mueve el motor
    else detieneCarril();

  }
  else if (funcionando) {                       // Si esta funcionando en "ciclo Macro" hace lo que tenga que hacer y refresca la pantalla
    cicloMacro();
  }
  else if (leeDigitales() && !menuIn) {         // Si hay alguna entrada digital activa y no esta modificando un valor, dispara la camara
//    indicaDisparo();
    disparaCamara();
    delay(250);                                 // espero 0.25 segundo antes de volver a estar disponible
    muestraMenu();
    ponCursor();
  }
  else  {                                       // Si se pulsa alguna tecla se trata la entrada digital y se muestran los valores

    opcion = (bjoy | bmas | bmasmas | bmenos | bmenosmenos | bup | bupup | bdown | bdowndown);

    if (opcion) {   // Si se ha pulsado algun boton o se ha movido el joystick hace lo necesario

      leeEntradas();

      if (!moviendoeje) {     // Si no esta en la opcion de "Ajustar Posicion" actualiza el cursor y los valores
        ponCursor();
        if (menuIn) ponValores();
      }

    }   // if opcion ...

  }

  /*

    // PARA TESTEAR LOS VALORES DEL JOYSTICK
    char *linea="                   ";
    sprintf(linea, "DX:%4d  DY:%4d",analogRead(J_DX),analogRead(J_DY));
    Serial.println(linea);

  */

}


void leeEntradasMenu()
{

  bjoy = !digitalRead(J_BTN);

  bmas = bmasmas = bmenos = bmenosmenos = bup = bupup = bdown = bdowndown = 0;

  if (analogRead(J_DY) > 680) {
    if (analogRead(J_DY) < 1000) bmas = 1;
    else bmasmas = 1;
  }

  if (analogRead(J_DY) < 320) {
    if (analogRead(J_DY) > 10) bmenos = 1;
    else bmenosmenos = 1;
  }

  if (analogRead(J_DX) > 680) {
    if (analogRead(J_DX) < 1000) bdown = 1;
    else bdowndown = 1;
  }

  if (analogRead(J_DX) < 320) {
    if (analogRead(J_DX) > 10) bup = 1;
    else bupup = 1;
  }

  // Si hay alguna entrada modificada activo el LCD

  if (bjoy || bmas || bmasmas || bmenos || bmenosmenos || bup || bupup || bdown || bdowndown ) {
    lcd0x27.display(); lcd0x27.backlight();
    lcd0x3F.display(); lcd0x3F.backlight();
    Antes = millis();   // Actualizo el tiempo "inicial"
  }
  else if (tiempoLCD > 0) {
    Ahora = millis();
    if (((Ahora - Antes) / 1000) > tiempoLCD) {
      lcd0x27.noBacklight();  // Si ha pasado el tiempo especificado sin mover un boton desactiva la luz del LCD
      lcd0x3F.noBacklight();
    }
    if (((Ahora - Antes) / 1000) > (tiempoLCD * 2)) {
      lcd0x27.noDisplay();    // Si ha pasado el doble del tiempo especificado sin mover un boton apaga el display
      lcd0x3F.noDisplay();
    }
  }

}


void detieneCarril()      // Funcion para detener el movimiento del carril
{
  if (autoEN > 0) digitalWrite(EN, 1);  // Desactiva el driver del motor
  iniCarrera = finCarrera = 0;                          // Desactiva el movimiento del carril
}


boolean leeDigitales()    // Devuelve 1 si se ha activado alguna entrada digital de disparo de camara
{

  // Si estamos en modo TEST solo lee la entrada nivel bajo por si no tenemos resistencias pull down

  if (!TEST) return (digitalRead(IN2) | !digitalRead(IN1));
  else return (!digitalRead(IN1));

}


void ajustaM012()
{
  boolean MS0 = 0, MS1 = 0, MS2 = 0;

  // Para la version v4 ajusta el valor de M0, M1 y M2 segun los micropulsos programados

  switch (pulsosPaP[pulsosPaso]) {
    case 1: MS0 = 0; MS1 = 0; MS2 = 0; break;
    case 2: MS0 = 1; MS1 = 0; MS2 = 0; break;
    case 4: MS0 = 0; MS1 = 1; MS2 = 0; break;
    case 8: MS0 = 1; MS1 = 1; MS2 = 0; break;
    case 16: MS0 = 0; MS1 = 0; MS2 = 1; break;
    default: pulsosPaso = 5; MS0 = 0; MS1 = 1; MS2 = 1; break;    // Por defecto pone el driver a 32 uP x paso
  }

  digitalWrite(M0, MS0);
  digitalWrite(M1, MS1);
  digitalWrite(M2, MS2);

}


// Dispara la camara con el modo programado

void disparaCamara()
{

  digitalWrite(CH7, !HAS_RELE);
  digitalWrite(CH8, !HAS_RELE);

  delay(400);     // Espera 400 milisegundos

  digitalWrite(CH8, HAS_RELE);
  digitalWrite(CH7, HAS_RELE);

}


// Mueve el motor los pasos en la direccion indicada

void mueveMotor(int pasos, bool direccion)
{
  long i, pulsos = long(pasos) * long(pulsosPaP[pulsosPaso]);

  if (precision == 1) pulsos = pulsos / 2;          // Si se ha fijado la precision (>1) ajusta los micropasos reales que deben enviarse al motor
  else if (precision == 2) pulsos = pulsos / 4;

  digitalWrite(DIR, direccion);           // Fijamos una direcion

  for (i = 0; i < pulsos; i++) {     // Mandamos pulsos al motor, 1 paso = 1 pulso * num. pulsosPaso
    digitalWrite(STEP, HIGH);
    delayMicroseconds(delayPaP);          // Esperamos xxx microsegundos
    digitalWrite(STEP, LOW);
    delayMicroseconds(delayPaP);          // Esperamos xxx microsegundos
  }

}


// Mueve el motor los pasos en la direccion indicada y usando los micropulsos indicados

void mueveMotor(int pasos, bool direccion, byte micropulsos)
{
  long i, pulsos = long(pasos) * long(micropulsos);

  if (precision == 1) pulsos = pulsos / 2;          // Si se ha fijado la precision (>1) ajusta los micropasos reales que deben enviarse al motor
  else if (precision == 2) pulsos = pulsos / 4;

  digitalWrite(DIR, direccion);           // Fijamos una direcion

  for (i = 0; i < pulsos; i++) {          // Mandamos pulsos al motor, 1 paso = 1 pulso * micropulsos
    digitalWrite(STEP, HIGH);
    delayMicroseconds(delayPaP);          // Esperamos xxx microsegundos
    digitalWrite(STEP, LOW);
    delayMicroseconds(delayPaP);          // Esperamos xxx microsegundos
  }

}


/*
   Obtengo la distancia de un paso y la que debe recorrer entre foto y foto, en micras
*/

void calculaUnPaso()
{

  grados = 360.0F / (float)pvuelta;               // Calculo los grados de giro de un paso

  unPaso = float(lead * 500.0F) / (float)pvuelta; // Calculo la distancia lineal de un paso en funcion del lead del husillo (lead * 1000 / 2 ya que lead va de 0.5 en 0.5 mm y trabajo en micras)
  disPaso = dPaso * unPaso;                       // La distancia entre fotos = # de pasos * distancia de 1 paso

  // Aqui obtego los valores en caso de que se haya fijado una precisión

  switch (precision) {

    case 1:  unPaso = unPaso / 2;
      disPaso = disPaso / 2;
      break;

    case 2:  unPaso = unPaso / 4;
      disPaso = disPaso / 4;
      break;
  }

  // Aqui saco la parte entera y decimal

  pEntera = long(disPaso);                      // Parte entera de un paso
  pDecimal = (disPaso - long(disPaso)) * 100;   // Parte decimal de un paso con 2 digitos

  if (unidades) {                               // Si trabajo con milimetros ajusto la parte entera y decimales
    pDecimal = (pEntera / 10) % 100;            // Primero se calcula la parte decimal ya que usa la parte entera original
    pEntera = pEntera / 1000;
  }

}


/*
   Calcula la distancia total que recorrera el carril, para mostrarla correctamente
*/

void calculaCuantosPasos()
{
  float dTotal;

  if (cuantosPasos == 0) dTotal = (nFotosM1 - 1) * disPaso; // En programa por num. de fotos calcula la distancia total a recorrer
  else dTotal = unPaso * abs(cuantosPasos);

  // Aqui saco la parte entera y decimal

  pEnteraT = long(dTotal);                      // Parte entera total
  pDecimalT = (dTotal - long(dTotal)) * 100;    // Parte decimal total con 2 digitos

  if (unidades) {                               // Si trabajo con milimetros ajusto la parte entera y decimales
    pDecimalT = (pEnteraT / 10) % 100;          // Primero se calcula la parte decimal ya que usa la parte entera original
    pEnteraT = pEnteraT / 1000;
  }

  // Calculo el numero de fotos que se deben hacer

  nFotosM2 = (abs(cuantosPasos) / dPaso) + 1;      // Hay que sumar 1 para que llegue al final del recorrido

}
