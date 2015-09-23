#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();

uint8_t keystates[24];
uint8_t keystates_old[24];

uint8_t row_pin[4] = {2, 3, 4, 5};  //read from these
uint8_t col_pin[6] = {7, 8, 9, 10, 12, 13};  //control these

#include "Wire.h"
#include "PanelComponents.h"
#define quantizeSelectorPin A0
PanelSelector quantizeSelector;

uint8_t midiChannel = 1;




void setup()
{
	quantizeSelector.init(quantizeSelectorPin, 255, 0, 16 ); //With max and min ranges, points

  for( int i = 0; i < 24; i++ )
  {
    keystates[i] = 1;
    keystates_old[i] = 1;
  }
  
  //led
  pinMode( 6, OUTPUT );
  
  pinMode(row_pin[0], INPUT_PULLUP);
  pinMode(row_pin[1], INPUT_PULLUP);
  pinMode(row_pin[2], INPUT_PULLUP);
  pinMode(row_pin[3], INPUT_PULLUP);
  pinMode(col_pin[0], OUTPUT);
  pinMode(col_pin[1], OUTPUT);
  pinMode(col_pin[2], OUTPUT);
  pinMode(col_pin[3], OUTPUT);
  pinMode(col_pin[4], OUTPUT);
  pinMode(col_pin[5], OUTPUT);
  
  //Turn on all columns
  digitalWrite(col_pin[0], 1);
  digitalWrite(col_pin[1], 1);
  digitalWrite(col_pin[2], 1);
  digitalWrite(col_pin[3], 1);
  digitalWrite(col_pin[4], 1);
  digitalWrite(col_pin[5], 1);

  
  //Serial.begin(9600);
  MIDI.begin();
}

void loop()
{
  for( int i = 0; i < 32000; i++ )
  {
  }
  
  getstates();
  
  for( int i = 0; i < 24; i++ )
  {
    if(keystates[i] != keystates_old[i])
    {
      if( keystates[i] == 0)
      {
        //write midi
        MIDI.sendNoteOn(i + 48, 127, midiChannel);
        delay(2);
        //Serial.print(i);
        //Serial.print(" = ");
        //Serial.println(keystates[i]);
        
        digitalWrite( 6, 0 );
      }
      else
      {
        //write midi
        MIDI.sendNoteOff(i + 48, 0, midiChannel);
        delay(2);
        //Serial.print(i);
        //Serial.print(" = ");
        //Serial.println(keystates[i]);
        digitalWrite( 6, 1 );
      }
    }
    
    keystates_old[i] = keystates[i];
  }
  
  quantizeSelector.update();
  if( quantizeSelector.serviceChanged() )
  {
	midiChannel = 16 - quantizeSelector.getState();
  }

}

void getstates()
{
  int col = 0;
  int row = 0;
  for( col = 0; col < 6; col++ )
  {
    //lower one of the cols
    digitalWrite(col_pin[col], 0);
    
    for( row = 0; row < 4; row++ )
    {
      //key index = (4 * row) + col
      keystates[((6 * row) + col)] = digitalRead(row_pin[row]);
    }
    
    //set the col back
    digitalWrite(col_pin[col], 1);
  }
}