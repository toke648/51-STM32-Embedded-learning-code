#include <REGX52.H>
#include <INTRINS.H>
#include "Delay.h"
#include "Nixie.h"

void main() {
	int i,j,carry;
	carry=8;
	
//	Nixie(7, 9);

    while(1) {
		
		//更好的思路
		for(i=1;i<=99;i++){
			Nixie(6, (i / 10) % 10);
			Delay(20);
			Nixie(6, i / 10);
			Delay(20);
			Nixie(7, i % 10);
			Delay(20);
		}

    }
}

