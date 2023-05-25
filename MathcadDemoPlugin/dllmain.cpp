// dllmain.cpp : Defines the entry point for the DLL application.
#include "MCADINCL.H"
#pragma comment (lib, "mcaduser.lib")
//#include "pch.h"

#define  INTERRUPTED            1
#define  INSUFFICIENT_MEMORY    2
#define  MUST_BE_REAL           3
#define  VALUE_TOO_COOL         4
#define  NUMBER_OF_ERRORS       4

// table of error messages
// if your function never returns an
// error -- you do not need to create this
// table
char* myErrorMessageTable[NUMBER_OF_ERRORS] =
{   (char*)"(interrupted)",
    (char*)"insufficient memory",
    (char*)"must be real",
    (char*)"value too cool"
};

LRESULT MyFunction(MCSTRING* dataRes, const COMPLEXSCALAR parameter)
{
    // The string needs enough allocation space to store a terminating NULL,
    // so "Hello" requires 6 bytes
    // dataRes->str=MathcadAllocate(5);
    dataRes->str = MathcadAllocate(6);
    dataRes->str = strcpy(dataRes->str, "Hello");
    return 0;
}

// fill out a FUNCTIONINFO structure with
// the information needed for registering the
// function with Mathcad
FUNCTIONINFO myfunction =
{
    // Name by which mathcad will recognize the function
    (char*)"myfunction",

    // description of "multiply" parameters to be used
    // by the Insert Function dialog box
    (char*)"x",

    // description of the function for the Insert Function dialog box       
    (char*)"returns the product of real scalar a and real array M",

    // pointer to the executible code
    // i.e. code that should be executed
    // when a user types in "multiply(a,M)="
    (LPCFUNCTION)MyFunction,

    // multiply(a,M) returns a complex array
    STRING,

    // multiply(a,M) takes on two arguments
    1,

    // the first is a complex scalar, the second a complex array 
    { STRING }
};


LRESULT Allocate(COMPLEXSCALAR* dataRes, LPCCOMPLEXSCALAR size) {
    char* ptr = MathcadAllocate(size->real);

    if (!ptr)
    {
        return INSUFFICIENT_MEMORY;
    }

    dataRes->real = reinterpret_cast<uintptr_t>(ptr);

    return 0;
}

LRESULT Release(COMPLEXSCALAR* dataRes, LPCOMPLEXSCALAR location) {
    char* ptr = reinterpret_cast<char*>(location->real, sizeof(location->real));

    MathcadFree(ptr);

    dataRes->real = 0;

    return 0;
}

FUNCTIONINFO allocate =
{
    // Name by which mathcad will recognize the function
    (char*)"allocate",

    // description of "multiply" parameters to be used
    // by the Insert Function dialog box
    (char*)"s",

    // description of the function for the Insert Function dialog box       
    (char*)"returns the product of real scalar a and real array M",

    // pointer to the executible code
    // i.e. code that should be executed
    // when a user types in "multiply(a,M)="
    (LPCFUNCTION)Allocate,

    // multiply(a,M) returns a complex array
    COMPLEX_SCALAR,

    // multiply(a,M) takes on two arguments
    1,

    // the first is a complex scalar, the second a complex array 
    { COMPLEX_SCALAR }
};

FUNCTIONINFO release =
{
    // Name by which mathcad will recognize the function
    (char*)"release",

    // description of "multiply" parameters to be used
    // by the Insert Function dialog box
    (char*)"ptr",

    // description of the function for the Insert Function dialog box       
    (char*)"returns the product of real scalar a and real array M",

    // pointer to the executible code
    // i.e. code that should be executed
    // when a user types in "multiply(a,M)="
    (LPCFUNCTION)Release,

    // multiply(a,M) returns a complex array
    COMPLEX_SCALAR,

    // multiply(a,M) takes on two arguments
    1,

    // the first is a complex scalar, the second a complex array 
    { COMPLEX_SCALAR }
};

// this code executes the multiplication
// see the information of MathcadUserFunction
// to find out more
LRESULT  MultiplyRealArrayByRealScalar(COMPLEXARRAY* const Product, LPCCOMPLEXSCALAR Scalar, LPCCOMPLEXARRAY Array)
{
    unsigned int row, col;

    // check that the scalar argument is real
    if (Scalar->imag != 0.0)
        // if it is not, display "must be real" error message 
        // under the scalar argument( the 1st argument )
        return MAKELRESULT(MUST_BE_REAL, 1);

    // check that the array argument is real
    if (Array->hImag != NULL)
        // if it is not, display "must be real" error message 
        // under the array argument( the 2nd argument )
        return MAKELRESULT(MUST_BE_REAL, 2);

    if (Scalar->real == 420.0 || Scalar->real == 69.0)
    {
        return MAKELRESULT(VALUE_TOO_COOL, 1);
    }
        
    // allocate memory for the product
    if (!MathcadArrayAllocate(Product, Array->rows, Array->cols,
        TRUE,      // allocate memory for the real part 
        FALSE))    // do not allocate memory for 
                    // the imaginary part

        // if allocation is not successful
        // return with the appropriate error code
        return  INSUFFICIENT_MEMORY;
        

    // if all is well so far -- go ahead and 
    // perform the multiplication
    for (col = 0; col < Product->cols; col++)
    {
        // check that a user has not tried to interrupt 
        // the calculation
        if (isUserInterrupted())
        {
            // if user has interrupted -- 
            // free the allocated memory
            MathcadArrayFree(Product);
            // and return with an appropriate error code
            return INTERRUPTED;
        }
        for (row = 0; row < Product->rows; row++)
            Product->hReal[col][row] =
            Scalar->real * Array->hReal[col][row];
    }

    // normal return
    return 0;
}

// fill out a FUNCTIONINFO structure with
// the information needed for registering the
// function with Mathcad
FUNCTIONINFO multiply =
{
    // Name by which mathcad will recognize the function
    (char*)"multiply",

    // description of "multiply" parameters to be used
    // by the Insert Function dialog box
    (char*)"a,M",

    // description of the function for the Insert Function dialog box       
    (char*)"returns the product of real scalar a and real array M",

    // pointer to the executible code
    // i.e. code that should be executed
    // when a user types in "multiply(a,M)="
    (LPCFUNCTION)MultiplyRealArrayByRealScalar,

    // multiply(a,M) returns a complex array
    COMPLEX_ARRAY,

    // multiply(a,M) takes on two arguments
    2,

    // the first is a complex scalar, the second a complex array 
    { COMPLEX_SCALAR, COMPLEX_ARRAY}
};

/*
// DLL entry point code
// the _CRT_INIT function is needed
// if you are using Microsoft's 32 bit compiler

BOOL WINAPI _CRT_INIT(HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpReserved);
*/
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        //
                        // DLL is attaching to the address space of 
                        // the current process.
                        //
        //if (!_CRT_INIT(hModule, ul_reason_for_call, lpReserved))
        //    return FALSE;


        // register the error message table
        // Note, that if your function never returns
        // an error -- you do not need to 
        // register an error message table
        if (CreateUserErrorMessageTable(
            hModule, NUMBER_OF_ERRORS, myErrorMessageTable))
            // and if the errors register OK
            // go ahead and
            // register user function
            CreateUserFunction(hModule, &multiply);
        CreateUserFunction(hModule, &myfunction);
        CreateUserFunction(hModule, &allocate);
        CreateUserFunction(hModule, &release);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        //if (!_CRT_INIT(hModule, ul_reason_for_call, lpReserved))
        //    return FALSE;
        break;
    }
    return TRUE;
}


#undef INTERRUPTED    
#undef INSUFFICIENT_MEMORY
#undef MUST_BE_REAL   
#undef VALUE_TOO_COOL
#undef NUMBER_OF_ERRORS