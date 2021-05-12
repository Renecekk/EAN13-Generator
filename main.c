#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>
#include "generateEAN13.c"
#include "generateBITMAP.c"

int main()
{
	char cont = 0x40;
	printf("Welcome to EAN13 Generator.");
	while(cont != 0x71 && cont != 0x51)
	{
		cont = 0x40;
		char imgFname[20];
		int numerics = 0;
		printf("\nEnter 12 NUMERIC Only characters: ");

		while(lenOfToEncode!=12 || numerics!=12) //get user input while conditions not met (length + NUMS only)
		{
			numerics = 0;
			scanf("%s", &toEncode);
			for(lenOfToEncode=0; toEncode[lenOfToEncode]!=0x00; lenOfToEncode++); //get length of string
			for(int i=0; i<12; i++) {
				if(0x30>toEncode[i] || toEncode[i]>0x39) {
					break;
				}
				else
				{
					numerics++;
				}
			}
			if(lenOfToEncode!=12 || numerics!=12)
			{
				printf("Error. Enter 12 NUMERIC Only characters: ");
			}
		}

		printf("\nNumber You want to generate barcode from is: %s\n", toEncode); //recapitulation
	    build(toEncode, data);

	    for(int i=0; i<12; i++)
		{
			imgFname[i]=toEncode[i];
		}
		if(checksum>9)
		{
			checksum=0;
		}
		imgFname[12] = 0x30+checksum;
		imgFname[13] = 0x2e;
		imgFname[14] = 0x62;
		imgFname[15] = 0x6D;
		imgFname[16] = 0x70;
		imgFname[17] = 0x00;
		

	    generateAndSaveBarcode(data, sizeof(data), imgFname);

	    printf("\n\nPress any key to continue or q to quit...");
	    cont = getch();
	}

	return 0;
}

//#Ladomirjak René, 230491, BPC_MET/02