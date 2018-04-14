#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void zero_init(int inp[], int size)
{
	int i;
	for ( i = 0; i < size; i++ )
	{
		inp[ i ] = 0;
	}
}

int eval_poly(int pow_table[])
{   //printf("\n evaluated polynnomial: ");
	int i, sum = 0;
	for( i = 0; i < 8; i++ )
	{ //  printf("%d", pow_table[i]);
		sum = sum + ( pow_table[ i ] * pow(2, i) );
	}
	return sum;
}


void debug_print ( int inp[], int size ) // macchar maaro!
 {

     int i;


     for ( i = 0; i < size; i++ )
     {
       //  printf("%d" ,inp[i]);

     }
   // printf("\n");
 }


void shift_poly(int pow_table[])
{
 	int round_off[] ={ 0, 2, 3, 4 };
 	int length_round = 4;
	int i;
     //printf("\n shifted table: ");
    int dupli_pow[9];

    for( i = 0; i < 9; i++ )
    {
        dupli_pow[ i ] = pow_table [ i ];
    }
 	for( i = 0; i < 8; i++ )
 	{
        pow_table[ i + 1 ] = dupli_pow[ i ];
       // debug_print(pow_table, 9);
 	}

 	pow_table [ 0 ] = 0;

 	if ( pow_table[ 8 ] == 1 )
 	{
 		for ( i = 0; i < length_round; i++)
 		{
 			if( pow_table[ round_off [i] ] == 1 )
 			{ //  printf("round off 1 at: %d   ", round_off[i]);
 				pow_table[ round_off [i] ]  = 0;
 			}

			else
			{  // round_off
				pow_table[ round_off [i] ]  = 1;
			}
 		}

    pow_table[8] = 0;
	}
	debug_print(pow_table, 9);
}


// gf me field elements hain
// ginv me root ki power hai , log jaise:)
//field generator


void gf_genrator(int gf[], int ginv[])
 {

 	int i, j=0;
 	int pow_table[9];				 //9th wala buffer wala, sb moh maaya hai!

 	zero_init ( pow_table, 8);
 	zero_init (gf, 256);


 	// Shurooat ke 8 element calculate karo.
 	for ( i = 1; i < 9; i++ )
 	{
 		gf[ i ] = pow( 2, i - 1 );
 	    ginv[ gf[i] ] = i - 1;
  }

 	// a8 = a4 + a3+ a2+ 1
   int round_off[] ={ 0, 2, 3, 4 };
   int length_round = 4;

   for ( i =0; i < length_round; i++)
   {

   	pow_table[ round_off[i] ] = 1;

   }

 	for ( i = 9; i < 256; i++ )
 	{
            gf[ i ] = eval_poly( pow_table );
            //printf("\n element number[%d] = %d \n",i, gf[i]);
            shift_poly( pow_table );
            ginv[ gf[i] ] = i-1;

	}

	for ( i = 0; i < 256; i++)
    {
        printf( "ginv[%d] = %d \n", i , ginv[i]);
    }
 for ( i = 0; i < 256; i++)
    {
        printf( "gf[%d] = %d \n", i , gf[i]);
    }

 }



int g_mul(int a,int b, int ginv_table[], int gf_table[]) // gaolis field multiplication
{

 int i,j;

   if (a==0 || b == 0) return (0);

  i = ginv_table[a];
  j = ginv_table[b];

  int m = (i + j) % 255 ;
  m++;

  return (gf_table[m]);
}

void LFSR_encryption( uint8_t msg[], int gf_table[], int ginv_table[], int parity[]) // LFSR based parity generator.
{

	// 64 elements generator polynomial
    int generator_poly[] = {45 ,216 ,239 ,24 ,253 ,104 ,27 ,40 ,107 ,50 ,163 ,210 ,227 ,134 ,224 ,158 ,119 ,13 ,158 ,1 ,238 ,164 ,82 ,43 ,15 ,232 ,246 ,142 ,50 ,189 ,29 ,232 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0};


     /*************** CONVENTION K ACCORDING NPARITY SET KAR LENA **********************/

		int n_parity = 32;  																	// Change as you wish!
		int cryp[33]; 																			// Yha pe nparity + 1 kar dena

		int tap = cryp[ n_parity - 1 ]; 														// tapping at second last parity bit
		int length = 223;

		int i, j;
		zero_init(cryp, 33);


		for ( i =0; i < length ; i++) 															// fill length with 256 - no. parity bits ie. length
		{


		tap = msg[i] ^ cryp[ n_parity - 1 ];


		for ( j = n_parity - 1 ; j > 0; j-- )
		{



		cryp[ j ] = cryp [ j - 1] ^ g_mul( generator_poly[j], tap, ginv_table, gf_table );
		printf("%d. %d * % d = %d \n ",j, generator_poly[j], tap, g_mul( generator_poly[j], tap, ginv_table, gf_table ));


		}

		cryp[ 0 ] = g_mul(generator_poly[0], tap, ginv_table, gf_table);

		}

		for ( i = 0; i < n_parity; i++ )
		{
		parity [ i ] = cryp [ i ];
		}


}

void RS_encoder( uint8_t  msg[], uint8_t code_word[] ) // msg - 224 Bytes, code_word - 256 byes for 32 bytes of parity
{
	int gf_table[256], ginv_table[256];
	gf_genrator(gf_table, ginv_table);
	int gnerator_poly[] = {45 ,216 ,239 ,24 ,253 ,104 ,27 ,40 ,107 ,50 ,163 ,210 ,227 ,134 ,224 ,158 ,119 ,13 ,158 ,1 ,238 ,164 ,82 ,43 ,15 ,232 ,246 ,142 ,50 ,189 ,29 ,232 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0};

	int parity[32];
	LFSR_encryption(msg, gf_table, ginv_table, parity);

	int i;

	for ( i = 0; i < 223; i++ )
	{
		code_word[ i ] = msg [ i ];
        printf("codeword[%d] = %d \n", i, code_word[i]);
	}

	for ( i = 0; i < 32; i++ )
	{

		code_word[ 223 + i ] = parity[i];
        printf("codeword[%d] = %d \n", i + 223, code_word[ i + 223 ]);
	}

}




// void polynnomial_generator ( int genpoly[], int npar)

int main()
{
    int gf_table[256], ginv_table[256];
    printf("Hello world!\n");
    gf_genrator(gf_table, ginv_table);
    int gnerator_poly[] = {45 ,216 ,239 ,24 ,253 ,104 ,27 ,40 ,107 ,50 ,163 ,210 ,227 ,134 ,224 ,158 ,119 ,13 ,158 ,1 ,238 ,164 ,82 ,43 ,15 ,232 ,246 ,142 ,50 ,189 ,29 ,232 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0};
    // char msg[]="Random String is the one I will be using here, any problem with that?";
    int length = 52;
    int parity[32];
    uint8_t code_word[ 255 ];
    uint8_t msg[ 223 ];

    int i;

    for ( i = 0; i < 223; i++ )
    {
        msg[ i ] = i;
    }

    RS_encoder(msg, code_word);


    getch();
    return 0;

}
