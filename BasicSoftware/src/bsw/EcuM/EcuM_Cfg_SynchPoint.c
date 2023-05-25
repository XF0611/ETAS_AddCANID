/*************************************************************************************************
 * Function Definitions
 *************************************************************************************************/

/**********************************************************************************
 Function name     :   EcuM_Synch_Point_<id>
 Description       :   This Function Synchromizes the respective core from where it 
 is called. <id> is different for each Synch_Point configured.
 Parameter (in)    :   None
 Parameter (inout) :   None
 Parameter (out)   :   None
 Return value      :   None
 Remarks           :
 ***********************************************************************************/
void EcuM_Synch_Point(void);
void EcuM_Synch_Point(void) {
	/* This Function is empty because No Synchronization Point is configured (or)  The Configuration does NOT meet necessary 
	 conditions that are needed for having synch point.*/
	return;
}
