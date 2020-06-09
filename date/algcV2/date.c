/************************************************************************
 Ficheiro de implementação do Tipo de Dados Abstracto Data (date.c).
 A estrutura de dados de suporte do tempo é um registo constituído pelos
 três campos de tipo inteiro Day, Month e Year.

 Autor : CATARINA ALEXANDRA CORREIA SILVA               NMEC : 76399           
************************************************************************/

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "date.h" /* Ficheiro de interface do TDA - ADT Interface file */

/*************** Definição da Estrutura de Dados da Data ***************/

struct date
{
	unsigned int Day; unsigned int Month; unsigned int Year;
};

/******************** Controlo Centralizado de Erro ********************/

static unsigned int Error = OK;	/* inicialização do erro */

static char *ErrorMessages[] = { "sem erro", "data inexistente",
                                 "memoria esgotada", "data invalida",
				 "ponteiro inexistente" };

static char *AbnormalErrorMessage = "erro desconhecido";

/*********** Número de mensagens de erro previstas no módulo ***********/

#define N (sizeof (ErrorMessages) / sizeof (char *))

/***************** Protótipos dos Subprogramas Internos ****************/

static int LeapYear (int); /* ano bissexto */
static int ValidDate (int, int, int); /* validar data */

/********************** Definição dos Subprogramas *********************/

void DateClearError (void)
{ Error = OK; }

int DateError (void)
{ return Error; }

char *DateErrorMessage (void)
{
	if (Error < N) return ErrorMessages [Error];
	else return AbnormalErrorMessage; /* sem mensagem de erro */
}

PtDate DateCreate (int pday, int pmonth, int pyear) /* construtor inicializador */
{
	if(!ValidDate(pday, pmonth, pyear))
		return NULL;
	PtDate Date;
	
	//cria a data
	if((Date = malloc (sizeof(struct date))) == NULL){
		Error = NO_MEM;
		return NULL;
	}
	
	//armazenamento de informação das datas
	Date->Day = pday;
	Date->Month = pmonth;
	Date->Year = pyear;
	
	Error = OK;

	return Date;
	
}

void DateDestroy (PtDate *pdate) /* destrutor */
{
	PtDate TmpDate = *pdate;
	
	//verificar se a data existe
	if(TmpDate == NULL){
		Error = NO_DATE;
		return ;
	}
	
	free(TmpDate); //libertar a memória
	
	Error = OK;
	*pdate = NULL;
}

PtDate DateStringCreate (char *pstrdate) /* construtor a partir de uma string */
{
	if (pstrdate == NULL) { Error = NULL_PTR; return NULL; }
	if (strlen (pstrdate) != 10) { Error = INVALID; return NULL; }

	char * date[4]; int i = 0;

	date[i] = strtok (pstrdate, "-");
	while (date[i] != NULL) date[++i] = strtok (NULL, "-");

	if (i != 3) { Error = INVALID; return NULL; }

	int Year, Month, Day;
	sscanf (date[0], "%d", &Year);
	sscanf (date[1], "%d", &Month);
	sscanf (date[2], "%d", &Day);

	return DateCreate (Day, Month, Year);
}

PtDate DateCopy (PtDate pdate)	/* construtor cópia */
{
	if (pdate == NULL) { Error = NO_DATE; return NULL; }
	else return DateCreate (pdate->Day, pdate->Month, pdate->Year);
}

int  DateGetYear (PtDate pdate) /* observador do ano */
{
	//verificar se o ano existe
	if(pdate == NULL){
		Error = NO_DATE;
		return 0;
	}
	
	Error = OK;
	return pdate->Year;
}

int DateGetMonth (PtDate pdate) /* observador do mês */
{
	if (pdate == NULL) { Error = NO_DATE; return 0; }
	else { Error = OK; return pdate->Month; }
}

int DateGetDay (PtDate pdate) /* observador do dia */
{
	//verificar se o ano existe
	if(pdate == NULL){
		Error = NO_DATE;
		return 0;
	}
	
	Error = OK;
	return pdate->Day;
}

void DateSet (PtDate pdate, int pday, int pmonth, int pyear) /* modificador da data */
{
	if (pdate == NULL) { 
		Error = NO_DATE; 
		return ; //função void --> não devolve nada
	}
	
	if(!ValidDate(pday, pmonth, pyear) == 0){
		Error = NO_DATE;
		return ; //função void --> não devolve nada
	}
				
	
}

//verificar se duas datas sao iguais
int DateEquals (PtDate pdate1, PtDate pdate2)  /* comparador de igualdade */
{
	if((pdate1 == NULL) || (pdate2 == NULL)){
		Error = NO_DATE;
		return 0;
	}

	//sao iguais se o dia1 = dia2, mes1 = mes2, ano1=ano2
	// 1 --> sao iguais ; 0 --> nao sao iguais
	if(pdate1->Day == pdate2->Day && pdate1->Month == pdate2->Month && pdate1->Year == pdate2->Year)
		return 1;
	else
		return 0; 		
}

//devolve:
// 0 - iguais
// 1 - ano1 > ano2 --> pdate1 > pdate2
// -1 - ano1 < ano2 --> pdate1 < pdate2
int DateCompareTo (PtDate pdate1, PtDate pdate2)  /* operador relacional (> == <) */
{
	if((pdate1 == NULL) || (pdate2 == NULL)){
		Error = NO_DATE;
		return 0;
	}

	if(DateEquals(pdate1, pdate2) == 1)
		return 0; // sao iguais
	
	if(pdate1->Year > pdate2->Year) // ano1 > ano2 --> pdate1 > pdate2
		return 1;
	if(pdate1->Year < pdate2->Year) // ano1 < ano2 --> pdate1 < pdate2
		return -1;
	
	if(pdate1->Year == pdate2->Year){
		if(pdate1->Month < pdate2->Month)
			return -1;
		if(pdate1->Month > pdate2->Month)
			return 1;
		if(pdate1->Month == pdate2->Month){
			if(pdate1->Day < pdate2->Day)
				return -1;
			if(pdate1->Day > pdate2->Day)
				return 1;
		}
	}
	
}

int DateDayMonth (int pmonth, int pyear) /* dias de um mês */
{
	int MonthDays[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

}

char * DateToString (PtDate pdate)  /* data no formato dd/mm/yyyy */
{
	/* verifica se a data existe - verifies if date exists */
	if (pdate == NULL) { Error = NO_DATE; return NULL; }

	char * Str;
	/* cria a sequência de caracteres - allocating the string */
	if ((Str = calloc (11, sizeof (char))) == NULL)
	{ Error = NO_MEM; return NULL; }

	sprintf (Str, "%02d/%02d/%04d", pdate->Day, pdate->Month, pdate->Year);
	Error = OK;
	return Str;
}

char * DateToFullString (PtDate pdate)  /* data por extenso */
{
	char* FullMonth[] = {"janeiro", "fevereiro", "março", "abril",
						 "maio", "junho", "julho", "agosto",
						 "setembro", "outubro", "novembro", "dezembro"};

	/* verifica se a data existe - verifies if date exists */
	if (pdate == NULL) { Error = NO_DATE; return NULL; }

	char * Str;  
	/* cria a sequência de caracteres - allocating the string */
	if ((Str = calloc (23, sizeof (char))) == NULL)
	{ Error = NO_MEM; return NULL; }

	sprintf (Str, "%02d de %s de %04d", pdate->Day, FullMonth[pdate->Month-1], pdate->Year);
	Error = OK;
	return Str;
}

PtDate DateTomorrow (PtDate pdate)  /* nova data com o dia seguinte */
{
	int day = pdate->Day;
	int month = pdate->Month;
	int year = pdate->Year;
	
	int nextDay = day, nextMonth = month, nextYear = year;

	//meses com 31 dias
	if(month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12){
		if(day >= 1 && day < 31)
			nextDay = day+1;
		else{
			nextDay = 1; // primeiro dia do mes seguinte
			if(month != 12)
				nextMonth = month+1; //proximo mes
			else{
				nextDay = 1;
				nextMonth= 1; // janeiro
				nextYear = year+1;
			}
		}
	} 
	
	//meses com 30 dias
	if(month == 4 || month == 6 || month == 9 || month == 11){
		if (day >= 1 && day <= 30)
			nextDay = day+1;
		else{
			nextDay = 1;
			if(month != 12)
				nextMonth = month+1;
			else{
				nextDay = 1;
				nextMonth = 1;
				nextYear = year+1;
			}
		}	
	}

	//mes de fevereiro
	if(month == 2){
		// ano bissexto --> 29 dias
		if(LeapYear(year) == 1){ 
			if(day < 29)
				nextDay = day+1;
			else{
				nextDay = 1;
				nextMonth = 3;
			}
		}
		//fevereiro --> 28 dias
		else{
			if(day < 28)
				nextDay = day + 1;
			else{
				nextDay = 1;
				nextMonth = 3;
			}
		}
	}

	else{
		Error = NO_DATE; 
		pdate = NULL;
	}
	
	return DateCreate (nextDay, nextMonth, nextYear);
			
}

PtDate DateYesterday (PtDate pdate)  /* nova data com o dia anterior */
{
	int day = pdate->Day;
	int month = pdate->Month;
	int year = pdate->Year;
	
	int previousDay = day, previousMonth = month, previousYear = year;
	
	//data anterior ao dia 1 de janeiro: (muda o dia, o mes e o ano) 1-1-XXXX --> 31-12-XXXX-1
	if(month == 1){
		if(day >= 1 && day <= 31)
			previousDay = day-1;
		if(day == 1){
			previousDay = 31;
			previousMonth = 12;
			previousYear = year-1;
		}
		else{
			Error = NO_DATE; 
			pdate = NULL;
		}
	}
	
	// o dia anterior a 1 de Março depende se o ano é bissexto ou não
	if(month == 3){
		if(day >= 1 && day <= 31)
			previousDay = day-1;	
		// ano é bissexto
		if(day == 1){
			previousMonth = 2;

			if(LeapYear(year) == 1) 	
				previousDay = 29;
								
			
			else
				previousDay = 28;
			
		}
		else{
			Error = NO_DATE; 
			pdate = NULL;
		}
		
	}

	if(month == 2){
		if((day >= 1 && day <= 29) || (day >= 1 && day <= 28)){
				previousDay = day - 1;
			if(day == 1){
				previousMonth = 1;
				previousDay = 31;
			}
			else{
				Error = NO_DATE; 
				pdate = NULL;
			}
		}		
	}
	
	//meses a seguir aos meses de 31
	if(month == 4 || month == 6 || month == 8 || month == 9 || month == 11){
		if(day >= 1 && day <= 30)
			previousDay = day-1;
		if(day == 1){
			previousDay = 31;
			previousMonth = month - 1;
		}
		else{
			Error = NO_DATE; 
			pdate = NULL;
		}	
	}

	//meses a seguir aos meses de 30
	if(month == 5 || month == 7 || month == 10 || month == 12){
		if(day >= 1 && day <= 31)
			previousDay = day - 1;
		if(day == 1){
			previousDay = 30;
			previousMonth = month-1;
		}	
	}

	else{
		Error = NO_DATE; 
		pdate = NULL;
	}
	
	return DateCreate (previousDay, previousMonth, previousYear);
}

/*************** Implementação dos Subprogramas Internos ***************/

/*******************************************************************************
 Função auxiliar que verifica se um ano é bissexto. Devolve 1 em caso afirmativo
  e 0 em caso contrário. Valores de erro: OK.
 
 Auxiliary function to verify if a year is a leap year. Returns 1 in affirmative
 case and 0 otherwise. Error codes: OK.
*******************************************************************************/
static int LeapYear (int pyear)
{
	Error = OK;
	return ((pyear % 4 == 0) && (pyear % 100 != 0)) || ((pyear % 400 == 0));
}

/*******************************************************************************
 Função auxiliar que verifica se uma data definida por dia, mês e ano é válida.
 Devolve 1 em caso afirmativo e 0 em caso contrário. Valores de erro: OK ou INVALID.
 
 Auxiliary function to verify if a date defined by day, month and year is valid.
 Returns 1 in affirmative case and 0 otherwise. Error codes: OK or INVALID.
*******************************************************************************/

static int ValidDate (int pday, int pmonth, int pyear)
{
	int leapyear;

	Error = OK;
	switch (pmonth)
	{
		case  1:
		case  3:
		case  5:
		case  7:
		case  8:
		case 10:
    		case 12: if (pday < 1 || pday > 31) Error = INVALID;
			 break;
    		case  4:
    		case  6:
    		case  9:
    		case 11: if (pday < 1 || pday > 30) Error = INVALID;
			 break;
    		case  2: leapyear = LeapYear (pyear);
                	 if (pday < 1 || (pday > 29 && leapyear) || (pday > 28 && !leapyear))
				Error = INVALID;
                	 break;
	}
	if (Error == OK) return 1; else return 0;
}
