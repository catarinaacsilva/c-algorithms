/*******************************************************************************
  Ficheiro de implementação do Tipo de Dados Abstracto SEQ_LIST (seqlist.c).
  A estrutura de dados de suporte da sequência é uma estrutura, constituída pelos
  campos de tipo inteiro Size para indicar o número de elementos armazenados na
  sequência e os campos de tipo ponteiro para nós de lista biligada Head e Tail,
  para representar, respectivamente, a cabeça e a cauda da lista biligada onde
  são armazenados os números inteiros.

Autor : CATARINA ALEXANDRA CORREIA SILVA       NMEC : 76399       
 *******************************************************************************/

#include <stdio.h> 
#include <stdlib.h>

#include "seqlist.h"  /* Ficheiro de interface do TDA - ADT Interface file */

/************ Definição da Estrutura de Dados Interna da Sequência ************/

typedef struct binode *PtBiNode;
struct binode /* definição do nó da lista biligada */
{
  int Elem; /* o elemento da lista */
  PtBiNode PtPrev, PtNext; /* ponteiros para o nós anterior e seguinte */
};

struct seqlist
{
  int Size; /* número de elementos - sequence's size */
  PtBiNode Head; /* ponteiro para o início da lista (cabeça da lista) - list head */
  PtBiNode Tail; /* ponteiro para o fim da lista (cauda da lista) - list tail */
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
  "ficheiro inexistente - File does not exist",
  "ponteiro nulo - Null pointer"
};

static char *AbnormalErrorMessage = "erro desconhecido - Unknown error";

/************** Número de mensagens de erro previstas no módulo ***************/

#define N (sizeof (ErrorMessages) / sizeof (char *))

/******************** Protótipos dos Subprogramas Internos ********************/

PtBiNode BiNodeCreate (int);
void BiNodeDestroy (PtBiNode *);
void DoubleListDestroy (PtBiNode *);

/*************************** Definição das Funções ****************************/

void SeqListClearError (void)
{ Error = OK; }

int SeqListError (void)
{ return Error; }

char *SeqListErrorMessage (void)
{
  if (Error < N) return ErrorMessages[Error];
  else return AbnormalErrorMessage;  /* sem mensagem de erro - no error message */
}

PtSeqList SeqListCreate ()
{
  PtSeqList list = NULL; //criar um ponteiro para lista

  // criar uma lista;
  if ((list = (PtSeqList) malloc (sizeof (struct seqlist))) == NULL){ 
    Error = NO_MEM; 
    return NULL; 
  }

  list->Size = 0; // ainda nao tem elementos --> lista vazia
  list->Head = NULL;
  list->Tail = NULL;

  Error = OK;
  return list; // devolve uma lista
}

void SeqListDestroy (PtSeqList *pseq)
{
  if (*pseq == NULL){ 
    Error = NO_SEQ; 
    return; 
  }

  Error = OK;
  if((*pseq)->Size > 0) {
    DoubleListDestroy(&(*pseq)->Head);
  }

  free (*pseq);
  *pseq = NULL;
}

PtSeqList SeqListCopy (PtSeqList pseq)
{
  PtSeqList Copy;

  /* verifica se a sequência existe - verifies if sequence exists */
  if (pseq == NULL) { Error = NO_SEQ; return NULL; }

  /* criação da sequência copia nulo - creating an empty sequence */
  if ((Copy = SeqListCreate ()) == NULL) return NULL;

  /* fazer a copia da sequência - copying the components */
  for (PtBiNode Node = pseq->Head; Node != NULL; Node = Node->PtNext)
  {
    SeqListInsert (Copy, Node->Elem);
    if (Error == NO_MEM) break;
  }

  if (Error == NO_MEM) { SeqListDestroy (&Copy); return NULL; }

  Copy->Size = pseq->Size;
  Error = OK;
  return Copy;  /* devolve a sequência copia - returning the new sequence */
}

PtSeqList SeqListFileCreate (char *pfname)
{
  PtSeqList Seq; FILE *PtF; unsigned int Size, I; int Elem;

  /* abertura com validacao do ficheiro para leitura - opening the text file for reading */
  if ( (PtF = fopen (pfname, "r")) == NULL) { Error = NO_FILE; return NULL; }

  /* leitura da dimensão da sequência e do número de elementos */
  /* reading the sequence's dimension and the number of elements */
  fscanf (PtF, "%u", &Size);
  if (Size < 1) { Error = BAD_SIZE; fclose (PtF); return NULL; }

  /* criação da sequência vazia - creating an empty sequence */
  if ((Seq = SeqListCreate ()) == NULL) { fclose (PtF); return NULL; }

  Error = OK;
  /* leitura da sequência do ficheiro - reading the sequence's components from the text file */
  for (I = 0; I < Size; I++)
  {
    fscanf (PtF, "%d", &Elem);
    SeqListInsert (Seq, Elem);
    if (Error == NO_MEM) break;
  }

  if (Error == NO_MEM) { SeqListDestroy (&Seq); return NULL; }

  fclose (PtF);  /* fecho do ficheiro - closing the text file */
  return Seq;  /* devolve a sequência criada - returning the new sequence */
}

void SeqListStoreFile (PtSeqList pseq, char *pfname)
{
  FILE *PtF;

  /* verifica se a sequência existe - verifies if sequence exists */
  if (pseq == NULL) { Error = NO_SEQ; return ; }

  /* verifica se a sequência tem elementos - verifies if sequence has elements */
  if (pseq->Size == 0) { Error = SEQ_EMPTY; return ; }

  /* abertura com validacao do ficheiro para escrita - opening the text file for writing */
  if ((PtF = fopen (pfname, "w")) == NULL) { Error = NO_FILE; return ; }

  /* escrita do número de elementos armazenados na sequência */
  /* writing the number of elements */
  fprintf (PtF, "%u\n", pseq->Size);

  /* escrita da sequência - writing the sequence's components in the text file */
  for (PtBiNode Node = pseq->Head; Node != NULL; Node = Node->PtNext)
    fprintf (PtF, "%d\n", Node->Elem);

  Error = OK;
  fclose (PtF);  /* fecho do ficheiro - closing the text file */
}

int SeqListGetSize (PtSeqList pseq)
{
  if(pseq == NULL){
    Error = NO_SEQ;
    return 0;
  }

  Error = OK;
  return pseq->Size;	
}

int SeqListGetElement (PtSeqList pseq, int pindex)
{
  int elem = 0;

  if(pindex <  -(pseq->Size) && pindex >= pseq->Size) {
    Error = BAD_INDEX;
  } else {
    Error = OK;
    int idx = (pindex < 0) ? pseq->Size-pindex : pindex;

    PtBiNode node = pseq->Head;
    for(int i = 0; i < idx; node = node->PtNext, i++);

    elem = node->Elem;	
  }

  return elem;

}

void SeqListSetElement (PtSeqList pseq, int pindex, int pvalue)
{
  if(pindex <  -(pseq->Size) && pindex >= pseq->Size) {
    Error = BAD_INDEX;
    return ;
  } else {
    Error = OK;
    int idx = (pindex < 0) ? pseq->Size-pindex : pindex;

    PtBiNode node = pseq->Head;
    for(int i = 0; i < idx; node = node->PtNext, i++);

    node->Elem = pvalue;	
  }	
}

int SeqListEquals (PtSeqList pseq1, PtSeqList pseq2)
{
  int equals = 0;

  if(pseq1 == NULL || pseq2 == NULL){
    Error = NO_SEQ;
  }
  else{
    Error = OK;
    if(pseq1->Size == pseq2->Size){
      PtBiNode node1 = pseq1->Head;
      PtBiNode node2 = pseq2->Head;
      equals = 1;
      for(int i = 0; i < pseq1->Size; i++){
        if(node1->Elem != node2->Elem) {
          equals = 0;
          break;
        }
        node1 = node1->PtNext;
        node2 = node2->PtNext;
      }
    }
  }

  return equals;
}

void SeqListInsert (PtSeqList pseq, int pvalue)
{
  if(pseq == NULL){
    Error = NO_SEQ;
    return;
  } else {
    Error = OK;
    PtBiNode node = BiNodeCreate(pvalue);
    if(Error == NO_MEM) {
      return;
    }
    node->PtNext = NULL;
    if(pseq->Size == 0) {
      pseq->Head = node;
      pseq->Tail = node;
    } else {
      node->PtPrev = pseq->Tail;
      pseq->Tail->PtNext = node;
      pseq->Tail = node;
    }
    
    pseq->Size++;
  }
}  

void SeqListDelete (PtSeqList pseq, int pvalue)
{
  if(pseq == NULL){
    Error = NO_SEQ;
    return;
  } 

  if(pseq->Size == 0){
    Error = SEQ_EMPTY;
    return;
  }

  PtBiNode node = pseq->Head;
  for(; node != NULL && node->Elem != pvalue; node=node->PtNext);

  if(node == NULL) {
    Error = NO_NUMBER;
    return;
  }

  Error = OK;

  if(pseq->Head == node) {
    pseq->Head = node->PtNext;
  }

  if(pseq->Tail == node) {
    pseq->Tail = node->PtPrev;
  }

  if(node->PtNext != NULL) {
    node->PtNext->PtPrev = node->PtPrev;
  }

  if(node->PtPrev != NULL) {
    node->PtPrev->PtNext = node->PtNext;
  }
  BiNodeDestroy(&node);
  pseq->Size--;
}

int SeqListIsOrdered (PtSeqList pseq)
{
  int rv = 0;

   if(pseq == NULL){
    Error = NO_SEQ;
    return rv;
  } 

  if(pseq->Size == 0){
    Error = SEQ_EMPTY;
    return rv;
  }

  Error = OK;

  rv = 1;
  PtBiNode node = pseq->Head;
  int pn = node->Elem;
  node = node->PtNext;
  for(;node != NULL; node=node->PtNext) {
    if(node->Elem < pn) {
      rv = 0;
      break;
    } else {
      pn = node->Elem;
    }
  }

  return rv;
}

int SeqListIsEvenOdd (PtSeqList pseq)
{
	int result = 1;
	
	if(pseq == NULL){
		Error = NO_SEQ;
	    	result = 0;
	} 

	else if (pseq->Size == 0){
	    	Error = SEQ_EMPTY;
	    	result = 0;
	}
	
	else{
		Error = OK;
		PtBiNode node = pseq->Head;
		int parity = node->Elem % 2 == 0; //é par
		node = node->PtNext; // i++
  		for(;node != NULL; node=node->PtNext) {
			int newParity = node->Elem % 2 == 0;
			if((!parity) != newParity){
				result = 0;
				break;
			}
			parity = newParity;		
		}
	}

	return result;
}

int SeqListNumberOfMultiples (PtSeqList pseq, int pvalue)
{
	int elem = 0, count = 0;

	if(pseq == NULL){
		Error = NO_SEQ;
	} 
	
	else if (pseq->Size == 0){
	    	Error = SEQ_EMPTY;
	}
	
	else{
		Error = OK;
		for(PtBiNode node = pseq->Head;node != NULL; node=node->PtNext) {
			elem = node->Elem;
			if(elem % pvalue == 0)
				count++;
		}
	}
	
	return count;
}

void SeqListSmallerBigger (PtSeqList pseq, int * psmall, int * pbig)
{
	if(pseq == NULL){
		Error = NO_SEQ;
	    	return ;
	} 

	else if (pseq->Size == 0){
	    	Error = SEQ_EMPTY;
	    	return ;
	}
	
	else{
		Error = OK;
		int big = pseq->Head->Elem;
		int small = pseq->Head->Elem;
		PtBiNode node = pseq->Head;
		node = node->PtNext;
		for(;node != NULL; node=node->PtNext) {
			if(node->Elem > big)
				big = node->Elem;
			else if(node->Elem < small)
				small = node->Elem;
		}
		*psmall = small;
		*pbig = big;
	}
	
}

int SeqListIsPalindromic (PtSeqList pseq)
{
	int palindromic=0;

	if(pseq == NULL){
		Error = NO_SEQ;
	} 

	else if (pseq->Size == 0){
	    	Error = SEQ_EMPTY;
		palindromic = 0;
	    	return palindromic;
	}
	
	else{
		Error = OK;
		palindromic=1;
		for(PtBiNode l = pseq->Head, r = pseq->Tail; l != NULL; l=l->PtNext, r = r->PtPrev) {
			if(l->Elem != r->Elem) {
			palindromic = 0;
			break;	
			}
		}
	}

	return palindromic;
}

double SeqListAverage (PtSeqList pseq)
{
	double soma=0, count=0, result=0;	

	if(pseq == NULL){
		Error = NO_SEQ;
	    	result = 0;
	} 

	else if (pseq->Size == 0){
	    	Error = SEQ_EMPTY;
	    	result = 0;
	}
	
	else{
		Error = OK;
		PtBiNode node = pseq->Head;
		for(;node != NULL; node=node->PtNext) {
			soma += node->Elem;
			count++;
			
		}
		result = soma/count;
	}
	
	return result;
  
}

/*******************************************************************************
  Função auxiliar para criar um nó da lista biligada. Valores de erro: OK ou NO_MEM.

  Auxiliary function to create a binode. Error codes: OK or NO_MEM.
 *******************************************************************************/

PtBiNode BiNodeCreate (int pelem)	/* alocação do nó */
{
  PtBiNode Node;

  if ((Node = (PtBiNode) malloc (sizeof (struct binode))) == NULL)
  { Error = NO_MEM; return NULL; }

  Node->Elem = pelem;	/* copiar a informação */
  Node->PtPrev = NULL;	/* apontar para detrás para NULL */
  Node->PtNext = NULL;	/* apontar para a frente para NULL */

  Error = OK;
  return Node;
}

/*******************************************************************************
  Função auxiliar para libertar um nó da lista biligada. Valores de erro: OK ou NULL_PTR.

  Auxiliary function to free a binode. Error codes: OK or NULL_PTR.
 *******************************************************************************/
void BiNodeDestroy (PtBiNode *pnode)	/* libertação do nó */
{
  if (*pnode == NULL) { Error = NULL_PTR; return; }
  free (*pnode);	/* libertação do nó */
  *pnode = NULL;	/* colocar o ponteiro a nulo */
  Error = OK;
}

/*******************************************************************************
  Função auxiliar para destruir uma lista biligada. Valores de erro: OK ou NULL_PTR.

  Auxiliary function to destroy a double link list. Error codes: OK or NULL_PTR.
 *******************************************************************************/
void DoubleListDestroy (PtBiNode *phead)
{
  PtBiNode TmpHead = *phead; PtBiNode Node;

  if (TmpHead == NULL) { Error = NULL_PTR; return; }
  while (TmpHead != NULL)
  {
    Node = TmpHead; TmpHead = TmpHead->PtNext;
    BiNodeDestroy (&Node);
  }
  Error = OK;
}
