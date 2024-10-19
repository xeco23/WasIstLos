#include "Sound.hpp"
#include <iostream>
#include "Config.hpp"

namespace wil::util
{
    Sound::Sound()
        : m_context{nullptr}
    {
        ca_context_create(&m_context);

        ca_context_change_props(m_context, CA_PROP_APPLICATION_ID, WIL_APP_ID, nullptr);
        ca_context_change_props(m_context, CA_PROP_APPLICATION_NAME, WIL_FRIENDLY_NAME, nullptr);
        ca_context_change_props(m_context, CA_PROP_APPLICATION_VERSION, WIL_VERSION, nullptr);
        ca_context_change_props(m_context, CA_PROP_APPLICATION_ICON_NAME, WIL_ICON, nullptr);
        ca_context_change_props(m_context, CA_PROP_EVENT_DESCRIPTION, WIL_FRIENDLY_NAME " Notification", nullptr);
    }

    Sound::~Sound()
    {
        ca_context_destroy(m_context);
    }

    void Sound::play(std::string_view name)
    {
        // This is simply the id we've picked which is somehow irrelevant
        constexpr auto const id = 3U;

        auto playing = 0;
        ca_context_playing(m_context, id, &playing);
        if (playing != CA_SUCCESS)
        {
            ca_context_cancel(m_context, id);
        }

        auto const ret = ca_context_play(m_context, id, CA_PROP_EVENT_ID, name.data(), nullptr);
        if (ret != CA_SUCCESS)
        {
            std::cerr << "Sound: Failed to play " << name << ": " << ca_strerror(ret) << std::endl;
        }
    }
}
