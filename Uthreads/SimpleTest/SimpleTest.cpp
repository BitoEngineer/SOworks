// SimpleTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "List.h"

/////////////////////////////////////////////
//
// CCISEL 
// 2007-2011
//
// UThread    Library First    Test
//
// Jorge Martins, 2011
////////////////////////////////////////////
#define DEBUG

#define MAX_THREADS 10

#include <crtdbg.h>
#include <stdio.h>



///////////////////////////////////////////////////////////////
//															 //
// Test Part I  //
//															 //
///////////////////////////////////////////////////////////////

ULONG Test1_Count;

static
EVENT ev;

static
HANDLE h1, h2;

INT RUNNING = 0, READY = 1, BLOCKED = 2;

VOID TestState1_Thread(UT_ARGUMENT Argument) {
	_ASSERT(UtThreadState(h1) == RUNNING);
	printf("\n ********** I AM THREAD 1 **************\n");
	printf("\n BEFORE EventWait \n");
	printf("\n Thread 1 State: %d --> RUNNING", UtThreadState(h1));
	printf("\n Thread 2 State: %d --> READY \n", UtThreadState(h2));

	EventWait(&ev);

	UCHAR Char;
	ULONG Index;
	Char = (UCHAR) Argument;
	//Estado da thread depois do UtRun(), ou seja, Running
	printf("\n After Thread 2 \n");
	printf("\n Thread 1 State: %d  -->RUNNING  \n", UtThreadState(h1));
	++Char;
}

VOID TestState2_Thread(UT_ARGUMENT Argument) { 
	_ASSERT(UtThreadState(h1) == BLOCKED);
	_ASSERT(UtThreadState(h2) == RUNNING);
	printf("\n ********** I AM THREAD 2 **************\n");
	printf("\n AFTER EventWait \n");
	printf("\n Thread 1 State: %d --> BLOCKED = 2", UtThreadState(h1));
	printf("\n Thread 2 State: %d --> RUNNING = 0 \n", UtThreadState(h2));
	EventSet(&ev);
	printf("\n AFTER EventSet \n");
	printf("\n Thread 1 State: %d --> READY = 1", UtThreadState(h1));
	printf("\n Thread 2 State: %d --> RUNNING = 0 \n", UtThreadState(h2));


	
}

VOID TestAlive_Thread(UT_ARGUMENT Argument) {}

VOID ThreadToSwitch2_Thread(UT_ARGUMENT Argument) {
	_ASSERT(UtThreadState(h2) == RUNNING);
	//_ASSERT(UtThreadState(h1) == READY);
	printf("\n ********** I AM THREAD 2 **************\n");
	printf("\n After calling UtSwitchTo \n");
	printf("\n Running thread state (Thread 2): %d  -->RUNNING \n", UtThreadState(h2));
}

VOID ThreadToSwitch1_Thread(UT_ARGUMENT Argument) {
	printf("\n ********** I AM THREAD 1 **************\n");
	h2 = UtCreate(ThreadToSwitch2_Thread, (UT_ARGUMENT)('2'));
	_ASSERT(UtThreadState(h1) == RUNNING);
	printf("\n Running thread state (Thread 1): %d  -->RUNNING Thread To Switch State (Thread 2): %d -->READY \n", UtThreadState(h1), UtThreadState(h2));
	UtSwitchTo(h2);
	printf("\n After Switch (Thread 1 state): %d \n", UtThreadState(h1));
}



VOID TestState ()  {

	EventInit(&ev,FALSE);
	printf("\n BEGIN Test Part I 1. a) \n\n");
	//HANDLE que vai permitir invocar a função para saber o estado da thread correspondente ao mesmo
	h1=UtCreate(TestState1_Thread, (UT_ARGUMENT) ('1'));
	h2=UtCreate(TestState2_Thread, (UT_ARGUMENT) ('2'));
	//Estado da thread antes do UtRun(), ou seja, Ready
	printf("\n Before UtRun() : Thread 1 State=%d  Thread 2 State=%d  -->READY \n", UtThreadState(h1), UtThreadState(h2));
	_ASSERT (UtThreadState(h1)==READY);
	UtRun();
	
	printf("\n\n END Test Part I 1. a) \n");
}


VOID TestAlive() {

	printf("\n BEGIN Test Part I 1. b) \n\n");
	//HANDLE que vai permitir invocar a função para saber o estado de vida da thread correspondente ao mesmo
	h1 = UtCreate(TestAlive_Thread, (UT_ARGUMENT)('1'));
	//Estado de vida da thread antes do UtRun(), ou seja, TRUE
	printf("\n Is thread alive before UtRun() and after UtCreate()? %d \n", UtAlive(h1));
	
	UtRun();
	
	//Estado de vida da thread depois do UtRun(), ou seja, FALSE
	printf("\n Is thread alive after UtRun()? %d \n", UtAlive(h1));
	printf("\n\n END Test Part I 1. b) \n");
}

VOID TestSwitch() {

	printf("\n BEGIN Test Part I 1. c) \n\n");

	h1 = UtCreate(ThreadToSwitch1_Thread, (UT_ARGUMENT)('1'));

	UtRun();

	printf("\n\n END Test Part I 1. c) \n");
}


int main () {
	//TEST 1. a)
	UtInit();
	TestState();		 
	UtEnd();

	///TEST 1. b)
	UtInit();
	TestAlive();
	UtEnd();

	//TEST 1. c)
	UtInit();
	TestSwitch();
	UtEnd();

	return 0;
}


