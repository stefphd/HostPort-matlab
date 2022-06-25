function make(cmd)
    if nargin < 1
        cmd = 'hostport';
    end

    switch cmd
        case 'hostport'
            target = 'HostPortMex';
            include = './include';
            src = 'src/HostPortMex.cpp src/Serial.cpp src/HostPort.cpp';
            flags = '';
            cflags = 'std:c++17';
        case 'serial'
            target = 'SerialMex';
            include = './include';
            src = 'src/SerialMex.cpp src/Serial.cpp';
            flags = '';
            cflags = 'std:c++17';
        otherwise
            error('invalid command');
    end

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