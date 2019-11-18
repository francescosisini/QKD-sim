#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "terminale_lib.h"
/*_____________________________________

               Alice e Bob
          @francesco_sisini 2019
             _scuola_sisini_
 */

#define DIM_CHIAVE 32
#define RIGA 12
#define DR 16
#define DR2 16
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

/*____________________________________________________*/

void descrizione(int valore, char * buffer,DIZIONARIO diz);

char eva_intercetta();


/*____________________________________________________*/
int main()
{
  /* inizializza il terminale */
  init();

   /* presentazione */
  printf("\x1b[2J\x1b[1;1H\nquanta_: correlazione quantistica (bob) \n");
  printf("\x1b[1;1H");
  printf("(C)Scuola_Sisini 2019 https://pumar.it\n\n\n\r");
  printf("_________________________________________________________________\n\r");
  printf("     <a> ricevi fotone       (c. quantistico)\n\r");
  printf("CTRL-<s> salva qubit         (locale)\n\r");
  printf("     <i> vedi base Alice     (c. classico)\n\r");
  printf("CTRL-<i> imposta base Alice  (c. classico)\n\r");
  printf("CTRL-<b> imposta base Bob    (locale)\n\r");
  printf("_________________________________________________________________\n\r"); 
  
  printf("\x1b[%d;1H|-----------------------------------------|",RIGA);
  printf("\x1b[%d;1H|Qubit rivelato|Base Alice   |Base Bob    |",RIGA+1);
  printf("\x1b[%d;1H|-----------------------------------------|",RIGA+2);
  printf("\x1b[%d;1H|              |              |           |",RIGA+3);
  printf("\x1b[%d;1H|-----------------------------------------|",RIGA+4);

  printf("\n\r\n\r");
  printf("\n\r_________________________________________________________________\n\r");
  printf("FRECCE SX/DX  seleziona qubit\n\r");
  printf("     <c>      confronta qubit selezionato\n\r");
  printf("CTRL-<k>      elimina qubit selezionato\n\r");
  printf("CTRL-<w>      confronta la chiave\n\r");
  printf("CTRL-<x>      esci\n\r");
  printf("_________________________________________________________________\n\r");
  printf("\x1b[%d;1HQubit salvati :",RIGA+5+11);
  printf("\x1b[%d;1HCHIAVE Bob    :",RIGA+6+11);
  printf("\x1b[%d;1HCHIAVE Alice  :",RIGA+7+11);
  fflush(stdout);

  //Flag di intrusione di Eva
  char eva_flag = 0;

  //Le chiavi di Bob e Alice corrispondono?
  char chiavi_flag = 1;
  
  //Riga e colonna del cursore
  int riga = RIGA+DR+1;
  int colonna = COLONNA;
  //buffer per stringa di descrizioni
  char desc[15];

  //chiavi di criptazione
  QNODO  chiave_bob[DIM_CHIAVE];
  QNODO  chiave_alice[DIM_CHIAVE];
  QNODO  chiave_eva[DIM_CHIAVE];

  for(int i=0;i<DIM_CHIAVE;i++)
    {
      chiave_eva[i].accettato = -1;
      chiave_bob[i].accettato = -1;
      chiave_alice[i].accettato = -1;
    }
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

  /* per l'attesa duranta l'animazione */
  struct timespec tc;
  tc.tv_sec = 0.3;
  tc.tv_nsec =333333333;
 
  srand(time(0));
  
  eva_flag = eva_intercetta();
  
  descrizione(bob_base, desc,BASI);
  printf("\x1b[%d;32H%s",RIGA+3,desc);
  
  
  
  while(1)
    {
      int ch = leggi_tastiera();
      printf("\x1b[%d;%dH",riga,colonna);
      fflush(stdout);
      
      switch (ch)
        {
          /* esci */
        case CTRL_KEY('q'):
          write(STDOUT_FILENO, "\x1b[2J", 4);
          write(STDOUT_FILENO, "\x1b[H", 3);
          write(STDOUT_FILENO,"\x1b[48:5:0m",9);
          exit(0);
          break;
          
        case CTRL_KEY('x'):
          write(STDOUT_FILENO, "\x1b[2J", 4);
          write(STDOUT_FILENO, "\x1b[H", 3);
          write(STDOUT_FILENO,"\x1b[48:5:0m",9);
          exit(0);
          break;
          
          /* mostra la chiave di Bob e quella di Alice e le confronta */
        case CTRL_KEY('I'):
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
          break;
          
          /* imposta base Bob */
        case CTRL_KEY('B'):
          //Bob cambia la propia base
          if(bob_base == STANDARD)
            bob_base = HADAMARD;
          else
            bob_base = STANDARD;
          
          descrizione(bob_base, desc,BASI);
          printf("\x1b[%d;32H%s",RIGA+3,desc);
          
          printf("\x1b[%d;%dH",riga,colonna);
          fflush(stdout);
          break;
          
        case BACKSPACE:
          break;
          
          /* Salva qubit */
        case CTRL_KEY('S'):
          //salva qubit nella chiave di Bob
          chiave_bob[indice_qubit_chiave_bob].polarizzazione = pz;
          chiave_bob[indice_qubit_chiave_bob].accettato = 1;
          indice_qubit_chiave_bob++;
          //salva qubit nella chiave di ALice
          chiave_alice[indice_qubit_chiave_alice].polarizzazione = alice_f.polarizzazione;
          chiave_alice[indice_qubit_chiave_alice].accettato = 1;
          indice_qubit_chiave_alice++;
          
          for(int i=0; i<indice_qubit_chiave_bob;i++)
            {
              if(chiave_bob[i].accettato == 1)
                printf("\x1b[%d;%dH%d",RIGA+DR2+1,COLONNA+i,chiave_bob[i].polarizzazione);
              else
                printf("\x1b[%d;%dH-",RIGA+DR2+1,COLONNA+i);
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
          break;
          
           /* Elimina qubit selezionato */
        case CTRL_KEY('K'):
          //posizione del cursore rispetto l'inizio della sequenza di qubit salvati
          inx = colonna-(COLONNA) ;
          chiave_alice[inx].accettato=0 ;
          chiave_bob[inx].accettato=0 ; 
          printf("\x1b[%d;%dH-",riga,COLONNA+inx);
          printf("\x1b[%d;%dH-",riga+1,COLONNA+inx);
          
          printf("\x1b[%d;%dH",riga,colonna);
          fflush(stdout);
          break;
          
          /* Accetta la chiave */
        case CTRL_KEY('W'):
          for(int i=0; i<indice_qubit_chiave_bob;i++)
            {
              if(chiave_bob[i].accettato == 1)
                 {
                   //il flag è zero se le due chiavi non corrispondono
                   chiavi_flag = chiavi_flag && chiave_bob[i].polarizzazione == chiave_alice[i].polarizzazione;
                   printf("\x1b[%d;1HCHIAVE Bob  :\x1b[%d;%dH%d",RIGA+DR+10,RIGA+DR+10,14+i,chiave_bob[i].polarizzazione);
                   printf("\x1b[%d;1HCHIAVE Alice:\x1b[%d;%dH%d",RIGA+DR+11,RIGA+DR+11,14+i,chiave_alice[i].polarizzazione);
                 }
              fflush(stdout);
            }
          if(chiavi_flag)
            {
              printf("\x1b[%d;%dHBENE: le chiavi corrispondono",RIGA+DR+12,1);
            }
          else
            {
              printf("\x1b[%d;%dHMALE: le chiavi NON corrispondono",RIGA+DR+12,1);
            }
          
          fflush(stdout);
          
          if(eva_flag)
            {
              printf("\x1b[%d;1H*****La comunicazione è stata intercettata*****",RIGA+DR+14);
              for(int i=0; i<indice_qubit_chiave_bob;i++)
                printf("\x1b[%d;1HCHIAVE Eva :\x1b[%d;%dH%d",RIGA+DR+15,RIGA+DR+15,14+i,chiave_eva[i].polarizzazione);
              printf("\n\r");
              fflush(stdout);
              terminale_cucinato();
              exit(0);
            }
          
          printf("\x1b[%d;%dH",riga,colonna);
          break;
          
          
        case ARROW_UP:
          break;
          
        case ARROW_DOWN:
           break;
           
           /* cursore a sinistra */
        case ARROW_LEFT:
          if(colonna>COLONNA)
            colonna--;
          printf("\x1b[%d;%dH",riga,colonna);
          fflush(stdout);
          break;
          
          /* cursore a destra */
        case ARROW_RIGHT:
          if(colonna<COLONNA+DIM_CHIAVE)
            colonna++;
          printf("\x1b[%d;%dH",riga,colonna);
          fflush(stdout);
          break;
        default:
          if(32<=ch&&ch<=127)
            {
              /* Ricevi fotone */
              if(ch == 'a')
                {
                  /* una coppia di fotoni viene emessa nel sistema di Alice */
                  descrizione(alice_base, desc,BASI);
                  printf("\x1b[%d;17H%s",RIGA+3,desc);
                  fflush(stdout);
                  
                  if(eva_flag)
                    {
                      alice_f =  quanto_emetti_fotone(alice_base);
                      bob_f=quanto_intercetta
                        ( alice_f, alice_base,chiave_eva);
                    }
                  else
                    {
                      alice_f = quanto_emetti_fotone(alice_base);
                      bob_f =  alice_f;
                    }
                  
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
                 }
               
               /* Scopri se Eva sta intercettando */
               if(ch == 'k')
                 {
                   printf("\x1b[35;1HBackdoor di EVA: %d",eva_flag);
                   fflush(stdout);
                 }
               
               /* Vedi base Alice  */
               if(ch == 'i')
                 {
                   descrizione(bob_f.base, desc,BASI);
                   printf("\x1b[%d;17H%s",RIGA+3,desc);
                   
                   printf("\x1b[%d;%dH",riga,colonna);
                   fflush(stdout);
                 }
               
               /* Confronta qubit selezionato */
               if(ch == 'c')
                 {
                   //l'indice del qubit è dato dalla
                   //posizione del cursore rispetto l'inizio della sequenza di qubit salvati
                   inx = colonna-(COLONNA) ;
                   tmp = chiave_alice[inx].polarizzazione ;
                   if(chiave_bob[inx].accettato>0 && inx<DIM_CHIAVE)
                     {
                       printf("\x1b[%d;%dH%d",RIGA+DR+2,COLONNA+inx,chiave_alice[inx].polarizzazione);                    
                       printf("\x1b[%d;%dH",riga,colonna);
                       fflush(stdout);
                     }
                   //Il confronto tra Bob e Alice prevede la trasmissione del valore
                   //del qubit sul canale classico:
                   //Eva scopre il valore di questo qubit
                   chiave_eva[inx].polarizzazione = chiave_alice[inx].polarizzazione;
                   break;
                 }
             }
           
           break;
         }
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
  
  Con una certa probabilità Eva decide di intercettare
  la comunicazione della chiave tra Alice e Bob
 */
char eva_intercetta()
{
  float f = (float)rand()/(float)RAND_MAX;
  if(f>0.5) return 1;
  return 0;
}

/*____________________________________________________

  Bob ha ricevuto un fotone da Alice. Il fotone di 
  Alice è stato già rivelato nel sistema di Alice 
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
FOTONE quanto_intercetta
(FOTONE emesso, BASE base_alice, QNODO chiave_eva[])
{
  BASE b = base_alice;
  FOTONE p = emesso;
  FOTONE impostore;
   
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
  di fotoni entangled
  uno viene rivelato dal rivelatore di Alice
  l'altro viene mandato a Bob che lo rivela e
  vi associa il valore di un qubit
 */
FOTONE
quanto_emetti_fotone(BASE base_suggerita)
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

