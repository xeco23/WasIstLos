#pragma once

#include <iostream>
#include <type_traits>
#include <glibmm/keyfile.h>

namespace wil::util
{
    class SettingMap
    {
        public:
            SettingMap();

        public:
            bool loadFromFile(std::string const& fileName);
            bool saveToFile(std::string const& fileName);

            template<typename ValueType>
            void setValue(Glib::ustring const& group, Glib::ustring const& key, ValueType value);

            template<typename ValueType>
            ValueType getValue(Glib::ustring const& group, Glib::ustring const& key, ValueType defaultValue = {}) const noexcept;

        private:
            Glib::KeyFile m_map;
    };




    inline SettingMap::SettingMap() = default;

    inline bool SettingMap::loadFromFile(std::string const& fileName)
    {
        try
        {
            return m_map.load_from_file(fileName);
        }
        catch (Glib::Error const& error)
        {
            std::cerr << "SettingMap: Failed to load setting map from file: " << error.what().c_str() << std::endl;
            return false;
        }
    }

    inline bool SettingMap::saveToFile(std::string const& fileName)
    {
        try
        {
            return m_map.save_to_file(fileName);
        }
        catch (Glib::Error const& error)
        {
            std::cerr << "SettingMap: Failed to save setting map to file: " << error.what().c_str() << std::endl;
            return false;
        }
    }

    template<typename ValueType>
    inline void SettingMap::setValue(Glib::ustring const& group, Glib::ustring const& key, ValueType value)
    {
        if constexpr (std::is_same_v<ValueType, bool>)
        {
            m_map.set_boolean(group, key, value);
        }
        else if constexpr (std::is_same_v<ValueType, int>)
        {
            m_map.set_integer(group, key, value);
        }
        else if constexpr (std::is_same_v<ValueType, double>)
        {
            m_map.set_double(group, key, value);
        }
        else if constexpr (std::is_same_v<ValueType, Glib::ustring>)
        {
            m_map.set_string(group, key, value);
        }
        else
        {
            m_map.set_value(group, key, value);
        }
    }

    template<typename ValueType>
    inline ValueType SettingMap::getValue(Glib::ustring const& group, Glib::ustring const& key, ValueType defaultValue) const noexcept
    {
        try
        {
            if constexpr (std::is_same_v<ValueType, bool>)
            {
                return m_map.get_boolean(group, key);
            }
            else if constexpr (std::is_same_v<ValueType, int>)
            {
                return m_map.get_integer(group, key);
            }
            else if constexpr (std::is_same_v<ValueType, double>)
            {
                return m_map.get_double(group, key);
            }
            else if constexpr (std::is_same_v<ValueType, Glib::ustring>)
            {
                return m_map.get_string(group, key);
            }
            else
            {
                return m_map.get_value(group, key);
            }
        }
        catch (Glib::KeyFileError const& error)
        {
            return defaultValue;
        }
    }
}
