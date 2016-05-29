function [outBuffer, referenceDSP] = genericProcessing(inBuffer, blockID, referenceDSP)

str = referenceDSP.execution.dsp{blockID};

% Start to generate code for the rtprocDSP function call
% Output variables
lhs = '';
rhs1 = '[';
rhs2 = '';
for(ind = 1:str.numberOutputChannels)
    if(ind == 1)
        lhs = [ lhs 'out' num2str(ind)];
        rhs1= [ rhs1 num2str(str.buffersize)];
    else
        lhs = [ lhs ', out' num2str(ind)];
        rhs1= [ rhs1 ', ' num2str(str.buffersize)];
    end
end
lhs = [lhs];
rhs1 = [rhs1 ']'];

% Input variables
for(ind = 1:str.numberInputChannels)
    if(ind <= size(inBuffer,1))
        if(ind == 1)
            rhs2= [ rhs2 'inBuffer(' num2str(ind) ',:)'];
        else
            rhs2= [ rhs2 ', inBuffer(' num2str(ind) ',:)'];
        end
    else
        if(ind == 1)
            rhs2= [ rhs2 'zeros(1, ' num2str(size(inBuffer,2)) ')'];
        else
            rhs2= [ rhs2 ', zeros(1, ' num2str(size(inBuffer,2)) ')'];
        end
    end        
end

% Generate the function call to execute the remote function
tokenTxt =  ['[success ' lhs '] = rtProcDSP_executeRemoteFunctionDSP('...
    'str.functionID,' rhs1 ', ' rhs2 ');'];

% Execute remote function call
eval(tokenTxt);

% Catch possible error messages
if(~success)
    if(str.numberOutputChannels > 0)
        error(['Simulink error: ' out1]);
    else
        error(['Simulink error: Remote function call failed.']);
    end
end

% Produce the output variable from single output buffers, buffer after
% buffer
for(ind = 1:str.numberOutputChannels)
    token = ['outBuffer(' num2str(ind) ',:) = out' num2str(ind) ';'];
    eval(token);
end
 
% Additional info: Increment frame counter
str.frameCounter = str.frameCounter + 1;

% Output frame counter info
disp(['-->DSP Processing: Block ID# ' num2str(str.blockID) ', Frame #' num2str(str.frameCounter)]);

% Write back the data
referenceDSP.execution.dsp{blockID} = str;