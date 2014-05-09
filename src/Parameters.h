#ifndef ParametersH
#define ParametersH

//#define DEBUG_MODE


#define DAY  11
#define MONTH   6
#define YEAR  8


AnsiString ver = "2.5.6";

#define INF 99999999999

#define initialNavigationArray 100000

//---------------------Speed
#define SPEED_STEP 200
#define MAX_SPEED 1000
#define MIN_SPEED 40000
#define DEFAULT_K 50
#define initialDelay 10000
#define NO_FLICK_START_SPEED 40000
#define NO_FLICK_SPEED_STEP 100


//-------------------Color
#define COLOR_FREE_LINES 0xffff0000
#define COLOR_FORWARD clRed
#define COLOR_BACKWARD clBlue
#define COLOR_MODEL_MAP clBlack
#define COLOR_GAS_OFF clGreen

struct Parameters{
        double dx;//50.531261803179915504970379089624;
        double dy;//50.531261803179915504970379089624;

        char forwardCommandX;
        char forwardCommandY;

};

#endif