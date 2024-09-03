while(1)
    {
        digitalWrite (2, HIGH);
        printf("Press the R button for Receiver Mode or any other key for Sender Moden");
        scanf(" %c",&mode);
        
        if (mode=='R'||mode=='r')
        {
            digitalWrite(2,LOW);
            modeReceiver=true;
        }
        
        if (mode!='R'&&mode!='r')
        {
            digitalWrite(2,LOW);
            modeReceiver=false;
            
            char dataName[NAME_MAX];
            char dataExtension[NAME_MAX];
            
            
            printf("n Name of file WITHOUT extension: ");
            scanf("%s",dataName);

            printf("n Extension: ");
            scanf("%s",dataExtension);

            if (read_file(dataName, dataExtension, file_content) != OK)
            {
                printf("File read error, size exceeds array sizen");
                return -1;
            }
            BuildDataFrame(dataName, dataExtension, file_content);
        }
        
        
        
        while(modeReceiver)
        {
            gettimeofday(&tval_after, NULL);
            timersub(&tval_after, &tval_before, &tval_result);
            double time_elapsed = (double)tval_result.tv_sec + ((double)tval_result.tv_usec/1000000.0f);
            
            while(time_elapsed < 0.001)
            {
                gettimeofday(&tval_after, NULL);
                timersub(&tval_after, &tval_before, &tval_result);
                time_elapsed = (double)tval_result.tv_sec + ((double)tval_result.tv_usec/1000000.0f);
            }
            gettimeofday(&tval_before, NULL);
            
            int data = digitalRead(0);
            
            
            switch (state)
            {
                case 0:
                    //looking for preamble pattern
                    synchro_Done=false;
                    LookForSynchro(data);
                    
                    if (synchro_Done==true)
                    {
                        state=1;
                    }
                    break;
                    
                case 1:
                    //receive the actual data
                    receiveData_Done=false;
                    senderState=false;
                    ReceiveData(data);
                    
                    if(receiveData_Done&&senderState==false)
                    {
                        state=0;
                    }
                    if(senderState==true){
                        senderState=false;
                        state=0;
                        modeReceiver=false;
                        }
                break;
            }
        }
    }