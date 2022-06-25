function make()

    target = 'HostPortMex';
    include = './include';
    src = 'src/HostPortMex.cpp src/Serial.cpp src/HostPort.cpp';
    flags = '';

    if ispc
        cflags = ['COMPFLAGS=''$COMPFLAGS /' 'std:c++17' ''''];
     elseif isunix
        cflags = ['CXXFLAGS=''$CXXFLAGS -' 'std=c++17' ''''];
    else
        error('Unsuppoted OS');
    end

    cmd = ['mex ' src ' -I' include ' -output ' target ' ' flags ' ' cflags];
    disp(cmd);
    eval(cmd);
end