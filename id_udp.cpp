// ID_UDP.C

/*
=============================================================================

Refer to LW_UDPCOMMS for a general description of what this is

=============================================================================
*/

#include <sys/types.h>
#if defined _WIN32
    #include <io.h>
#elif defined _arch_dreamcast
    #include <unistd.h>
#else
    #include <sys/uio.h>
    #include <unistd.h>
#endif
#include "SDL_net.h"

#include "wl_def.h"
#pragma hdrstop

using namespace Comms;

namespace Comms
{
    Uint16 port = 0;
    UDPsocket udpsock;
}

Parameter::Parameter(
    char **argv_,
    const int argc_,
    int &i_,
    bool &hasError_,
    bool &showHelp_
    ) :
    argv(argv_),
    argc(argc_),
    i(i_),
    hasError(hasError_),
    showHelp(showHelp_)
{
}

#define IFARG(str) if(!strcmp(arg, (str)))

bool Parameter::check(const char *arg)
{
    IFARG("--port")
    {
        if(++i >= argc)
        {
            printf("The port option is missing the udp server port "
                "argument!\n");
            hasError = true;
        }
        else
        {
            Comms::port = atoi(argv[i]);
        }

        return true;
    }
    else
    {
        return false;
    }
}

const char *Comms::parameterHelp(void)
{
    return " --port                 UDP server port\n";
}

void Comms::startup(void)
{
    SDL_version compile_version;
    const SDL_version *link_version = SDLNet_Linked_Version();
    SDL_NET_VERSION(&compile_version);
    printf("compiled with SDL_net version: %d.%d.%d\n", 
            compile_version.major,
            compile_version.minor,
            compile_version.patch);
    printf("running with SDL_net version: %d.%d.%d\n", 
            link_version->major,
            link_version->minor,
            link_version->patch);

    //if(SDL_Init(0)==-1)
    //{
    //    Quit("SDL_Init: %s\n", SDL_GetError());
    //    exit(1);
    //}
    if(SDLNet_Init()==-1)
    {
        Quit("SDLNet_Init: %s\n", SDLNet_GetError());
    }

    // create a UDPsocket on port
    udpsock = SDLNet_UDP_Open(port);
    if(!udpsock)
    {
        Quit("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
    }
}

void Comms::shutdown(void)
{
    SDLNet_UDP_Close(udpsock);
    udpsock = NULL;

    SDLNet_Quit();
}