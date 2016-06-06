%%LOAD ALL TYPES
load('HRIR_CIRC360.mat')
load('HRIR_CIRC360RM.mat')
load('HRIR_FULL2DEG.mat')
load('HRIR_L2354.mat')
load('HRIR_L2702.mat')
%%PRINT COMMENTS AND CONTEXT
HRIR_CIRC360.comments
HRIR_CIRC360.context
HRIR_CIRC360RM.comments
HRIR_CIRC360RM.context
HRIR_FULL2DEG.comments
HRIR_FULL2DEG.context
HRIR_L2354.comments
HRIR_L2354.context
HRIR_L2702.comments
HRIR_L2702.context
%%change all to DEG and show closests
HRIR_CIRC360 = HRIR_CIRC360.setDEG();
HRIR_CIRC360RM = HRIR_CIRC360RM.setDEG();
HRIR_FULL2DEG = HRIR_FULL2DEG.setDEG();
HRIR_L2354 = HRIR_L2354.setDEG();
HRIR_L2702 = HRIR_L2702.setDEG();

fprintf('\nLets try to find AZ:77 EL:11 in degrees in all matrices');

[irID, azFound, elFound] = HRIR_CIRC360.closestIr(77,11); % <<< Here is the command  
    fprintf('\nirID    : %d\n', irID);
    fprintf('azFound : %f DEG\n', azFound);
    fprintf('elFound : %f DEG\n\n', elFound);
    %%Get the impulse response at irID 
    closestIR = HRIR_CIRC360.getIR(irID);  % <<< Here is the command
    hgr = figure(1);
    plot(closestIR ,'r')
    title(['HRIR_CIRC360,  irID:', num2str(irID)]);
    xlabel('Samples');
    ylabel('Amplitude');
    
    
[irID, azFound, elFound] = HRIR_CIRC360RM.closestIr(77,11); % <<< Here is the command  
    fprintf('\nirID    : %d\n', irID);
    fprintf('azFound : %f DEG\n', azFound);
    fprintf('elFound : %f DEG\n\n', elFound);
    %%Get the impulse response at irID 
    closestIR = HRIR_CIRC360RM.getIR(irID);  % <<< Here is the command
    hgr2 = figure(2);
    plot(closestIR ,'r')
    title(['HRIR_CIRC360RM,  irID:', num2str(irID)]);
    xlabel('Samples');
    ylabel('Amplitude');
    
    
[irID, azFound, elFound] = HRIR_FULL2DEG.closestIr(77,11); % <<< Here is the command  
    fprintf('\nirID    : %d\n', irID);
    fprintf('azFound : %f DEG\n', azFound);
    fprintf('elFound : %f DEG\n\n', elFound);
    %%Get the impulse response at irID 
    closestIR = HRIR_FULL2DEG.getIR(irID);  % <<< Here is the command
    hgr = figure(3);
    plot(closestIR ,'r')
    title(['HRIR_FULL2DEG,  irID:', num2str(irID)]);
    xlabel('Samples');
    ylabel('Amplitude');
    
[irID, azFound, elFound] = HRIR_L2354.closestIr(77,11); % <<< Here is the command  
    fprintf('\nirID    : %d\n', irID);
    fprintf('azFound : %f DEG\n', azFound);
    fprintf('elFound : %f DEG\n\n', elFound);
    %%Get the impulse response at irID 
    closestIR = HRIR_L2354.getIR(irID);  % <<< Here is the command
    hgr = figure(4);
    plot(closestIR ,'r')
    title(['HRIR_L2354,  irID:', num2str(irID)]);
    xlabel('Samples');
    ylabel('Amplitude');
    
[irID, azFound, elFound] = HRIR_L2702.closestIr(77,11); % <<< Here is the command  
    fprintf('\nirID    : %d\n', irID);
    fprintf('azFound : %f DEG\n', azFound);
    fprintf('elFound : %f DEG\n\n', elFound);
    %%Get the impulse response at irID 
    closestIR = HRIR_L2702.getIR(irID);  % <<< Here is the command
    hgr = figure(5);
    plot(closestIR ,'r')
    title(['HRIR_L2702,  irID:', num2str(irID)]);
    xlabel('Samples');
    ylabel('Amplitude');
    
    input('  <Enter to continue>');
    clear(all)
