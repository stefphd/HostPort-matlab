classdef HostPort < handle
    %HOSTPORT HostPort class for communication to a microcontroller
    
    properties (SetAccess = private, Hidden)
        ptr_ = uint64([]);
    end
    
    properties (Dependent)
        Port (1,1) {mustBeInteger, mustBeNonnegative, mustBeLessThan(Port,256)}
        Baud (1,1) {mustBeInteger, mustBeNonnegative, mustBeLessThan(Baud,4294967296)}
        Header (1,1) {mustBeInteger, mustBeNonnegative, mustBeLessThan(Header,4294967296)}
        Terminator (1,1) {mustBeInteger, mustBeNonnegative, mustBeLessThan(Terminator,4294967296)}
        IsInit 
    end
    
    methods (Static)
        
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
            %HOSTPORT Construct an instance of this class
            obj.ptr_ = HostPortMex(); %assign pointer of new object
        end

        %destructor
        function delete(obj)
            if any(obj.ptr_ == HostPortMex('getHandles'))
                 HostPortMex('delete',obj.ptr_); %delete object
            end
        end

        %begin, restart and close mathods
        function exit = begin(obj, port, baud, varargin)
            %check input
            arguments
                obj
                port (1,1) {mustBeInteger, mustBeNonnegative, mustBeLessThan(port,256)}
                baud (1,1) {mustBeInteger, mustBeNonnegative, mustBeLessThan(baud,4294967296)}
            end
            arguments (Repeating)
                varargin  (1,1) {mustBeInteger, mustBeNonnegative, mustBeLessThan(varargin,4294967296)}
            end
            if (nargin-1)<3
                exit = HostPortMex('begin',obj.ptr_,uint32(port),uint32(baud));
            else
                exit = HostPortMex('begin',obj.ptr_,uint32(port),uint32(baud),uint32(varargin{1}),uint32(varargin{2}));                
            end
        end

        function exit = restart(obj) 
            exit = HostPortMex('restart',obj.ptr_);
        end

        function exit = close(obj)
           exit = HostPortMex('close',obj.ptr_); 
        end
        
        %read and write
        function [data, exit] = read(obj, len, type)
            arguments
                obj
                len {mustBeInteger, mustBeNonnegative, mustBeLessThan(len,4294967296)}
                type char {mustBeMember(type,{'uint8','uint16','uint32','uint64','int8','int16','int64','single','double'})} = 'uint8'
            end
           [data, exit] = HostPortMex('read',obj.ptr_,len,type); 
        end
        
        function exit = write(obj, data)
            exit = HostPortMex('write',obj.ptr_,data);            
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

        function set.Port(obj, port)
           HostPortMex('setPort',obj.ptr_,uint32(port));
        end

        function set.Baud(obj, baud)
           HostPortMex('setBaud',obj.ptr_,uint32(baud));
        end

        function set.Header(obj, header)
           HostPortMex('setHeader',obj.ptr_,uint32(header)); 
        end

        function set.Terminator(obj, terminator)
           HostPortMex('setTerminator',obj.ptr_,uint32(terminator)); 
        end

    end
end

