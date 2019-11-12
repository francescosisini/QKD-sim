#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "terminale_lib.h"
/*_____________________________________

               Alice e Bob
            @francesco_sisini
             _scuola_sisini_
 */

#define DIM_CHIAVE 32
#define RIGA 18
#define DR 5
#define COLONNA 19

typedef enum
  {
    BASI,
    FOTONI

  } DIZIONARIO;

typedef enum
  {
    VERTICALE = 0,
    ORIZZONTALE
  } POLARIZZAZIONE;

typedef enum
  {
    STANDARD,
    HADAMARD,
    INDETERMINATA
  } BASE;

typedef struct
{
  //Polarizzazione del fotone
  POLARIZZAZIONE polarizzazione;
  //Base della polarizzazione
  BASE base;
  
} FOTONE;

typedef struct qnodo {
  POLARIZZAZIONE polarizzazione;
  char accettato;
} QNODO;



/*____________________________________________________*/
FOTONE quanto_emetti_fotone(BASE base_emissione);

FOTONE quanto_intercetta(FOTONE f, BASE base_emissione, QNODO chiave[]);

POLARIZZAZIONE quanto_rivela_fotone(FOTONE f,BASE b);

BASE classico_bob2alice_richiedi_base(FOTONE f);

char classico_bob2aliceconferma_polarizzazione(POLARIZZAZIONE p);

/*____________________________________________________*/
void scrivi_qubit(POLARIZZAZIONE p, QNODO * chiave);

void elimina_qubit(QNODO * qn);

void descrizione(int valore, char * buffer,DIZIONARIO diz);

char eva_intercetta();


/*____________________________________________________*/
int main()
{
  /* inizializza il terminale */
  init();

   /* presentazione */
  printf("\x1b[2J\x1b[1;1H\nquanta_: criptazione quantica (bob) \n");
  printf("\x1b[1;1H");
  printf("(C)Scuola_Sisini 2019 https://pumar.it\n\n\n\r");

  printf("CTRL-<a> chiedi ad alice di inviare un qubit\n\r");
  printf("CTRL-<s> salva il qubit nella chiave\n\r");
  printf("<i> scegli la base per Alice\n\r");
  printf("<c> chiede ad alice che base ha usato\n\r");
  printf("<b> cambia la tua base\n\r");
  printf("____________________________________________________\n\r");
  printf("FRECCE SX/DX cursore a sinistra/destra\n\r");
  printf("CTRL-<c> verifica con alice il  qubit sotto il curosre\n\r");
  printf("CTRL-<k> elimina qubit sotto il curosre\n\r");
  printf("____________________________________________________\n\r");
  printf("CTRL-<w> accetta la chiave\n\r");
  printf("CTRL-<x> rifiuta la chiave\n\r");
  
  
  printf("\x1b[%d;1H|-----------------------------------------|",RIGA);
  printf("\x1b[%d;1H|qubit rivelato|Base Alice   |Base Bob    |",RIGA+1);
  printf("\x1b[%d;1H|-----------------------------------------|",RIGA+2);
  printf("\x1b[%d;1H|              |              |           |",RIGA+3);
  printf("\x1b[%d;1H|-----------------------------------------|",RIGA+4);
   printf("\x1b[%d;1H|Qubit salvati  :",RIGA+5);
  printf("\x1b[%d;1H|CHIAVE Bob  :",RIGA+6);
  printf("\x1b[%d;1H|CHIAVE Alice:",RIGA+7);

  //Flag di intrusione di Eva
  char eva_flag = 0;

  //Le chiavi di Bob e Alice corrispondono?
  char chiavi_flag = 1;
  
  //Riga e colonna del cursore
  int riga = RIGA+DR;
  int colonna = COLONNA;
  //buffer per le descrizioni dei valori
  char desc[15];
  
  QNODO  chiave_bob[DIM_CHIAVE];
  QNODO  chiave_alice[DIM_CHIAVE];
  QNODO  chiave_eva[DIM_CHIAVE];

  for(int i=0;i<DIM_CHIAVE;i++)
    chiave_eva[i].accettato = -1;
  
   /* qubit attivo su cui scrivere */
  int indice_qubit_chiave_bob=0;
  int indice_qubit_chiave_alice=0;
  int indice_qubit_chiave_eva=0;

  FOTONE bob_f, alice_f;
  BASE bob_base = STANDARD;
  BASE alice_base = INDETERMINATA;
  POLARIZZAZIONE pz;
  POLARIZZAZIONE tmp;
  int inx;

  struct timespec tc;
  tc.tv_sec = 0.3;
  tc.tv_nsec =333333333;
  
  
 

  int n_qubit_segnati = 0;

  srand(time(0));

  eva_flag = eva_intercetta();

   descrizione(bob_base, desc,BASI);
   printf("\x1b[%d;32H%s",RIGA+3,desc);
  

  while(n_qubit_segnati<DIM_CHIAVE)
    {
      while(1)
        {
          int ch = leggi_tastiera();
          
          switch (ch)
            {      
            case CTRL_KEY('q'):
              write(STDOUT_FILENO, "\x1b[2J", 4);
              write(STDOUT_FILENO, "\x1b[H", 3);
              write(STDOUT_FILENO,"\x1b[48:5:0m",9);
              exit(0);
              break;
              
            case CTRL_KEY('A'):

              /* una coppia di fotoni viene emessa nel sistema di Alice */

                descrizione(alice_base, desc,BASI);
                printf("\x1b[%d;17H%s",RIGA+3,desc);
                fflush(stdout);
              
              if(eva_flag)
                {
                  alice_f =  quanto_emetti_fotone(alice_base);
                  bob_f=quanto_intercetta
                    ( alice_f, alice_base,chiave_eva);
                  //printf("\x1b[35;1Hbob: %d, Alice:%d",bob_f.polarizzazione,alice_f.polarizzazione);
                }
              else
                {
                  alice_f = quanto_emetti_fotone(alice_base);
                  bob_f =  alice_f;
                }
              //->salva il qubit di Alice
             

              //->salva il qubit di Bob
              pz = quanto_rivela_fotone(bob_f,bob_base);

              if(pz)
                {
                  // evidenzia il nuovo qubit
                  printf("\x1b[%d;6H>    | ",RIGA+3);
                  fflush(stdout);
                  nanosleep(&tc, 0);
                  printf("\x1b[%d;6H >   | ",RIGA+3);
                  fflush(stdout);
                  nanosleep(&tc, 0);
                  printf("\x1b[%d;6H  >  | ",RIGA+3);
                  fflush(stdout);
                  nanosleep(&tc, 0);
                  printf("\x1b[%d;6H   > | ",RIGA+3);
                  fflush(stdout);
                  nanosleep(&tc, 0);
                  printf("\x1b[%d;6H    >| ",RIGA+3);
                  fflush(stdout);
                  nanosleep(&tc, 0);
                  printf("\x1b[%d;6H     * ",RIGA+3);
                  fflush(stdout);
                  nanosleep(&tc, 0);
                  printf("\x1b[%d;6H     | ",RIGA+3);
                  fflush(stdout);
                  nanosleep(&tc, 0);
                  printf("\x1b[%d;6H       ",RIGA+3);
                  fflush(stdout);
                  nanosleep(&tc, 0);
                }
              else
                {
                  // evidenzia il nuovo qubit
                  printf("\x1b[%d;6H>    | ",RIGA+3);
                  fflush(stdout);
                  nanosleep(&tc, 0);
                  printf("\x1b[%d;6H >   | ",RIGA+3);
                  fflush(stdout);
                  nanosleep(&tc, 0);
                  printf("\x1b[%d;6H  >  | ",RIGA+3);
                  fflush(stdout);
                  nanosleep(&tc, 0);
                  printf("\x1b[%d;6H   > | ",RIGA+3);
                  fflush(stdout);
                  printf("\x1b[%d;6H    >| ",RIGA+3);
                  fflush(stdout);
                  nanosleep(&tc, 0);
                  printf("\x1b[%d;6H     > ",RIGA+3);
                  fflush(stdout);
                  nanosleep(&tc, 0);
                  printf("\x1b[%d;6H     |>",RIGA+3);
                  fflush(stdout);
                  nanosleep(&tc, 0);
                  printf("\x1b[%d;6H       ",RIGA+3);
                  fflush(stdout);
                  nanosleep(&tc, 0);
                  
                }
              printf("\x1b[%d;9H%d",RIGA+3,pz);

              printf("\x1b[%d;%dH",riga,colonna);
              fflush(stdout);
              break;
              
              
            case BACKSPACE:

              break;
            case CTRL_KEY('S'):
              //salva qubit nella chiave di Bob
              chiave_bob[indice_qubit_chiave_bob].polarizzazione = pz;
              chiave_bob[indice_qubit_chiave_bob].accettato = 1;
              indice_qubit_chiave_bob++;
              //salva qubit nella chiave di ALice
              chiave_alice[indice_qubit_chiave_alice].polarizzazione = alice_f.polarizzazione;
              chiave_alice[indice_qubit_chiave_alice].accettato = 1;
              indice_qubit_chiave_alice++;
              //salva qubit nella chiave di Eva
              
              
              for(int i=0; i<indice_qubit_chiave_bob;i++)
                {
                  if(chiave_bob[i].accettato == 1)
                    printf("\x1b[%d;%dH%d",RIGA+5,COLONNA+i,chiave_bob[i].polarizzazione);
                  else
                    printf("\x1b[%d;%dH-",RIGA+5,COLONNA+i);
                }

              //Cancella il qubit registrato dallo schermo
              printf("\x1b[%d;9H-",RIGA+3);

              //Pone la base suggerita per alice ad indeterminata
              alice_base = INDETERMINATA;
              descrizione(alice_base, desc,BASI);
              printf("\x1b[%d;17H%s",RIGA+3,desc);
              
              printf("\x1b[%d;%dH",riga,colonna);
              fflush(stdout);
              break;

            case CTRL_KEY('C'):
              //l'indice del qubit è dato dalla
              //posizione del cursore rispetto l'inizio della sequenza di qubit salvati
              inx = colonna-(COLONNA) ;
              tmp = chiave_alice[inx].polarizzazione ;
              if(chiave_bob[inx].accettato)
                {
                  printf("\x1b[%d;%dH%d",RIGA+DR+2,COLONNA+inx,chiave_alice[inx].polarizzazione);
                  
                  printf("\x1b[%d;%dH",riga,colonna);
                  fflush(stdout);
                }
              //Eva scopre il valore di questo qubit
              chiave_eva[inx].polarizzazione = chiave_alice[inx].polarizzazione;
              break;
              
            case CTRL_KEY('K'):
              //posizione del cursore rispetto l'inizio della sequenza di qubit salvati
              inx = colonna-(COLONNA) ;
              chiave_alice[inx].accettato=0 ;
              chiave_bob[inx].accettato=0 ; 
              printf("\x1b[%d;%dH-",RIGA+DR+2,COLONNA+inx);
              printf("\x1b[%d;%dH-",RIGA+DR,COLONNA+inx);

              printf("\x1b[%d;%dH",riga,colonna);
              fflush(stdout);
              break;

            case CTRL_KEY('W'):

              /* mostra la chiave di Bob e quella di Alice e le confronta */

              for(int i=0; i<indice_qubit_chiave_bob;i++)
                {
                  if(chiave_bob[i].accettato == 1)
                    {
                      chiavi_flag = chiavi_flag && chiave_bob[i].polarizzazione == chiave_alice[i].polarizzazione;
                      printf("\x1b[%d;1HCHIAVE Bob  :\x1b[%d;%dH%d",RIGA+10,RIGA+10,14+i,chiave_bob[i].polarizzazione);
                      printf("\x1b[%d;1HCHIAVE Alice:\x1b[%d;%dH%d",RIGA+11,RIGA+11,14+i,chiave_alice[i].polarizzazione);
                      
                    }
                      fflush(stdout);
                }
              if(chiavi_flag)
                {
                  printf("\x1b[%d;%dHBENE: le chiavi corrispondono",RIGA+12,1);

                }
              else
                {
                   printf("\x1b[%d;%dHMALE: le chiavi NON corrispondono",RIGA+12,1);
                }
              if(eva_flag)
                {
                  for(int i=0; i<indice_qubit_chiave_bob;i++)
                    printf("\x1b[%d;1HCHIAVE Eva :\x1b[%d;%dH%d",RIGA+13,RIGA+13,14+i,chiave_eva[i].polarizzazione);
                  fflush(stdout);
                }
              printf("\x1b[%d;%dH",riga,colonna);
              break;
              
              
            case ARROW_UP:
              
              break;
            case ARROW_DOWN:
              break;
            case ARROW_LEFT:
              if(colonna>COLONNA)
                colonna--;
              printf("\x1b[%d;%dH",riga,colonna);
              fflush(stdout);
              break;
            case ARROW_RIGHT:
              if(colonna<COLONNA+DIM_CHIAVE)
                colonna++;
               printf("\x1b[%d;%dH",riga,colonna);
               fflush(stdout);
              break;
            default:
              if(32<=ch&&ch<=127)
                {
                  if(ch == 'k')
                    {
                      printf("\x1b[1;1HBackdoor di EVA: %d",eva_flag);
                      fflush(stdout);
                    }
                  if(ch == 'c')
                    {
                      // Bob chiede ad Alice che base ha usato
                      
                      descrizione(bob_f.base, desc,BASI);
                      printf("\x1b[%d;17H%s",RIGA+3,desc);

                      printf("\x1b[%d;%dH",riga,colonna);
                      fflush(stdout);

                    }
                   if(ch == 'i')
                    {
                      // Bob indica ad Alice la base da usare
                       if(alice_base == STANDARD)
                        alice_base = HADAMARD;
                       else if(alice_base == HADAMARD)
                           alice_base = INDETERMINATA;
                       else if(alice_base == INDETERMINATA)
                             alice_base = STANDARD;
                       
                       descrizione(alice_base, desc,BASI);
                       printf("\x1b[%d;17H%s",RIGA+3,desc);
                       
                       printf("\x1b[%d;%dH",riga,colonna);
                       fflush(stdout);

                    }

                  
                  if(ch == 'b')
                    {
                      //Bob cambia la propia base
                      if(bob_base == STANDARD)
                        bob_base = HADAMARD;
                      else
                        bob_base = STANDARD;

                      descrizione(bob_base, desc,BASI);
                      printf("\x1b[%d;32H%s",RIGA+3,desc);

                      printf("\x1b[%d;%dH",riga,colonna);
                      fflush(stdout);

                    }
                }
              
            break;
          }
    }

      /* controlla se le basi corrispondono */
      if(1 == 1)
        {
          n_qubit_segnati ++;
          indice_qubit_chiave_bob ++;
        }
    }

   for(int i=0;i<DIM_CHIAVE;i++)
    {
      printf("%d\t",chiave_bob[i].polarizzazione);

    }

}

/*_______________________________________________________
  
  traslitera in italiano il valore nel dizionario di 
  riferimento. Memorizza il risultato in buffer
*/

void descrizione(int valore, char * buffer,DIZIONARIO diz)
{
  switch (diz)
    {
    case BASI:
      if(valore == STANDARD)
        sprintf(buffer,"STANDARD     ");
     
      if(valore == HADAMARD)
        sprintf(buffer,"HADAMARD     ");
     
       if(valore == INDETERMINATA)
        sprintf(buffer,"INDETERMINATA");
      break;
      
    case FOTONI:
      if(valore == VERTICALE)
        sprintf(buffer,"VERTICALE");
      
      if(valore == ORIZZONTALE)
        sprintf(buffer,"ORIZZONTALE");
      break;

    }
}

/*____________________________________________________
  
  Con una certa probabilità Eva cerca di intercettare
  il  canale quantistico di comunicazione

 */
char eva_intercetta()
{
  float f = (float)rand()/(float)RAND_MAX;
  if(f>0.5) return 1;
  return 0;
}

/*____________________________________________________

  Bon ha ricevuto un fotone da Alice. Il fotone di 
  alice è stato già rivelato nel sistema di ALice 
  e ora verrà rivelato in quello di Bob. Lo stato 
  in cui si trova il fotone di Bob è lo stesso di quello
  di ALice, ma se Bob usa una base di rivelazione
  differente allora non può essere sicuro di rivelare
  lo stesso stato di polarizzazione
 */
POLARIZZAZIONE quanto_rivela_fotone(FOTONE f,BASE b)
{

    
  if( b == f.base)
    return f.polarizzazione;

  float quanto_psi2 = (float)rand()/(float)RAND_MAX;
  
  if(quanto_psi2>0.5)
    return VERTICALE;
  else
    return ORIZZONTALE;
}

 /*_____________________________________________________
   
   Eva intercetta il fotone emesso da Alice verso Bob
   Se Eva conosce la base usata da Alice, allora sarà
   in grado di conoscere lo stato di polarizzazione
   e di ritrasmetterlo identico a Bob senza che lui se ne
   accorga
  */
FOTONE quanto_intercetta(FOTONE emesso, BASE base_alice, QNODO chiave_eva[])
{
  BASE b = base_alice;
  FOTONE p = emesso;
  FOTONE impostore;
  printf("\x1b[30;1H                      ");
  printf("\x1b[31;1H                      ");
  
   
  if(base_alice == INDETERMINATA)
    {
     
      //Bob non ha chiesto ad Alice la base prima che lei inviasse
      //il fotone, quindi Eva non può conoscerla
      
      float classico_scelta = (float)rand()/(float)RAND_MAX;
      
      if(classico_scelta>0.5)
        {
          b = STANDARD;
        }
      else
        {
          b = HADAMARD;
        }

      if(b == emesso.base)
        {
          
          //Eva ha azzeccato la base
          impostore =  emesso;
        }
      else
        {
          
          float quanto_psi2 = (float)rand()/(float)RAND_MAX;
          
          if(quanto_psi2>0.5)
            {
              p.polarizzazione = VERTICALE;
            }
          else
            {
              p.polarizzazione = ORIZZONTALE;
            }
       
          impostore.base = b;
          impostore.polarizzazione = p.polarizzazione;
        }
    }
  else
     {
      
      //Eva conosce la base e riproduce esattamente il fotone
      //intercettato
      impostore =  emesso;
    }
  int i =0;
  //Cerca l'ultimo elemento scritto
  while(chiave_eva[i].accettato>=0)i++;
  chiave_eva[i].polarizzazione = impostore.polarizzazione;
  chiave_eva[i].accettato = 0;

  return impostore;
  
}

  /*_____________________________________________________
 
  Bob chiede ad alice di emettere un fotone
  Alice sceglie una base ed emette una copia
  di fotoni che hanno lo stesso stato quantico
  uno viene rivelato dal rivelatore di alice
  che inserisce nella sua chiave il valore
  di polarizzazione misurato
  l'altro viene mandato a Bob che lo rivela e 
  inserisce a sua volta il valore di polarizzazione
  nella sua chiave
 */
FOTONE quanto_emetti_fotone(BASE base_suggerita)
{
  FOTONE p;
  BASE b;

  if(base_suggerita !=INDETERMINATA)
    b = base_suggerita;
  else
    {
      float classico_scelta = (float)rand()/(float)RAND_MAX;
      
      if(classico_scelta>0.5)
        {
          b = STANDARD;
        }
      else
        {
          b = HADAMARD;
        }
    }
  
  float quanto_psi2 = (float)rand()/(float)RAND_MAX;
  
  if(quanto_psi2>0.5)
    {
      p.polarizzazione = VERTICALE;
    }
  else
    {
      p.polarizzazione = ORIZZONTALE;
    }
  
   FOTONE f;
   f.base = b;
   f.polarizzazione = p.polarizzazione;

   return f;
  
}

