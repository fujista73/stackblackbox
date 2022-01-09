void ponPantallaInicio()
{

  if (SBB_V4) {
    sprintf(linea2, " StackBlackBox Pro  ");
  }
  else {
    sprintf(linea2, " StackBlackBox Std  ");
  }

  sprintf(linea1, "********************");
  sprintf(linea3, " (r) 2019 / v9.41   ");
  sprintf(linea4, "********************");

  ponTodaPantalla();

}


void ponMensajeModoU(int ModoU)
{

  // Muestra el mensaje de presentacion 1 segundo

  sprintf(linea4, "--------------------");
  sprintf(linea2, "  Config. guardada  ");
  sprintf(linea3, "  en el Modo U%d     ", ModoU);
  sprintf(linea4, "--------------------");

  ponTodaPantalla();

  delay(1000);

}

/*
void indicaDisparo()
{

  byte eDigital;

  if (digitalRead(IN2))  eDigital = 2;
  if (!digitalRead(IN1)) eDigital = 1;

  // ***** Muestra una pantalla indicando que ha disparado la camara por entrada digital directa

  sprintf(linea4, "--------------------");
  sprintf(linea2, " Entrada Digital: %d", eDigital);
  sprintf(linea3, "                    ");
  sprintf(linea4, "--------------------");

  ponTodaPantalla();

}
*/

void ponTodaPantalla()
{

  lcd0x27.setCursor(0, 0); lcd0x27.print(linea1);
  lcd0x27.setCursor(0, 1); lcd0x27.print(linea2);
  lcd0x27.setCursor(0, 2); lcd0x27.print(linea3);
  lcd0x27.setCursor(0, 3); lcd0x27.print(linea4);

  lcd0x3F.setCursor(0, 0); lcd0x3F.print(linea1);
  lcd0x3F.setCursor(0, 1); lcd0x3F.print(linea2);
  lcd0x3F.setCursor(0, 2); lcd0x3F.print(linea3);
  lcd0x3F.setCursor(0, 3); lcd0x3F.print(linea4);

}


void ponLinea(byte x, byte y, byte linea)
{

  lcd0x27.setCursor(x, y);
  lcd0x3F.setCursor(x, y);

  switch (linea) {

    case 1: lcd0x27.print(linea1); lcd0x3F.print(linea1); break;
    case 2: lcd0x27.print(linea2); lcd0x3F.print(linea2); break;
    case 3: lcd0x27.print(linea3); lcd0x3F.print(linea3); break;
    case 4: lcd0x27.print(linea4); lcd0x3F.print(linea4); break;

  }

}
