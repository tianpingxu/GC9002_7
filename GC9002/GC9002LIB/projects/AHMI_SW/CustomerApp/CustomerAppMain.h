
#ifndef CUSTOMERAPP_MAIN_H
#define CUSTOMERAPP_MAIN_H

#ifdef __cplusplus 
extern "C" {
#endif

/* Customer need to define self's initialization enterance function here , it will be compounded with AHMI system */
#define CUSTOMER_INIT               CustApp_vInitEnterance()


#ifndef CUSTOMER_INIT
#define CUSTOMER_INIT
#endif

void CustApp_vInitEnterance(void);


#ifdef __cplusplus 
}
#endif

#endif  //end of CUSTOMERAPP_MAIN_H



