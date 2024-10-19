#pragma once

#include <libayatana-appindicator/app-indicator.h>
#include <gtkmm/menu.h>

namespace wil::ui
{
    class TrayIcon
    {
        public:
            TrayIcon();

        public:
            void setVisible(bool visible);
            bool isVisible() const;
            void setAttention(bool attention);

        public:
            sigc::signal<void>& signalShow() noexcept;
            sigc::signal<void>& signalAbout() noexcept;
            sigc::signal<void>& signalQuit() noexcept;

        private:
            AppIndicator*      m_appIndicator;
            Gtk::Menu          m_popupMenu;
            sigc::signal<void> m_signalShow;
            sigc::signal<void> m_signalAbout;
            sigc::signal<void> m_signalQuit;
    };
}
