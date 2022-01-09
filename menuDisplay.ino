/*
 * 
 *     1. Iniciar
 *     2. Fija Ini: <--->
 *     3. Ajustar Posicion
 *     4. Dis.Paso: 00,000 mm
 *    
 *     5. NumFotos
 *     6. Fot.Ciclo
 *     7. Tp.Espera
 *     8. Tp.Estab.
 *    
 *     9. Ajustes trabajo 1
 *    10. Ajustes trabajo 2
 *    11. Ajustes trabajo 3
 *    12. Ajustes trabajo 4
 *    
 *    13. Lead
 *    14. Sentido
 *    15. Volver0
 *    16. Auto EN
 *    
 *    17. TpMotor
 *    18. uPxPaso
 *    19. Precis.
 *    20. Delay P
 *    
 *    21. Tipo Mov
 *    22. An.Rocac
 *    23. Desc.LCD
 *    24. Unidades  
 * 
 */


void muestraMenu()
{

  if (modoMenu <= 4) {

    if (tipoMov == 0) {
      sprintf(linea1, " Iniciar            ");
      if (!unidades) sprintf(linea4, " Dis.Paso:%4d,%02d u ", int(pEntera), int(pDecimal));
      else sprintf(linea4, " Dis.Paso:%4d,%02d mm", int(pEntera), int(pDecimal));
    }
    else {
      sprintf(linea1, " Iniciar Rotacion   ");
      sprintf(linea4, " Dis.Paso: --,-- mm ");
    }

    sprintf(linea2, " Ajustar Posicion   ");

    if (cuantosPasos == 0) sprintf(linea3, " Fijar inicio |---| ");
    else {

        if (!unidades) {
          if (cuantosPasos > 0) sprintf(linea3, " |<%4d.%02d u   #%04d", int(pEnteraT), int(pDecimalT), int(nFotosM2));
          else sprintf(linea3, " %4d.%02d u  >| #%04d", int(pEnteraT), int(pDecimalT), int(nFotosM2));         
        }
        else {
          if (cuantosPasos > 0) sprintf(linea3, " |<%4d.%02d mm  #%04d", int(pEnteraT), int(pDecimalT), int(nFotosM2));
          else sprintf(linea3, " %4d.%02d mm >| #%04d", int(pEnteraT), int(pDecimalT), int(nFotosM2));
        }      
    }

  }
  else if (modoMenu <= 8) {

    sprintf(linea1, " Num.Fotos: %04d    ", (cuantosPasos)?int(nFotosM2):int(nFotos));
    sprintf(linea2, " Fot.Ciclo: %03d     ", (unsigned)fCiclo);
    sprintf(linea3, " Tp.Espera: %4d s  ", (unsigned)pausa);
    sprintf(linea4, " Tp.Estab.: %4d s  ", (unsigned)tEstab);

  }
  else if (modoMenu <= 12) {
    
    sprintf(linea1, " Ajustes usuario 1  ");
    sprintf(linea2, " Ajustes usuario 2  ");
    sprintf(linea3, " Ajustes usuario 3  ");
    sprintf(linea4, " Ajustes usuario 4  ");

  }
  else if (modoMenu <= 16) {

    if (tipoMov == 0)
      if (!unidades) sprintf(linea1, " Lead   : %02d.%02d u   ", (lead/2),(lead%2)*50);
      else sprintf(linea1, " Lead   : %02d.%02d mm  ", (lead/2),(lead%2)*50);
    else
      sprintf(linea1, " Lead   : ---       ");

    if (sentido == 0)
      sprintf(linea2, " Sentido: A -> B    ");
    else
      sprintf(linea2, " Sentido: B -> A    ");

    if (volver == 0)
      sprintf(linea3, " Volver0: No        ");
    else
      sprintf(linea3, " Volver0: Si        ");

    switch (autoEN) {
      case 0: sprintf(linea4, " Auto EN: No        "); break;
      case 1: sprintf(linea4, " Auto EN: Si        "); break;
      case 2: sprintf(linea4, " Auto EN: Solo Mov. "); break;
    }

  }
  else if (modoMenu <= 20) {

    sprintf(linea1, " TpMotor: %04d pasos", pvuelta); 
    sprintf(linea2, " uPxPaso: %03d       ", pulsosPaP[pulsosPaso]);

    switch (precision) {
      case 0: sprintf(linea3, " Precis.:  x1       "); break;
      case 1: sprintf(linea3, " Precis.:  x2       "); break;
      case 2: sprintf(linea3, " Precis.:  x4       "); break;
    }

    sprintf(linea4, " Delay P:%4d uS    ", delayPaP);

  }
  else if (modoMenu <= 24) {

    if (tipoMov == 0) {
      sprintf(linea1, " Tipo Mov: Lineal   ");
      sprintf(linea2, " An.Rotac: ----,-   ");
    }
    else {
      sprintf(linea1, " Tipo Mov: Rotativo ");
      sprintf(linea2, " An.Rotac: %4u,%0u   ", unsigned(aRotacion * grados), unsigned(aRotacion * (grados*10)) % 10);
    }

    sprintf(linea3, " Desc.LCD: %02d s     ", tiempoLCD);

    if (unidades == 0)
      sprintf(linea4, " Unidades: micras   ");
    else
      sprintf(linea4, " Unidades: mm       ");

  }

  ponTodaPantalla();

}


void ponValores()
{

  switch (modoMenu) {

    case 4: if (!tipoMov) {
        sprintf(linea4, "%4d,%02d", int(pEntera), int(pDecimal));     // Lo muestra si el movimiento es lineal
      }
      else
        sprintf(linea4, " ---,--");
      ponLinea(10, 3, 4);
      break;

    case 5: sprintf(linea1, "%04d", (cuantosPasos)?int(nFotosM2):int(nFotos));
      ponLinea(12, 0, 1);
      break;

    case 6: sprintf(linea2, "%03d", fCiclo);
      ponLinea(12, 1, 2);
      break;

    case 7: sprintf(linea3, "%4d", pausa);
      ponLinea(12, 2, 3);
      break;

    case 8: sprintf(linea4, "%4d", tEstab);
      ponLinea(12, 3, 4);
      break;

    case 13: if (!tipoMov)
        sprintf(linea1, "%02d.%02d", (lead/2),(lead%2)*50);
      else
        sprintf(linea1, "--");
      ponLinea(10, 0, 1);
      break;

    case 17: sprintf(linea1, "%04d", pvuelta); 
      ponLinea(10, 0, 1);
      break;
      
    case 18: sprintf(linea2, "%3d", pulsosPaP[pulsosPaso]);
      ponLinea(10, 1, 2);
      break;

    case 20: sprintf(linea4, "%4d", delayPaP);
      ponLinea(9, 3, 4);
      break;

    case 22: if (tipoMov) sprintf(linea2, "%4u,%0u", unsigned(aRotacion * grados), unsigned(aRotacion * (grados*10)) % 10);  // Lo muestra si el movimiento es Rotativo
             else sprintf(linea2, "----,- ");
      ponLinea(11, 1, 2);
      break;

    case 23: sprintf(linea3, "%02d", tiempoLCD);
      ponLinea(11, 2, 3);
      break;

  }

  if (!moviendoeje) delay(180);   // Espera un poco antes de volver a leer los botones

}


void ponCursor()
{

  // Quito la marca del menu de las 4 opciones

  lcd0x27.setCursor(0, 0); lcd0x27.print(" ");
  lcd0x27.setCursor(0, 1); lcd0x27.print(" ");
  lcd0x27.setCursor(0, 2); lcd0x27.print(" ");
  lcd0x27.setCursor(0, 3); lcd0x27.print(" ");

  lcd0x3F.setCursor(0, 0); lcd0x3F.print(" ");
  lcd0x3F.setCursor(0, 1); lcd0x3F.print(" ");
  lcd0x3F.setCursor(0, 2); lcd0x3F.print(" ");
  lcd0x3F.setCursor(0, 3); lcd0x3F.print(" ");

  // Coloco el cursor donde debe estar segun la opcion y pongo el cursor de posicion

  switch (modoMenu%4) {  

    case 1: lcd0x27.setCursor(0, 0); lcd0x3F.setCursor(0, 0); break;
    case 2: lcd0x27.setCursor(0, 1); lcd0x3F.setCursor(0, 1); break;
    case 3: lcd0x27.setCursor(0, 2); lcd0x3F.setCursor(0, 2); break;
    case 0: lcd0x27.setCursor(0, 3); lcd0x3F.setCursor(0, 3); break;
    
  }

  if (!menuIn) {
    lcd0x27.print(">");
    lcd0x3F.print(">");
  }
  else {
    lcd0x27.print("*");
    lcd0x3F.print("*");
  }

  if (!moviendoeje && !menuIn) delay(180);   // Si no esta dentro de la opcion de mover el eje Z, espera un poco antes de volver a leer los botones

}
