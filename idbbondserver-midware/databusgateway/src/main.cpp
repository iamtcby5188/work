#include <iostream>
#include <boutil/process.h>
#include <log.h>
#include "data_bus_gateway.h"

#define VERSION "version 0.5"

int main (int argc, char** argv)
{
    if (argc > 1)
    {
	if (('-' == argv[1][0])
         && ('v' == argv[1][1] || 'V' == argv[1][1]))
	{
	    std::cout << VERSION << std::endl;
	}else
	{
	    std::cout << "usage: databusgateway [-vV]\n"
		      << "   vV: show the version.\n"
	              << std::endl;
	}
    }else
    {
        init_logger (boutil::process::run_path () + SLASH_CHAR + ".."
                + SLASH_CHAR + "cfg" + SLASH_CHAR + "log.cfg");
        LOG4_INFO ("%s is starting..", argv[0]);
        data_bus_gateway gateway;
        gateway.start ();
        LOG4_INFO ("%s is stopping..", argv[0]);
    }

    std::cout << "done." << std::endl;
    return 0;
}


