function make()

    target = 'HostPortMex';
    include = './include';
    src = 'src/HostPortMex.cpp src/Serial.cpp src/HostPort.cpp';
    flags = '';
    cflags = 'std:c++17';

    if ispc
        cflags = ['COMPFLAGS=''$COMPFLAGS /' cflags ''''];
     elseif isunix
        cflags = ['CFLAGS=''$CFLAGS -' cflags ''''];
    else
        error('Unsuppoted OS');
    end

    cmd = ['mex ' src ' -I' include ' -output ' target ' ' flags ' ' cflags];
    disp(cmd);
    eval(cmd);
end