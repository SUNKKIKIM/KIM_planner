#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define STRING_MAX 3000

typedef unsigned int BOOL;

enum
{
	FALSE,
	TRUE
};

//날짜
typedef struct _DATE
{
	int m_nYear;
	int m_nMonth;
	int m_nDay;
	int m_nHour;
	int m_nMinute;
}DATE;

void PrintDate(DATE date);
void ModifyMonth(DATE* pDate, int nMonth);
void ModifyDay(DATE* pDate, int nDay);

int CompareDate(DATE target, DATE compare);
int CompareDateAndTime(DATE target, DATE compare);

DATE InputDate(void);
DATE GetToday(void);
DATE current;

//스케줄
typedef struct _SCHEDULE
{
	DATE m_date;
	char m_szText[STRING_MAX];

	struct _SCHEDULE* next;
}SCHEDULE;

void KillAllScheduleNode(SCHEDULE* pHead);
void AddScheduleNode(SCHEDULE* pHead, SCHEDULE schedule);
void PrintAllSchedule(SCHEDULE* pHead);
void PrintScheduleByDate(SCHEDULE* pHead, DATE date);
void DeleteScheduleByDateAndTime(SCHEDULE* pHead, DATE date);
void ChangeScheduleByDateAndTime(SCHEDULE* pHead, SCHEDULE schedule);

void LoadScheduleFromFile(SCHEDULE* pHead, char szFileName[]);
void SaveScheduleToFile(SCHEDULE* pHead, char szFileName[]);

SCHEDULE* InitScheduleHead(void);
SCHEDULE* GetSchedulePointerByDate(SCHEDULE* pHead, DATE date);
SCHEDULE* GetSchedulePointerByDateAndTime(SCHEDULE* pHead, DATE date);
SCHEDULE InputSchedule(SCHEDULE* pHead);

void DrawCalendar(SCHEDULE* pHead, DATE date);
void ShowAllScheduleByDay(SCHEDULE* pHead, DATE date);

//달력
int GetLastDayByMonthAndYear(int nYear, int nMonth);
void DrawCalendar(SCHEDULE* pHead, DATE date);


//메인
enum MENU
{
	PREV_MONTH = 'a',
	NEXT_MONTH = 'd',
	PREV_DAY = 'w',
	NEXT_DAY = 's',
	ADD_SCHEDULE = '1',
	DELETE_SCHEDULE = '2',
	CHANGE_SCHEDULE = '3',
	QUIT = 'q'
};

void ShowAllScheduleByDay(SCHEDULE* pHead, DATE date);
void DeleteSchedule(SCHEDULE* pHead);
void AddSchedule(SCHEDULE* pHead);
void Changeschedule(SCHEDULE* pHead);

char GetSelectedMenu(void);

int main(void)
{
	SCHEDULE* pHead;
	//DATE current;

	pHead = InitScheduleHead();
	LoadScheduleFromFile(pHead, "schedule.txt");

	current = GetToday();
	system("cls");

	while (TRUE)
	{
		char ch;

		DrawCalendar(pHead, current);
		ShowAllScheduleByDay(pHead, current);

		ch = GetSelectedMenu();

		if (ch == QUIT)
		{
			break;
		}

		switch (ch)
		{
		case PREV_MONTH:
			ModifyMonth(&current, -1);
			break;

		case NEXT_MONTH:
			ModifyMonth(&current, 1);
			break;

		case PREV_DAY:
			ModifyDay(&current, -1);
			break;

		case NEXT_DAY:
			ModifyDay(&current, 1);
			break;

		case ADD_SCHEDULE:
			AddSchedule(pHead);
			break;

		case DELETE_SCHEDULE:
			DeleteSchedule(pHead);
			break;

		case CHANGE_SCHEDULE:
			Changeschedule(pHead);
			break;
		}

		system("cls");
	}


	SaveScheduleToFile(pHead, "schedule.txt");
	KillAllScheduleNode(pHead);
}

char GetSelectedMenu(void)
{
	char ch;

	printf("          ┌------------------------┐\n");
	printf("          │          MENU          │\n");
	printf("          │  d : 다음 달로 넘기기  │\n");
	printf("          │  a : 이전 달로 넘기기  │\n");
	printf("          │  s : 다음 날로 넘기기  │\n");
	printf("          │  w : 이전 날로 넘기기  │\n");
	printf("          │  1 : 일정 추가         │\n");
	printf("          │  2 : 일정 삭제         │\n");
	printf("          │  3 : 일정 수정         │\n");
	printf("          │  4 : 일의 완성도       │\n");
	printf("          │  q : 프로그램 종료     │\n");
	printf("          └------------------------┘\n\n");
	printf("           메뉴를 입력하시오 >> ");
	ch = getchar();
	return ch;
}

void ShowAllScheduleByDay(SCHEDULE* pHead, DATE date)
{
	SCHEDULE* pFind = pHead->next;

	printf("\n");
	printf("              *  %d/%d/%d의 일정 *\n", date.m_nYear,
		date.m_nMonth,
		date.m_nDay);

	printf("-------------------------------------------------\n");

	while (pFind != NULL)
	{
		if (CompareDate(pFind->m_date, date) == 0)
		{
			printf("%d시 : %s\n",
				pFind->m_date.m_nHour,
				pFind->m_szText);
		}

		pFind = pFind->next;
	}

	printf("-------------------------------------------------\n\n");
}

void DeleteSchedule(SCHEDULE* pHead)
{
	DATE date;
	DATE current;
	current = GetToday();

	system("cls");
	DrawCalendar(pHead, current);
	ShowAllScheduleByDay(pHead, current);
	printf("   ┌-----------------------------------------------------┐\n");
	printf("   │                                                     │\n");
	printf("   │  (일정을 삭제합니다) 년, 월, 일, 시간 순으로 입력   │\n");
	printf("   │               (예시)2021 11 10 18                   │\n");
	printf("   │                                                     │\n");
	printf("   └-----------------------------------------------------┘\n\n");
	printf("	삭제 할 날짜 입력  >> ");

	scanf("%d %d %d %d",
		&date.m_nYear,
		&date.m_nMonth,
		&date.m_nDay,
		&date.m_nHour);

	DeleteScheduleByDateAndTime(pHead, date);
	system("cls");
}

void AddSchedule(SCHEDULE* pHead)
{
	SCHEDULE schedule;

	schedule = InputSchedule(pHead);

	AddScheduleNode(pHead, schedule);
}

void Changeschedule(SCHEDULE* pHead)
{
	SCHEDULE ret;
	DATE current;
	current = GetToday();

	system("cls");
	DrawCalendar(pHead, current);
	ShowAllScheduleByDay(pHead, current);

	printf("   ┌-----------------------------------------------------┐\n");
	printf("   │                                                     │\n");
	printf("   │            년, 월, 일, 시간 순으로 입력             │\n");
	printf("   │                 (예시)2017 10 31 18                 │\n");
	printf("   │                                                     │\n");
	printf("   └-----------------------------------------------------┘\n\n");
	printf("	날짜 입력     >> ");
	scanf("%d %d %d %d", &ret.m_date.m_nYear, &ret.m_date.m_nMonth, &ret.m_date.m_nDay, &ret.m_date.m_nHour);
	printf("   수정할 일정 입력   >>");

	ChangeScheduleByDateAndTime(pHead, ret);
	system("cls");

}

//날짜
int CompareDate(DATE target, DATE compare)
{
	int nTargetSum = 0;
	int nCompareSum = 0;

	if (target.m_nYear > compare.m_nYear)
	{
		return -1;
	}
	else if (target.m_nYear < compare.m_nYear)
	{
		return 1;
	}

	if (target.m_nMonth > compare.m_nMonth)
	{
		return -1;
	}
	else if (target.m_nMonth < compare.m_nMonth)
	{
		return 1;
	}

	if (target.m_nDay > compare.m_nDay)
	{
		return -1;
	}
	else if (target.m_nDay < compare.m_nDay)
	{
		return 1;
	}

	return 0;
}

int CompareDateAndTime(DATE target, DATE compare)
{
	if (target.m_nYear > compare.m_nYear)
	{
		return -1;
	}
	else if (target.m_nYear < compare.m_nYear)
	{
		return 1;
	}

	if (target.m_nMonth > compare.m_nMonth)
	{
		return -1;
	}
	else if (target.m_nMonth < compare.m_nMonth)
	{
		return 1;
	}

	if (target.m_nDay > compare.m_nDay)
	{
		return -1;
	}
	else if (target.m_nDay < compare.m_nDay)
	{
		return 1;
	}

	if (target.m_nHour > compare.m_nHour)
	{
		return -1;
	}
	else if (target.m_nHour < compare.m_nHour)
	{
		return 1;
	}

	return 0;
}

void PrintDate(DATE date)
{
	printf("%d년 %d월 %d일 %d시\n", date.m_nYear, date.m_nMonth, date.m_nDay, date.m_nHour);
}

DATE InputDate(void)
{
	DATE ret;

	printf("년 : ");
	scanf("%d", &ret.m_nYear);

	printf("월 : ");
	scanf("%d", &ret.m_nMonth);

	printf("일 : ");
	scanf("%d", &ret.m_nDay);

	printf("시 : ");
	scanf("%d", &ret.m_nHour);

	return ret;
}

DATE GetToday(void)
{
	DATE ret;
	time_t ltime;
	struct tm* today;

	time(&ltime);
	today = localtime(&ltime);

	ret.m_nYear = today->tm_year + 1900;
	ret.m_nMonth = today->tm_mon + 1;
	ret.m_nDay = today->tm_mday;

	ret.m_nHour = today->tm_hour;

	return ret;
}

void ModifyMonth(DATE* pDate, int nMonth)
{
	if (nMonth > 0)
	{
		if (pDate->m_nMonth + nMonth > 12)
		{
			pDate->m_nMonth = pDate->m_nMonth + nMonth - 12;
			pDate->m_nYear = pDate->m_nYear + 1;
			return;
		}
	}

	else if (nMonth < 0)
	{
		if (pDate->m_nMonth + nMonth < 1)
		{
			pDate->m_nMonth = 12 + (nMonth + 1);
			pDate->m_nYear = pDate->m_nYear - 1;
			return;
		}
	}

	pDate->m_nMonth = pDate->m_nMonth + nMonth;
}

void ModifyDay(DATE* pDate, int nDay)
{
	int nMaxDay = GetLastDayByMonthAndYear(pDate->m_nYear, pDate->m_nMonth);
	int nPrevMaxDay = GetLastDayByMonthAndYear(pDate->m_nYear, pDate->m_nMonth - 1);

	if (nDay > 0)
	{
		if (pDate->m_nDay + nDay > nMaxDay)
		{
			pDate->m_nDay = 1;
			ModifyMonth(pDate, 1);
			return;
		}
	}

	else if (nDay < 0)
	{
		if (pDate->m_nDay + nDay < 1)
		{
			pDate->m_nDay = nPrevMaxDay;
			ModifyMonth(pDate, -1);
			return;
		}
	}

	pDate->m_nDay = pDate->m_nDay + nDay;
}

//스케줄
SCHEDULE* InitScheduleHead(void)
{
	SCHEDULE* pRet;

	pRet = (SCHEDULE*)malloc(sizeof(SCHEDULE));
	pRet->next = NULL;

	return pRet;
}

SCHEDULE* GetSchedulePointerByDate(SCHEDULE* pHead, DATE date)
{
	SCHEDULE* pFind = pHead->next;

	while (pFind != NULL)
	{
		if (CompareDate(pFind->m_date, date) == 0)
		{
			return pFind;
		}

		pFind = pFind->next;
	}

	return NULL;
}

SCHEDULE* GetSchedulePointerByDateAndTime(SCHEDULE* pHead, DATE date)
{
	SCHEDULE* pFind = pHead->next;

	while (pFind != NULL)
	{
		if (CompareDateAndTime(pFind->m_date, date) == 0)
		{
			return pFind;
		}

		pFind = pFind->next;
	}

	return NULL;
}

void AddScheduleNode(SCHEDULE* pHead, SCHEDULE schedule)
{
	SCHEDULE* pPrev = pHead;
	SCHEDULE* pFind = pHead->next;
	SCHEDULE* pNew;

	pNew = (SCHEDULE*)malloc(sizeof(SCHEDULE));
	*pNew = schedule;

	while (pFind != NULL)
	{
		if (CompareDateAndTime(schedule.m_date, pFind->m_date) == -1)
		{
			pPrev = pPrev->next;
			pFind = pFind->next;
		}
		else
		{
			break;
		}
	}

	pPrev->next = pNew;
	pNew->next = pFind;
}

void DeleteScheduleByDateAndTime(SCHEDULE* pHead, DATE date)
{
	SCHEDULE* pPrev = pHead;
	SCHEDULE* pFind = pHead->next;
	SCHEDULE* pTemp;

	while (pFind != NULL)
	{
		if (CompareDateAndTime(pFind->m_date, date) == 0)
		{
			pTemp = pFind;
			pPrev->next = pFind->next;
			free(pTemp);
			break;
		}

		pFind = pFind->next;
		pPrev = pPrev->next;
	}
}

void ChangeScheduleByDateAndTime(SCHEDULE* pHead, SCHEDULE schedule)
{
	SCHEDULE* pFind = pHead->next;
	SCHEDULE* pTemp;

	while (pFind != NULL)
	{
		if (CompareDateAndTime(pFind->m_date, schedule.m_date) == 0)
		{
			pTemp = pFind;
			scanf("%s", pTemp->m_szText);
			break;
		}
	}

}

void KillAllScheduleNode(SCHEDULE* pHead)
{
	SCHEDULE* pFind = pHead->next;
	SCHEDULE* pPrev = pHead;

	while (pFind != NULL)
	{
		pFind = pPrev->next;

		free(pPrev);

		pPrev = pFind;
		pFind = pFind->next;
	}
}

void PrintAllSchedule(SCHEDULE* pHead)
{
	SCHEDULE* pFind = pHead->next;

	while (pFind != NULL)
	{
		printf("%s\n", pFind->m_szText);
		PrintDate(pFind->m_date);

		pFind = pFind->next;
	}
}

void LoadScheduleFromFile(SCHEDULE* pHead, char szFileName[])
{
	SCHEDULE news;
	FILE* fp;
	int nCnt = 0;
	char szBuf[STRING_MAX];

	fp = fopen(szFileName, "r");

	if (fp == NULL)
	{
		fp = fopen(szFileName, "w");
		fclose(fp);

		return;
	}

	while (!feof(fp))
	{
		fscanf(fp, "%s", szBuf);

		switch (nCnt)
		{
		case 0:
			news.m_date.m_nYear = atoi(szBuf);
			break;

		case 1:
			news.m_date.m_nMonth = atoi(szBuf);
			break;

		case 2:
			news.m_date.m_nDay = atoi(szBuf);
			break;

		case 3:
			news.m_date.m_nHour = atoi(szBuf);
			break;

		case 4:
			strcpy(news.m_szText, szBuf);
			break;
		}

		nCnt++;

		if (nCnt == 5)
		{
			AddScheduleNode(pHead, news);
			nCnt = 0;
		}
	}

	fclose(fp);
}

void SaveScheduleToFile(SCHEDULE* pHead, char szFileName[])
{
	FILE* fp;
	SCHEDULE* pFind = pHead->next;

	fp = fopen(szFileName, "w");

	while (pFind != NULL)
	{
		fprintf(fp, "%d %d %d %d %s\n",
			pFind->m_date.m_nYear,
			pFind->m_date.m_nMonth,
			pFind->m_date.m_nDay,
			pFind->m_date.m_nHour,
			pFind->m_szText
		);

		pFind = pFind->next;
	}

	fclose(fp);
}

SCHEDULE InputSchedule(SCHEDULE* pHead)
{
	SCHEDULE ret;
	//DATE current;
	current = GetToday();

	system("cls");
	DrawCalendar(pHead, current);
	ShowAllScheduleByDay(pHead, current);

	while (TRUE)
	{
		printf("   ┌-----------------------------------------------------------┐\n");
		printf("   │                                                           │\n");
		printf("   │  (일정을 추가합니다) 년, 월, 일, 시간, 일정 순으로 입력   │\n");
		printf("   │                 (예시)2021 11 10 18 축제                  │\n");
		printf("   │                                                           │\n");
		printf("   └-----------------------------------------------------------┘\n\n");
		printf("	날짜와 일정 입력  >> ");

		scanf("%d %d %d %d %s",
			&ret.m_date.m_nYear,
			&ret.m_date.m_nMonth,
			&ret.m_date.m_nDay,
			&ret.m_date.m_nHour,
			ret.m_szText);

		if (GetSchedulePointerByDateAndTime(pHead, ret.m_date) == NULL)
		{
			break;
		}

		printf("   ┌--------------------------------------┐\n");
		printf("   │                                      │\n");
		printf("   │  이미 같은 시간에 저장되어 있습니다  │\n");
		printf("   │                                      │\n");
		printf("   └--------------------------------------┘\n\n");

	}
	system("cls");
	return ret;
}

//달력
int GetLastDayByMonthAndYear(int nYear, int nMonth)
{
	int nLastDay;

	if (nMonth == 1 ||
		nMonth == 3 ||
		nMonth == 5 ||
		nMonth == 7 ||
		nMonth == 8 ||
		nMonth == 10 ||
		nMonth == 12)
	{
		nLastDay = 31;
	}
	else if (nMonth == 4 ||
		nMonth == 6 ||
		nMonth == 9 ||
		nMonth == 11)
	{
		nLastDay = 30;
	}

	if (nMonth == 2)
	{
		nLastDay = 28;
	}

	if (nMonth == 2 &&
		(nYear % 4) == 0 &&
		(nYear % 100 != 0) || (nYear % 400 == 0))
	{
		nLastDay = 29;
	}

	return nLastDay;
}

void DrawCalendar(SCHEDULE* pHead, DATE date)
{
	char szBuf[STRING_MAX];

	int nYear;
	int nMonth;
	int nLastDay;
	int nTh;
	int nDay;
	int nDate;
	int nX;
	int nZ;
	int count = 0;

	nYear = date.m_nYear;
	nMonth = date.m_nMonth;
	nLastDay = GetLastDayByMonthAndYear(nYear, nMonth);

	/*
	if ( nMonth < 3 )
	{
		nYear = nYear - 1;
		nMonth = nMonth + 12;
	}
	*/
	if (nMonth > 9)
		printf("┌----------------- %d년  %d월 ----------------┐\n", nYear, nMonth);
	else
		printf("┌----------------- %d년  %d월 ----------------┐\n", nYear, nMonth);
	printf("│%6s%6s%6s%6s%6s%6s%6s     │\n", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat");

	nDate = 1, nX = 0, nZ = 0;

	while (nX <= 6)
	{
		nX = nX + 1;
		nTh = nYear / 100;
		nYear = nYear % 100;
		nDay = ((21 * nTh / 4) +
			(5 * nYear / 4) + (26 * (nMonth + 1) / 10) + nDate - 1) % 7;

		while (nZ < 7)
		{
			if (nX = 1 && nZ < nDay)
			{
				printf("%6s", " ");
			}
			else
			{
				DATE temp;
				temp = date;

				temp.m_nDay = nDate;

				if (CompareDate(date, temp) == 0)
				{
					sprintf(szBuf, "[%d]", nDate);
					printf("%6s", szBuf);
				}
				else if (GetSchedulePointerByDate(pHead, temp)
					!= NULL)
				{
					sprintf(szBuf, "*%d", nDate);
					printf("%6s", szBuf);
				}
				else
				{
					count++;
					printf("%6d", nDate);
				}

				if (nDate < nLastDay)
				{
					nDate = nDate + 1;
				}
				else if (nDate >= nLastDay)
				{
					break;
				}
			}

			nZ = nZ + 1;
		}
		if (nZ == 0)
			printf("\t\t\t\t\t\t│\n");
		else if (nZ == 1)
			printf("\t\t\t\t\t│\n");
		else if (nZ == 2)
			printf("\t\t\t\t│\n");
		else if (nZ == 3)
			printf("\t\t\t│\n");
		else if (nZ == 4)
			printf("\t\t│\n");
		else if (nZ == 5)
			printf("\t\t│\n");
		else
			printf("\t│\n");

		if (nDate >= nLastDay)
		{
			break;
		}

		nZ = 0;
	}
	printf("│                                               │\n");
	printf("└-----------------------------------------------┘\n\n");

}