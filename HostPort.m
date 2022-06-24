classdef HostPort < handle
    %HOSTPORT HostPort class for communication to a microcontroller
    
    properties (SetAccess = private, Hidden)
        ptr_ = uint64(0);
    end
    
    properties (Dependent)
        Port (1,1) {mustBeInteger, mustBeNonnegative, mustBeLessThan(Port,256)}
        Baud (1,1) {mustBeInteger, mustBeNonnegative, mustBeLessThan(Baud,4294967296)}
        Header (1,1) {mustBeInteger, mustBeNonnegative, mustBeLessThan(Header,4294967296)}
        Terminator (1,1) {mustBeInteger, mustBeNonnegative, mustBeLessThan(Terminator,4294967296)}
        IsInit 
        %Pointer;
    end
    
    properties (SetAccess = private)
        Data 
    end
    
    methods(Static)
        
        function clear()
            HostPortMex('delete');
        end
        
        function ports = getPorts()
            ports = HostPortMex('getAvailablePort');
        end  
        
    end
    
    methods (Access = public)
        
        %constructor
        function obj = HostPort()
            %HOSTPORT Construct an instance of this classa
            obj.ptr_ = HostPortMex('new'); %assign pointer of new object
        end

        %destructor
        function delete(obj)
            if any(obj.ptr_ == HostPortMex('getHandles'))
                 HostPortMex('delete',obj.ptr_); %delete object
            end
        end

        %begin, restart and close mathods
        function exit = begin(obj, port, baud, header, terminator)
            %check input
            if rem(port,1)~=0 || port<0
                error('Port must be a positive integer');
            end
            if rem(baud,1)~=0 || baud<0
                error('Baud must be a positive integer');
            end
            if (nargin-1)<3
                exit = HostPortMex('begin',obj.ptr_,uint32(port),uint32(baud));
            else
                exit = HostPortMex('begin',obj.ptr_,uint32(port),uint32(baud),uint32(header),uint32(terminator));                
            end
        end

        function exit = restart(obj) 
            exit = HostPortMex('restart',obj.ptr_);
        end

        function exit = close(obj)
           exit = HostPortMex('close',obj.ptr_); 
        end
        
        %read and write
        function exit = read(obj, len)
           [obj.Data, exit] = HostPortMex('read',obj.ptr_,len); 
        end
        
        function exit = write(obj, buf)
            buf = typecast(buf, 'uint8');
            exit = HostPortMex('write',obj.ptr_,buf);            
        end
        
        function data = decode(obj, type)
            arguments
                obj
                type char {mustBeMember(type,{'uint8','uint16','uint32','single','double'})} = 'uint8'
            end
            data = typecast(obj.Data, type);
        end
        
        %logical operators
        function exit = logical(obj)
            exit = HostPortMex('isInit',obj.ptr_);
        end

        function exit = not(obj)
            exit = ~HostPortMex('isInit',obj.ptr_);
        end
        
    end

    methods %get & set methods
        
        function port = get.Port(obj)
            port = HostPortMex('getPort',obj.ptr_);
        end

        function baud = get.Baud(obj)
            baud = HostPortMex('getBaud',obj.ptr_);
        end

        function header = get.Header(obj)
            header = HostPortMex('getHeader',obj.ptr_);
        end

        function terminator = get.Terminator(obj)
            terminator = HostPortMex('getTerminator',obj.ptr_);
        end

        function isInit = get.IsInit(obj)
            isInit = HostPortMex('isInit',obj.ptr_);
        end

        %function ptr = get.Pointer(obj)
        %    ptr = obj.ptr_;
        %end

        function set.Port(obj, port)
           HostPortMex('setPort',obj.ptr_,uint32(port));
        end

        function set.Baud(obj, baud)
           if (baud < 0 || rem(baud,1)~=0)
               error('Baud must be a positive integer');
           end
           HostPortMex('setBaud',obj.ptr_,uint32(baud));
        end

        function set.Header(obj, header)
           if (header < 0 || rem(terminator,1)~=0)
               error('Header must be a header integer');
           end
           HostPortMex('setHeader',obj.ptr_,uint32(header)); 
        end

        function set.Terminator(obj, terminator)
           if (terminator < 0 || rem(terminator,1)~=0)
               error('Terminator must be a positive integer');
           end
           HostPortMex('setTerminator',obj.ptr_,uint32(terminator)); 
        end

    end
end

