#include <stdio.h>
#include <stdlib.h>
#define ESTADO_ERROR 0
#define ESTADO_ABIERTO 1
#define ESTADO_CERRADO 2
#define ESTADO_ABRIENDO 3
#define ESTADO_CERRANDO 4
#define ESTADO_INTERMEDIO 5
#define ESTADO_INIT 6
#define FALSE 0
#define TRUE 1
#define inTrue 0
#define inFalse 1
#define TIME_Ca 60

int Func_ESTADO_ERROR(void);
int Func_ESTADO_ABIERTO(void);
int Func_ESTADO_CERRADO(void);
int Func_ESTADO_ABRIENDO(void);
int Func_ESTADO_CERRANDO(void);
int Func_ESTADO_INTERMEDIO(void);
int Func_ESTADO_INTI(void);
volatile int ESTADO_ANTERIOR = ESTADO_INIT;
volatile int ESTADO_ACTUAL = ESTADO_INIT;
volatile int ESTADO_SIGUIENTE = ESTADO_INIT;
volatile int CntTimeCa = 0; // contador de un segundo

volatile struct INOUT
{
    unsigned int Sc : 1;
    unsigned int Sa : 1;
    unsigned int Mc : 1;
    unsigned int Ma : 1;
    unsigned int Bc : 1;
    unsigned int Ba : 1;
    unsigned int Led : 1;
} inout;

int main()
{

    for (;;)
    {
        if (ESTADO_SIGUIENTE == ESTADO_INIT)
        {
            ESTADO_SIGUIENTE = Func_ESTADO_INTI();
        }
        if (ESTADO_SIGUIENTE == ESTADO_ABIERTO)
        {
            ESTADO_SIGUIENTE = Func_ESTADO_ABIERTO();
        }
        if (ESTADO_SIGUIENTE == ESTADO_CERRADO)
        {
            ESTADO_SIGUIENTE = Func_ESTADO_CERRADO();
        }
        if (ESTADO_SIGUIENTE == ESTADO_ABRIENDO)
        {
            ESTADO_SIGUIENTE = Func_ESTADO_ABRIENDO();
        }
        if (ESTADO_SIGUIENTE == ESTADO_CERRANDO)
        {
            ESTADO_SIGUIENTE = Func_ESTADO_CERRANDO();
        }
        if (ESTADO_SIGUIENTE == ESTADO_INTERMEDIO)
        {
            ESTADO_SIGUIENTE = Func_ESTADO_INTERMEDIO();
        }
        if (ESTADO_SIGUIENTE == ESTADO_ERROR)
        {
            ESTADO_SIGUIENTE = Func_ESTADO_ERROR();
        }
    }
}

int Func_ESTADO_ERROR(void)
{
    static int led_status1 = 0;
    static int led_status2 = 0;
    for (;;)
    {
        led_status1 = inout.Led;
        inout.Led = led_status2;
        led_status2 = led_status1;
        HAL_Delay(199);
    }
}

int Func_ESTADO_ABIERTO(void)
{
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_ABIERTO;
    inout.Ma = FALSE;
    inout.Mc = FALSE;
    inout.Led = FALSE;
    CntTimeCa = 0; // reset del contador

    for (;;)
    {
        if (CntTimeCa > TIME_Ca)
        {
            return ESTADO_CERRANDO;
        }
        if (inout.Bc == TRUE)
        {
            return ESTADO_CERRANDO;
        }
    }
}
int Func_ESTADO_CERRADO(void)
{
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_CERRADO;
    inout.Mc = 0;
    inout.Ma = 0;
    inout.Led = 1;

    for (;;)
    {
        if (inout.Ba == TRUE)
        {
            return ESTADO_ABRIENDO;
        }
    }
}
int Func_ESTADO_ABRIENDO(void)
{
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_ABRIENDO;
    inout.Led = FALSE;
    inout.Mc = FALSE;
    inout.Ma = TRUE;

    for (;;)
    {
        if (inout.Sa == TRUE)
        {
            return ESTADO_ABIERTO;
        }
        else if (inout.Bc == TRUE)
        {
            return ESTADO_CERRANDO;
        }
    }
}
int Func_ESTADO_CERRANDO(void)
{
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_CERRANDO;
    inout.Mc = TRUE;
    inout.Ma = FALSE;
    inout.Led = FALSE;

    for (;;)
    {
        if (inout.Sc == TRUE)
        {
            return ESTADO_CERRADO;
        }
        else if (inout.Ba == TRUE)
        {
            return ESTADO_ABRIENDO;
        }
    }
}
int Func_ESTADO_INTERMEDIO(void)
{
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_INTERMEDIO;
    inout.Mc = FALSE;
    inout.Ma = 0;
    inout.Led = TRUE;
    for (;;)
    {
        if ((inout.Ba == TRUE))
        {
            return ESTADO_ABRIENDO;
        }

        if ((inout.Bc == TRUE))
        {
            return ESTADO_CERRANDO;
        }
    }
}
int Func_ESTADO_INTI(void)
{
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_INIT;

    inout.Led = 1;
    inout.Ma = FALSE;
    inout.Mc = FALSE;

    if ((inout.Sa == inTrue) && (inout.Sc == inTrue))
    {
        return ESTADO_ERROR; // Error
    }
    if ((inout.Sa == 1) && (inout.Sc == 0))
    {
        return ESTADO_ABIERTO;
    }
    if ((inout.Sc == 1) && (inout.Sa == 0))
    {
        return ESTADO_CERRADO;
    }
    if ((inout.Sa == 0) && (inout.Sc == 0))
    {
        return ESTADO_INTERMEDIO;
    }
}

void intGeneral100ms(void)
{

    static int cont_int = 0;

    cont_int++;

    if (cont_int == 9)
    {
        CntTimeCa++;
        cont_int = 0;
    }
    if (HAL_GPIO_READ(PUERTO, PIN))
    {
    }
}
