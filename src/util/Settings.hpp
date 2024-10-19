#pragma once

#include "SettingMap.hpp"

namespace wil::util
{
    class Settings
    {
        public:
            static Settings& getInstance();

        public:
            Settings(Settings const&)            = delete;
            Settings& operator=(Settings const&) = delete;

        public:
            template<typename ValueType>
            void setValue(Glib::ustring const& group, Glib::ustring const& key, ValueType value);

            template<typename ValueType>
            ValueType getValue(Glib::ustring const& group, Glib::ustring const& key, ValueType defaultValue = {});

        private:
            Settings();
            ~Settings();

        private:
            void setAutostart(bool autostart);
            bool getAutostart();

        private:
            SettingMap m_settingMap;

            Glib::ustring m_configDir;
            Glib::ustring m_configAppDir;
            Glib::ustring m_configFilePath;
            Glib::ustring m_autostartDesktopFilePath;
    };




    template<typename ValueType>
    inline void Settings::setValue(Glib::ustring const& group, Glib::ustring const& key, ValueType value)
    {
        if ((group == "general") && (key == "autostart") && std::is_same_v<ValueType, bool>)
        {
            setAutostart(value);
        }
        else
        {
            m_settingMap.setValue(group, key, value);
        }

        m_settingMap.saveToFile(m_configFilePath);
    }

    template<typename ValueType>
    inline ValueType Settings::getValue(Glib::ustring const& group, Glib::ustring const& key, ValueType defaultValue)
    {
        if ((group == "general") && (key == "autostart") && std::is_same_v<ValueType, bool>)
        {
            return getAutostart();
        }
        else
        {
            return m_settingMap.getValue(group, key, defaultValue);
        }
    }
}
