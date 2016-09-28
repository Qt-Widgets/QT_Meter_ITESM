#-------------------------------------------------------------------------------
# Name:        SemanaI_SerialModule
# Purpose:      Provide an interface for Semana I UART porotocol
#
# Author:      10012325
#
# Created:     20/08/2015
# Copyright:   (c) 10012325 2015
# Licence:     Yazaki Service
#-------------------------------------------------------------------------------

#import PySerial for UART communication
import serial
#import time for sleep handling
import time

#-------------------------------------------------------------------------------
#|                                                                             |
#|                          CONSTANST DEFINITIONS                              |
#|                                                                             |
#-------------------------------------------------------------------------------

#Valid hexadecimal characters
HEXA_CHAR_LIST = ['0','1','2','3','4','5','6','7','8','9',
'A','B','C','D','E','F','a','b','c','d','e','f']
#Base time for CAN message scheduling in ms
CAN_BASE_TIME_MS = 10
#Maximum number of bytes in DLC
MAXIMUM_DLC_BYTE = 8
#Number of byte where Data begins in UART Format frame
DATA_BYE_POSITION = 4
#Maximum number of sending retries
MAX_SEND_RETRY = 3
#Timeout for sending retry in seconds
RETRY_TIMEOUT = .3
#Sleep duration while reading confirmation
CONFIRMATION_PERIOD = 0.02
#Mask to trim the value to only one byte
ONE_BYTE_MASK = 0xFF
#Mask to detect a receive command
COMMAND_MASK = 0xB0
#Mask to detect a send confirmation
SEND_CONFIRMATION_MASK = 0xD0
#Command frame byte size
COMMAND_FRAME_SIZE = 14
#Confirmation frame byte size
CONFIRMATION_FRAME_SIZE = 5

#CAN ID Dictonary
#Tuple data structure:
# 0 : Mailbox number
# 1 : Reception is required
# 2 : Timeout required for message reception
# 3 : Is Message currently being sent preiodically

CAN_ID_DICTONARY = {
    "0x0160":(1,False,0,True),
    "0x0093":(2,True,0.5,False),
    "0x0081":(3,True,0.5,False),
    "0x0130":(4,True,0.5,False),
}


#-------------------------------------------------------------------------------
#|                                                                             |
#|                          CLASS DEFINITIONS                                  |
#|                                                                             |
#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------
# className:CAN_MEssage
# purpose: Holds the information for the CAN Message and validate the parameters
#-------------------------------------------------------------------------------
class CAN_MEssage:

    def __init__(self,ID,DLC,Data,Period):

        self.valid = True

        #Confirm a valid CAN ID value. Considering only 11 bit Identifier
        if ID >= 0x0000 and ID < 0x0800:
            self.ID = ID
        else:
            self.ID = ID = 0
            self.DLC = 0
            self.Data = ""
            self.Period = 0
            self.valid = False
            print("CAN ID invalido")
            return

        #Confirm a valid DLC is set
        if DLC >= 0 and DLC <= 8:
            self.DLC = DLC
        else:
            self.ID = ID = 0
            self.DLC = 0
            self.Data = ""
            self.Period = 0
            self.valid = False
            print("Data Length Code invalido")
            return

        #Check that length of strings are similar to DLC defined
        #Check that Data contains valid strings
        Data_list = set(Data)
        if len(Data)/2 == DLC and Data_list.issubset(HEXA_CHAR_LIST):
            self.Data = Data
        else:
            self.ID = ID = 0
            self.DLC = 0
            self.Data = ""
            self.Period = 0
            self.valid = False
            print("Datos invalidos")
            return

        #Check that Period is greater than 0
        if Period >= 0:
            #round down to match the closest Base time multiple
            Period = ( Period / CAN_BASE_TIME_MS ) * CAN_BASE_TIME_MS
            self.Period = Period
        else:
            self.ID = ID = 0
            self.DLC = 0
            self.Data = ""
            self.Period = 0
            self.valid = False
            print("Periodo invalido")
            return

#-------------------------------------------------------------------------------
#|                                                                             |
#|                          PRIVATE FUNCTIONS                                  |
#|                                                                             |
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# functionName: __Init()
# purpose: Setup the initial variables and call the initial required functions
#-------------------------------------------------------------------------------
def __Init():
    #Display the initial text when adding this script
    __DisplayInitialMessage()


#-------------------------------------------------------------------------------
# functionName: __DisplayInitialMessage()
# purpose: Provides a banner message at the initial running
#-------------------------------------------------------------------------------
def __DisplayInitialMessage():
    print("*-----------------------------------------------------------------*")
    print("|                             YAZAKI                              |")
    print("|                              ITESM                              |")
    print("|                            Semana I                             |")
    print("*-----------------------------------------------------------------*")
    print("* Teclear ayuda() para mas detalles")

#-------------------------------------------------------------------------------
# functionName: __parseMessage(CAN_information)
# purpose: Return a byte array with the Frame format for UART message request
#-------------------------------------------------------------------------------
def __parseMessage(CAN_information):
    message_bytes = list()

    #Byte 0 - Command and mailbox

    #Confirm if a mailbox is available for this CAN ID, default mailbox 0
    CAN_ID = "0x" + format(CAN_information[0],'04x')
    if CAN_ID in CAN_ID_DICTONARY:
        message_bytes.append(0xB0 + CAN_ID_DICTONARY[CAN_ID][0])
    else:
        message_bytes.append(0xB0)
    #Byte 1 - Highest byte of CAN ID
    message_bytes.append(CAN_information[0] >> 8)

    #Byte 2 - Lowest byte of CAN ID
    message_bytes.append(CAN_information[0] & ONE_BYTE_MASK)

    #Byte 3 - DLC
    DLC  = CAN_information[1]
    message_bytes.append(DLC)

    #Byte 4 ~ Byte 11 - Data
    byte_counter = 0

    while byte_counter < MAXIMUM_DLC_BYTE :

        #Used bytes would be filled with the Data provided by user
        if byte_counter < DLC:
            init_range = byte_counter * 2
            data_string = CAN_information[2][init_range:init_range + 2]
            data_hex = int(data_string, 16)
            message_bytes.append(data_hex)
        #Unused bytes would be filled up with 0
        else:
            message_bytes.append(0)
        byte_counter += 1

    #Byte 12 - Period
    message_bytes.append(CAN_information[3])

    #Byte 13 - Checksum
    message_bytes.append(sum(message_bytes) & ONE_BYTE_MASK)

    return message_bytes

#-------------------------------------------------------------------------------
# functionName: __waitConfirmation()
# purpose: Wait for RETRY_TIMEOUT time to check if confirmation is received
#-------------------------------------------------------------------------------
def __waitConfirmation():
    isConfirmed = False
    #Mark the initial time to confirm the time gap
    initial_time = time.time()

    #try reading a confirmation each CONFIRMATION_PERIOD while RETRY_TIMEOUT
    while (time.time() - initial_time) < RETRY_TIMEOUT and isConfirmed == False:
       isConfirmed = __readConfirmation()
       time.sleep(CONFIRMATION_PERIOD)

    return isConfirmed

#-------------------------------------------------------------------------------
# functionName: __readConfirmation()
# purpose: read from Serial port and check if confirmation is received
#-------------------------------------------------------------------------------
def __readConfirmation():
    checksum = 0
    isConfirmed = False
    isFrameFormatGood = False
    data_bytes = list()

    #Read all bytes or until confirmation is parsed
    while puertoSerie.inWaiting() > 0 and isFrameFormatGood == False:
        current_byte = __readByte()
        #Look for the begin of the confirmation frame
        if ( current_byte & SEND_CONFIRMATION_MASK ) == SEND_CONFIRMATION_MASK:
            #If confirmation frame is detected read the information
            if puertoSerie.inWaiting() >= CONFIRMATION_FRAME_SIZE - 1:
                data_bytes.append(current_byte)
                byte_count = 0
                #Read all bytes in confirmation frame
                while byte_count < CONFIRMATION_FRAME_SIZE - 1:
                    data_bytes.append(__readByte())
                    byte_count += 1
                isFrameFormatGood = True
    if isFrameFormatGood:
        #Calculate checksum
        checksum = sum(data_bytes[:-1])
        checksum &= ONE_BYTE_MASK
        #If checksum is ok and Code = 1 the message is confirmed
        if checksum == data_bytes[-1] and data_bytes[3] == 1:
            isConfirmed = True
            #flush the input buffer just in case more bytes were received
            puertoSerie.flushInput()

    return isConfirmed

#-------------------------------------------------------------------------------
# functionName: __readByte()
# purpose: return one byte in integer format read from the serial Port
#-------------------------------------------------------------------------------
def __readByte():
    serial_string = puertoSerie.read(1)
    print "0x" + format(int(serial_string.encode('hex'),16),'04x')
    return int(serial_string.encode('hex'),16)

#-------------------------------------------------------------------------------
#|                                                                             |
#|                          PUBLIC FUNCTIONS                                   |
#|                                                                             |
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# functionName: abrir(numero,baudrate)
# purpose: Open a port with the defined number and baudrate
#-------------------------------------------------------------------------------
def abrir(numero,baudrate):
    #if port is already open
    if puertoSerie.isOpen() == True:
        cerrar()
    #Configure the port with the inputed parameters
    puertoSerie.port = numero
    puertoSerie.baudrate = baudrate
    #Configure the port with the default values
    puertoSerie.bytesize = serial.EIGHTBITS
    puertoSerie.parity = serial.PARITY_NONE
    puertoSerie.stopbits = serial.STOPBITS_ONE
    puertoSerie.timeout = 5
    puertoSerie.xonxoff = False
    puertoSerie.rtscts = False
    puertoSerie.writeTimeout = None
    puertoSerie.dsrdtr = False
    puertoSerie.interCharTimeout = None
    #Try to open and provide some feedback
    try:
        puertoSerie.open()
        print("Puerto Serial: " + str(numero) + " abierto correctamente")
    except:
        print("Error de configuracion en el puerto serie")

#-------------------------------------------------------------------------------
# functionName: cerrar()
# purpose: Close the serial port
#-------------------------------------------------------------------------------
def cerrar():
    if puertoSerie.isOpen() == False:
        print("Puerto Serial ya se encuentra cerrado")
    else:
        try:
            puertoSerie.close()
            print("Puerto Serial cerrado correctamente")
        except:
            print("Error al cerrar el Puerto Seial")

#-------------------------------------------------------------------------------
# functionName: esperar(segundos)
# purpose: Wait for the specified n seconds to perfor the next instruction
#-------------------------------------------------------------------------------
def esperar(segundos):
    print("Esperando por " + str(segundos) +" segundos...")
    time.sleep(segundos)

#-------------------------------------------------------------------------------
# functionName: enviar(ID,DLC,Data,Period)
# purpose: Request to send a CAN Message with the specified information
#-------------------------------------------------------------------------------
def enviar(ID,DLC,Data,Period):

    retry_counter = 0
    isConfirmed = False

    CAN_ID = "0x" + format(ID,'04x')

    #Construct and validate CAN Message input
    mensaje = CAN_MEssage(ID,DLC,Data,Period)

    #Parse the information to a byte array to be sent by RS232
    if mensaje.valid == True:
        CAN_information = ID,DLC,Data,Period
        message_bytes = __parseMessage(CAN_information)
        #Send message via Serial
        if puertoSerie.isOpen() == True:
            #resend while no confirmation is received until MAX_SEND_RETRY
            while retry_counter < MAX_SEND_RETRY and isConfirmed == False:
                puertoSerie.write(message_bytes)
                print("Mensaje CAN ID: 0x" + format(ID,'04x') + " enviado" )
                #Check if it is confirmed other wise retry
                isConfirmed = __waitConfirmation()
                retry_counter += 1

            #Provide feedback depending if confirmation was received
            if isConfirmed:
                print("Envio de mensaje CAN ID 0x" + format(ID,'04x') + " confirmado")
            else:
                print("Error de comunicacion con AVR: Confirmacion no recibida")
        else:
            print("Puerto Serial se encuentra cerrado")

        #If it is required for the message to wait for a response
        if CAN_ID_DICTONARY[CAN_ID][1] == True:
            #Wait for the specified amount in the parameters
            #__waitResponse(CAN_ID_DICTONARY[CAN_ID][2])
            algo = 1

    else:
        print("Mensaje " + format(ID,'04x') + " no enviado")


#-------------------------------------------------------------------------------
# functionName: ayuda()
# purpose: Provides a hint about how to use this script
#-------------------------------------------------------------------------------
def ayuda():
    print("___________________________________________________________________")
    print("")
    print("abrir( NumeroPuerto, Baudrate) ")
    print("Descripcion: Abre el puerto serial seleccionado")
    print("NumeroPuerto = Numero de puerto serial")
    print("Baudrate = Velocidad de transimision.")
    print("___________________________________________________________________")
    print("")
    print("cerrar( )")
    print("Descripcion: Cierra el puerto serial y script")
    print("___________________________________________________________________")
    print("")
    print("enviar( CAN_ID, DLC, Data, Periodo )")
    print("Descripcion: Envia el mensaje con los parametros deifnidos")
    print("CAN_ID: Identificador de mensaje de CAN. Rango: 0x0001 ~ 0x07FF")
    print("DLC: Data Lenght Code. Rango: 0 ~ 8")
    print("Data: Datos con informacion a mandar. Formato texto ej. 01EF ")
    print("Periodo: Intervalo en ms para mander mensaje cicliccamente")
    print("     Tiempo minimio = 10 milisegundos")
    print("     Si Periodo = 0 el mensaje se enviara inmediatamente")
    print("___________________________________________________________________")
    print("")
    print("esperar( Segundos )")
    print("Segundos: Tiempo a esperar para ejecutar la siguiente instruccion")
    print("___________________________________________________________________")

#-------------------------------------------------------------------------------
#|                                                                             |
#|                          INITIALIZATION                                     |
#|                                                                             |
#-------------------------------------------------------------------------------
__Init()

puertoSerie = serial.Serial()
#Debug

#abrir(0,19200)
#enviar(0x0160,8, "4200000000000000",0)
#enviar(0x0093,8, "1200000000000000",3)
#enviar(0x0081,8, "3000000000000000",1)
#enviar(0x0002,8,"ABCDEF0123456789",0)
#enviar(0x0003,8,"89ABCDEF00000000",0)
#enviar(0x0001,8,"0123456789ABCDEF",0)
#enviar(0x0002,8,"ABCDEF0123456789",0)
#enviar(0x0003,8,"89ABCDEF00000000",0)
#cerrar()
