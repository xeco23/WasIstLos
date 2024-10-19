#pragma once

#include <gtkmm/builder.h>
#include <gtkmm/window.h>
#include <gtkmm/switch.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/spinbutton.h>
#include "TrayIcon.hpp"
#include "WebView.hpp"

namespace wil::ui
{
    class PreferencesWindow : public Gtk::Window
    {
        public:
            PreferencesWindow(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& refBuilder, TrayIcon& trayIcon, WebView& webView);

        private:
            bool onCloseToTrayChanged(bool state);
            bool onStartInTrayChanged(bool state) const;
            bool onStartMinimizedChanged(bool state) const;
            bool onAutostartChanged(bool state) const;
            bool onNotificationSoundsChanged(bool state) const;
            bool onPreferDarkThemeChanged(bool state) const;
            bool onAllowPermissionsChanged(bool state) const;
            void onHwAccelChanged() const;
            void onMinFontSizeChanged(Gtk::SpinButton* spinButtonMinFontSize) const;

        private:
            TrayIcon*          m_trayIcon;
            WebView*           m_webView;
            Gtk::Switch*       m_switchStartInTray;
            Gtk::Switch*       m_switchStartMinimized;
            Gtk::Switch*       m_switchNotificationSounds;
            Gtk::ComboBoxText* m_comboboxHwAccel;
    };
}
