//-- Libraries Included --------------------------------------------------------------
  #include <ESP8266WiFi.h>
//------------------------------------------------------------------------------------
  // Define I/O Pins
  #define     LED0      2           // WIFI Module LED
  #define     LED1      D0          // Connectivity With Client #1
  #define     LED2      13          // OUTPUT LED Indicator for debugging
//------------------------------------------------------------------------------------
  // Authentication Variables
  char*       ssid;              // SERVER WIFI NAME
  char*       password;          // SERVER PASSWORD
//------------------------------------------------------------------------------------
  #define     MAXSC     6           // MAXIMUM NUMBER OF CLIENTS

  WiFiServer  OneServer(9001);      // THE SERVER AND THE PORT NUMBER
  WiFiClient  OneClient[MAXSC];     // THE SERVER CLIENTS
//====================================================================================

  void setup()
  {
    // Setting The Serial Port
    Serial.begin(9600);           // Computer Communication
    
    // Setting The Mode Of Pins
    pinMode(LED0, OUTPUT);          // WIFI OnBoard LED Light
    pinMode(LED1, OUTPUT);          // Indicator For Client #1 Connectivity
    pinMode(LED2, OUTPUT);

    Serial.println("/*********************************SERVER******************************/");
    
    // Print Message Of I/O Setting Progress
    Serial.println();
    Serial.println("I/O Pins Modes Set .... Done");

    // Setting Up A Wifi Access Point
    SetWifi("SERVER", "");
  }

//====================================================================================
  
  void loop()
  {
    
    IsClients();
    
  }

//====================================================================================
  
  void SetWifi(char* Name, char* Password)
  {
    // Stop Any Previous WIFI
    WiFi.disconnect();

    // Setting The Wifi Mode
    WiFi.mode(WIFI_AP_STA);
    Serial.println("WIFI Mode : AccessPoint Station");
    
    // Setting The AccessPoint Name & Password
    ssid      = Name;
    password  = Password;
    
    // Starting The Access Point
    WiFi.softAP(ssid, password);
    Serial.println("WIFI < " + String(ssid) + " > ... Started");
    
    // Wait For Few Seconds
    delay(1000);
    
    // Getting Server IP
    IPAddress IP = WiFi.softAPIP();
    
    // Printing The Server IP Address
    Serial.print("AccessPoint IP : ");
    Serial.println(IP);

    // Starting Server
    OneServer.begin();
    Serial.println("Server Started");
  }

//====================================================================================

  void IsClients()
  {
    if(OneServer.hasClient())
    {
      WiFiClient OneClient = OneServer.available();
      
      if(digitalRead(LED0) == HIGH) digitalWrite(LED0, LOW);
      
      digitalWrite(LED1, HIGH);

      while(1)
      {
              
        //---------------------------------------------------------------
        // If Clients Are Connected 
        //---------------------------------------------------------------
        
        if(OneClient.available())
        {
          // Here We Read The Message
          String Message = OneClient.readStringUntil('\r');
          // Here We Print The Message On The Screen
          Serial.println("Client : "+ Message);
          if(Message == "OneClient LED13 : ON")
          {
             digitalWrite(LED2,HIGH);
          
          // Here We Reply To The Client With A Message
          OneClient.print("Turned ON LED13");
         
          }
          else
          {
             digitalWrite(LED2,LOW);
          // Here We Reply To The Client With A Message
          OneClient.print("Turned OFF LED13");


         
          }
          
          OneClient.flush();
        }
        
        //---------------------------------------------------------------
        // If Clients Are Disconnected
        //---------------------------------------------------------------
        
        if(!OneClient || !OneClient.connected())
        {
          // Here We Turn Off The LED To Indicated The Its Disconnectted
          digitalWrite(LED1, LOW);
          // Here We Jump Out Of The While Loop
          break;
        }

        //----------------------------------------------------------------
        
      }
      
    }
    else
    {
      // This LED Blinks If No Clients Where Available
      digitalWrite(LED0, HIGH);
      delay(250);
      digitalWrite(LED0, LOW);
      delay(250);
    }
  }

//====================================================================================