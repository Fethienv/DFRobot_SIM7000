 /*
  * File  : DFRobot_SIM7000_MQTT.ino
  * Power : SIM7000 needs 7-12V DC power supply
  * Brief : This example verify MQTT send verification
  *         With initialization completed, we connect to iot.dfrobot.com.cn
  *         Then send data to a topic
  *         Thus we finished the MQTT send verification 
  * Note  : If you use Mega please connect PIN8 PIN10 and set PIN_RX = 10
  */

#include <Wire.h>
#include <DFRobot_SIM7000.h>

//Login website to register an account ,fill the following information based on your account
#define serverIP        "iot.dfrobot.com.cn"
#define IOT_CLIENT      " CLIENT NAME "
#define IOT_USERNAME    " USER   NAME "
#define IOT_KEY         " PASSWORD    "
#define IOT_TOPIC       " TOPIC       "

#define PIN_TX          7
#define PIN_RX          8

SoftwareSerial          mySerial(PIN_RX,PIN_TX);
DFRobot_SIM7000         sim7000;

void setup(){
    int signalStrength;
    Serial.begin(115200);
    delay(2000);
    sim7000.begin(mySerial);
    delay(5000);

    Serial.println("Turn ON SIM7000......");
    if(sim7000.turnON()){                                        //Turn ON SIM7000
        Serial.println("Turn ON !");
    }

    Serial.println("Set baud rate......");
    if(sim7000.setBaudRate(19200)){                              //Set baud rate from 115200 to 19200
        Serial.println("Set baud rate:19200");
    }else{
        Serial.println("Faile to set baud rate");
        return;
    }

    Serial.println("Check SIM card......");
    if(sim7000.checkSIMStatus()){                                //Check SIM card
        Serial.println("SIM card READY");
    }else{
        Serial.println("SIM card ERROR");
        return;
    }

    Serial.println("Set net mode......");
    if(sim7000.setNetMode(NB)){                                  //Set net mod NB-IOT
        Serial.println("Set NB-IOT mode");
    }else{
        Serial.println("Fail to set mode");
    }

    Serial.println("Get signal quality......");
    delay(500);
    signalStrength=sim7000.checkSignalQuality();                 //Check signal quality from (0-30)
    Serial.print("signalStrength =");
    Serial.println(signalStrength);
    delay(500);

    Serial.println("Attaching service......");
    if(sim7000.attacthService()){                                //Open the connection
        Serial.println("Attach service");
    }else{
        Serial.println("Fail to Attach service");
        return;
    }
    delay(200);
}

void loop(){
    char  sendData[100];
    Serial.print("Connect to :");
    Serial.println(serverIP);
    if(sim7000.openNetwork(TCP,serverIP,1883)){                  //Connect to server
        Serial.println("Connected !");
    }else{
        Serial.println("Failed to connect");
        return;
    }
    delay(200);

    Serial.print("Connect to : ");
    Serial.println(IOT_USERNAME);
    if(sim7000.MQTTconnect(IOT_CLIENT,IOT_USERNAME,IOT_KEY)){    //MQTT connect request
        Serial.println("Connected !");
    }else{
        Serial.println("Failed to connect");
        return;
    }
    delay(200);

    Serial.println("Input data : ");
    readSerial(sendData);
    Serial.print("Send data : ");
    Serial.print(sendData);
    Serial.println(" ......");
    if(sim7000.MQTTpublish(IOT_TOPIC,sendData)){                 //Send data to topic
        Serial.println("Send OK");
    }else{
        Serial.println("Failed to send");
        return;
    }
    delay(200);

    Serial.println("Close connection......");
    if(sim7000.close()){                                         //Close connection
        Serial.println("Close connection !");
    }else{
        Serial.println("Fail to close connection !");
        return;
    }
    delay(2000);
}

int readSerial(char result[]){
    int i = 0;
    while(1){
        while(Serial.available() > 0){
            char inChar = Serial.read();
            if(inChar == '\n'){
                result[i] = '\0';
                Serial.flush();
                return 0;
            }
            if(inChar != '\r'){
                result[i] = inChar;
                i++;
            }
        }
    }
}