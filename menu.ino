boolean leeEntradas()
{
  int pulsos;
  byte menuAnterior = modoMenu;   // Guardo la opcion actual del menu por si cambia de pantalla

  if (menuIn == 0) {    // Si NO esta en modo de cambio de parametros o en ciclo se mueve por el menu

    if (bdown | bdowndown) {
      modoMenu = (--modoMenu < 1) ? 24 : modoMenu;
      if (menuAnterior == 1 || menuAnterior == 5 || menuAnterior == 9 || menuAnterior == 13 || menuAnterior == 17 || menuAnterior == 21) muestraMenu();
    }

    if (bup | bupup) {
      modoMenu = (modoMenu++ > 23) ? 1 : modoMenu;
      if (menuAnterior == 4 || menuAnterior == 8 || menuAnterior == 12 || menuAnterior == 16 || menuAnterior == 20 || menuAnterior == 24) muestraMenu();
    }

    if (bjoy) {

      switch (modoMenu) {

        case 1: if (nFotos > 0 || cuantosPasos != 0) {  // Si se ha programado alguna foto, inicia el ciclo programado, activa el driver del motor PaP

            funcionando = fotosHechas = 1;

            totalPasos = 0;                 // Pongo a 0 el total de pasos dados, sirve para volver a la posicion inicial

            if (cuantosPasos != 0) {        // Si funcionamos en el modo 2 ....
              nFotosM1 = nFotos;            // Guardo el num de fotos que habia programadas
              nFotos = nFotosM2;            // Pongo en nFotos las fotos que debe hacer para este modo de funcionamiento
            }

            ponMacro();                     // Muestro la pantalla de datos del ciclo de macro

            delay(pausa * 1000);            // Me espero el tiempo establecido entre foto y foto para estabilizar al haber pulsado el boton del joystick o teclas

            if (autoEN == 1) digitalWrite(EN, 0);   // Habilito el driver del motor solo si el modo autoEN = 1
          }
          else {                                    // Si el numero de fotos = 0, solamente mueve el carril la distancia programada

            if (autoEN == 1 || autoEN == 2) digitalWrite(EN, 0);    // Habilito el driver del motor

            if (!tipoMov) pulsos = dPaso;         // En movimientos lineales, siempre envio los pulsos seleccionados en dPaso, la distancia visible es la que cambia segun el tipo de motor
            else pulsos = aRotacion;              // En movimientos rotatorios enviamos directamente los pasos de aRotacion

            mueveMotor(pulsos, !sentido);         // Ahora mueve el motor con los pasos calculados

            if (autoEN == 1 || autoEN == 2) digitalWrite(EN, 1);    // Deshabilito el motor

          } break;

        case 9:   ponModoU(1); modoMenu = 1; muestraMenu(); break;              // Modo usuario 1

        case 10:  ponModoU(2); modoMenu = 1; muestraMenu(); break;              // Modo usuario 2

        case 11:  ponModoU(3); modoMenu = 1; muestraMenu(); break;              // Modo usuario 3

        case 12:  ponModoU(4); modoMenu = 1; muestraMenu(); break;              // Modo usuario 4

        case 14:  sentido = !sentido; guardaSetup(); muestraMenu(); break;      // Invierte el sentido actual

        case 15:  volver = !volver; guardaSetup(); muestraMenu(); break;        // Establece si vuelve a la posicion original o no

        case 16:  autoEN = (autoEN + 1) % 3;                                    // Establece el activado del motor: 0 - siempre, 1 - en todo el ciclo, 2 - solo para mover el motor
          if (autoEN == 0) digitalWrite(EN, 0);
          else digitalWrite(EN, 1);
          guardaSetup();
          muestraMenu();
          break;

        case 19:  if (pulsosPaP[pulsosPaso] > 8) precision = (precision + 1) % 3; // Ajusta la precision permitida segun el numero de micropasos fijados
          else if (pulsosPaP[pulsosPaso] > 4) precision = (precision + 1) % 2;
          else precision = 0;
          calculaUnPaso();
          calculaCuantosPasos();
          guardaSetup(); muestraMenu(); break;    // Ajusta la precision del movimiento siempre que los micropasos > 8

        case 21:  tipoMov = !tipoMov; guardaSetup(); muestraMenu(); break;      // Selecciona el tipo de movimiento del eje que desplazara: 0 - lineal, 1 = rotativo

        case 24:  unidades = !unidades; 
                  calculaUnPaso(); 
                  calculaCuantosPasos();
                  guardaSetup(); muestraMenu(); break;    // Cambia las unidades: micras o milimetros

        default:  menuIn = 1; if (modoMenu == 2 || modoMenu == 3) if (autoEN == 1 || autoEN == 2) digitalWrite(EN, 0);       // Si estamos en la opcion mover motor habilita el driver del PaP

      }

    }
    else if (modoMenu == 2 && !iniCarrera && !finCarrera) {      // Si estamos en la opcion "Ajustar Posicion" hay que ver si se activan los finales de carrera

      if ((bmenosmenos) && (cuantosPasos == 0)) {               // Joystick izquierda - ir principio del carril

        iniCarrera = 1;
        finCarrera = 0;
        if (autoEN == 1 || autoEN == 2) digitalWrite(EN, 0);

      }
      else if ((bmasmas) && (cuantosPasos == 0)) {          // Joystick derecha - ir al final del carril

        iniCarrera = 0;
        finCarrera = 1;
        if (autoEN == 1 || autoEN == 2) digitalWrite(EN, 0);

      }

    }
    else if (bmasmas) {               // Boton rojo o Joystick a derecha

      switch (modoMenu) {

        case 1:   if (autoEN == 1 || autoEN == 2) digitalWrite(EN, 0);          // Envia una vuelta al motor (pasos por vuelta - pvuelta)
          mueveMotor(pvuelta, !sentido);
          if (autoEN == 1 || autoEN == 2) digitalWrite(EN, 1);
          break;

        case 9:   guardaU(1); defectoU(1); ponMensajeModoU(1); modoMenu = 1; muestraMenu(); break;          // Guarda los valores actuales en el Modo usuario 1

        case 10:  guardaU(2); defectoU(2); ponMensajeModoU(2); modoMenu = 1; muestraMenu(); break;          // Guarda los valores actuales en el Modo usuario 2

        case 11:  guardaU(3); defectoU(3); ponMensajeModoU(3); modoMenu = 1; muestraMenu(); break;          // Guarda los valores actuales en el Modo usuario 3

        case 12:  guardaU(4); defectoU(4); ponMensajeModoU(4); modoMenu = 1; muestraMenu(); break;          // Guarda los valores actuales en el Modo usuario 4

        case 3:  cuantosPasos = nFotosM2 = 0; muestraMenu(); break;        // Pone a 0 el numero de pasos = empezara a mover desde esta posicion

      }

    }
    else if (bmenosmenos) {    // Boton verde o joystick a izquierda

      switch (modoMenu) {

        case 1: reset(); break;     // Hace un reset

      }

    }

  }
  else {

    // comprobar si se ha pulsado el boton para salir de menuIn .....

    if (bjoy) {
      menuIn = moviendoeje = 0;
      if (autoEN == 1 || autoEN == 2) digitalWrite(EN, 1);    // Deshabilita el driver del PaP
    }
    else
      opcionesMenu();     // SE TRATA EL RESTO DE OPCIONES

  } // if menuIn==0 ... else ....

}



void opcionesMenu()     // Compruebo los botones si estoy en modo menuIn=1
{

  if (modoMenu == 2 && cuantosPasos == 0) {    // Opcion Ajustar Posicion - Mueve el rail en pasos grandes o pequeños SI NO ESTA ACTIVA LA OPCION DE CICLO ENTRE PUNTOS

    int valordelayPap = EEPROMReadInt(14);
    moviendoeje = 1;

    // Ahora muevo el motor con el delayPaP preajustado

    if (bmas)             mueveMotor(1, !sentido, pulsosPaP[pulsosPaso] / 2);   // mueve el motor medio paso hacia delante
    else if (bmasmas)     mueveMotor(1, !sentido, pulsosPaP[pulsosPaso]);       // mueve el motor un paso hacia delante
    else if (bmenos)      mueveMotor(1, sentido, pulsosPaP[pulsosPaso] / 2);    // mueve el motor medio paso hacia atras
    else if (bmenosmenos) mueveMotor(1, sentido, pulsosPaP[pulsosPaso]);        // mueve el motor un paso hacia atras
    else if (bup)         mueveMotor(5, sentido);                               // mueve el motor 5 pasos hacia delante
    else if (bupup)       mueveMotor(10, sentido);                              // mueve el motor 10 pasos hacia delante
    else if (bdown)       mueveMotor(5, !sentido);                              // mueve el motor 5 pasos hacia detras
    else if (bdowndown)   mueveMotor(10, !sentido);                             // mueve el motor 10 pasos hacia detras

  }
  else if (modoMenu == 3 && tipoMov == 0) {   // Solo permite poner la posicion final en modo de funcionamiento lineal (no rotativo)

    moviendoeje = 1;

    // Debe colocar el rail en la posicion incial y contamos los pasos que se mueve el motor

    if (cuantosPasos > -9999 && cuantosPasos < 9999) {

      if (bmas) {
        mueveMotor(1, !sentido);   // Movemos de paso en paso
        cuantosPasos ++;
      }
      else if (bmasmas) {
        mueveMotor(2, !sentido);
        cuantosPasos += 2 ;
      }
      else if (bmenos) {
        mueveMotor(1, sentido);
        cuantosPasos --;
      }
      else if (bmenosmenos) {
        mueveMotor(2, sentido);
        cuantosPasos -= 2;
      }

      if (bdown) {
        mueveMotor(10, !sentido);   // Movemos de paso en paso
        cuantosPasos += 5;
      }
      else if (bdowndown) {
        mueveMotor(20, !sentido);   // Movemos de paso en paso
        cuantosPasos += 10;
      }
      else if (bup) {
        mueveMotor(10, sentido);
        cuantosPasos -= 5;
      }
      else if (bupup) {
        mueveMotor(20, sentido);
        cuantosPasos -= 10;
      }

      // Aqui tengo que calcular cuantas fotos debe hacer y asignarlas a nFotosM2, que se vera en la linea de nFotos si cuantosPasos>0

      calculaCuantosPasos();

      if (cuantosPasos == 0) sprintf(linea3, "Fijar Inicio |---| ");
      else {

        if (!unidades) {
          if (cuantosPasos > 0) sprintf(linea3, "|<%4d.%02d u   #%04d", int(pEnteraT), int(pDecimalT), int(nFotosM2));
          else sprintf(linea3, "%4d.%02d u  >| #%04d", int(pEnteraT), int(pDecimalT), int(nFotosM2));         
        }
        else {
          if (cuantosPasos > 0) sprintf(linea3, "|<%4d.%02d mm  #%04d", int(pEnteraT), int(pDecimalT), int(nFotosM2));
          else sprintf(linea3, "%4d.%02d mm >| #%04d", int(pEnteraT), int(pDecimalT), int(nFotosM2));
        }      
      }

      ponLinea(1, 2, 3);

    }

  }
  else if (modoMenu == 4) {           // Indica pasos que se mueve el motor y se calcula la distancia de paso

    if (bmas) dPaso += 1;                 // paso + 0.005  - trabajo en micras, luego se muestra la distancia correcta segun el lead
    else if (bmasmas) dPaso += 5;        // paso + 0.050
    else if (bmenos) dPaso -= 1;
    else if (bmenosmenos) dPaso -= 5;

    if (bup)            dPaso -= 10;
    else if (bupup)     dPaso -= 20;
    else if (bdown)     dPaso += 10;
    else if (bdowndown) dPaso += 20;

    if (!unidades) {
      if ((dPaso*unPaso)>9999) dPaso = 0;   //  En micras, la distancia real del paso es como maximo 9999 u
      if (dPaso<0) dPaso = 9999/unPaso;
    }
    else {
      if ((dPaso*unPaso)>99999) dPaso = 0;   //  En milimetros, la distancia real es como maximo 99 mm
      if (dPaso<0) dPaso = 99999/unPaso;        
    }
      
    calculaUnPaso();
    calculaCuantosPasos();        

  }
  else if (modoMenu == 5) {       // Los botones cambian el numero de fotos. Si fotos = 0, solo avanza el carril una vez la distancia programada.

    if (cuantosPasos == 0) {      // Si estamos en modo de funcionamiento por distancias, no deja modificar el num. de fotos porque se calculan mas abajo

      if (bmas) nFotos++;                     // fotos + 1
      else if (bmasmas) nFotos += 5;          // fotos + 10
      else if (bmenos) --nFotos;              // fotos - 1
      else if (bmenosmenos) nFotos -= 5;      // fotos - 10

      if (bup)            nFotos -= 10;
      else if (bupup)     nFotos -= 25;
      else if (bdown)     nFotos += 10;
      else if (bdowndown) nFotos += 25;

      if (nFotos < 0) nFotos = 9999;
      if (nFotos > 9999) nFotos = 0;

    }

  }
  else if (modoMenu == 6) {                // Establece el numero de fotos por ciclo

    int tmp=fCiclo;     // la necesito entera para poder usar el valor 0

    if (bmas)             tmp++;
    else if (bmasmas)     tmp += 5;
    else if (bmenos)      tmp--;
    else if (bmenosmenos) tmp -= 5;

    if (bup)              tmp -= 10;
    else if (bupup)       tmp -= 20;
    else if (bdown)       tmp += 10;
    else if (bdowndown)   tmp += 20;

    if (tmp > 199) tmp = 1;
    if (tmp < 1) tmp = 199;

    fCiclo = tmp;

  }
  else if (modoMenu == 7) {               // Opcion T.Espera - pausa entre fotos en segundos

    if (bmas)             pausa++;        // pausa + 1 s
    else if (bmasmas)     pausa += 10;    // pausa + 10 s
    else if (bmenos)      --pausa;        // pausa - 1 s
    else if (bmenosmenos) pausa -= 10;    // pausa - 10 s

    if (bup)            pausa -= 50;
    else if (bupup)     pausa -= 100;
    else if (bdown)     pausa += 50;
    else if (bdowndown) pausa += 100;

    if (pausa < 1) pausa = 9999;
    if (pausa > 9999) pausa = 1;

  }
  else if (modoMenu == 8) {             // Tiempo de estabilizacion, tiempo desde que se mueve el motor hasta que se dispara la camara de nuevo

    int tmp = tEstab;

    if (bmas)             tmp++;
    else if (bmasmas)     tmp += 5;
    else if (bmenos)      --tmp;
    else if (bmenosmenos) tmp -= 5;

    if (tmp < 0) tmp = 99;
    if (tmp > 99) tmp = 0;

    tEstab = tmp;

  }
  else if (modoMenu == 13) {                // Establece el lead, o distancia que se mueve por cada vuelta completa del motor paso a paso

    if (bmas)             lead++;
    else if (bmasmas)     lead += 2;
    else if (bmenos)      lead--;
    else if (bmenosmenos) lead -= 2;

    if (bup)            lead -= 4;
    else if (bupup)     lead -= 8;
    else if (bdown)     lead += 4;
    else if (bdowndown) lead += 8;

    if (lead > 100) lead = 2;
    if (lead < 2)  lead = 100;
    
    calculaUnPaso();
    calculaCuantosPasos();

  }
  else if (modoMenu == 17) {                // Establece el numero de pasos para una vuelta del motor (antes solo 200 o 400)

      if (bmas)             pvuelta ++;
      else if (bmasmas)     pvuelta += 8;
      else if (bmenos)      pvuelta --;
      else if (bmenosmenos) pvuelta -= 8;
      else if (bup)         pvuelta -= 16;
      else if (bupup)       pvuelta -= 32;
      else if (bdown)       pvuelta += 16;
      else if (bdowndown)   pvuelta += 32;

      if (pvuelta > 9990) pvuelta = 1;
      if (pvuelta < 1)  pvuelta = 9990;
  
      calculaUnPaso();
      calculaCuantosPasos();
      
  }
  else if (modoMenu == 18) {          // Ajusta el indice del array de pulsos x paso = 1,2,4,8,16,32,64,128

    if (bmas || bmasmas) pulsosPaso = ++pulsosPaso > 7 ? 0 : pulsosPaso;                  // Pulsos x Paso + 1
    else if (bmenos || bmenosmenos) pulsosPaso = pulsosPaso > 0 ? --pulsosPaso : 7;       // Pulsos x Paso - 1

    if (SBB_V4 == 1) ajustaM012();        // Ajusta las salidas digitales para establecer los micropulsos en el driver

    if (pulsosPaP[pulsosPaso] < 16) {     // Si los micropasos son 8 o menos la precision no puede ser > 1.
      precision = 0;
      sprintf(linea3, "x1");
      ponLinea(11, 2, 3);
    }

  }
  else if (modoMenu == 20) {                // Establece los microsegundos que se dan a cada micropulso o pulso del motor paso a paso

    if (bmas) delayPaP++;                   // delayPaP + 1
    else if (bmasmas) delayPaP += 10;       // delayPaP + 10
    else if (bmenos) delayPaP--;            // delayPaP - 1
    else if (bmenosmenos) delayPaP -= 10;   // delayPaP - 10

    if (bup)            delayPaP -= 50;
    else if (bupup)     delayPaP -= 100;
    else if (bdown)     delayPaP += 50;
    else if (bdowndown) delayPaP += 100;

    if (delayPaP < 1) delayPaP = 5000;
    if (delayPaP > 5000) delayPaP = 1;

  }

  else if (modoMenu == 22) {              // Establece los pulsos que se dan para movimientos rotativos. El angulo de rotacion = pulsos * 1.8 ? 0.9. Se calcula al mostrar informacion.

    if (tipoMov) {

      if (bmas)             aRotacion ++;
      else if (bmasmas)     aRotacion += 10;
      else if (bmenos)      aRotacion--;
      else if (bmenosmenos) aRotacion -= 10;

      if (bup)            aRotacion -= 50;
      else if (bupup)     aRotacion -= 100;
      else if (bdown)     aRotacion += 50;
      else if (bdowndown) aRotacion += 100;

      if (aRotacion > 5550) aRotacion = 1;
      if (aRotacion < 1)  aRotacion = 5550;

    }

  }
  else if (modoMenu == 23) {      // Tiempo en el que se apaga el LCD si no se toca ningun boton

    int tiempoLCD_ = tiempoLCD;   // Auxiliar para poder poner valores < 0

    if (bmas)             tiempoLCD_ ++;
    else if (bmasmas)     tiempoLCD_ += 5;
    else if (bmenos)      tiempoLCD_ --;
    else if (bmenosmenos) tiempoLCD_ -= 5;

    if (bup)            tiempoLCD_ -= 5;
    else if (bupup)     tiempoLCD_ -= 10;
    else if (bdown)     tiempoLCD_ += 5;
    else if (bdowndown) tiempoLCD_ += 10;

    if (tiempoLCD_ > 99) tiempoLCD_ = 0;
    if (tiempoLCD_ < 0) tiempoLCD_ = 99;

    tiempoLCD = tiempoLCD_;

  }

  // Si cambia algun valor que debo guardar, guardo los valores establecidos en la EEPROM

  if (modoMenu != 2 && modoMenu != 3) guardaSetup();      // SI ESTO SE ACTIVA EL MOVIMIENTO DEL AJUSTE DE POSICION VA COMO A PAUSAS

}
