
/*
 * basic description:  
 *
 *     macros and calibration code for building auto-graders
 *     checking for both expected behavior and meeting
 *     runtime requirements.
 *
 *     Also declares and initializes various globals used
 *     for auto-scoring.
 *
 */

#ifndef _TEST_H
#define _TEST_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>

#define TT_COLOR_CODE 0x1B
#define TT_COLOR_RED "[0;91m"
#define TT_COLOR_GREEN "[1;32m"
#define TT_COLOR_YELLOW "[1;33m"
#define TT_COLOR_BOLD_BLUE "[0;94m"
#define TT_COLOR_BOLD_CYAN "[1;96m"
#define TT_COLOR_CYAN "[0;96m"
#define TT_COLOR_PURPLE "[0;95m"
#define TT_COLOR_RESET "[0m"

int ___N = 2000;
int TIME_TRIALS = 100;

using namespace std::chrono;
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::high_resolution_clock::time_point ClockSample;


using std::cout;
using std::endl;
using std::string;

bool Verbose = false;

#ifdef VERBOSE
Verbose = true;
#endif

int TestNum = 1;
int NPassed = 0;
int NFailed = 0;

float Score = 0;
float MaxPts = 0;
float ThisScore = 0;
float ThisMaxPts = 0;

float MaxPtsAuto= 0;
float MaxPtsManual= 0;

float PtsPer = 1.0;

char *CurrFunc;
// const char *CurrDesc;
std::string CurrDesc;



void _red_txt(){
  printf("~~%c%s", TT_COLOR_CODE, TT_COLOR_RED);
}
void _green_txt(){
  printf("~~%c%s", TT_COLOR_CODE, TT_COLOR_GREEN);
}
void _yellow_txt(){
  printf("~~%c%s", TT_COLOR_CODE, TT_COLOR_YELLOW);
}
void _BLUE_txt(){
  printf("~~%c%s", TT_COLOR_CODE, TT_COLOR_BOLD_BLUE);
}
void _CYAN_txt(){
  printf("~~%c%s", TT_COLOR_CODE, TT_COLOR_BOLD_CYAN);
}
void _cyan_txt(){
  printf("~~%c%s", TT_COLOR_CODE, TT_COLOR_CYAN);
}
void _purple_txt(){
  printf("~~%c%s", TT_COLOR_CODE, TT_COLOR_PURPLE);
}
void _normal_txt(){
  printf("~~%c%s", TT_COLOR_CODE, TT_COLOR_RESET);
}


// #define TIME_TRIALS 100000


void set_ntrials(int ntrials) {
  TIME_TRIALS = ntrials;
}


void ERRMSG(string msg) {
  cout << "~ " << msg << endl;
}
void ERRMSG(string msg, int a) {
  cout << "~ " << msg << " " << a << endl;
}
void ERRMSG(string msg, int a, int b) {
  cout << "~ " << msg << " " << a << " " << b << endl;
}
void ERRMSG(string msg, int a, int b, int c) {
  cout << "~ " << msg << " " << a << " " << b <<  " " << c << endl;
}


static unsigned long __last_rand;
static unsigned long __a = 1664525;
static unsigned long __m = 4294967296;
static unsigned long __c = 1013904223;

void _srand(long x) {
  __last_rand = x;
}

unsigned long _rand(){

  __last_rand = (__a*__last_rand + __c) % __m;

  return __last_rand;
}



#define TA_MESSAGE(msg) { \
	printf("~  #### TA-TODO : #####\n"); \
	printf("~  ####    %s\n", msg); \
	printf("~  #### END TA-TODO ###\n"); } \
	

#define TEST_RET(func_call, msg, answer, pts) { \
	int ret; \
  _purple_txt(); \
	printf("~  ###  BEGIN TEST     %i ########### \n", TestNum); \
  _cyan_txt(); \
	printf("~       POINTS       : %.3f points\n",  pts); \
	printf("~       FUNCTION CALL: %s\n",  "\""#func_call"\"\n"); \
	printf("~       DESCRIPTION  : %s\n", msg); \
	ret = func_call;  \
	printf("~       RESULTS: \n"); \
	printf("~         Returned Value       : %i\n", ret); \
	printf("~         Expected Return Value: %i\n", answer); \
	if(ret == answer) { \
    _green_txt(); \
    NPassed++; ThisScore+=pts; Score += pts; \
    printf("~             PASSED!!\n"); \
    _normal_txt(); \
  } \
	else { \
    NFailed++; \
    _red_txt(); \
    printf("~     FAILED\n"); \
    _normal_txt();  \
  } \
	MaxPts += pts; \
	MaxPtsAuto += pts; \
  _cyan_txt(); \
	printf("~\n       CUMMULATIVE SCORE: %.3f / %.3f\n",  ThisScore, \
      MaxPts); \
  _purple_txt(); \
	printf("~  ###  END TEST %i #######\n~\n", TestNum); \
  _normal_txt();\
	TestNum++; \
}

#define TEST_RET_MESSAGE(func_call, msg, answer, pts) { \
	int ret; \
	printf("~  ###  TEST %i:   %.3f points\n", TestNum,  pts); \
	printf("~          DESC: %s\n", msg); \
	ret = func_call;  \
	printf("~     Returned Value      : %i\n", ret); \
	printf("~     Correct Return Value: %i\n", answer); \
	if(ret == answer) {NPassed++; ThisScore+=pts; Score += pts; printf("~     PASSED\n");} \
	else { NFailed++; printf("~     FAILED\n");} \
	printf("~  ###  END TEST %i\n~\n", TestNum); \
	MaxPts += pts; \
	MaxPtsAuto += pts; \
	printf("~     Score So Far For This Test Suite: %.3f / %.3f\n", ThisScore, MaxPts); \
	TestNum++; \
}

// allows us to specify different expected
//    return values for each function call
#define TIME_RATIO2(func_callA, func_callB, desc, answerA, answerB, max_ratio, pts) { \
	int i, retA, retB; \
	ClockSample start, end; \
        long diffA, diffB; \
        double  ratio; \
  _purple_txt(); \
	printf("~  ###  BEGIN TEST %i #######\n", TestNum); \
  _cyan_txt(); \
	printf("~       POINTS:    %.3f\n",  pts); \
	printf("~       FUNCTION-CALL-A:    %s\n",  "\""#func_callA"\""); \
	printf("~       FUNCTION-CALL-B:    %s\n",  "\""#func_callB"\""); \
  printf("~       GENERAL DESCRIPTION: this is a timed test.\n"); \
  printf("~            FUNCTION-CALL-A and FUNCTION-CALL-B are both called\n"); \
  printf("~            NUM_TRIALS times\n"); \
  printf("~            THE RATIO (TIME-FOR-CALL-B)/(TIME-FOR-CALL-A)\n"); \
  printf("~            IS TESTED VS MAX-ALLOWED\n"); \
  printf("~            COMMON USAGE: CALL-B is on a problem instance\n"); \
  printf("~              twice of that used in CALL-A\n"); \
  printf("~            NOTE: must pass both correctness & runtime\n"); \
  printf("~                  (zero points for getting the wrong answer fast!)\n"); \
  printf("~       SPECIFIC DESCRIPTION: %s\n~\n", desc); \
  _normal_txt(); \
	MaxPts += pts; \
	MaxPtsAuto += pts; \
	ThisMaxPts += pts; \
	start  = Clock::now(); \
	for(i=0; i<TIME_TRIALS; i++) { \
	  retA = func_callA; \
	  if(retA != answerA) break; \
	} \
	end = Clock::now(); \
	diffA = (end-start).count(); \
  printf("~    RESULTS OF THIS TEST:\n"); \
	if(retA != answerA){ \
      _red_txt(); \
	    printf("~     FAILED (correctness); \n"); \
	    printf("~     Returned Value      : %i\n", retA); \
	    printf("~     Correct Return Value: %i\n", answerA); \
	    NFailed++; \
      _normal_txt(); \
	} \
  else { \
    start = Clock::now(); \
    for(i=0; i<TIME_TRIALS; i++) { \
      retB = func_callB; \
        if(retB != answerB) break; \
    } \
    end = Clock::now(); \
    if(retB != answerB){ \
      _red_txt(); \
          printf("~     FAILED (correctness); \n"); \
          printf("~     Returned Value      : %i\n", retB); \
          printf("~     Correct Return Value: %i\n", answerB); \
          NFailed++; \
      _normal_txt(); \
    } \
    else { \
      diffB =  (end-start).count(); \
      ratio = ((double)diffB)/diffA; \
      if(ratio > max_ratio) { \
          _red_txt(); \
          printf("~   FAILED \n"); \
          printf("~   TOO SLOW!\n"); \
          printf("~   MAX RATIO T(2N)/T(N): %.2f ;", max_ratio); \
          printf("~ OBSERVED RATIO T(2N)/T(N): %.2f\n", ratio); \
          printf("~     (NUM-TRIALS:       %i)\n", TIME_TRIALS); \
          NFailed++; \
      } \
      else { \
          _green_txt(); \
          printf("~   PASSED!!:  correctness & runtime criteria satisfied\n"); \
          printf("~   MAX RATIO T(2N)/T(N): %.2f ;", max_ratio); \
          printf("~ OBSERVED RATIO T(2N)/T(N): %.2f\n", ratio); \
	        Score += pts; \
	        NPassed++; \
	        ThisScore += pts; \
          _normal_txt(); \
       } \
     } \
   } \
  _cyan_txt(); \
	printf("~\n     CUMMULATIVE SCORE: %.3f / %.3f\n", ThisScore, MaxPts); \
  _purple_txt(); \
	printf("~  ###  END TEST %i #######\n~\n", TestNum); \
  _normal_txt(); \
	TestNum++;\
}


/**
 * TIME_RATIO MACRO
 *
 * When both we expect both function calls to return the
 * same value.
 */
#define TIME_RATIO(func_callA, func_callB, desc, answer, max_ratio, pts) { \
  TIME_RATIO2(func_callA, func_callB, desc, answer, answer, max_ratio, pts); \
}



	
	
#define MANUAL(func_call, expected, pts) { \
  _purple_txt(); \
	printf("~  ###  TEST %i:  %s; %.3f points\n", TestNum, "\""#func_call"\"",  pts); \
	printf("~  MANUAL TA TODO:  DETERMINE IF OUTPUT MATCHES EXPECTED AND ASSIGN SCORE\n~\n");\
  _cyan_txt(); \
	printf("~    EXPECTED OUTPUT: \n~\n"); \
	printf("~        %s \n~\n", expected); \
	printf("~    ACTUAL OUTPUT: \n~\n"); \
	func_call; \
	printf("~    \n END STUDENT OUTPUT: \n~\n"); \
	printf("~     TA REPORTED SCORE: ____ / %.3f\n", pts); \
  _purple_txt(); \
	printf("~  ###  END TEST %i\n~\n", TestNum); \
  _normal_txt(); \
	TestNum++;\
	MaxPts += pts; \
	MaxPtsManual += pts; \
}


	
	


#define START(desc) { \
  _BLUE_txt(); \
      printf("~\n~##################################################\n"); \
      printf("~~  BEGIN TEST SUITE.  \n"); \
      printf("~~  DESCRIPTION: "); \
      std::cout << "~ " << desc << std::endl; \
      ThisScore = 0.0; \
      TestNum = 1; \
      CurrDesc = desc; \
      printf("~~   FYI... Numtrials for runtime tests: %i\n~\n", TIME_TRIALS);  \
      printf("~~##################################################\n~\n"); \
  _normal_txt(); \
}
	

	

#define END { \
  _BLUE_txt(); \
      printf("~\n~##################################################\n"); \
      printf("~~  END TEST SUITE.  \n"); \
      printf("~~  DESCRIPTION: "); \
      std::cout << "~ " << CurrDesc << std::endl; \
      printf("~~  RESULTS:\n"); \
    printf("~~       AUTO__SCORED:      %.3f / %.3f\n",  ThisScore, MaxPts); \
    ThisScore = 0.0; ThisMaxPts=0.0; \
      printf("~\n~##################################################\n"); \
    _normal_txt();  }

#define END_MANUAL { \
      printf("~~  END TEST SUITE.  \n"); \
      printf("~~  DESCRIPTION: "); \
    std::cout << "'" << CurrDesc << "' ~~~~~~~" << std::endl; \
    printf("~~~~~   TA TODO:  ENTER TOTAL POINTS FOR MANUAL TESTS ABOVE\n~\n"); \
    printf("~~~~~   MANUAL__SCORE:   ______ / %.3f\n",  MaxPts); \
    ThisScore = 0.0; ThisMaxPts=0.0; \
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n~\n"); }

void report() {
	printf("~\n~\n~ #####   TESTS_COMPLETED ######  \n");
	printf("~\n~\n~    FINAL RESULTS OF TESTS:  \n");

	printf("~\n~\n~        AUTOSCORED:    %.3f / %.3f\n", Score, MaxPtsAuto);

	printf("~\n~\n~          AUTO TESTS PASSED:  %i\n", NPassed);
	printf("~~          AUTO TESTS FAILED:  %i\n", NFailed);

	if(MaxPtsManual > 0.0){
		printf("~\n~\n~        POINTS NEEDING HUMAN EVAL:   %.3f \n~\n", MaxPtsManual);
		printf("~\n~\n~        TA INSTRUCTIONS:  TALLY HUMAN-SCORED TOTAL BELOW\n~\n");

		printf("~\n~\n~            HUMAN-EVAL:    ______ / %.3f\n~\n", MaxPtsManual);

		printf("~\n~\n            GRAND-TOTAL:    ______ / %.3f\n~\n", MaxPtsManual+MaxPts);

		printf("~\n~\n            TA DOING EVALUATION:  ____________________________\n~\n");
	}
}

int baseline(){
  return 1;
}


  

void parray(int a[], int n) {
int i;

  printf("~~[ ");
  for(i=0; i<n; i++) {
    printf("~ %i ", a[i]);
  }
  printf("~]\n~");
}


  

  
  
int arrays_eq(int *a, int *b, int n) {
int i;

  for(i=0; i<n; i++){ 
	if(a[i] != b[i])
	  return 0;
  }
  return 1;
}


int num_occ(int a[], int n, int x) {
  int matches=0;
  int i;

  for(i=0; i<n; i++) {
    if(a[i]==x) matches++;
  }
  return matches;
}

int same_contents_arr(int a[], int b[], int n) {
int i, x;

  for(i=0; i<n; i++) {
	x = a[i];
	if(num_occ(a, n, x) != num_occ(b, n, x))
		return 0;
  }
  return 1;
}






clock_t Fudge = 1;


void cal_unit_fudge() {
  Fudge = 1;
}
void cal_big_fudge() {
  Fudge = 12;
}
void cal_huge_fudge() {
  Fudge = 24;
}
void cal_custom_fudge(clock_t fudge) {
  Fudge = fudge;
}







  

#endif

