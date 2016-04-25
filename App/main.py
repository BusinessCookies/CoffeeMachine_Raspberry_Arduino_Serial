import thread
import time
from threading import Timer
import subprocess
import serial

path = "/home/Coffee/Documents/CoffeeMachine2/"

currentuser = OtherClasses.CurrentUser()
Users = CSV.Users()
mycoffeelist = CSV.MycoffeeList()

Normalcoffee=OtherClasses.CoffeePriceClass("Normal")
ExpCoffee = OtherClasses.CoffeePriceClass("Expensive")
MinMonney = OtherClasses.CoffeePriceClass("MinMonney")


def UpdateDB():
    print time.time()
    thread.start_new_thread(subprocess.call, (["sudo","sh", "~/Documents/CoffeeMachine2//update_network.sh"],))
    Timer(600, UpdateDB, ()).start()
    return


def SerialManagement():
    global path
    ser = serial.Serial('/dev/tty0', 9600, timeout=1)#, bytesize=EIGHTBITS, stopbits=STOPBITS_ONE)
    #try: 
    #    ser.open()
    #except Exception, e:
    #    print "error open serial port: " + str(e)
    #exit()
    if ser.isOpen():
        try:
            ser.flushInput() #flush input buffer, discarding all its contents
            ser.flushOutput()#flush output buffer, aborting current output 

                     #and discard all that is in buffer
            i=0
            InputInfo[0] = b"1"
            while InputInfo[i] != b"0": #ASCII char 0 as end of receiving datas
                if i==0:
                    InputInfo[i] = ser.read()
                    ser.write(b"6") #(ASCII Ack char)
                    i += 1
                    pass
                else:
                    InputInfo.append(ser.read())
                    ser.write(b"6") #(ASCII Ack char)
                    i+=1

        #*************************** TODO ******************************#
        #here add InputInfo in date.csv
            
            UID = InputInfo[:-1]
            TypeOfCoffee = InputInfo[-1]
            currentuser.UID = UID
            
            Users.DelUserList()
            Users.importFromCsvFile()
            mycoffeelist.DelMycoffeeList()
            mycoffeelist.importFromCsvFile()
            for row in Users.users:
                if str(currentuser.UID) == row[3].strip():
                    if  row[1]=='ja':
                        currentuser.admin=True
                    else:
                        currentuser.admin=False
                    currentuser.money=row[2]
                    print currentuser.money
                    if float(currentuser.money) < MinMonney.val:
                        if currentuser.admin == False:
                            return
                    for row1 in mycoffeelist.Mycoffee:
                        if currentuser.UID == row1[0]:
                            currentuser.beans=row1[1]
                            currentuser.water=row1[2]
                            return
                    mycoffeelist.setDefaultuser(currentuser)
                    currentuser.beans='3'
                    currentuser.water='180'
                    return
                #Unregistered customer, should go here as it's already checked in the arduino program
            return
        
        
            #*************************** TODO ******************************#
            #here add InputInfo in date.csv
        
            coffee= CSV.Coffee([time.strftime('%Y%m%d%H%M%S'),currentuser.UID,TypeOfCoffee])
            coffee.WriteInCSV()
            coffee.DelLastCoffee()
  
            #*************************** TODO ****************************#
            #Here, locally modifie admin.csv to lower money of coffee taken
            
            if currentuser.admin == True:
                ad='ja'
            else:
                ad="nein"
            
            if currentuser.monney<0:
                price=Expcoffee.val
            else:
                price=Normalcoffee.val
            
            Users.SetUser([currentuser.UID,ad, str(float(currentuser.money)-price), currentuser.pin])
           
            #prepare data to send
            files = ["admin.csv", "ExpCoffee.txt", "NormCoffee.txt", "updated_label.txt", "MinMonney.txt", ""]
            
            
            
            
              #*******************************************************#
                #Here, we send data to the arduino via serial communication          
            
            DataToSend = ""
            for i in files: 
                pathToFile = path + i
                f = open(pathToFile, "rb")
                DataToSend += f.read()
                f.close()
                DataToSend += b"0" #end of file
            DataToSend += b"4" #end of communication
            
            ser.write(DataToSend[0])
            i=1
            while ser.read() == b"6" & DataToSend[i] != b"4":
                ser.write(DataToSend[i])
                i+=1

            
        except Exception, e1:
            print "error communicating...: " + str(e1)
        else:
            print "cannot open serial port "
            SerialManagement()
        
if __name__ == '__main__':
    #Timer(1, UpdateDB, ()).start()
    SerialManagement()