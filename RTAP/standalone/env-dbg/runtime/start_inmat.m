function [rtproc] = start_inmat(pathCComponents, pathMatlabSouces, namesDemo)

    display('=============================================');
    display('==> Matlab started for use with Jvx Tools.<==');
    display('=============================================');
   
    % Copnfigure system to use RTProc host remotely
    rtproc.pathCComponents = pathCComponents;
    rtproc.pathMatlabSouces = pathMatlabSouces;
    
    disp(['Adding directory ' rtproc.pathCComponents]);
    addpath(rtproc.pathCComponents);

	for(ind=1:size(rtproc.pathMatlabSouces,2))
		disp(['Adding directory ' rtproc.pathMatlabSouces{ind}]);
		addpath(rtproc.pathMatlabSouces{ind});
	end
    disp(['Usage: ']);
	for(ind = 1:size(namesDemo))	
		disp([num2str(ind) ') ' namesDemo{ind}]);
	end