/*  BLACK CODE INCLUDE START
    External libraries are worrying!
    Keep it simple and avoid anything non essential or obscure.
    Consider copying functions from libraries into the code so
    we have control. */
#include "mbed.h"
#include "uLCD_4DGL.h"
#include "max32630fthr.h"
#include <stdbool.h>
#include <string>

/*  BLACK CODE INCLUDE END */

/*  BLACK CODE DEFINE START */
#define ONN 0
#define OFFF 1
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
/*  BLACK CODE DEFINE END */

/*  BLACK CODE GLOBAL VAR START */
const int GPS_TIMEOUT=180;            //Wait three minutes maximum for GPS.
char gpsfix_last_utc_time[11] = {0};
char gpsfix_last_utc_date[7] = {0};
char gpsfix_longtitude[12] = {0};
char gpsfix_latitude[12] = {0};
char gpsfix_speed[8] = {0};     //Set but not used
char gpsfix_course[7] = {0};    //Set but not used
char gpsfix_variation[7] = {0}; //Set but not used
char gpsfix_mag_var_ew[1] = {0};//Set but not used
char gpsfix_ns = 0;
char gpsfix_ew = 0;
bool gps_data_present = false;  //If this is false we can't even use stale GPS data.
/*  BLACK CODE GLOBAL VAR END */

/*  BLACK CODE PERIPHERAL INIT START */
uLCD_4DGL uLCD(P3_1,P3_0,P5_2); 
DigitalOut red_led(LED1,1);
DigitalOut green_led(LED2,1);
DigitalOut gps_led(LED3,1); //Blue
AnalogIn joyX (AIN_0);
AnalogIn joyY (AIN_1);
DigitalIn joyButton(P4_0);
//Buttons
DigitalIn redButton (P5_6,PullUp);
DigitalIn blackButton (P5_5,PullUp);
//Keypad
DigitalIn keypadLine2(P3_2);
DigitalIn keypadLine7(P5_0);
DigitalIn keypadLine6(P6_0);
DigitalIn keypadLine4(P3_4);
DigitalOut keypadLine3(P3_5);
DigitalOut keypadLine1(P3_3);
DigitalOut keypadLine5(P5_1);
Serial pc(USBTX, USBRX);
Serial gps(P5_3, P5_4, 9600);
I2C i2c(P5_7,P6_0); // SDA, SCL
/*  BLACK CODE PERIPHERAL INIT END */
int one; 
int two;
int three;
int four;
int five;
int six;
int seven;
int eight;
int nine;  
void home(); 
void CompareKey();
void Getmessage();
void TrackingService();
void Sos();
char ChartoAlpha();
void shortcut();
void gps_final();
void ReadMessage();
void PrintChar(char key);
void tracking_power(bool state); 
int gps_update(void);
void Icon(); 
void DeleteMessage();
void interval();

char amessage[] = "1.This is first\n\n2.This is second\n3.This is third\n" ; 
char sendMessage[10]; 

void gps_power(bool state);

int i, k, pw2 , j,pwcount=0; int check ; int countcheck ; 
char sos[] = "999", entry[10];
char track2[] = "111", entry2[10]; bool track ;
char  getkey()
{
    
    keypadLine3 = 0;
    keypadLine1 = 1;
    keypadLine5 = 1;
    if (!keypadLine2) {
    entry2[pw2] = '1';
    pw2++; 
    wait(.5);
   return '1';
    }
    if (!keypadLine7) {
         entry[pwcount] = '4';
   
    wait(.5);
   
        return '4' ; 
    }
    if (!keypadLine6) {
         entry[pwcount] = '7';
    
    wait(.5);
        return '7';
    }
    if (!keypadLine4) {
         entry[pwcount] = '*';
  
    wait(.5);
       pw2=0;     
   for(j=0; j<=3; j++){
   if(track2[j] != entry2[j]){
    uLCD.cls();
     uLCD.text_string("Invalid\nKeypressed\n", 1, 4, FONT_7X8, GREEN);
     wait(3);
     uLCD.cls();
     for(j=0; j<10; j++)
   entry2[j]=0;
    TrackingService();
     break; 
   }
   if(j==2){
   uLCD.cls();
   track =! track ;
   if ( track == ONN)
   {
    uLCD.text_string("Tracking has\nswitched On!", 1, 4, FONT_7X8, GREEN);
      
     }
    if ( track == OFFF )
    {
        uLCD.text_string("Tracking has\nswitched OFF!", 1, 4, FONT_7X8, GREEN);
    }
    wait(3);
    uLCD.cls();
     for(j=0; j<10; j++)
   entry2[j]=0;
    home();
   break; 
   }
   
  }
  for(j=0; j<10; j++)
  entry2[j]=0;
       return '*';
    }
    keypadLine3 = 1;
    keypadLine1 = 0;
    if (!keypadLine2) {
         entry[pwcount] = '2';
    
  wait(.5);
    
         return '2' ; 
    }
    if (!keypadLine7) {
         entry[pwcount] = '5';
    
   wait(.5);
        return '5';
    }
    if (!keypadLine6) {
    entry2[pwcount] = '8';
    pw2++; 
     wait(.5);
       return '8';
    }
    if (!keypadLine4) {
         entry[pwcount] = '0';
    
   wait(.5);
      return '0';
    }
    keypadLine1 = 1;
    keypadLine5 = 0;
    if (!keypadLine2) {
         entry[pwcount] = '3';
    
    wait(.5);
    
      return '3' ; 
    }
    if (!keypadLine7) {
         entry[pwcount] = '6';

  wait(.5);
        return '6';
    }
    if (!keypadLine6) {
        entry[pwcount] = '9';
    pwcount++; 
  
  wait(.5);
       return '9';
    }
    if (!keypadLine4) {
     entry[pwcount] = '#';
    
     wait(.5);
 
   
     return '#';
   }
    if (blackButton == 0)
 {
          wait(.5);
       pwcount=0;     
for(k=0; k<=3; k++){
   if(sos[k] != entry[k]){
    uLCD.cls();
     uLCD.text_string("Wrong Number", 1, 4, FONT_7X8, GREEN);
     wait(3);
     uLCD.cls();
     for(k=0; k<10; k++)
   entry[k]=0;
     Sos();
     break; 
   }
   if(k==2){
   uLCD.cls();
    uLCD.text_string("SOS calling has\nreached!", 1, 4, FONT_7X8, GREEN);
    wait(3);
    uLCD.cls();
     uLCD.text_string("Requesting for\nyour GPS\ncoordinates.....", 1, 4, FONT_7X8, GREEN);
     wait(3);
    uLCD.cls();
     gps_power(ONN);
     wait(3);
    uLCD.cls();
     for(k=0; k<10; k++)
   entry[k]=0;
    home();
   break;
   }
   
  }
  for(k=0; k<10; k++)
  entry[k]=0;
} 

   return NULL; 
}

  /* 
void ReadMessage()
{
      uLCD.printf("\n1.Hey I have been busy..... ");
     uLCD.printf("\n2.Hi Sir,we would like..... ");
     uLCD.printf("\n3.Darling,Be safe.C you..... ");
     while ( true)
         {   char cut = getkey();
             switch ( cut )
             { 
                case '1' : uLCD.cls(); 
                           uLCD.text_string("Hey I have been\nbusy these weeks\nSorry I cannot meet\nwith you.see you\nwhen you'r back!", 1, 4, FONT_7X8, GREEN);
                           wait (5); 
                           uLCD.cls();
                            ReadMessage();
                           break; 
                
                case '2' : uLCD.cls(); 
                           uLCD.text_string("Hi Sir.\nwe would like\nto confirm for\nyour booking\non 2nd Nov 2017", 1, 4, FONT_7X8, GREEN);
                            wait (5); 
                           uLCD.cls();
                            ReadMessage();
                           break; 
                case '3' : uLCD.cls(); 
                           uLCD.text_string("Darling,\nBe safe.\nWe will join you\non 2nd Nov", 1, 4, FONT_7X8, GREEN);
                            wait (5); 
                           uLCD.cls();
                            ReadMessage();
                           break;
                
                           
                case '#' :
                           uLCD.cls();
                           home();
                           break; 
                           
                           
                           
                case '0' :
                           uLCD.cls();
                        Getmessage(); 
                           break; 
                           
                           
                } 
            }
}
void Deletemessage(char num)
{
    
      uLCD.printf("Please select the no of the message you would like to delete"); 
      wait(3);
      uLCD.cls();
       uLCD.printf("\n1.Hey I have been busy..... ");
     uLCD.printf("\n2.Hi Sir,we would like..... ");
     uLCD.printf("\n3.Darling,Be safe.C you..... ");
   
     
    while( true)
    { num = getkey();
      
    switch (num)
    { 
      case '1' : uLCD.cls();
                  uLCD.printf("\n1.Message has been sucessfully deleted");
                  uLCD.printf("\n1.Hi Sir,we would like..... ");
                  uLCD.printf("\n2.Darling,Be safe.C you..... ");
                  wait(5);
                  uLCD.cls();
                  Getmessage();
                  break; 
      case '2' : uLCD.cls();
                  uLCD.printf("\n1.Message has been sucessfully deleted");
                    uLCD.printf("\n1.Hey I have been busy..... ");
                   uLCD.printf("\n2.Darling,Be safe.C you..... ");
                   wait(5);
                  uLCD.cls();
                  Getmessage();
                  break;
      case '3' : uLCD.cls();
                  uLCD.printf("\n1.Message has been sucessfully deleted");
                   uLCD.printf("\n1.Hey I have been busy..... ");
                      uLCD.printf("\n2.Hi Sir,we would like..... ");
                      wait(5);
                  uLCD.cls();
                  Getmessage();
                 break;
      case '#'  : uLCD.cls();
                 home(); 
    }
    
    }
} 
*/

void ReadMessage()
{
for ( int i = 0 ; i < sizeof(amessage) ; i++)
{
uLCD.putc(amessage[i]); 
}
}


void DeleteMessage()
{

uLCD.printf("\n\nEnter the id of the message to delete:\n\n");
ReadMessage();
while(true)
{
char key = getkey(); 

if ( key)
{
switch ( key) 
{
    case '1' : for ( int j = 0 ; j < 16 ; j++)
              {   amessage[j] = ' ' ;  }
              uLCD.cls();
               uLCD.text_string("Message has been\nsuccessfully\ndeleted!.....", 1, 4, FONT_7X8, GREEN);
               wait(3);
              uLCD.cls();
              ReadMessage();
              wait(3);
              uLCD.cls();
              Getmessage();
              break; 
    case '2' : for ( int j = 16 ; j < 33 ; j++)
              {   amessage[j] = ' ' ;  }
               uLCD.cls();
                uLCD.text_string("Message has been\nsuccessfully\ndeleted!.....", 1, 4, FONT_7X8, GREEN);
               wait(3);
              uLCD.cls();
               
              ReadMessage();
               wait(3);
               uLCD.cls();
              Getmessage();
              break;
    case '3' :  for ( int j = 33 ; j < 53  ; j++)
              {   amessage[j] = ' ' ;  }
               uLCD.cls();
                uLCD.text_string("Message has been\nsuccessfully\ndeleted!.....", 1, 4, FONT_7X8, GREEN);
               wait(3);
              uLCD.cls();
                 ReadMessage();
                  wait(3);
                  uLCD.cls();
              Getmessage();
              break;
    case '#' : uLCD.cls();
               home(); 
    case '0' : uLCD.cls();
               Getmessage();

}
}
}

}

 
  char getkey2(void)
{
    keypadLine3 = 0;
    keypadLine1 = 1;
    keypadLine5 = 1;
    if (!keypadLine2) {
      one++;
      wait(.5);
        
        return '1';
        
    }
    if (!keypadLine7) {
        four++; 
        wait(.5);
        return '4';
    }
    if (!keypadLine6) {
        seven++; 
        wait(.5);
        return '7';
    }
    if (!keypadLine4) {
        
        wait(.5);
        return '*';
    }
    keypadLine3 = 1;
    keypadLine1 = 0;
    if (!keypadLine2) {
        
        two++; 
        wait(.5);
        
        return '2';
        
       
       
        
    }
    if (!keypadLine7) {
        five++;
        wait(.5);
        return '5';
    }
    if (!keypadLine6) {
        eight++;
        wait(.5);
        return '8';
    }
    if (!keypadLine4) {
        wait(.5);
          
        return '0';
    }
    keypadLine1 = 1;
    keypadLine5 = 0;
    if (!keypadLine2) {
        three++;
        wait(.5);
        return '3';
    }
    if (!keypadLine7) {
        six++; 
        wait(.5);
        return '6';
    }
    if (!keypadLine6) {
        nine++;
        wait(.5);
        return '9';
    }
    if (!keypadLine4) {
        
        wait(.5);
        return '#';
    }
     
  
         
    return NULL;
}


char ChartoAlpha()
{
     char num2 ; 
      if ( one == 1 )
     { 
       while ( true)
       { 
          num2 = getkey2();
          switch(num2)
        {  case '*' : 
           one = 0 ; 
           return 'S'; 
           
          case '0' : 
           one = 0 ; 
           return ' ';    
          case '#' : 
          one = 0 ; 
          return 'Z';    
         
        }
       }
      
    }
    
     if ( two == 1 )
     { 
       while ( true)
       { 
          num2 = getkey2();
          switch(num2)
        {  case '*' : 
           two = 0 ; 
           return 'A'; 
           
          case '0' : 
           two = 0 ; 
           return 'B';    
          case '#' : 
          two = 0 ; 
          return 'C';    
         
        }
       }
      
    }
    if ( three == 1 )
     { 
       while ( true)
       { 
          num2 = getkey2();
          switch(num2)
        {  case '*' : 
           three = 0 ; 
           return 'D';
          case '0' :
          three = 0 ; 
           return 'E';
          case '#' : 
          three = 0 ; 
          return 'F';
           
        }
       }
    }
    if ( four == 1 )
     { 
       while ( true)
       { 
          num2 = getkey2();
          switch(num2)
        {  case '*' :
         four = 0 ; 
         return 'G';
          case '0' :
           four = 0 ; 
           return 'H';
          case '#' :
          four = 0 ; 
           return 'I';
            
        }
       }
    }
    if ( five == 1 )
     { 
       while ( true)
       { 
          num2 = getkey2();
          switch(num2)
        {  case '*' : 
        five = 0 ; 
        return 'J';
          case '0' :
           five = 0 ; 
           return 'K';
          case '#' :
          five = 0 ;  
          return 'L';
            
        }
       }
    }
    if ( six == 1 )
     { 
       while ( true)
       { 
          num2 = getkey2();
          switch(num2)
        {  case '*' : six = 0 ; return 'M';
          case '0' : six = 0 ;  return 'N';
          case '#' :six = 0 ;  return 'O';
            
        }
       }
    }
    if ( seven == 1 )
     { 
       while ( true)
       { 
          num2 = getkey2();
          switch(num2)
        {  case '*' : seven = 0 ; return 'P';
          case '0' : seven = 0 ; return 'Q';
          case '#' : seven = 0 ; return 'R';
        
            
        }
       }
    }
    if ( eight == 1 )
     { 
       while ( true)
       { 
          num2 = getkey2();
          switch(num2)
        {  case '*' : eight = 0 ;  return 'T';
          case '0' : eight = 0 ;return 'U';
          case '#' :eight = 0 ; return 'V';
            
        }
       }
    }
    if ( nine == 1 )
     { 
       while ( true)
       { 
          num2 = getkey2();
          switch(num2)
        {  case '*' : nine = 0 ; return 'W';
          case '0' : nine = 0 ;  return 'X';
          case '#' : nine = 0 ; return 'Y';
            
        }
       }
    }
    
    return NULL; 
}
         
    
 void shortcut()
{
           uLCD.printf("\n Press 1 to call help!");
         uLCD.printf("\n Press 2 to say you are safe!");
         uLCD.printf("\n Press 3 to request for your GPS co-ordinates!");
         
         while ( true)
         {   char cut = getkey();
             switch ( cut )
             { 
                case '1' : uLCD.cls(); 
                           uLCD.text_string("Calling Help\nIt will\narrive in\n30-45mins!", 1, 4, FONT_7X8, GREEN);
                           wait (5); 
                           uLCD.cls();
                            shortcut();
                           break; 
                
                case '2' : uLCD.cls(); 
                           uLCD.text_string("Thank you.\nIf you need\nany help,\nPress 999 for \nSOS!", 1, 4, FONT_7X8, GREEN);
                            wait (5); 
                           uLCD.cls();
                            shortcut();
                           break; 
                case '3'  :  uLCD.cls(); 
                             TrackingService();
                           
                case '#' :
                           uLCD.cls();
                           home();
                           break; 
                           
                           
                case '0' :
                           uLCD.cls();
                        Getmessage(); 
                           break; 
                           
                           
                } 
     }
     }

void Getmessage()
{
     uLCD.printf("\n1.Type message"); 
     uLCD.printf("\n2.Short-cut message");
     uLCD.printf("\n3.Read message");
     uLCD.printf("\n4.Delete message");  
     
     char key = getkey();
     
uLCD.printf("\n\n Press # to go back to home page");
   while ( true) 
    { 
      char keyy = getkey();
     
      if ( keyy  == '#' )
       {  uLCD.cls(); 
           home(); } 
           
       if ( keyy  == '0' )
       {  uLCD.cls(); 
           Getmessage(); } 
           
          if ( keyy  == '1' )
     {    
          uLCD.cls(); 
          uLCD.printf("\n Type your message and joystick to send.\n");
          while (true)
          { 
              bool press; 
               char keypressed = getkey2();
               if (keypressed)
                {  press = ONN ;}  
             char alpha =  ChartoAlpha();
              uLCD.putc(alpha); 
           
             if ( joyButton == 0  )
        { 
        
           
        uLCD.cls(); 
         uLCD.text_string("Sending\nMessage.......", 1, 4, FONT_7X8, GREEN);
        uLCD.text_width(4); //4X size text
       uLCD.text_height(4);
       uLCD.color(RED);
       for (int i=10; i>=0; --i) {
        uLCD.locate(1,2);
        uLCD.printf("%2D",i);
        wait(.5);
          }
       uLCD.cls();
       
        if (press == ONN )   
     {   
       uLCD.text_string("Your message\nhas been\nsucessfully sent!", 1, 4, FONT_7X8, GREEN);
       wait(2);
       uLCD.cls();
       home();
       } 
       
       else 
           {
              uLCD.text_string("You haven't\ntyped anything!", 1, 4, FONT_7X8, GREEN);
              wait(2);
              uLCD.cls();
             uLCD.text_string("Your message\nhas failed\nto send!", 1, 4, FONT_7X8, GREEN);
             wait(2);
           uLCD.cls();
             Getmessage(); 
            }
     
       }
            
            
           } 
       }
     if ( keyy  == '2' )
     {   uLCD.cls(); 
        shortcut(); 
     }
     
     if ( keyy  == '3' )
     {   uLCD.cls(); 
         ReadMessage();
     }
     if ( keyy  == '4' )
     {   uLCD.cls(); 
        
         DeleteMessage();
     }
      
           
    }  

}




void TrackingService()
{
    
    if ( track == ONN )
    {
    
     Icon(); 
    uLCD.printf("\n\n\nTracking Service is On!");
    wait(3);
    uLCD.cls();
     uLCD.text_string("Requesting for\nyour GPS\ncoordinates.....", 1, 4, FONT_7X8, GREEN);
     wait(3);
    uLCD.cls();
     gps_power(ONN);
     wait(3);
    uLCD.cls();
      uLCD.printf("\n\n\nGPS fix will be checking at every %d interval!", check);
    uLCD.printf("\n\n Press 111 and * to turn off Tracking Service! \n\n Press # to go back to home page");
   
    
           
    }
    if ( track == OFFF )
    {
         
     gps_power(OFFF); 
         uLCD.printf("\n\n\nTracking Service is Off!Press 111 and * to turn on!");
    uLCD.printf("\n\n Press # to go back to home page");
    }
         
    
    
    while ( true) 
    { 
      char keyy = getkey();
      uLCD.putc(keyy); 
      if ( keyy  == '#' )
       {   uLCD.cls(); 
          home(); } 
    }  

}

void Sos()
{  
   k = 0 ; 
    uLCD.printf("\n Press 999 and black button for SOS service! ");
    
    uLCD.printf("\n\n Press # to go back to home page\n\n");
 while (true)
 { 
     char key = getkey();
     uLCD.putc(key); 
     
    if ( key == '#' )
      {   uLCD.cls(); 
          home(); } 
   
}  

}

void GPS()
{
     gps_power(ONN);
     uLCD.printf("\nYour co-ordinates now is X=%i Y=%i ", int(joyX.read_u16()), int(joyY.read_u16()));
   
  
   uLCD.printf("\n\n Press # to go back to home page");
   while ( true) 
    { 
      char keyy = getkey();
     
      if ( keyy  == '#' )
       {  uLCD.cls();  
         home(); } 
    } 
}

void Onoff()
{
   uLCD.printf("\nPress Red Button to power off device");
    
    
    uLCD.printf("\n\n Press # to go back to home page");
   while ( true) 
    { 
      char keyy = getkey();
        if (redButton == 0)
       {   uLCD.cls(); 
         uLCD.text_string("Shutting\nDown.......", 1, 4, FONT_7X8, GREEN);
          wait(3 );
          uLCD.reset();
          
        } 
          
      if ( keyy  == '#' )
       {   uLCD.cls(); 
          home(); } 
          
      
    } 
}

void Icon()
{
   uLCD.filled_circle(115 , 10 , 5 , 0xFF00FF);
}

void home()
{
    countcheck++;
    
  
       uLCD.printf("\nHello\nWelcome to SatChat.\n"); //Default Green on black text
     uLCD.printf("\n1.Message");
     uLCD.printf("\n2.Call for help(999)");
      uLCD.printf("\n3.Tracking Service");
      uLCD.printf("\n4.Request GPS co-ordinates");
      uLCD.printf("\n5.Power on/off");
 if ( track == ONN ) 
    {
    
     Icon(); 
    }
 
  
   int Y=  int(joyY.read_u16());
// uLCD.printf("%d",X); 
      while(true)
 {     
           gps_led= !gps_led ; 
        //uLCD.printf("\nJoystick: X=%i Y=%i ", int(joyX.read_u16()), int(joyY.read_u16()));
        if (joyButton==0){
            uLCD.printf("Joystick Button pressed");
        }
      
         char keypressed = getkey();
       if (keypressed == '1')
        { 
            uLCD.cls(); 
             Getmessage();
        }
         if (keypressed == '2')
        { 
         uLCD.cls(); 
            Sos();
        }
       if (keypressed == '3')
        { 
           uLCD.cls(); 
           
           TrackingService();
           
        }
        
         if (keypressed == '4')
        { 
          uLCD.cls(); 
             GPS();
        }
         if (keypressed == '5')
        { 
          uLCD.cls(); 
             Onoff();
        }
         if (redButton == 0)
       {   uLCD.cls(); 
         uLCD.text_string("Shutting\nDown.......", 1, 4, FONT_7X8, GREEN);
          wait (3);
          uLCD.reset();
          
        } 
        if (blackButton == 0){
               uLCD.printf(",Black button pressed");
               uLCD.cls();
             
        }
        
        
        
        int X = int(joyX.read_u16()) ; 
 
    /*   if ( X != 26585 )
       {
           uLCD.cls(); 
           uLCD.text_string("Your location\nhas changed.....", 1, 4, FONT_7X8, GREEN);
          wait(3);
          uLCD.cls();
          uLCD.text_string("Tracking\nyour location", 1, 4, FONT_7X8, GREEN);
          wait(3);
          uLCD.cls();
          uLCD.text_string("Tracking is\nsuccessful", 1, 4, FONT_7X8, GREEN);
            wait(3);
          uLCD.cls();
          home();
        
         }*/
  
      
     }
     
     
   
}

void gps_power(bool state)
/*  BLACK CODE
    MAX32630FTHR routine to control the output of the 3v3 line.
    This is achieved by issuing commands to the MAX14690 controller.
    In this case when the GPS is shutdown we clear any serial
    data to avoid issues with mbeds buggy serial code  */
{
    //char    data[2];
  //  data[0] = 0x16;     //MAX14690 LDO3cfg register
    uLCD.printf("GPS Power:");
    if (state == ONN) {
   //   data[0] = 0xE2; //Enable LDO3
    //  i2c.write( 0x50, data, 2 );
    //  gps_led=ONN;
        uLCD.printf("ON\n\r");
    } else {
    //data[0] = 0xE0; //Disable LDO3
  //  i2c.write( 0x50, data, 2 );
     // gps_led=OFFF;
        while (gps.readable()) {
                char dummy = gps.getc();    //Empty serial buffer because overflows reveal MBED bugs :-(
        }
        uLCD.printf("OFF\n\r");
    }
}

int get_epoch_from_last_gps_time(void)
/*  BLACK CODE
    Given appropriate global char arrays of time and date information,
    return a unix time epoch.
*/
{
    struct tm t;
    time_t epoch;
    char two_char_str[3] = {0};
    memcpy(two_char_str, gpsfix_last_utc_date+4, 2);
    t.tm_year = atoi(two_char_str)+100;         //Years since 1900
    memcpy(two_char_str, gpsfix_last_utc_date+2, 2);
    t.tm_mon = atoi(two_char_str)-1;            // Month, 0 - jan gpsfix_last_utc_date
    memcpy(two_char_str, gpsfix_last_utc_date, 2);
    t.tm_mday = atoi(two_char_str);             // Day of the month gpsfix_last_utc_date
    memcpy(two_char_str, gpsfix_last_utc_time, 2);
    t.tm_hour = atoi(two_char_str);
    memcpy(two_char_str, gpsfix_last_utc_time+2, 2);
    t.tm_min = atoi(two_char_str);
    memcpy(two_char_str, gpsfix_last_utc_time+4, 2);
    t.tm_sec = atoi(two_char_str);
    t.tm_isdst = 0;        // Is DST on? 1 = yes, 0 = no, -1 = unknown
    epoch = mktime(&t);
    return epoch;
    //BLACK CODE
}

int gps_update(void)
/*  BLACK CODE
    gps_update
    Reads NMEA data from a serial interface defined as "gps".
    The function waits for a valid $GPRMC sentence. It then decodes the sentence and populates the
    following global variables which are assumed to exist.
    
    gpsfix_last_utc_time[11] = {0}; char gpsfix_last_utc_date[7] = {0};char gpsfix_longtitude[12] = {0};
    char gpsfix_latitude[12] = {0}; char gpsfix_speed[8] = {0}; char gpsfix_course[7] = {0};
    char gpsfix_variation[7] = {0}; char gpsfix_mag_var_ew[1] = {0}; char gpsfix_ns = 0; char gpsfix_ew = 0;
    
    The following are also assumed to be part of the global declarations.
    #define EXIT_SUCCESS 0
    #define EXIT_FAILURE 1
    const int GPS_TIMEOUT=180;
    a gps_power() function that controls power to the GPS unit.
    
    If the function is successful it returns a 0.  If a valid fix is not obtined within the GPS_TIMEOUT 
    period a 1 is returned.
    
    The code has been tested with a uBlox 6M but other GPS units may work.
    The code is deliberately blocking as the mbed OS seems to crash on serial interrupts and buffer overflow.
    The serial port is continuously read while waiting for a fix. Once a fix is obtained or a timeout occurs
    the GPS is powered down and remaining data read out of the buffer.
*/
{
    gps_power(ONN);
    time_t gps_on_time = time(NULL);    //Start time for GPS timeout calculation.
    bool wait_for_fix = true;           //Set this to false once a fix is obtained.
    while (wait_for_fix) {              //Keep monitoring the GPS until we get a fix.
        if ((time(NULL) - gps_on_time) > GPS_TIMEOUT) {
            gps_power(OFFF);
            return EXIT_FAILURE;        //Return an error if the GPS takes too long for a fix.
        }
        int checksum = 0;
        char nmea_sentence[83] = {0};   //NMEA length max is 82 + 1 terminator. Fill with NULL terminators to save doing it later.
        while (gps.getc()!='$');        //wait for start of sentence
        int nmea_index = 0;
        nmea_sentence[nmea_index] = '$';    //Manually insert the '$' because we don't want it included in the checksum loop
        char nmea_char = gps.getc();        //get sentence first char from GPS
        while (nmea_char != '*') {          //Loop, building sentence and calc'ing CS until a * is seen
            checksum ^= nmea_char;          //Calc checksum as we read sentence
            if ((nmea_sentence[nmea_index] == ',')&&(nmea_char == ',')) {
                nmea_sentence[++nmea_index] = ' ';      //Pad consecutive comma with a space to make it possible to use strtok with empty values
            }
            nmea_sentence[++nmea_index] = nmea_char;    //build the sentence with the next character
            if (nmea_index > 81) {
                nmea_index=81;          //Don't overflow sentence buffer
            }
            nmea_char = gps.getc();     //get next char from GPS
        }
        //Last character was the '*' so next two are CS
        char hex_checksum[3] = {0};
        hex_checksum[0] = gps.getc();
        hex_checksum[1] = gps.getc();
        if (checksum == (int)strtol(hex_checksum, NULL, 16) ) {     //Compare calc and read checksums.
            //Valid sentence so check if it's a GPRMC
            const char gprmc[7] = "$GPRMC";
            char *token;
            token = strtok(nmea_sentence, ",");
            if (strcmp(token,gprmc) == 0) {     //GPRMC
uLCD.printf( " %s\n\r", token );  //Get the time
                if (token != NULL) {
                    token = strtok(NULL, ",");
                    if (*token != 32) {         //If there is a time present (anything but a space), record it.
                        //uLCD.printf("Time: %s\n\r",token);
                        gps_led =! gps_led;   //Flash blue LED
                        memcpy(gpsfix_last_utc_time, token, sizeof gpsfix_last_utc_time - 1);
                    }
                }
                if (token != NULL) {
                    token = strtok(NULL, ",");
if (*token == 'V') {
       uLCD.printf("VOID");
} 
                } 
                if (*token == 'A') {                //Is this an 'A'ctive (valid) fix?
                    uLCD.printf("Got a fix\n\r");
                    gps_power(OFFF);                 //Yes - No need for GPS now
                    wait_for_fix = false;           //Stop looping now we have a fix.
                    if (token != NULL) {
                        token = strtok(NULL, ",");
uLCD.printf("Latitude: %s\n\r",token);
                        memcpy(gpsfix_latitude, token, sizeof gpsfix_latitude - 1);
                    }
                    if (token != NULL) {
                        token = strtok(NULL, ",");
uLCD.printf("North/South: %s\n\r",token);
                        gpsfix_ns = *token;
                    }
                    if (token != NULL) {
                        token = strtok(NULL, ",");
uLCD.printf("Longitude: %s\n\r",token);
                        memcpy(gpsfix_longtitude, token, sizeof gpsfix_longtitude - 1);
                    }
                    if (token != NULL) {
                        token = strtok(NULL, ",");
uLCD.printf("East/West: %s\n\r",token);
                        gpsfix_ew = *token;
                    }
                    if (token != NULL) {
                        token = strtok(NULL, ",");
                        //uLCD.printf("Speed in knots: %s\n\r",token);
                    }
                    if (token != NULL) {
                        token = strtok(NULL, ",");
                        //uLCD.printf("True course: %s\n\r",token);
                    }
                    if (token != NULL) {
                        token = strtok(NULL, ",");
uLCD.printf("Date: %s\n\r",token);
                        memcpy(gpsfix_last_utc_date, token, sizeof gpsfix_last_utc_date - 1);
                    }
                    if (token != NULL) {
                        token = strtok(NULL, ",");
                        //uLCD.printf("Variation: %s\n\r",token);
                    }
                    if (token != NULL) {
                        token = strtok(NULL, ",");
                        //uLCD.printf("Variation East/West: %s\n\r",token);
                    }
                }
            }
        }
    }
    return EXIT_SUCCESS;
    //BLACK CODE
}

void gps_final()
{   /*Set the power button behaviour.
      char    data[2];
      data[0] = 0x1A;     //MAX14690 BootCfg register
      data[1] = 0x30;     //Always-On Mode, off state via PWR_OFF_CMD
      i2c.write( 0x50, data, 2 );
    */
    
    //Set the voltage to 3v3 for the GPS.
    char data[2];
    data[0] = 0x17;     //MAX14690 LDO3Vset register
    data[1] = 0x19;     //3.3V
    i2c.write( 0x50, data, 2 );
    gps_power(OFFF);

    while (1) {
        if (gps_update()==EXIT_SUCCESS) {
            
            gps_data_present = true;
            int gps_epoch = get_epoch_from_last_gps_time();
            set_time(gps_epoch);
            uLCD.printf("Got a GPS fix and time.\n\r");
        } else {
            uLCD.printf("GPS timed out and we have no existing fix.\n\r");
            uLCD.printf("We can send an Iridium packet but coordinates are rough.\n\r");
        }
        time_t seconds = time(NULL);
        //uLCD.printf("Time as a basic string = %s", ctime(&seconds));
        wait(60);
        seconds = time(NULL);

        wait(33);
        seconds = time(NULL);
        uLCD.printf("Time as a basic string = %s", ctime(&seconds));
        wait(60);
    } 
}


void Trace()

{ 
    uLCD.printf("\nTracking Service is On!");
     uLCD.printf("\n\nThis Unit require your GPS fix at every interval");
     wait (3);
     uLCD.cls(); 
     uLCD.printf("\n\nPlesae enter your desirable interval for us to check on you");
     
  
  while ( true)
  {   char mins = getkey(); 
      int min = mins  ; 
      check = min - 48 ; 
     if ( mins)
    {    uLCD.printf("\n\nYou have entered %d mins for the interval",check);
        wait (3);
        uLCD.cls(); 
        home();
    }

     if (redButton == 0)
       {   uLCD.cls(); 
         uLCD.text_string("Shutting\nDown.......", 1, 4, FONT_7X8, GREEN);
          wait(3 );
          uLCD.reset();
          
        } 
        
      
    
 } 
 }



void interval()
{
    
     check = 0 ; 
   
       uLCD.cls();
    uLCD.text_string("Requesting for\nyour GPS\ncoordinates.....", 1, 4, FONT_7X8, GREEN);
     wait(3);
    uLCD.cls();
     gps_power(ONN);
     wait(3);
    uLCD.cls();
     home();
}




main()
{   /*Set the power button behaviour.
      char    data[2];
      data[0] = 0x1A;     //MAX14690 BootCfg register
      data[1] = 0x30;     //Always-On Mode, off state via PWR_OFF_CMD
      i2c.write( 0x50, data, 2 );
    */

    char data[2];
    data[0] = 0x17;     //MAX14690 LDO3Vset register
    data[1] = 0x19;     //3.3V
    i2c.write( 0x50, data, 2 );
     //tracking_power(OFFF);
            
            
    Trace(); 
   
    
    
    while ( true)
    
    {
        if ( joyX > 0.3f || joyY > 0.3f )
       { 
          uLCD.cls();
         uLCD.text_string("Your location\nhas changed.....", 1, 4, FONT_7X8, GREEN);
          wait(3);
          uLCD.cls();
          uLCD.text_string("Tracking\nyour location", 1, 4, FONT_7X8, GREEN);
          wait(3);
          uLCD.cls();
          uLCD.text_string("Tracking is\nsuccessful", 1, 4, FONT_7X8, GREEN);
            wait(3);
          uLCD.cls();
          home();
        }
      
        if (redButton == 0)
       {   uLCD.cls(); 
         uLCD.text_string("Shutting\nDown.......", 1, 4, FONT_7X8, GREEN);
          wait(3 );
          uLCD.reset();
          
        } 
    } 
     
     
   
}  