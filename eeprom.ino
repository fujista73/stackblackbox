
void reset()
{

  valoresDefecto();         // Carga los valores por defecto para el modo activo
  valoresDefectoU();        // Carga los valores por defecto para los modos de usuario
  EEPROM.write(1, vEEPROM); // Indica que ya se ha ejecutado el programa
  guardaSetup();            // Guarda en EEPROM los valores actuales del modo activo
  escribeValoresU(1);       // Guardo los valores por defecto de los modos de usuario
  escribeValoresU(2);
  escribeValoresU(3);
  escribeValoresU(4);
  modoMenu = 1;
  muestraMenu();

  calculaUnPaso();
  calculaCuantosPasos();

}


// Establece como valores del ciclo actual los valores del modo especificado como parametro

void ponModoU(byte modo)
{

  unidades = modoU[modo - 1].unidades;
  pausa = modoU[modo - 1].pausa;
  dPaso = modoU[modo - 1].dPaso;
  nFotos = modoU[modo - 1].nFotos;
  tEstab = modoU[modo - 1].tEstab;
  pvuelta = modoU[modo - 1].pvuelta;
  sentido = modoU[modo - 1].sentido;
  pulsosPaso = modoU[modo - 1].pulsosPaso;
  precision = modoU[modo - 1].precision;
  delayPaP = modoU[modo - 1].delayPaP;
  tipoMov = modoU[modo - 1].tipoMov;
  aRotacion = modoU[modo - 1].aRotacion;
  fCiclo = modoU[modo - 1].fCiclo;
  volver = modoU[modo - 1].volver;
  lead = modoU[modo - 1].lead;
  autoEN = modoU[modo - 1].autoEN;

  guardaSetup();    // Guardo en EEPROM las opciones seleccionadas

  calculaUnPaso();
  calculaCuantosPasos();

}


// Pone los valores por defecto para el modo actual

void valoresDefecto()
{
  unidades = 1;     // Trabajamos en mm
  dPaso = 25;
  tEstab = 3;
  nFotos = 80;
  pausa = 5;
  sentido = tipoMov = 0;
  pvuelta = 400;
  modoMenu =  1;
  pulsosPaso = 5;         // pulsosPaso = 5  en el array significa 32 pulsos por paso
  precision = 0;
  delayPaP = 45;
  aRotacion = 5;          // Numero de pulsos, 1 pulso = 1.8 grados o 0.9 grados
  fCiclo = 1;
  volver = 0;
  lead = 4;
  autoEN = 2;
  tiempoLCD = 60;         // 60 segundos sin hacer nada se apaga la luz del LCD

  calculaUnPaso();
  calculaCuantosPasos();
}


void defectoU(byte modo)      // Asigna los valores actuales al modo de usuario indicado
{

  int U = modo - 1;

  modoU[U].unidades = unidades;
  modoU[U].nFotos = nFotos;
  modoU[U].dPaso = dPaso;
  modoU[U].pausa = pausa;
  modoU[U].pvuelta = pvuelta;
  modoU[U].tEstab = tEstab;
  modoU[U].sentido = sentido;
  modoU[U].pulsosPaso = pulsosPaso;
  modoU[U].precision = precision;
  modoU[U].delayPaP = delayPaP;
  modoU[U].tipoMov = tipoMov;
  modoU[U].aRotacion = aRotacion;
  modoU[U].fCiclo = fCiclo;
  modoU[U].volver = volver;
  modoU[U].lead = lead;
  modoU[U].autoEN = autoEN;

}


void valoresDefectoU()      // Asigna los valores por defecto a los modos de usuario
{

  modoU[0].unidades = 1;
  modoU[0].nFotos = 40;
  modoU[0].dPaso = 100;
  modoU[0].pausa = 3;
  modoU[0].pvuelta = 400;
  modoU[0].tEstab = 3;
  modoU[0].sentido = 0;
  modoU[0].pulsosPaso = 5;          // pulsosPaso = 5  en el array significa 32 pulsos por paso
  modoU[0].precision = 0;
  modoU[0].delayPaP = 75;
  modoU[0].tipoMov = 0;
  modoU[0].aRotacion = 5;
  modoU[0].fCiclo = 1;
  modoU[0].volver = 0;
  modoU[0].lead = 4;
  modoU[0].autoEN = 2;

  modoU[1].unidades = 1;
  modoU[1].nFotos = 80;
  modoU[1].dPaso = 50;
  modoU[1].pausa = 5;
  modoU[1].pvuelta = 400;
  modoU[1].tEstab = 3;
  modoU[1].sentido = 0;
  modoU[1].pulsosPaso = 5;          // pulsosPaso = 5  en el array significa 32 pulsos por paso
  modoU[1].precision = 0;
  modoU[1].delayPaP = 150;
  modoU[1].tipoMov = 0;
  modoU[1].aRotacion = 5;           // Numero de pulsos, 1 pulso = 360/pvuelta grados
  modoU[1].fCiclo = 1;
  modoU[1].volver = 0;
  modoU[1].lead = 4;
  modoU[1].autoEN = 2;

  modoU[2].unidades = 1;
  modoU[2].nFotos = 120;
  modoU[2].dPaso = 25;
  modoU[2].pausa = 5;
  modoU[2].pvuelta =400;
  modoU[2].tEstab = 3;
  modoU[2].sentido = 0;
  modoU[2].pulsosPaso = 5;          // pulsosPaso = 5  en el array significa 32 pulsos por paso
  modoU[2].precision = 0;
  modoU[2].delayPaP = 250;
  modoU[2].tipoMov = 0;
  modoU[2].aRotacion = 5;           // Numero de pulsos, 1 pulso = 360/pvuelta grados
  modoU[2].fCiclo = 1;
  modoU[2].volver = 0;
  modoU[2].lead = 4;
  modoU[2].autoEN = 2;

  modoU[3].unidades = 1;
  modoU[3].nFotos = 200;
  modoU[3].dPaso = 5;
  modoU[3].pausa = 7;
  modoU[3].pvuelta = 400;
  modoU[3].tEstab = 3;
  modoU[3].sentido = 0;
  modoU[3].pulsosPaso = 5;         // pulsosPaso = 5  en el array significa 32 pulsos por paso
  modoU[3].precision = 0;
  modoU[3].delayPaP = 350;
  modoU[3].tipoMov = 0;
  modoU[3].aRotacion = 5;         // 1 pulso = 360/pvuelta grados
  modoU[3].fCiclo = 1;
  modoU[3].volver = 0;
  modoU[3].lead = 4;
  modoU[3].autoEN = 2;

}


void leeValoresU(byte modo)        // Lee desde EEPROM los valores de usuario almacenados
{

  int pinicial = modo * 40;

  modoU[modo - 1].nFotos = EEPROMReadInt(pinicial + 2);
  modoU[modo - 1].dPaso = EEPROMReadInt(pinicial + 4);
  modoU[modo - 1].pausa = EEPROMReadInt(pinicial + 6);
  modoU[modo - 1].pvuelta = EEPROMReadInt(pinicial + 8);
  modoU[modo - 1].sentido = EEPROM.read(pinicial + 11);
  modoU[modo - 1].unidades = EEPROM.read(pinicial + 12);
  modoU[modo - 1].pulsosPaso = EEPROM.read(pinicial + 13);
  modoU[modo - 1].delayPaP = EEPROMReadInt(pinicial + 14);
  modoU[modo - 1].tipoMov = EEPROM.read(pinicial + 16);

  modoU[modo - 1].fCiclo = EEPROM.read(pinicial + 18);
  modoU[modo - 1].volver = EEPROM.read(pinicial + 19);
  modoU[modo - 1].lead = EEPROM.read(pinicial + 20);
  modoU[modo - 1].autoEN = EEPROM.read(pinicial + 21);
  modoU[modo - 1].tEstab = EEPROM.read(pinicial + 23);
  modoU[modo - 1].precision = EEPROM.read(pinicial + 24);
  modoU[modo - 1].aRotacion = EEPROMReadInt(pinicial + 26);
}


void escribeValoresU(byte modo)        // Guarda en EEPROM los valores de modos de usuario
{

  int pinicial = modo * 40;

  EEPROMWriteInt(pinicial + 2, modoU[modo - 1].nFotos);
  EEPROMWriteInt(pinicial + 4, modoU[modo - 1].dPaso);
  EEPROMWriteInt(pinicial + 6, modoU[modo - 1].pausa);
  EEPROMWriteInt(pinicial + 8, modoU[modo - 1].pvuelta);
  EEPROM.write(pinicial + 11, modoU[modo - 1].sentido);
  EEPROM.write(pinicial + 12, modoU[modo - 1].unidades);
  EEPROM.write(pinicial + 13, modoU[modo - 1].pulsosPaso);
  EEPROMWriteInt(pinicial + 14, modoU[modo - 1].delayPaP);
  EEPROM.write(pinicial + 16, modoU[modo - 1].tipoMov);

  EEPROM.write(pinicial + 18, modoU[modo - 1].fCiclo);
  EEPROM.write(pinicial + 19, modoU[modo - 1].volver);
  EEPROM.write(pinicial + 20, modoU[modo - 1].lead);
  EEPROM.write(pinicial + 21, modoU[modo - 1].autoEN);
  EEPROM.write(pinicial + 23, modoU[modo - 1].tEstab);
  EEPROM.write(pinicial + 24, modoU[modo - 1].precision);
  EEPROMWriteInt(pinicial + 26, modoU[modo - 1].aRotacion);

}


void guardaU(byte modo)     // Guarda en EEPROM en el modo U los valores actuales
{

  int pinicial = modo * 40;

  EEPROMWriteInt(pinicial + 2, nFotos);
  EEPROMWriteInt(pinicial + 4, dPaso);
  EEPROMWriteInt(pinicial + 6, pausa);
  EEPROMWriteInt(pinicial + 8, pvuelta);
  EEPROM.write(pinicial + 11, sentido);
  EEPROM.write(pinicial + 12, unidades);
  EEPROM.write(pinicial + 13, pulsosPaso);
  EEPROMWriteInt(pinicial + 14, delayPaP);
  EEPROM.write(pinicial + 16, tipoMov);

  EEPROM.write(pinicial + 18, fCiclo);
  EEPROM.write(pinicial + 19, volver);
  EEPROM.write(pinicial + 20, lead);
  EEPROM.write(pinicial + 21, autoEN);
  EEPROM.write(pinicial + 23, tEstab);
  EEPROM.write(pinicial + 24, precision);
  EEPROMWriteInt(pinicial + 26, aRotacion);
  
}


void guardaSetup()
{
  EEPROMWriteInt(2, nFotos);
  EEPROMWriteInt(4, dPaso);
  EEPROMWriteInt(6, pausa);
  EEPROMWriteInt(8, pvuelta);
  EEPROM.write(11, sentido);
  EEPROM.write(12, unidades);
  EEPROM.write(13, pulsosPaso);
  EEPROMWriteInt(14, delayPaP);
  EEPROM.write(16, tipoMov);

  EEPROM.write(18, fCiclo);
  EEPROM.write(19, volver);
  EEPROM.write(20, lead);
  EEPROM.write(21, autoEN);
  EEPROM.write(22, tiempoLCD);
  EEPROM.write(23, tEstab);
  EEPROM.write(24, precision);
  EEPROMWriteInt(26, aRotacion);

}


void leeSetup()
{
  nFotos = EEPROMReadInt(2);
  dPaso = EEPROMReadInt(4);
  pausa = EEPROMReadInt(6);
  pvuelta = EEPROMReadInt(8);
  sentido = EEPROM.read(11);
  unidades = EEPROM.read(12);
  pulsosPaso = EEPROM.read(13);
  delayPaP = EEPROMReadInt(14);
  tipoMov = EEPROM.read(16);

  fCiclo = EEPROM.read(18);
  volver = EEPROM.read(19);
  lead = EEPROM.read(20);
  autoEN = EEPROM.read(21);
  tiempoLCD = EEPROM.read(22);
  tEstab = EEPROM.read(23);
  precision = EEPROM.read(24);
  aRotacion = EEPROMReadInt(26);
}


// Este codigo de lectura y escritura de EEPROM se ha obtenido de la web de Arduino

void EEPROMWriteInt(int p_address, unsigned int p_value)
{
  byte lowByte = ((p_value >> 0) & 0xFF);
  byte highByte = ((p_value >> 8) & 0xFF);

  EEPROM.write(p_address, lowByte);
  EEPROM.write(p_address + 1, highByte);
}


unsigned int EEPROMReadInt(int p_address)
{
  byte lowByte = EEPROM.read(p_address);
  byte highByte = EEPROM.read(p_address + 1);

  return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
}
