
//include MATLAB mex
#include "mex.h"

//include std
#include <map>
#include <string>

//include HostPort class
#include "HostPort.h"

//define
#define getUint64 (uint64_t*) mxGetData
#define getUint32 (uint32_t*) mxGetData
#define getUint16 (uint16_t*) mxGetData
#define getUint8 (uint8_t*) mxGetData

//#define DEBUG

//List of class method
enum class Action {
    New,
    Delete,
    Begin,
    Close,
    Restart,
    Read,
    Write,
    SetPort,
    SetBuad,
    SetHeader,
    SetTerminator,
    GetPort,
    GetBaud,
    GetHeader,
    GetTerminator,
    IsInit,
    GetHandles,
    DeleteHandles,
    GetAvailablePort,
};

//Map string to Action
std::map<std::string, Action> actionTypeMap = {
    {"new",             Action::New             },
    {"delete",          Action::Delete          },
    {"begin",           Action::Begin           },
    {"close",           Action::Close           },
    {"restart",         Action::Restart         },
    {"read",            Action::Read            },
    {"write",           Action::Write           },
    {"setPort",         Action::SetPort         },
    {"setBaud",         Action::SetBuad         },
    {"setHeader",       Action::SetHeader       },
    {"setTerminator",   Action::SetTerminator   },
    {"getPort",         Action::GetPort         },
    {"getBaud",         Action::GetBaud         },
    {"getHeader",       Action::GetHeader       },
    {"getTerminator",   Action::GetTerminator   },
    {"isInit",          Action::IsInit          },
    {"getHandles",      Action::GetHandles      },
    {"getAvailablePort",Action::GetAvailablePort},
};

//Map handle to pointer
std::map<uint64_t, std::shared_ptr<HostPort>> handleMap;

//exit fnc
int mexAtExit(void (*ExitFcn)(void)) {
    for (const auto& [k, v] : handleMap) 
        handleMap.erase(k), mexUnlock();
    return 0;
}

//main
void mexFunction(int nlhs, mxArray *plhs[],int nrhs, const mxArray *prhs[]) {

    if (nrhs < 1 || !mxIsChar(prhs[0])) //check input
        mexErrMsgTxt("First input must be an action string ('new', 'delete', or a method name).");
    
    char *actionCstr = mxArrayToString(prhs[0]);
    std::string actionStr(actionCstr);
    mxFree(actionCstr);
    
    if (actionTypeMap.count(actionStr) == 0)
        mexErrMsgTxt(("Unrecognized action (not in actionTypeMap): " + actionStr).c_str());
    
    Action action = actionTypeMap.at(actionStr);
    
    //single-argument methods
    switch (action) {

        case Action::New: {
            plhs[0] = mxCreateNumericMatrix(1,1,mxUINT64_CLASS,mxREAL);
            std::shared_ptr<HostPort> handle(new HostPort());
            uint64_t handle_id = (uint64_t) handle.get();
            handleMap[handle_id] = handle;
            *(getUint64(plhs[0])) = handle_id;
            mexLock();
            return;
        }
        
        case Action::GetHandles: {
            plhs[0] = mxCreateNumericMatrix(handleMap.size(),1,mxUINT64_CLASS,mxREAL);
            int i = 0;
            for (const auto& [k, v] : handleMap)
                *(getUint64(plhs[0])+i) = k, ++i;
            return;
        }

        case Action::Delete: {
            if (nrhs == 1) { //delete all if no ptr given
                for (const auto& [k, v] : handleMap) 
                    handleMap.erase(k), mexUnlock();
                return;
            }
            break;
        }

        case Action::GetAvailablePort: {
            std::vector<int> ports = Serial::get_availableSerialPorts(255);
            plhs[0] = mxCreateNumericMatrix(ports.size(),1,mxUINT32_CLASS,mxREAL);
            for (int i = 0; i < ports.size(); ++i) 
                *(getUint32(plhs[0])+i) = ports[i];
            return;
        }
    }
    
    //multiple-argument methods
    if (nrhs < 2 || (mxGetClassID(prhs[1]) != mxUINT64_CLASS)) //check inputs
                mexErrMsgTxt("Second input must be an unsigned integer.");
    
    uint64_t handle_id = static_cast<uint64_t>(mxGetScalar(prhs[1]));
    if (handleMap.count(handle_id) == 0) //check if valid handle
        mexErrMsgTxt("Unrecognized handle");

    HostPort* handle = handleMap[handle_id].get(); //pointer to HostPort object
    
    switch (action) {

        case Action::Delete: {
            handleMap.erase(handle_id);
            mexUnlock();
            return;
        }
        
        case Action::Begin: { //ptr, port, baud, header, terminator
            if (nrhs < 4 || (mxGetClassID(prhs[2]) != mxUINT32_CLASS) || (mxGetClassID(prhs[3]) != mxUINT32_CLASS))
                mexErrMsgTxt("Third and fourth inputs must be a scalar.");
            unsigned int port = static_cast<unsigned int>(mxGetScalar(prhs[2]));
            unsigned int baud = static_cast<unsigned int>(mxGetScalar(prhs[3]));
            unsigned int header = HostPort::HEADER;
            unsigned int terminator = HostPort::TERMINATOR;
            unsigned int timeout = HostPort::TIMEOUT;
            if (nrhs > 4) header = static_cast<unsigned int>(mxGetScalar(prhs[4]));
            if (nrhs > 5) terminator = static_cast<unsigned int>(mxGetScalar(prhs[5]));
            //if (nrhs > 6) timeout = static_cast<unsigned int>(mxGetScalar(prhs[6]));
            plhs[0] = mxCreateLogicalMatrix(1,1); 
            *(mxGetLogicals(plhs[0])) = handle->begin(port, baud, header, terminator, timeout);        
            #ifdef DEBUG
            printf("begin with\n");
            printf("\tport:\t%u\n",handle->getPort());
            printf("\tbaud:\t%u\n",handle->getBaud());
            printf("\theader:\t%u\n",handle->getHeader());
            printf("\tterminator:\t%u\n",handle->getTerminator());
            #endif
            return;
        }

        case Action::Close: {
            plhs[0] = mxCreateLogicalMatrix(1,1); 
            *(mxGetLogicals(plhs[0])) = handle->close();
            return;
        }

        case Action::Restart: {
            plhs[0] = mxCreateLogicalMatrix(1,1); 
            *(mxGetLogicals(plhs[0])) = handle->restart();
            return;
        }

        case Action::IsInit: {
            plhs[0] = mxCreateLogicalMatrix(1,1); 
            *(mxGetLogicals(plhs[0])) = handle->isInit();
            return;
        }

        case Action::SetPort: {
            if (nrhs > 3 || (mxGetClassID(prhs[2]) != mxUINT32_CLASS))
                mexErrMsgTxt("Third input must be a scalar.");
            unsigned int port = static_cast<unsigned int>(mxGetScalar(prhs[2]));
            plhs[0] = mxCreateLogicalMatrix(1,1); 
            *(mxGetLogicals(plhs[0])) = handle->setPort(port);
            return;
        }

        case Action::SetBuad: {
            if (nrhs < 3 || (mxGetClassID(prhs[2]) != mxUINT32_CLASS))
                mexErrMsgTxt("Third input must be a scalar.");
            unsigned int baud = static_cast<unsigned int>(mxGetScalar(prhs[2]));
            plhs[0] = mxCreateLogicalMatrix(1,1); 
            *(mxGetLogicals(plhs[0])) = handle->setBaud(baud);
            return;
        }

        case Action::SetHeader: {
            unsigned int header = HostPort::HEADER;
            if (nrhs > 2) header = static_cast<unsigned int>(mxGetScalar(prhs[2]));
            plhs[0] = mxCreateLogicalMatrix(1,1); 
            *(mxGetLogicals(plhs[0])) = handle->setHeader(header);
            return;
        }

        case Action::SetTerminator: {
            unsigned int terminator = HostPort::TERMINATOR;
            if (nrhs > 2) terminator = static_cast<unsigned int>(mxGetScalar(prhs[2]));
            plhs[0] = mxCreateLogicalMatrix(1,1); 
            *(mxGetLogicals(plhs[0])) = handle->setTerminator(terminator);
            return;
        }

        case Action::GetPort: {
            plhs[0] = mxCreateNumericMatrix(1,1,mxUINT32_CLASS,mxREAL);
            *(getUint32(plhs[0])) = handle->getPort();
            return;
        }

        case Action::GetBaud: {
            plhs[0] = mxCreateNumericMatrix(1,1,mxUINT32_CLASS,mxREAL);
            *(getUint32(plhs[0])) = handle->getBaud();
            return;
        }

        case Action::GetHeader: {
            plhs[0] = mxCreateNumericMatrix(1,1,mxUINT32_CLASS,mxREAL);
            *(getUint32(plhs[0])) = handle->getHeader();
            return;
        }

        case Action::GetTerminator: {
            plhs[0] = mxCreateNumericMatrix(1,1,mxUINT32_CLASS,mxREAL);
            *(getUint32(plhs[0])) = handle->getTerminator();
            return;
        }

        case Action::Read: {
            if (nrhs < 3 || (mxGetClassID(prhs[2]) != mxDOUBLE_CLASS))
                mexErrMsgTxt("Third input must be a scalar.");
            unsigned int size = static_cast<unsigned int>(mxGetScalar(prhs[2]));
            unsigned char* buf;
            buf = (unsigned char*) malloc(size);
            plhs[0] = mxCreateNumericMatrix(size,1,mxUINT8_CLASS,mxREAL);
            plhs[1] = mxCreateLogicalMatrix(1,1);
            if (buf == NULL)
                mexErrMsgTxt("Fatal error in memory allocation");
            if (handle->read(buf, size)) {
                memcpy(getUint8(plhs[0]), buf, size);
                *(mxGetLogicals(plhs[1])) = true;
            } else 
                *(mxGetLogicals(plhs[1])) = false;
            free(buf);
            return;
        }

        case Action::Write: {
            if ((nrhs < 3) || (mxGetClassID(prhs[2]) != mxUINT8_CLASS))
                mexErrMsgTxt("Third input must be a vector of bytes.");
            unsigned int size = mxGetNumberOfElements(prhs[2]);
            unsigned char* buf = getUint8(prhs[2]);
            plhs[0] = mxCreateLogicalMatrix(1,1);
            *(mxGetLogicals(plhs[0])) = handle->write(buf, size);
            mxFree(buf);
            return;
        }
    }
}



