/* 
scs.cpp
Copyright (c) 2013 Sarrailh Remi GPL3
http://www.tldrlegal.com/l/GPL3

Pour compiler ce code:
gcc -o scs scs.cpp

Ce programme permet de controler des prises S316-2 et S3063 de chez SCS
http://scs-laboutique.com

Vous pouvez trouver l'emetteur ici
ttp://snootlab.com/composants/147-emetteur-rf-434-mhz.html

// Vous pouvez aussi changer le GPIO de l'emetteur ici:
// Voir http://elinux.org/RPi_Low-level_peripherals
*/

////PARAMETRES //////////////////////////////
int transmitter = 7; // GPIO de l'emetteur
int nbrepeat = 10;   // Nombre de repetition du code
/////////////////////////////////////////////

#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

int  mem_fd;
unsigned char *gpio_mem, *gpio_map;
char *spi0_mem, *spi0_map;
char code[79]; 	// Taille du code

// I/O access
volatile unsigned *gpio;

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

// Code de prise
char list_code[79][79]; // liste des codes a appeller  


// Prototype des fonctions
void setup_io();
void SendCode(char* code);

int main(int argc, char **argv)
{ 

//SCS 3162

//A1
strcpy(list_code[0],"0001001001001101001101001101001101001101001101001101001101001001101101001001000");
strcpy(list_code[1],"0001001001001101001101001101001101001101001101001101001101001001001001101101000");

//A2
strcpy(list_code[2],"0001001001001101001101001101001101001101001101001101001001001101101101001001000");
strcpy(list_code[3],"0001001001001101001101001101001101001101001101001101001001001101001001101101000");

//A3
strcpy(list_code[4],"0001001001001101001101001101001101001101001101001001001101001101101101001001000");
strcpy(list_code[5],"0001001001001101001101001101001101001101001101001001001101001101001001101101000");

//A4
strcpy(list_code[6],"0001001001001101001101001101001101001101001001001101001101001101101101001001000");
strcpy(list_code[7],"0001001001001101001101001101001101001101001001001101001101001101001001101101000");

//B1
strcpy(list_code[8],"0001001101001001001101001101001101001101001101001101001101001001101101001001000");
strcpy(list_code[9],"0001001101001001001101001101001101001101001101001101001101001001001001101101000");

//B2
strcpy(list_code[10],"0001001101001001001101001101001101001101001101001101001001001101101101001001000");
strcpy(list_code[11],"0001001101001001001101001101001101001101001101001101001001001101001001101101000");

//B3
strcpy(list_code[12],"0001001101001001001101001101001101001101001101001001001101001101101101001001000");
strcpy(list_code[13],"0001001101001001001101001101001101001101001101001001001101001101001001101101000");

//B4
strcpy(list_code[14],"0001001101001001001101001101001101001101001001001101001101001101101101001001000");
strcpy(list_code[15],"0001001101001001001101001101001101001101001001001101001101001101001001101101000");

//C1
strcpy(list_code[16],"0001001101001101001001001101001101001101001101001101001101001001101101001001000");	
strcpy(list_code[17],"0001001101001101001001001101001101001101001101001101001101001001001001101101000");	

//C2
strcpy(list_code[18],"0001001101001101001001001101001101001101001101001101001001001101101101001001000");	
strcpy(list_code[19],"0001001101001101001001001101001101001101001101001101001001001101001001101101000");	

//C3
strcpy(list_code[20],"0001001101001101001001001101001101001101001101001001001101001101101101001001000");	
strcpy(list_code[21],"0001001101001101001001001101001101001101001101001001001101001101001001101101000");	

//C4
strcpy(list_code[22],"0001001101001101001001001101001101001101001001001101001101001101001001101001000");	
strcpy(list_code[23],"0001001101001101001001001101001101001101001001001101001101001101001001101101000");	

//D1
strcpy(list_code[24],"0001001101001101001101001001001101001101001101001101001101001001101101001001000");	
strcpy(list_code[25],"0001001101001101001101001001001101001101001101001101001101001001001001101101000");

//D2
strcpy(list_code[26],"0001001101001101001101001001001101001101001101001101001001001101101101001001000");	
strcpy(list_code[27],"0001001101001101001101001001001101001101001101001101001001001101001001101101000");	

//D3
strcpy(list_code[28],"0001001101001101001101001001001101001101001101001001001101001101101101001001000");	
strcpy(list_code[29],"0001001101001101001101001001001101001101001101001001001101001101001001101101000");	

//D4
strcpy(list_code[30],"0001001101001101001101001001001101001101001001001101001101001101101101001001000");	
strcpy(list_code[31],"0001001101001101001101001001001101001101001001001101001101001101001001101101000");	

//E1
strcpy(list_code[32],"0001001101001101001101001101001001001101001101001101001101001001101101001001000");	
strcpy(list_code[33],"0001001101001101001101001101001001001101001101001101001101001001001001101101000");	

//E2
strcpy(list_code[34],"0001001101001101001101001101001001001101001101001101001001001101101101001001000");	
strcpy(list_code[35],"0001001101001101001101001101001001001101001101001101001001001101001001101101000");	

//E3
strcpy(list_code[36],"0001001101001101001101001101001001001101001101001001001101001101101101001001000");	
strcpy(list_code[37],"0001001101001101001101001101001001001101001101001001001101001101001001101101000");	

//E4
strcpy(list_code[38],"0001001101001101001101001101001001001101001001001101001101001101101101001001000");	
strcpy(list_code[39],"0001001101001101001101001101001001001101001001001101001101001101001001101101000");	

//F1
strcpy(list_code[40],"0001001101001101001101001101001101001001001101001101001101001001101101001001000");	
strcpy(list_code[41],"0001001101001101001101001101001101001001001101001101001101001001001001101101000");	

//F2
strcpy(list_code[42],"0001001101001101001101001101001101001001001101001101001001001101101101001001000");	
strcpy(list_code[43],"0001001101001101001101001101001101001001001101001101001001001101001001101101000");	

//F3
strcpy(list_code[44],"0001001101001101001101001101001101001001001101001001001101001101101101001001000");	
strcpy(list_code[45],"0001001101001101001101001101001101001001001101001001001101001101001001101101000");	

//F4
strcpy(list_code[46],"0001001101001101001101001101001101001001001001001101001101001101101101001001000");	
strcpy(list_code[47],"0001001101001101001101001101001101001001001001001101001101001101001001101101100");	

//SCS 3063 ====================================================================================================

//I1
strcpy(list_code[48],"0001001001001101001101001101001001001101001101001101001101001101001101001001000"); 
strcpy(list_code[49],"0001001001001101001101001101001001001101001101001101001101001101001101001101000"); 

//I2
strcpy(list_code[50],"0001001001001101001101001101001101001001001101001101001101001101001101001001000"); 
strcpy(list_code[51],"0001001001001101001101001101001101001001001101001101001101001101001101001101000"); 
                    
//I3
strcpy(list_code[52],"0001001001001101001101001101001101001101001001001101001101001101001101001001000"); 
strcpy(list_code[53],"0001001001001101001101001101001101001101001001001101001101001101001101001101000"); 

//I4
strcpy(list_code[54],"0001001001001101001101001101001101001101001101001001001101001101001101001001000"); 
strcpy(list_code[55],"0001001001001101001101001101001101001101001101001001001101001101001101001101000"); 

//II1
strcpy(list_code[56],"0001001101001001001101001101001001001101001101001101001101001101001101001001000"); 
strcpy(list_code[57],"0001001101001001001101001101001001001101001101001101001101001101001101001101000"); 

//II2
strcpy(list_code[58],"0001001101001001001101001101001101001001001101001101001101001101001101001001000"); 
strcpy(list_code[59],"0001001101001001001101001101001101001001001101001101001101001101001101001101000"); 

//II3
strcpy(list_code[60],"0001001101001001001101001101001101001101001001001101001101001101001101001001000"); 
strcpy(list_code[61],"0001001101001001001101001101001101001101001001001101001101001101001101001101000"); 

//II4
strcpy(list_code[62],"0001001101001001001101001101001101001101001101001001001101001101001101001001000"); 
strcpy(list_code[63],"0001001101001001001101001101001101001101001101001001001101001101001101001101000"); 

//III1
strcpy(list_code[64],"0001001101001101001001001101001001001101001101001101001101001101001101001001000"); 
strcpy(list_code[65],"0001001101001101001001001101001001001101001101001101001101001101001101001101000"); 

//III2
strcpy(list_code[66],"0001001101001101001001001101001101001001001101001101001101001101001101001001000"); 
strcpy(list_code[67],"0001001101001101001001001101001101001001001101001101001101001101001101001101000"); 

//III3
strcpy(list_code[68],"0001001101001101001001001101001101001101001001001101001101001101001101001001000"); 
strcpy(list_code[69],"0001001101001101001001001101001101001101001001001101001101001101001101001101000"); 

//III4
strcpy(list_code[70],"0001001101001101001001001101001101001101001101001001001101001101001101001001000"); 
strcpy(list_code[71],"0001001101001101001001001101001101001101001101001001001101001101001101001101000"); 

//IV1
strcpy(list_code[72],"0001001101001101001101001001001001001101001101001101001101001101001101001001000"); 
strcpy(list_code[73],"0001001101001101001101001001001001001101001101001101001101001101001101001101000"); 

//IV2
strcpy(list_code[74],"0001001101001101001101001001001101001001001101001101001101001101001101001001000"); 
strcpy(list_code[75],"0001001101001101001101001001001101001001001101001101001101001101001101001101000"); 

//IV3
strcpy(list_code[76],"0001001101001101001101001001001101001101001001001101001101001101001101001001000"); 
strcpy(list_code[77],"0001001101001101001101001001001101001101001001001101001101001101001101001101000"); 

//IV4
strcpy(list_code[78],"0001001101001101001101001001001101001101001101001001001101001101001101001001000"); 
strcpy(list_code[79],"0001001101001101001101001001001101001101001101001001001101001101001101001101000"); 

	if (argc == 3)
	{
		//Majuscules sur toutes les lettres 
		argv[1][0] = toupper(argv[1][0]);
		argv[1][1] = toupper(argv[1][1]);
		argv[1][2] = toupper(argv[1][2]);
		argv[2][0] = toupper(argv[2][0]);
		argv[2][1] = toupper(argv[2][1]);
		argv[2][2] = toupper(argv[2][2]);

		if(!strcmp(argv[1],"A1"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[0]);
			printf ("La prise A1 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[1]);
			printf ("La prise A1 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"A2"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[2]);
			printf ("La prise A2 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[3]);
			printf ("La prise A2 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"A3"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[4]);
			printf ("La prise A3 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[5]);
			printf ("La prise A3 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"A4"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[6]);
			printf ("La prise A4 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[7]);
			printf ("La prise A4 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"B1"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[8]);
			printf ("La prise B1 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[9]);
			printf ("La prise B1 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"B2"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[10]);
			printf ("La prise B2 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[11]);
			printf ("La prise B2 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"B3"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[12]);
			printf ("La prise B3 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[13]);
			printf ("La prise B3 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"B4"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[14]);
			printf ("La prise B4 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[15]);
			printf ("La prise B4 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"C1"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[16]);
			printf ("La prise C1 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[17]);
			printf ("La prise C1 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"C2"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[18]);
			printf ("La prise C2 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[19]);
			printf ("La prise C2 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"C3"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[20]);
			printf ("La prise C3 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[21]);
			printf ("La prise C3 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"C4"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[22]);
			printf ("La prise C4 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[23]);
			printf ("La prise C4 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"D1"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[24]);
			printf ("La prise D1est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[25]);
			printf ("La prise D1 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"D2"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[26]);
			printf ("La prise D2 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[27]);
			printf ("La prise D2 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"D3"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[28]);
			printf ("La prise D3 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[29]);
			printf ("La prise D3 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"D4"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[30]);
			printf ("La prise D4 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[31]);
			printf ("La prise D4 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"E1"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[32]);
			printf ("La prise E1 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[33]);
			printf ("La prise E1 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"E2"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[34]);
			printf ("La prise E2 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[35]);
			printf ("La prise E2 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"E3"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[36]);
			printf ("La prise E3 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[37]);
			printf ("La prise E3 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"E4"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[38]);
			printf ("La prise E4 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[39]);
			printf ("La prise E4 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"F1"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[40]);
			printf ("La prise F1est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[41]);
			printf ("La prise F1 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"F2"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[42]);
			printf ("La prise F2 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[43]);
			printf ("La prise F2 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"F3"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[44]);
			printf ("La prise F3 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[45]);
			printf ("La prise F3 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"F4"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[46]);
			printf ("La prise F4 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[47]);
			printf ("La prise F4 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"I1"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[48]);
			printf ("La prise I1 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[49]);
			printf ("La prise I1 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"I2"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[50]);
			printf ("La prise I2 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[51]);
			printf ("La prise I2 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"I3"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[52]);
			printf ("La prise I3 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[53]);
			printf ("La prise I3 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"I4"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[54]);
			printf ("La prise I4 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[55]);
			printf ("La prise I4 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"II1"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[56]);
			printf ("La prise II1 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[57]);
			printf ("La prise II1 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"II2"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[58]);
			printf ("La prise II2 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[59]);
			printf ("La prise II2 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"II3"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[60]);
			printf ("La prise II3 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[61]);
			printf ("La prise II3 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"II4"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[62]);
			printf ("La prise II4 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[63]);
			printf ("La prise II4 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"III1"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[64]);
			printf ("La prise III1 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[65]);
			printf ("La prise III1 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"III2"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[66]);
			printf ("La prise III2 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[67]);
			printf ("La prise III2 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"III3"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[68]);
			printf ("La prise III3 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[69]);
			printf ("La prise III3 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"III4"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[70]);
			printf ("La prise III4 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[71]);
			printf ("La prise III4 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"IV1"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[72]);
			printf ("La prise IV1 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[73]);
			printf ("La prise IV1 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"IV2"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[74]);
			printf ("La prise IV2 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[75]);
			printf ("La prise IV2 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"IV3"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[76]);
			printf ("La prise IV3 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[77]);
			printf ("La prise IV3 est allumee\n");
			}
			
		}
		
		if(!strcmp(argv[1],"IV4"))
		{
			if(!strcmp(argv[2],"OFF"))
			{
			strcpy(code,list_code[78]);
			printf ("La prise IV4 est eteinte\n");
			}
			
			if(!strcmp(argv[2],"ON"))
			{
			strcpy(code,list_code[79]);
			printf ("La prise IV4 est allumee\n");
			}
			
		}

		//Verifie si le parametre est correcte
		if (strlen(code) > 0)
		{
		// Parametres les GPIO
		setup_io();

		// Met le GPIO en mode Output
		INP_GPIO(transmitter); // On doit mettre INP_GPIO avant de pouvoir utiliser OUT_GPIO
		OUT_GPIO(transmitter);

        //Envoi le code specifie precedemment
		SendCode(code);
		
		// Met le GPIO sur 0
		GPIO_CLR = 1<<transmitter;
		return 0;
		}
		else
		{
			printf("Mauvais Parametres!\n");
			printf("------------------\n");
			printf("Les prises vont de A1 a F4 et de I1 a IV4\n");
			printf("Utilisation: scs nomprise on/off \n");
			printf("Exemple: ./scs a1 on\n");
			return 1;
		}
	

	}
	else
	{
	printf("Pas assez de parametres!\n");
	printf("------------------------\n");
	printf("Les prises vont de A1 a F4 et de I1 a IV4\n");
	printf("Utilisation: scs nomprise on/off \n");
	printf("Exemple: ./scs I1 on\n");
	return 1;
	}

} 


//
// Setup des GPIO
//
void setup_io()
{
	/* open /dev/mem */
	if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) 
	{
		printf("Can't open /dev/mem, try to execute this script as root \n");
		exit (-1);
	}

	/* mmap GPIO */
	// Allocate MAP block
	if ((gpio_mem = (unsigned char*)malloc(BLOCK_SIZE + (PAGE_SIZE-1))) == NULL) 
	{
		printf("Allocation error \n");
		exit (-1);
	}

	// Make sure pointer is on 4K boundary
	if ((unsigned long)gpio_mem % PAGE_SIZE)
	{
		gpio_mem += PAGE_SIZE - ((unsigned long)gpio_mem % PAGE_SIZE);
	}

	// Now map it
	gpio_map = (unsigned char *)mmap(
		(caddr_t)gpio_mem,
		BLOCK_SIZE,
		PROT_READ|PROT_WRITE,
		MAP_SHARED|MAP_FIXED,
		mem_fd,
		GPIO_BASE
		);

	if ((long)gpio_map < 0)
	{
		printf("mmap error %d\n", (int)gpio_map);
		exit (-1);
	}

	// Always use volatile pointer!
	gpio = (volatile unsigned *)gpio_map;
} // setup_io

//
// Cette fonction envoi le code
//
void SendCode(char* code)
{
	timespec sleeptime;
	timespec remtime;

	for (int repeater = 1 ; repeater <= nbrepeat ; repeater++)
	{
		sleeptime.tv_sec = 0;
		sleeptime.tv_nsec = 550000; // Delai des prises SCS , pour d'autres prises ce delai sera different
  
		for (int i = 1 ; i <= 79 ; i++)
		{
			if (code[i] == '1')
			{
				GPIO_SET = 1<<transmitter;
			}
			else
			{
				GPIO_CLR = 1<<transmitter;
			}
			nanosleep(&sleeptime,&remtime);
			
		}
		sleeptime.tv_nsec = 1000000; //10ms Ce delai est envoye entre chaque repetition du code
		nanosleep(&sleeptime,&remtime);
	}
}


