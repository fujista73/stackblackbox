void ponMacro()
{

  sprintf(linea1, " Modo fotos + mov.  ");
  sprintf(linea2, " Foto %04d de %04d  ", int(fotosHechas), int(nFotos));

  if (!tipoMov) {
    sprintf(linea3, "    0,00/%5ld,%02d   ", long(pEnteraT), int(pDecimalT));
  }
  else sprintf(linea3, " R.Tot:    0,0 grad ");

  if (sentido == 0) sprintf(linea4, "%5d,%02d-00/%02d-AB   ", long(pEntera), int(pDecimal), fCiclo);
  else sprintf(linea4, "%5d,%02d-00/%02d-BA   ", long(pEntera), int(pDecimal), fCiclo);

  ponTodaPantalla();

}


void cicloMacro()
{
  float actual = (fotosHechas-1) * disPaso;        
  long dActualE;
  unsigned dActualD, nF, pulsos;

  // Aqui obtego los valores en caso de que se haya fijado una precisión

  if (precision == 1) {
    actual = actual / 2.0F;
  }
  else if (precision == 2) {
    actual = actual / 4.0F; 
  }
  
  // Aqui se calcula la parte entera y decimal del movimiento actual

  dActualE = long(actual);                        // Parte entera de la distancia actual
  dActualD = int((actual-long(actual))*100);      // Parte con 2 decimales de la distancia actual

  if (fotosHechas == 1) ponMacro();         // Pone las lineas del contador de fotos solo una vez

  sprintf(linea2, "%04d", fotosHechas); ponLinea(6, 1, 2);            // Refresca las fotos hechas
  sprintf(linea4, "00");  ponLinea(8, 3, 4);                          // Refresca el num. de fotos por ciclo

  if (!tipoMov) {
    sprintf(linea3, "%5ld,%02d", long(dActualE), int(dActualD));      // Refresca la distancia lineal avanzada
    ponLinea(0, 2, 3);
  }
  else {
    sprintf(linea3, "%4d,%0d", int((fotosHechas - 1) * aRotacion * grados), int((fotosHechas - 1) * aRotacion * (grados*10)) % 10); // Refresca el movimiento rotatorio girado
    ponLinea(8, 2, 3);
  }

  // Controlo el modo de ciclo, comprobando si se ha movido el joystick

  if (bmasmas) {
    modoCiclo = 0; // Modo normal de funcionamiento
    sprintf(linea1, " Modo fotos + mov.  ");
    ponLinea(0, 0, 1);
  }

  if (bupup) {
    modoCiclo = 2; // Modo 2, no mueve el motor pero todo lo demas igual
    sprintf(linea1, " Modo SIN movimiento");
    ponLinea(0, 0, 1);
  }

  if (bdowndown) {
    modoCiclo = 1;   // Modo 1, no hace fotos pero todo lo demas igual
    sprintf(linea1, " Modo SIN fotos     ");
    ponLinea(0, 0, 1);
  }

  // El programa esta funcionando, tiene que hacer una foto y esperar el tiempo establecido en pausa

  fotosHechas++;         // Incrementa el num de fotos hechas

  for (int i = 0; (i < fCiclo && digitalRead(J_BTN)); i++) {   // Ejecuto el ciclo de disparar la camara las veces establecidas en fCiclo

    sprintf(linea4, "%02d", (i + 1));       // Actualizo el num de foto dentro de cada ciclo
    ponLinea(8, 3, 4);

    if (modoCiclo != 1) disparaCamara();    // Si el modoCiclo != 1, dispara la camara

    delay(pausa * 1000);                    // Me espero el tiempo establecido en segundos

  }

  // ******* Comprueba si se ha llegado al final o si se ha pulsado el boton de parar el programa

  if ((TEST == 0 && digitalRead(FC)) || !digitalRead(J_BTN) || fotosHechas > nFotos) {    // Fin de carrera, boton ó final del ciclo, finaliza el programa

    if (volver && (TEST == 0 && !digitalRead(FC))) {  // Si debe volver a su posicion inicial y NO estaba pulsado el final de carrera

      if (!tipoMov) pulsos = dPaso;    // En movimientos lineales, siempre envio los pulsos seleccionados en dPaso, la distancia visible es la que cambia segun el tipo de motor
      else pulsos = aRotacion;

      // llevo el carril a la posicion original, moviendolo en sentido contrario los pasos que se movio

      if (autoEN == 2) digitalWrite(EN, 0);   // Si es necesario activo el driver del motor

      for (int i = 0; i < totalPasos; i++) {
        if (cuantosPasos <= 0)                // Si es modo 2, el motor debe moverse en sentido contrario al normal ya que lo hemos colocado en el final del recorrido
          mueveMotor(pulsos, sentido);
        else if (cuantosPasos > 0)
          mueveMotor(pulsos, !sentido);
      }

    }

    funcionando = menuIn = 0;                 // Reestablece todo y pone el menu por defecto

    if (cuantosPasos != 0) nFotos = nFotosM1; // Si es necesario, recupera el num. de fotos que habia programadas

    cuantosPasos = 0;     // Hago un reset de este modo.

    muestraMenu();
    ponCursor();
    ponValores();
    if (autoEN == 1 || autoEN == 2) digitalWrite(EN, 1);  // Desactiva el driver del motor PaP

    // Enciendo el LCD por si se hubiera apagado

    lcd0x27.display(); lcd0x27.backlight();
    lcd0x3F.display(); lcd0x3F.backlight();
    Antes = millis();

    delay(1000);            // Espero un tiempo para que no se vuelva a iniciar el ciclo de disparo

  }

  // **********

  if (funcionando && (dPaso > 0)) {

    // Si tipoMov = 0, es movimiento lineal, usa los pasos especificados en dPaso
    // Si tipoMov = 1, es movimiento rotativo, usa los pasos especificados en aRotacion

    if (!tipoMov) pulsos = dPaso;         // En movimientos lineales, siempre envio los pulsos seleccionados en dPaso, la distancia visible es la que cambia segun el tipo de motor
    else pulsos = aRotacion;              // En movimientos rotatorios enviamos directamente los pasos en aRotacion

    if (modoCiclo != 2) {                 // Si el modoCiclo != 0 , entonces si se puede mover el motor

      if (autoEN == 2) digitalWrite(EN, 0);   // Si es necesario activo el driver del motor

      if (cuantosPasos <= 0)                  // Si es modo 2, el motor debe moverse en sentido contrario al normal ya que lo hemos colocado en el final del recorrido
        mueveMotor(pulsos, !sentido);         // mueve el motor con los pulsos calculados
      else if (cuantosPasos > 0)
        mueveMotor(pulsos, sentido);          // mueve el motor con los pulsos calculados

      if (autoEN == 2) digitalWrite(EN, 1);   // Si es necesario desactivo el driver del motor

      totalPasos++;     // Si no esta activo el final de carrera, incremento el num de pasos totales dados

    }

    // Pone a 0 el num de fotos por ciclo y espera el tiempo de estabilizacion

    sprintf(linea4, "00");
    ponLinea(8, 3, 4);

    delay(tEstab * 1000);

  }

}
