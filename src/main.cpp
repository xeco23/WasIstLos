#include <clocale>
#include <glibmm/i18n.h>
#include "Config.hpp"
#include "ui/Application.hpp"
#include "util/Helper.hpp"

namespace
{
    void sigterm(int)
    {
        wil::ui::Application::getInstance().quit();
    }
}

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "");

    bindtextdomain(GETTEXT_PACKAGE, WIL_LOCALEDIR);
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    textdomain(GETTEXT_PACKAGE);

    auto app = wil::ui::Application{argc, argv};

    wil::util::redirectOutputToLogger();

    signal(SIGINT, sigterm);
    signal(SIGTERM, sigterm);
    signal(SIGPIPE, SIG_IGN);

    return app.run();
}
