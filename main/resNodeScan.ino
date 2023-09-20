//----------------------------------
void UECSupdate16529port( UECSTEMPCCM* _tempCCM){

  int packetSize = UECS_UDP16529.parsePacket();
   if(packetSize){
   	   
   	   ClearMainBuffer();
      _tempCCM->address = UECS_UDP16529.remoteIP();   
      UECSbuffer[UECS_UDP16529.read(UECSbuffer, BUF_SIZE-1)]='\0';
	  UDPFilterToBuffer();
	  
	  
      if(UECSresNodeScan()){
         UECS_UDP16529.beginPacket(_tempCCM->address, 16529);
         UECS_UDP16529.write(UECSbuffer);
         
         if(UECS_UDP16529.endPacket()==0)
         	{
  			UECSresetEthernet(); //when udpsend failed,reset ethernet status
         	}
      }     
   }
}



/********************************/
/* 16529 Response   *************/
/********************************/

boolean UECSresNodeScan(){
   	int i;
	int progPos = 0;
	int startPos = 0;
	
	if(!UECSFindPGMChar(UECSbuffer,&UECSccm_XMLHEADER[0],&progPos)){return false;}
	startPos+=progPos;

	if(!UECSFindPGMChar(&UECSbuffer[startPos],&UECSccm_UECSVER_E10[0],&progPos))
	{
		//other ver
		return false;
	}
	startPos+=progPos;
	
	//NODESCAN
	if(UECSFindPGMChar(&UECSbuffer[startPos],&UECSccm_NODESCAN1[0],&progPos) || UECSFindPGMChar(&UECSbuffer[startPos],&UECSccm_NODESCAN2[0],&progPos))
	{
		
    //NODESCAN response
	ClearMainBuffer();
    UDPAddPGMCharToBuffer(&(UECSccm_XMLHEADER[0]));
    UDPAddPGMCharToBuffer(&(UECSccm_UECSVER_E10[0]));
    UDPAddPGMCharToBuffer(&(UECSccm_NODENAME[0])); 
    UDPAddPGMCharToBuffer(&(U_name[0]));
    UDPAddPGMCharToBuffer(&(UECSccm_VENDER[0])); 
    UDPAddPGMCharToBuffer(&(U_vender[0]));
    UDPAddPGMCharToBuffer(&(UECSccm_UECSID[0])); 
    UDPAddPGMCharToBuffer(&(U_uecsid[0]));
    UDPAddPGMCharToBuffer(&(UECSccm_UECSID_IP[0]));
    
    
    
    if(U_orgAttribute.status & STATUS_SAFEMODE)
		{
		UDPAddPGMCharToBuffer(&(UECSdefaultIPAddress[0]));
		}
	else
		{
	    sprintf(UECStempStr20, "%d.%d.%d.%d", U_orgAttribute.ip[0], U_orgAttribute.ip[1], U_orgAttribute.ip[2], U_orgAttribute.ip[3]);
	    UDPAddCharToBuffer(UECStempStr20);
	    }
    
    UDPAddPGMCharToBuffer(&(UECSccm_MAC[0]));
    sprintf(UECStempStr20, "%02X%02X%02X%02X%02X%02X", U_orgAttribute.mac[0], U_orgAttribute.mac[1], U_orgAttribute.mac[2], U_orgAttribute.mac[3], U_orgAttribute.mac[4], U_orgAttribute.mac[5]);
    UDPAddCharToBuffer(UECStempStr20);
    UDPAddPGMCharToBuffer(&(UECSccm_NODECLOSE[0]));
    
    return true;
	}
    
	//CCMSCAN
	if(UECSFindPGMChar(&UECSbuffer[startPos],&UECSccm_CCMSCAN[0],&progPos))
	{

		
	short pageNum=0;
	startPos+=progPos;
	
	
	if(UECSGetValPGMStrAndChr(&UECSbuffer[startPos],&UECSccm_PAGE[0],'\"',&pageNum,&progPos))//format of page number written type
		{
		startPos+=progPos;
		//check close tag
		if(!(UECSFindPGMChar(&UECSbuffer[startPos],&UECSccm_CCMSCANCLOSE2[0],&progPos))){return false;}
		}
	else if(UECSFindPGMChar(&UECSbuffer[startPos],&UECSccm_CCMSCANCLOSE0[0],&progPos)) //format of page number abridged type
		{
		pageNum=1;
		}
	else
		{return false;}
	
	//CCMSCAN response
	ClearMainBuffer();
	UDPAddPGMCharToBuffer(&(UECSccm_XMLHEADER[0]));
	UDPAddPGMCharToBuffer(&(UECSccm_UECSVER_E10[0]));
    UDPAddPGMCharToBuffer(&(UECSccm_CCMNUM[0]));
	
	//count enabled ccm
	short enabledCCMNum=0;
	short returnCCMID=-1;
		for(i = 0; i < U_MAX_CCM; i++)
		{
	    if(U_ccmList[i].ccmLevel != NONE)
	    	{
	    	enabledCCMNum++;
	    	if(enabledCCMNum==pageNum){returnCCMID=i;}
	    	}
	    
	    
	    }
	    

	
	if(enabledCCMNum==0 || returnCCMID<0){return false;}//page num over
	
	UDPAddValueToBuffer(pageNum);
	UDPAddPGMCharToBuffer(&(UECSccm_TOTAL[0]));
	UDPAddValueToBuffer(enabledCCMNum);
    UDPAddPGMCharToBuffer(&(UECSccm_CLOSETAG[0]));
	UDPAddValueToBuffer(1);//Column number is always 1
    UDPAddPGMCharToBuffer(&(UECSccm_CCMNO[0]));
	UDPAddValueToBuffer(pageNum);//page number = ccm number
	
	
	for(i=0;i<3;i++)
	{
	 UDPAddPGMCharToBuffer(UECSattrChar[i]);
	 UDPAddValueToBuffer(U_ccmList[returnCCMID].baseAttribute[i]);
	}
	 UDPAddPGMCharToBuffer(UECSattrChar[AT_PRIO]);
	 UDPAddValueToBuffer(U_ccmList[returnCCMID].baseAttribute[AT_PRIO]);

	 
	 UDPAddPGMCharToBuffer(&(UECSccm_CAST[0]));
	 UDPAddValueToBuffer(U_ccmList[returnCCMID].decimal);
	 UDPAddPGMCharToBuffer(&(UECSccm_UNIT[0]));                          
	 UDPAddPGMCharToBuffer((U_ccmList[returnCCMID].unit));
	 UDPAddPGMCharToBuffer(&(UECSccm_SR[0]));
     if(U_ccmList[returnCCMID].sender){
     	 		UDPAddPGMCharToBuffer(UECSTxtPartS);
              }
              else{
     	 		UDPAddPGMCharToBuffer(UECSTxtPartR);
              }              
	 UDPAddPGMCharToBuffer(&(UECSccm_LV[0]));                          
	 UDPAddPGMCharToBuffer((UECSCCMLEVEL[U_ccmList[returnCCMID].ccmLevel]));
	 UDPAddPGMCharToBuffer(&(UECSccm_CLOSETAG[0]));
	 UDPAddCharToBuffer(U_ccmList[returnCCMID].typeStr);
	 UDPAddPGMCharToBuffer(&(UECSccm_CCMRESCLOSE[0]));  
    
    return true;
    }
    
    return false;
  
  
}
