#include "Helper.hpp"
#include <cstdio>
#include <unistd.h>
#include <iostream>
#include <giomm/file.h>
#include "Config.hpp"

namespace wil::util
{
    void redirectOutputToLogger()
    {
        if (!Gio::File::create_for_path("/dev/log")->query_exists() && !Gio::File::create_for_path("/var/run/syslog")->query_exists())
        {
            std::cerr << "Skipping redirection of output to logger since the user doesn't have an active syslog" << std::endl;
            return;
        }

        auto const fl = ::popen("logger -i -s -t " WIL_NAME, "w");
        if (!fl)
        {
            auto const errorNumber = errno;
            std::cerr << "Failed to open pipe to logger: " << strerror(errorNumber) << std::endl;
            return;
        }

        auto const fd = ::fileno(fl);
        ::dup2(fd, STDERR_FILENO);
    }
}
