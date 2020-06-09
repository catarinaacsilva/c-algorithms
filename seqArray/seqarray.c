/*******************************************************************************
  Ficheiro de implementação do Tipo de Dados Abstracto SEQ_ARRAY (seqarray.c).
  A estrutura de dados de suporte da sequência é um registo, constituída pelos
  campos de tipo inteiro Dim para indicar a capacidade de armazenamento da 
  sequência e Size para indicar o número de elementos efectivamente armazenados na
  sequência e o campo de tipo ponteiro Seq, para representar a sequência (array) 
  atribuída dinamicamente onde são armazenados os números inteiros.

Autor : CATARINA ALEXANDRA CORREIA SILVA              NMEC : 76399           
 *******************************************************************************/

#include <stdio.h> 
#include <stdlib.h>

#include "seqarray.h"  /* Ficheiro de interface do TDA - ADT Interface file */

/************ Definição da Estrutura de Dados Interna da Sequência ************/

struct seqarray
{
  unsigned int Dim; /* capacidade de armazenamento - array's size */
  int Size; /* número de elementos - sequence's size */
  int *Seq; /* ponteiro para armazenar a sequencia - array to store the numbers */
};

/*********************** Controlo Centralizado de Error ************************/

static unsigned int Error = OK;  /* inicialização do erro */

static char *ErrorMessages[] = {
  "sem erro - Without Error",
  "sequencia(s) inexistente(s) - Sequence(s) do not exist",
  "memoria esgotada - Out of memory",
  "indice errado - Wrong index",
  "elemento inexistente - Element does not exist",
  "sequencia vazia - Empty sequence",
  "sequencia cheia - Full sequence",
  "dimensao da sequencia errada - Wrong size",
  "ficheiro inexistente - File does not exist"
};

static char *AbnormalErrorMessage = "erro desconhecido - Unknown error";

/************** Número de mensagens de erro previstas no módulo ***************/

#define N (sizeof (ErrorMessages) / sizeof (char *))

/*************************** Definição das Funções ****************************/

void SeqArrayClearError (void)
{ Error = OK; }

int SeqArrayError (void)
{ return Error; }

char *SeqArrayErrorMessage (void)
{
  if (Error < N) return ErrorMessages[Error];
  else return AbnormalErrorMessage;  /* sem mensagem de erro - no error message */
}

PtSeqArray SeqArrayCreate (unsigned int pdim)
{
  PtSeqArray Sequence = NULL;
  if(pdim < 1) {
    Error = BAD_SIZE;
  } else {	
    //criar a sequencia
    if((Sequence = malloc (sizeof (struct seqarray))) == NULL){
      Error = NO_MEM;
    } else {
      if((Sequence -> Seq = calloc(pdim, sizeof(int))) == NULL) {
        Error = NO_MEM;
        free(Sequence);
        Sequence = NULL;      
      } else {			
        Sequence -> Dim = pdim;
        Sequence -> Size = 0;
        Error = OK;
      }
    }
  }
  return Sequence;
}

void SeqArrayDestroy (PtSeqArray *pseq)
{
  PtSeqArray TmpSeq = *pseq;
  // verifica se a sequencia existe
  if(TmpSeq == NULL) {
    Error = NO_SEQ;
  } else {
    // libertação da memoria dinamica
    free(TmpSeq->Seq);
    free(TmpSeq);
    Error = OK;
    *pseq = NULL; // programação defensiva
  }
}

PtSeqArray SeqArrayCopy (PtSeqArray pseq)
{
  PtSeqArray Copy; int I;

  /* verifica se a sequência existe - verifies if sequence exists */
  if (pseq == NULL) { Error = NO_SEQ; return NULL; }

  /* criação da sequência copia vazia - creating an empty sequence */
  if ((Copy = SeqArrayCreate (pseq->Dim)) == NULL) return NULL;

  /* fazer a copia dos elementos da sequência - copying the components */
  for (I = 0; I < pseq->Size; I++) Copy->Seq[I] = pseq->Seq[I];
  Copy->Size = pseq->Size;

  return Copy;  /* devolve a sequência copia - returning the new sequence */
}

PtSeqArray SeqArrayFileCreate (char *pfname)
{
  PtSeqArray Seq; FILE *PtF; unsigned int Dim, Size, I;

  /* abertura com validacao do ficheiro para leitura - opening the text file for reading */
  if ( (PtF = fopen (pfname, "r")) == NULL) { Error = NO_FILE; return NULL; }

  /* leitura da dimensão da sequência e do número de elementos armazenados */
  /* reading the sequence's dimension and the number of elements */
  fscanf (PtF, "%u %u", &Dim, &Size);
  if (Dim < 1) { Error = BAD_SIZE; fclose (PtF); return NULL; }

  /* criação da sequência com a dimensão pretendida - creating the sequence */
  if ((Seq = SeqArrayCreate (Dim)) == NULL) { fclose (PtF); return NULL; }

  /* leitura da sequência do ficheiro - reading the sequence's components from the text file */
  for (I = 0; I < Size; I++) fscanf (PtF, "%d", Seq->Seq+I);
  Seq->Size = Size;

  fclose (PtF);  /* fecho do ficheiro - closing the text file */
  Error = OK;
  return Seq;  /* devolve a sequência criada - returning the new sequence */
}

void SeqArrayStoreFile (PtSeqArray pseq, char *pfname)
{
  FILE *PtF; unsigned int I;

  /* verifica se a sequência existe - verifies if sequence exists */
  if (pseq == NULL) { Error = NO_SEQ; return ; }

  /* verifica se a sequência tem elementos - verifies if sequence has elements */
  if (pseq->Size == 0) { Error = SEQ_EMPTY; return ; }

  /* abertura com validacao do ficheiro para escrita - opening the text file for writing */
  if ((PtF = fopen (pfname, "w")) == NULL) { Error = NO_FILE; return ; }

  /* escrita da dimensão da sequência e do número de elementos armazenados na sequência */
  /* writing the sequence's dimension and the number of elements */
  fprintf (PtF, "%u %u\n", pseq->Dim, pseq->Size);

  /* escrita da sequência - writing the sequence's components in the text file */
  for (I = 0; I < pseq->Size; I++) fprintf (PtF, "%d\n", pseq->Seq[I]);

  Error = OK;
  fclose (PtF);  /* fecho do ficheiro - closing the text file */
}

int  SeqArrayGetDimension (PtSeqArray pseq)
{
  int rv = -1;
  if(pseq == NULL){
    Error = NO_SEQ;
  } else {
    Error = OK;
    rv = pseq -> Dim;
  }
  return rv;
}

int SeqArrayGetSize (PtSeqArray pseq)
{
  int rv = -1;
  if(pseq == NULL) {
    Error = NO_SEQ;
  } else {
    Error = OK;
    rv = pseq -> Size;
  }
  return rv; 
}

int SeqArrayGetElement (PtSeqArray pseq, int pindex)
{
  int elem=0;
  if(pseq == NULL){
    Error = NO_SEQ;
  } else if(pindex > pseq->Size || pindex < -pseq->Size) {
    Error = BAD_INDEX;
  }
  else{
    Error = OK;
    if(pindex >= 0) {
      elem = pseq -> Seq[pindex];
    } else {
      elem = pseq -> Seq[pseq->Size - pindex];
    }
  }

  return elem;

}

void SeqArraySetElement (PtSeqArray pseq, int pindex, int pvalue)
{
  if(pseq == NULL){
    Error = NO_SEQ;
  } else if(pindex > pseq->Size || pindex < -pseq->Size) {
    Error = BAD_INDEX;
  } else {
    Error = OK;
    if(pindex >= 0) {
      pseq -> Seq[pindex] = pvalue;
    } else {
      pseq -> Seq[pseq->Size - pindex] = pvalue;
    }
  }	
}

// 1 -> sequencias iguais ; 0 -> sequencias diferentes
int SeqArrayEquals (PtSeqArray pseq1, PtSeqArray pseq2)
{
  int compare = 1;
  if ((pseq1 == NULL) || (pseq2 == NULL)){ 
    Error = NO_SEQ; 
  }
  else{
    Error = OK;
    if(pseq1->Size == pseq2->Size){
      for(int i = 0; i < pseq1->Size; i++){ // como o tamanho da sequencia é o mesmo usa se um deles no for
        if(SeqArrayGetElement (pseq1, i) != SeqArrayGetElement (pseq2, i)) {
          compare = 0;
          break;
        }
      }
    }
    else {
      compare = 0;
    }
  }

  return compare;
}

// inserir um elemento na sequencia
void SeqArrayInsert (PtSeqArray pseq, int pvalue)
{
  if (pseq == NULL){ 
    Error = NO_SEQ; 
  } else if(pseq->Size == pseq->Dim) {
    Error = SEQ_FULL;	
  } else {
    Error = OK;
    int idx = pseq->Size;
    pseq->Size++;
    SeqArraySetElement(pseq, idx, pvalue);
  }
  return;
}

void SeqArrayDelete (PtSeqArray pseq, int pvalue) //falta o NO_NUMBER
{
  if (pseq == NULL){ 
    Error = NO_SEQ; 
  } else if(pseq->Size == 0){
    Error = SEQ_EMPTY;
  }else{
    // encontrar o elemento pvalue
    int idx = -1;
    for(int i = 0; i < pseq->Size; i++) {
      if(pseq->Seq[i] == pvalue) {
        idx = i;
        break;
      }
    }
    // verificar se o elemento existe
    if(idx > -1) {
      Error = OK;
      // copiar a metade direita para reescrever o elemento
      for(int i=idx+1; i < pseq->Size; i++){
        pseq -> Seq[i-1] = pseq -> Seq[i];
      }
      pseq->Size--;
    } else {
      Error = NO_NUMBER;
    }
  }
  return;
}

void SeqArrayDuplicate (PtSeqArray pseq)
{
  int *newSeq;
  /* verifica se a sequência existe - verifies if sequence exists */
  if (pseq == NULL) { Error = NO_SEQ; return; }

  /* verifica se a sequência não está vazia - verifies if sequence is not empty */  
  if (pseq->Size == 0) { Error = SEQ_EMPTY; return; }

  if ((newSeq = calloc (2*pseq->Size, sizeof (int))) == NULL)
  { Error = NO_MEM; return; }

  pseq->Dim = 2*pseq->Size;  /* armazenamento da nova dimensão - storing the new dimension */

  /* copiar os elementos - copying the elements */
  for (int I = 0; I < pseq->Size; I++) newSeq[I] = pseq->Seq[I];
  /* guardar o novo array e libertar o antigo - storing the new arry and freeing the old */
  int * Tmp = pseq->Seq; pseq->Seq = newSeq; free (Tmp);

  Error = OK;
}

// verificar se o array esta ordenado: 1--> ordenado ; 0--> desordenado
int SeqArrayIsOrdered (PtSeqArray pseq)
{	
  int order = 1;
  if (pseq == NULL){ 
    Error = NO_SEQ;  
  } else if(pseq->Size == 0){
    Error = SEQ_EMPTY;
  }
  else{
    Error = OK;
    for(int i=0; i<pseq->Size-1; i++){
      if(pseq->Seq[i] > pseq->Seq[i+1])
        order = 0;
      break;
    }
  }

  return order;

}

int SeqArrayIsEvenOdd (PtSeqArray pseq)
{
  int result = 1;
  if (pseq == NULL){ 
    Error = NO_SEQ; 
  } else if(pseq->Size == 0){
    Error = SEQ_EMPTY;
  }
  else{	
    Error = OK;
    int parity = pseq->Seq[0] % 2 == 0;
    for(int i = 1; i < pseq->Size; i++){
      int newParity = pseq->Seq[i] % 2 == 0;
      if((!parity) != newParity){
        result = 0;
        break;
      }
      parity = newParity;
    } 
  }
  return result;
}

int SeqArrayNumberOfMultiples (PtSeqArray pseq, int pvalue)
{
  int elem=0, count=0;	
  if (pseq == NULL){ 
    Error = NO_SEQ; 
  } else if(pseq->Size == 0){
    Error = SEQ_EMPTY;
  }
  else{
    Error = OK;
    for(int i=0; i<pseq->Size; i++){
      elem = pseq->Seq[i];
      if(elem % pvalue == 0)
        count++;
    }
  }
  return count;
}

void SeqArraySmallerBigger (PtSeqArray pseq, int * psmall, int * pbig)
{
  if (pseq == NULL){ 
    Error = NO_SEQ; 
  } else if(pseq->Size == 0) {
    Error = SEQ_EMPTY;
  } else {
    Error = OK;
    int big = pseq->Seq[0], small = pseq->Seq[0];
    for(int i=1; i < pseq->Size; i++){
      if(pseq->Seq[i] > big) {
        big=pseq->Seq[i];		
      } else if(pseq->Seq[i] < small) {
        small=pseq->Seq[i];
      }
    }

    *psmall = small;
    *pbig = big;
  }
  return;
}

int SeqArrayIsPalindromic (PtSeqArray pseq)
{
  int rv = 1;

  if (pseq == NULL){ 
    Error = NO_SEQ; 
  } else if(pseq->Size == 0) {
    Error = SEQ_EMPTY;
  } else {
    for(int i = 0; i < (pseq->Size/2); i++) {
      if(pseq->Seq[i] != pseq->Seq[pseq->Size-i-1]) {
        rv = 0;
        break;
      }
    }
  }

  return rv;
}

double SeqArrayAverage (PtSeqArray pseq)
{
  double sum = 0.0, result = 0.0;
  if (pseq == NULL){ 
    Error = NO_SEQ; 
  } else if(pseq->Size == 0){
    Error = SEQ_EMPTY;
  }
  else{
    Error = OK;	
    for(int i = 0; i < pseq->Size; i++){
      sum += pseq->Seq[i];	
    }
    result = sum/pseq->Size;
  }
  return result;
}
