#ifndef DID_H
#define DID_H

#include "DTC_DID_Config.h"
#include "DiagPlatform.h"

void GetDiagData(Typ_DID_Information * p, uint8 *tDiag_Tx_Buf);
void Get_Const_Data( Typ_DID_Information * p,  uint8 * data );
static uint8 BCD_to_hex(uint8 *source_data);

#endif
