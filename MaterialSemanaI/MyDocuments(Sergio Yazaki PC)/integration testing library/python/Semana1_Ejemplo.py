#-------------------------------------------------------------------------------
# Name:        module1
# Purpose:
#
# Author:      10012325
#
# Created:     15/09/2015
# Copyright:   (c) 10012325 2015
# Licence:     <your licence>
#-------------------------------------------------------------------------------

#Importar libreria para hacer test scripts
import SI_Serial

ECU_00_ID = 0x0160
ECU_01_ID = 0x0120
ECU_02_ID = 0x0132

BAUDRATE = 19200;
SERIAL_PORT = 0;

#Test Case
def main():
    #abrir puerto
    SI_Serial.abrir(SERIAL_PORT ,BAUDRATE)
    #inicializar los bytes no usados
    empty_data = "000000000000"
    #velocidad primero en 0
    velocidad = 0
    #deinifir los 8 bytes de datos. Convertir velocidad a string en hex
    data = format(velocidad,'04x') + empty_data
    #enviar mensaje
    SI_Serial.enviar(ECU_00_ID,8,data,10)

    #esperar 3 segundos
    SI_Serial.esperar(3)

    #cambiar velocidad a 20 km/h
    velocidad = 20
    data = format(velocidad,'04x') + empty_data
    #enviar mensaje
    SI_Serial.enviar(ECU_00_ID,8,data,10)
    #esperar 5 segundos
    SI_Serial.esperar(5)


    #Crear una lista con valores de entrada y ejecutar cada uno cada 5 segundos
    entradas = [0 , 10 , 15, 25, 40, 55, 70, 80, 100]

    for entrada in entradas:
        velocidad = entrada
        data = format(velocidad,'04x') + empty_data
        SI_Serial.enviar(ECU_00_ID,8,data,10)
        print "Enviando velocidad " + str(velocidad) + " km/h"
        SI_Serial.esperar(5)


    #cerrar puerto
    SI_Serial.cerrar()
    pass



if __name__ == '__main__':
    main()
