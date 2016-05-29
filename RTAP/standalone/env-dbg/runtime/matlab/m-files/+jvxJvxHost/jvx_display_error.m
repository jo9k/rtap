function [] = jvx_display_error(errCode, origFile, errTxt, errorsAsWarnings)
if(nargin <= 3)
    errorsAsWarnings = false;
end
dd = dbstack;
if(errorsAsWarnings)
    disp(['Warning in RTProc host <' num2str(errCode) ';' origFile ';' dd(2).name '>:' errTxt]);
else
    disp(['Error in RTProc host <' num2str(errCode) ';' origFile ';' dd(2).name '>:' errTxt]);
end

