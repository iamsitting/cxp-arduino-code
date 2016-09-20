#ifndef FUNCTION_H
#define FUNCTION_H

#include "globals.h"
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus




	/** Metric Functions **/
	
	//This is where we'll define the functions to 
	void test_getData(void);
	void test_getERPS(void);
	void test_updateData(void);
	void getTime(void);
	void getSpeed(void);
	void updateADS(void);
	void udpateBatteryLevel(void);





  /** Packet Functions **/
  void byteWrite(uint8_t protocol);
  
#ifdef __cplusplus
}
#endif //__cplusplus


#endif //FUNCTION_H
