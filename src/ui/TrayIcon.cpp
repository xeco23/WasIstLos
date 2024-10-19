#include "TrayIcon.hpp"
#include <utility>
#include <glibmm/i18n.h>
#include <gtkmm/icontheme.h>
#include "Config.hpp"

namespace wil::ui
{
    namespace
    {
        std::pair<char const*, char const*> getTrayIconNames()
        {
            constexpr auto WHATSAPP_TRAY            = "whatsapp-tray";
            constexpr auto WHATSAPP_TRAY_ATTENTION  = "whatsapp-tray-attention";
            constexpr auto WASISTLOS_TRAY           = WIL_ICON "-tray";
            constexpr auto WASISTLOS_TRAY_ATTENTION = WIL_ICON "-tray-attention";

            auto const iconTheme = Gtk::IconTheme::get_default();
            if (iconTheme->has_icon(WHATSAPP_TRAY) && iconTheme->has_icon(WHATSAPP_TRAY_ATTENTION))
            {
                return {WHATSAPP_TRAY, WHATSAPP_TRAY_ATTENTION};
            }
            else if (iconTheme->has_icon(WASISTLOS_TRAY) && iconTheme->has_icon(WASISTLOS_TRAY_ATTENTION))
            {
                return {WASISTLOS_TRAY, WASISTLOS_TRAY_ATTENTION};
            }
            else
            {
                return {"indicator-messages", "indicator-messages-new"};
            }
        }
    }

    TrayIcon::TrayIcon()
        : m_appIndicator{app_indicator_new(WIL_APP_ID ".Tray", "", APP_INDICATOR_CATEGORY_COMMUNICATIONS)}
        , m_popupMenu{}
        , m_signalShow{}
        , m_signalAbout{}
        , m_signalQuit{}
    {
        auto const [trayIconName, attentionIconName] = getTrayIconNames();
        app_indicator_set_icon_full(m_appIndicator, trayIconName, WIL_FRIENDLY_NAME " Tray");
        app_indicator_set_attention_icon_full(m_appIndicator, attentionIconName, WIL_FRIENDLY_NAME "Tray Attention");

        auto const showMenuItem  = Gtk::manage(new Gtk::MenuItem{_("Show")});
        auto const aboutMenuItem = Gtk::manage(new Gtk::MenuItem{_("About")});
        auto const quitMenuItem  = Gtk::manage(new Gtk::MenuItem{_("Quit")});
        m_popupMenu.append(*showMenuItem);
        m_popupMenu.append(*aboutMenuItem);
        m_popupMenu.append(*quitMenuItem);

        app_indicator_set_menu(m_appIndicator, m_popupMenu.gobj());

        showMenuItem->signal_activate().connect([this] { m_signalShow.emit(); });
        aboutMenuItem->signal_activate().connect([this] { m_signalAbout.emit(); });
        quitMenuItem->signal_activate().connect([this] { m_signalQuit.emit(); });

        m_popupMenu.show_all();

        app_indicator_set_status(m_appIndicator, APP_INDICATOR_STATUS_PASSIVE);
        app_indicator_set_menu(m_appIndicator, m_popupMenu.gobj());
    }

    void TrayIcon::setVisible(bool visible)
    {
        app_indicator_set_status(m_appIndicator, (visible ? APP_INDICATOR_STATUS_ACTIVE : APP_INDICATOR_STATUS_PASSIVE));
    }

    bool TrayIcon::isVisible() const
    {
        return (app_indicator_get_status(m_appIndicator) != APP_INDICATOR_STATUS_PASSIVE);
    }

    void TrayIcon::setAttention(bool attention)
    {
        if (!isVisible())
        {
            return;
        }

        app_indicator_set_status(m_appIndicator, (attention ? APP_INDICATOR_STATUS_ATTENTION : APP_INDICATOR_STATUS_ACTIVE));
    }

    sigc::signal<void>& TrayIcon::signalShow() noexcept
    {
        return m_signalShow;
    }

    sigc::signal<void>& TrayIcon::signalAbout() noexcept
    {
        return m_signalAbout;
    }

    sigc::signal<void>& TrayIcon::signalQuit() noexcept
    {
        return m_signalQuit;
    }
}
