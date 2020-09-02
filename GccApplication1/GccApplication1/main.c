#include <avr/io.h>
#include <stdint.h>

static volatile char glob_tom, glob_harry, glob_will;   
                                            //Declaring global variables (initialized to 0 at start)    
int main(void){
  uint8_t loc_tom = 5;                    //Declaring and initializing the variable to 5
  uint8_t loc_harry = 300;                //Value of loc_harry will be 44!!!
  //Multiply and modulo operators 
  uint8_t mult = loc_tom * 2;             //Value of mult will be 10
  uint8_t modu = loc_tom % 10;            //Value of modu will be 5
  //Importance of type casting
  float bad_dev = loc_tom / 4;            //Value of bad_dev will be 1!!!
  float good_dev = (float)loc_tom / 4;    //Now good_dev will be 1.25
  uint16_t reslt_is_16bit = (uint16_t)loc_tom * 100 + 100;
                                          //Upcasting loc_tom as (loc_tom * 100) > 255
  //Assigning characters, hex and binary values
  glob_tom = 'A';                         //Value of glob_tom will be 65
  glob_harry = 0x41;                      //Value of glob_harry will be 65
  glob_will = 0b01000001;                 //Value of glob_will will be 65
}