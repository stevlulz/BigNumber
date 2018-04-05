#include <Arduino.h>
extern "C"{
#include <libs/config.h>
#include <libs/helper.h>
#include <libs/bn.h>
}
#define LINES 3
#define COLUMNS 2


/*
* Included file config.h helper.h bn.h are included into a file called libs which is included with Arduino libraries 
*/
void calculate();


void setup() {
      Serial.begin(9600);
 
     long f = millis();
      calculate();
     long l = millis();
    
     String time = String(l-f);
     Serial.println(time);

}

void loop(){}
void calculate(){
    /*
    * estimator a = (s*part1-(part2*part3))/(s*part4-part5^2)
    */
    bn m[LINES][COLUMNS];
    bn v[LINES];
    bn a,b;

   
    bn part1,part2,part3,part4,part5,tmp,tmp2;
    parseStrToBN(&part1,"0");


    parseStrToBN(&part2,"0");
  
   
   
   
    parseStrToBN(&part3,"0");
   
       
    parseStrToBN(&part4,"0");
   
   
   
    parseStrToBN(&part5,"0");       
       
    //Part1
    for (int k = 0; k < LINES ; ++k) {
        for (int i = 0; i < COLUMNS; ++i) {
            uadd(part1,m[k][i],&part1);
        }
        umul(part1,v[k],&part1);
    }
   
    //Part2
    for (int k = 0; k < LINES ; ++k){
        for (int i = 0; i < COLUMNS; ++i) {
            uadd(part2,m[k][i],&part2);
        }
    }


    //Part3
    for (int j = 0; j <LINES ; ++j) {
        uadd(part3,m[j][COLUMNS-1],&part3);
    }



    //Part4
    for (int k = 0; k < LINES ; ++k) {

        for (int i = 0; i < COLUMNS; ++i) {
            umul(m[k][i],m[k][i],&tmp);
            uadd(part4,tmp,&part4);
        }
    }

    //Part5

    for (int k = 0; k < LINES ; ++k) {
        for (int i = 0; i < COLUMNS; ++i) {
            umul(m[k][i],m[k][i],&tmp);
            uadd(part5,tmp,&part5);
        }
    }
    umul(part5,part5,&part5);


    // a = (s*part1-(part2*part3))/(s*part4-part5);

    umul(part2,part3,&tmp);
    sub(part1,tmp,&tmp);

    sub(part4,part5,&tmp2);



    bc_divide(tmp,tmp2,&a);

}

