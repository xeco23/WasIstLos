#include "Settings.hpp"
#include <iostream>
#include <glibmm/miscutils.h>
#include <giomm/file.h>
#include "Config.hpp"

namespace wfl::util
{
    Settings& Settings::getInstance()
    {
        static auto instance = Settings{};
        return instance;
    }

    Settings::Settings()
        : m_settingMap{}
        , m_configDir{Glib::get_user_config_dir()}
        , m_configAppDir{m_configDir + "/" WFL_NAME}
        , m_configFilePath{m_configAppDir + "/settings.conf"}
        , m_autostartDesktopFilePath{m_configDir + "/autostart/" WFL_APP_ID ".desktop"}
    {
        if (auto const configFile = Gio::File::create_for_path(m_configFilePath); !configFile->query_exists())
        {
            if (auto configDir = Gio::File::create_for_path(m_configAppDir); !configDir->query_exists())
            {
                if (!configDir->make_directory())
                {
                    std::cerr << "Settings: Failed to create config directory" << std::endl;
                    return;
                }
            }

            if (!configFile->create_file())
            {
                std::cerr << "Settings: Failed to create config file" << std::endl;
                return;
            }
        }

        m_settingMap.loadFromFile(m_configFilePath);
    }

    Settings::~Settings()
    {
        m_settingMap.saveToFile(m_configFilePath);
    }

    void Settings::setAutostart(bool autostart)
    {
        if (auto autostartDir = Gio::File::create_for_path(m_configDir + "/autostart"); !autostartDir->query_exists())
        {
            if (!autostartDir->make_directory())
            {
                std::cerr << "Settings: Failed to create autostart directory" << std::endl;
                return;
            }
        }

        auto destDesktopFile = Gio::File::create_for_path(m_autostartDesktopFilePath);
        if (autostart)
        {
            constexpr auto const possibleDesktopFilePaths = std::array<char const*, 3>{"/usr/local/share/applications/" WFL_APP_ID ".desktop",
                "/usr/share/applications/" WFL_APP_ID ".desktop", "/snap/WasIstLos/current/share/applications/" WFL_APP_ID ".desktop"};

            auto const it = std::find_if(possibleDesktopFilePaths.begin(), possibleDesktopFilePaths.end(),
                [](auto const& elem)
                {
                    auto const file = Gio::File::create_for_path(elem);
                    return file->query_exists();
                });

            if (it == possibleDesktopFilePaths.end())
            {
                std::cerr << "Settings: Failed to find desktop file" << std::endl;
                return;
            }
            auto const srcDesktopFile = Gio::File::create_for_path(*it);
            if (!destDesktopFile->query_exists())
            {
                srcDesktopFile->copy(destDesktopFile);
            }
        }
        else
        {
            if (!destDesktopFile->query_exists())
            {
                std::cerr << "Settings: Desktop file in autostart path does not exist" << std::endl;
                return;
            }
            destDesktopFile->remove();
        }
    }

    bool Settings::getAutostart()
    {
        auto const desktopFile = Gio::File::create_for_path(m_autostartDesktopFilePath);
        return desktopFile->query_exists();
    }
}
