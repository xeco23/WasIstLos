#include "PreferencesWindow.hpp"
#include <glibmm/i18n.h>
#include <gtkmm/settings.h>
#include "../util/Settings.hpp"

namespace wil::ui
{
    PreferencesWindow::PreferencesWindow(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& refBuilder, TrayIcon& trayIcon, WebView& webView)
        : Gtk::Window{cobject}
        , m_trayIcon{&trayIcon}
        , m_webView{&webView}
        , m_switchStartInTray{nullptr}
        , m_switchStartMinimized{nullptr}
        , m_switchNotificationSounds{nullptr}
        , m_comboboxHwAccel{nullptr}
    {
        Gtk::Switch* switchCloseToTray = nullptr;
        refBuilder->get_widget("switch_close_to_tray", switchCloseToTray);
        switchCloseToTray->signal_state_set().connect(sigc::mem_fun(*this, &PreferencesWindow::onCloseToTrayChanged), false);

        refBuilder->get_widget("switch_start_in_tray", m_switchStartInTray);
        m_switchStartInTray->signal_state_set().connect(sigc::mem_fun(*this, &PreferencesWindow::onStartInTrayChanged), false);

        refBuilder->get_widget("switch_start_minimized", m_switchStartMinimized);
        m_switchStartMinimized->signal_state_set().connect(sigc::mem_fun(*this, &PreferencesWindow::onStartMinimizedChanged), false);

        Gtk::Switch* switchAutostart = nullptr;
        refBuilder->get_widget("switch_autostart", switchAutostart);
        switchAutostart->signal_state_set().connect(sigc::mem_fun(*this, &PreferencesWindow::onAutostartChanged), false);

        refBuilder->get_widget("switch_notification_sounds", m_switchNotificationSounds);
        m_switchNotificationSounds->signal_state_set().connect(sigc::mem_fun(*this, &PreferencesWindow::onNotificationSoundsChanged), false);

        Gtk::Switch* switchPreferDarkTheme = nullptr;
        refBuilder->get_widget("switch_prefer_dark_theme", switchPreferDarkTheme);
        switchPreferDarkTheme->signal_state_set().connect(sigc::mem_fun(*this, &PreferencesWindow::onPreferDarkThemeChanged), false);

        refBuilder->get_widget("combobox_hw_accel", m_comboboxHwAccel);
        m_comboboxHwAccel->signal_changed().connect(sigc::mem_fun(*this, &PreferencesWindow::onHwAccelChanged));
        m_comboboxHwAccel->append(_("On Demand"));
        m_comboboxHwAccel->append(_("Always"));
        m_comboboxHwAccel->append(_("Never"));

        Gtk::Switch* switchAllowPermissions = nullptr;
        refBuilder->get_widget("switch_allow_permissions", switchAllowPermissions);
        switchAllowPermissions->signal_state_set().connect(sigc::mem_fun(*this, &PreferencesWindow::onAllowPermissionsChanged), false);

        Gtk::SpinButton* spinMinFontSize = nullptr;
        refBuilder->get_widget("spinbutton_min_font_size", spinMinFontSize);
        spinMinFontSize->signal_value_changed().connect(sigc::bind(sigc::mem_fun(*this, &PreferencesWindow::onMinFontSizeChanged), spinMinFontSize));

        switchCloseToTray->set_state(m_trayIcon->isVisible());
        m_switchStartInTray->set_state(util::Settings::getInstance().getValue<bool>("general", "start-in-tray") && m_trayIcon->isVisible());
        m_switchStartInTray->set_sensitive(m_trayIcon->isVisible());
        m_switchStartMinimized->set_state(util::Settings::getInstance().getValue<bool>("general", "start-minimized"));
        switchAutostart->set_state(util::Settings::getInstance().getValue<bool>("general", "autostart"));
        m_switchNotificationSounds->set_state(util::Settings::getInstance().getValue<bool>("general", "notification-sounds", true));
        switchPreferDarkTheme->set_state(util::Settings::getInstance().getValue<bool>("appearance", "prefer-dark-theme"));
        m_comboboxHwAccel->set_active(util::Settings::getInstance().getValue<int>("web", "hw-accel", 1));
        switchAllowPermissions->set_state(util::Settings::getInstance().getValue<bool>("web", "allow-permissions"));
        spinMinFontSize->set_value(util::Settings::getInstance().getValue<int>("web", "min-font-size", 0));
    }

    bool PreferencesWindow::onCloseToTrayChanged(bool state)
    {
        m_trayIcon->setVisible(state);
        util::Settings::getInstance().setValue("general", "close-to-tray", state);

        if (!state)
        {
            m_switchStartInTray->set_state(false);
        }
        m_switchStartInTray->set_sensitive(state);

        return false;
    }

    bool PreferencesWindow::onStartInTrayChanged(bool state) const
    {
        util::Settings::getInstance().setValue("general", "start-in-tray", state);

        if (state)
        {
            m_switchStartMinimized->set_state(false);
        }
        m_switchStartMinimized->set_sensitive(!state);

        return false;
    }

    bool PreferencesWindow::onStartMinimizedChanged(bool state) const
    {
        util::Settings::getInstance().setValue("general", "start-minimized", state);

        if (state)
        {
            m_switchStartInTray->set_state(false);
        }
        m_switchStartInTray->set_sensitive(!state);

        return false;
    }

    bool PreferencesWindow::onAutostartChanged(bool state) const
    {
        util::Settings::getInstance().setValue("general", "autostart", state);

        return false;
    }

    bool PreferencesWindow::onNotificationSoundsChanged(bool state) const
    {
        util::Settings::getInstance().setValue("general", "notification-sounds", state);

        return false;
    }

    bool PreferencesWindow::onPreferDarkThemeChanged(bool state) const
    {
        auto const settings = Gtk::Settings::get_default();
        settings->property_gtk_application_prefer_dark_theme().set_value(state);

        util::Settings::getInstance().setValue("appearance", "prefer-dark-theme", state);

        return false;
    }

    bool PreferencesWindow::onAllowPermissionsChanged(bool state) const
    {
        util::Settings::getInstance().setValue("web", "allow-permissions", state);

        return false;
    }

    void PreferencesWindow::onHwAccelChanged() const
    {
        auto active = m_comboboxHwAccel->get_active_row_number();
        m_webView->setHwAccelPolicy(static_cast<WebKitHardwareAccelerationPolicy>(active));
        util::Settings::getInstance().setValue("web", "hw-accel", active);
    }

    void PreferencesWindow::onMinFontSizeChanged(Gtk::SpinButton* spinButtonMinFontSize) const
    {
        auto const fontSize = static_cast<int>(spinButtonMinFontSize->get_value());
        m_webView->setMinFontSize(fontSize);
        util::Settings::getInstance().setValue("web", "min-font-size", fontSize);
    }
}
