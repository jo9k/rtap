classdef databaseHRIR < handle
    
    properties
        data@double 
        lengthHRIR@double scalar
        fs@double scalar
    end
    
    
    methods
        function obj = databaseHRIR()
            load('matHRIRs');
            obj.data = cat( 3, matHRIRs_left ,  matHRIRs_right );
            obj.lengthHRIR = 308;
            obj.fs = 44100;
        end
        
        function HRIR = getHRIR(obj, angle)
            % angle is in deg and clock-wise           
           
            
            % first index points to initialAngle as specified in Hauke's HRIR matrix
            % format
            initialAngle = -1.819271939631079e+02;
            
            % angle step size per index as specified in Hauke's HRIR matrix
            % format
            granularity = 0.123616545556177;
            
            % make sure angle is between inititalAngle and inititalAngle + 360
            angle  = mod(angle - initialAngle , 360) + initialAngle;
            
            % last index points to lastAngle:
            % initialAngle + (length(matHRTFs) - 1) * granularity
            % lastAngle = 1.819999161542762e+02 ;
            
            % get HRIF index as rational number
            idx = ((angle - initialAngle) / granularity) + 1;
            
            % check if idx is integer
            if ceil (idx)  == floor (idx)
                HRIR = obj.data(idx, :)';
            else
                % linear interpolation of neighbours
                HRIR =  (idx - floor (idx)) * obj.data(ceil (idx) , :, : ) ...
                    + (ceil(idx) - idx ) * obj.data (floor (idx) , :, : ) ;
                HRIR = shiftdim(HRIR,1);
            end
        end
        
    end
    
end

