#include "MainWindow.hpp"
#include <glibmm/i18n.h>
#include <gtkmm/grid.h>
#include <gtkmm/button.h>
#include <gtkmm/modelbutton.h>
#include <gtkmm/aboutdialog.h>
#include <gtkmm/shortcutswindow.h>
#include "Application.hpp"
#include "Config.hpp"
#include "../util/Settings.hpp"

namespace wfl::ui
{
    MainWindow::MainWindow(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& refBuilder)
        : Gtk::ApplicationWindow{cobject}
        , m_trayIcon{}
        , m_webView{}
        , m_sound{}
        , m_pendingUrl{}
        , m_preferencesWindow{nullptr}
        , m_phoneNumberDialog{nullptr}
        , m_headerBar{nullptr}
        , m_shortcutsWindow{nullptr}
        , m_buttonZoomLevel{nullptr}
        , m_fullscreen{false}
    {
        auto const appIcon16x16   = Gdk::Pixbuf::create_from_resource("/main/image/icons/hicolor/16x16/apps/" WFL_ICON ".png");
        auto const appIcon32x32   = Gdk::Pixbuf::create_from_resource("/main/image/icons/hicolor/32x32/apps/" WFL_ICON ".png");
        auto const appIcon64x64   = Gdk::Pixbuf::create_from_resource("/main/image/icons/hicolor/64x64/apps/" WFL_ICON ".png");
        auto const appIcon128x128 = Gdk::Pixbuf::create_from_resource("/main/image/icons/hicolor/128x128/apps/" WFL_ICON ".png");
        set_icon_list({appIcon16x16, appIcon32x32, appIcon64x64, appIcon128x128});
        set_default_icon(appIcon64x64);

        Gtk::Grid* gridMain = nullptr;
        refBuilder->get_widget("grid_main", gridMain);
        gridMain->attach(m_webView, 0, 1, 1, 1);

        Gtk::Button* buttonOpenPhoneNumber = nullptr;
        refBuilder->get_widget("button_open_phone_number", buttonOpenPhoneNumber);
        buttonOpenPhoneNumber->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onOpenPhoneNumber));

        Gtk::Button* buttonRefresh = nullptr;
        refBuilder->get_widget("button_refresh", buttonRefresh);
        buttonRefresh->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onRefresh));

        refBuilder->get_widget("headerbar", m_headerBar);

        Gtk::ModelButton* buttonFullscreen = nullptr;
        refBuilder->get_widget("button_fullscreen", buttonFullscreen);
        buttonFullscreen->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onFullscreen));

        refBuilder->get_widget("button_zoom_level", m_buttonZoomLevel);
        m_buttonZoomLevel->set_label(m_webView.getZoomLevelString());
        m_buttonZoomLevel->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onResetZoom));

        Gtk::Button* buttonZoomIn = nullptr;
        refBuilder->get_widget("button_zoom_in", buttonZoomIn);
        buttonZoomIn->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onZoomIn));

        Gtk::Button* buttonZoomOut = nullptr;
        refBuilder->get_widget("button_zoom_out", buttonZoomOut);
        buttonZoomOut->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onZoomOut));

        Gtk::ModelButton* buttonPreferences = nullptr;
        refBuilder->get_widget("button_preferences", buttonPreferences);
        buttonPreferences->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onOpenPreferences));

        Gtk::ModelButton* buttonShortcuts = nullptr;
        refBuilder->get_widget("button_shortcuts", buttonShortcuts);
        buttonShortcuts->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onShortcuts));

        Gtk::ModelButton* buttonAbout = nullptr;
        refBuilder->get_widget("button_about", buttonAbout);
        buttonAbout->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onAbout));

        Gtk::ModelButton* buttonQuit = nullptr;
        refBuilder->get_widget("button_quit", buttonQuit);
        buttonQuit->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onQuit));

        m_webView.signalLoadStatus().connect(sigc::mem_fun(*this, &MainWindow::onLoadStatusChanged));
        m_webView.signalNotification().connect(sigc::mem_fun(*this, &MainWindow::onNotificationChanged));
        m_webView.signalNotificationClicked().connect(sigc::mem_fun(*this, &MainWindow::onShow));
        m_trayIcon.signalShow().connect(sigc::mem_fun(*this, &MainWindow::onShow));
        m_trayIcon.signalAbout().connect(sigc::mem_fun(*this, &MainWindow::onAbout));
        m_trayIcon.signalQuit().connect(sigc::mem_fun(*this, &MainWindow::onQuit));

        show_all();

        m_trayIcon.setVisible(util::Settings::getInstance().getValue<bool>("general", "close-to-tray"));
        m_headerBar->set_visible(util::Settings::getInstance().getValue<bool>("general", "header-bar", true));
    }

    void MainWindow::openUrl(std::string const& url)
    {
        if (m_webView.getLoadStatus() == WEBKIT_LOAD_FINISHED)
        {
            m_webView.sendRequest(url);
        }
        else
        {
            m_pendingUrl = url;
        }
    }

    bool MainWindow::on_key_press_event(GdkEventKey* keyEvent)
    {
        if (keyEvent->state & GDK_CONTROL_MASK)
        {
            switch (keyEvent->keyval)
            {
                case GDK_KEY_P:
                case GDK_KEY_p:
                    onOpenPreferences();
                    return true;

                case GDK_KEY_Q:
                case GDK_KEY_q:
                    onQuit();
                    return true;

                case GDK_KEY_question:
                    onShortcuts();
                    return true;

                case GDK_KEY_plus:
                    onZoomIn();
                    return true;

                case GDK_KEY_minus:
                    onZoomOut();
                    return true;

                default:
                    break;
            }
        }
        else if (keyEvent->state & GDK_MOD1_MASK)
        {
            switch (keyEvent->keyval)
            {
                case GDK_KEY_H:
                case GDK_KEY_h:
                {
                    auto const visible = !m_headerBar->is_visible();
                    m_headerBar->set_visible(visible);
                    util::Settings::getInstance().setValue("general", "header-bar", visible);
                    return true;
                }

                default:
                    break;
            }
        }
        else
        {
            switch (keyEvent->keyval)
            {
                case GDK_KEY_F11:
                    onFullscreen();
                    return true;

                default:
                    break;
            }
        }

        // This is required for keyboard shortcuts on WhatsApp Web to work. Since it is running
        // on WebKit, WhatsApp Web listens for Mac's [CMD] key instead of the standard [ALT] key.
        if (keyEvent->state & GDK_MOD1_MASK)
        {
            keyEvent->state |= GDK_META_MASK;
        }

        return Gtk::ApplicationWindow::on_key_press_event(keyEvent);
    }

    bool MainWindow::on_scroll_event(GdkEventScroll* scrollEvent)
    {
        if (scrollEvent->state & GDK_CONTROL_MASK)
        {
            switch (scrollEvent->direction)
            {
                case GDK_SCROLL_UP:
                case GDK_SCROLL_RIGHT:
                    onZoomIn();
                    return true;

                case GDK_SCROLL_DOWN:
                case GDK_SCROLL_LEFT:
                    onZoomOut();
                    return true;

                default:
                    break;
            }
        }

        return Gtk::ApplicationWindow::on_scroll_event(scrollEvent);
    }

    bool MainWindow::on_window_state_event(GdkEventWindowState* windowStateEvent)
    {
        m_fullscreen = (windowStateEvent->new_window_state & GDK_WINDOW_STATE_FULLSCREEN);

        return Gtk::ApplicationWindow::on_window_state_event(windowStateEvent);
    }

    bool MainWindow::on_delete_event(GdkEventAny*)
    {
        if (m_trayIcon.isVisible())
        {
            Application::getInstance().keepAlive();
            hide();
        }
        else
        {
            Application::getInstance().endKeepAlive();
        }

        return false;
    }

    void MainWindow::onRefresh()
    {
        m_webView.refresh();
    }

    void MainWindow::onLoadStatusChanged(WebKitLoadEvent loadEvent)
    {
        if ((loadEvent == WEBKIT_LOAD_FINISHED) && !m_pendingUrl.empty())
        {
            m_webView.sendRequest(m_pendingUrl);
            m_pendingUrl.clear();
        }
    }

    void MainWindow::onOpenPreferences()
    {
        if (!m_preferencesWindow)
        {
            auto const refBuilder = Gtk::Builder::create_from_resource("/main/ui/PreferencesWindow.ui");
            refBuilder->get_widget_derived("window_preferences", m_preferencesWindow, m_trayIcon, m_webView);
        }

        m_preferencesWindow->set_transient_for(*this);
        m_preferencesWindow->show_all();
    }

    void MainWindow::onOpenPhoneNumber()
    {
        if (!m_phoneNumberDialog)
        {
            auto const refBuilder = Gtk::Builder::create_from_resource("/main/ui/PhoneNumberDialog.ui");
            refBuilder->get_widget_derived("phone_number_dialog", m_phoneNumberDialog);

            m_phoneNumberDialog->signal_response().connect(sigc::mem_fun(*this, &MainWindow::onPhoneNumberDialogResponse));
        }

        m_phoneNumberDialog->set_transient_for(*this);
        m_phoneNumberDialog->show_all();
    }

    void MainWindow::onPhoneNumberDialogResponse(int responseId)
    {
        if (responseId == Gtk::ResponseType::RESPONSE_OK)
        {
            auto const phoneNumber = m_phoneNumberDialog->getPhoneNumber();
            m_webView.openPhoneNumber(phoneNumber);
        }
    }

    void MainWindow::onNotificationChanged(bool show)
    {
        if (!is_visible())
        {
            m_trayIcon.setAttention(show);
        }

        if (show && util::Settings::getInstance().getValue<bool>("general", "notification-sounds", true))
        {
            m_sound.play("message-new-instant");
        }
    }

    void MainWindow::onShow()
    {
        if (!is_visible())
        {
            m_trayIcon.setAttention(false);
        }

        present();
    }

    void MainWindow::onQuit()
    {
        if (m_trayIcon.isVisible())
        {
            m_trayIcon.setVisible(false);
        }
        close();
    }

    void MainWindow::onFullscreen()
    {
        m_fullscreen ? unfullscreen() : fullscreen();
    }

    void MainWindow::onZoomIn()
    {
        m_webView.zoomIn();
        m_buttonZoomLevel->set_label(m_webView.getZoomLevelString());
    }

    void MainWindow::onZoomOut()
    {
        m_webView.zoomOut();
        m_buttonZoomLevel->set_label(m_webView.getZoomLevelString());
    }

    void MainWindow::onResetZoom()
    {
        m_webView.resetZoom();
        m_buttonZoomLevel->set_label(m_webView.getZoomLevelString());
    }

    void MainWindow::onShortcuts()
    {
        if (!m_shortcutsWindow)
        {
            auto const refBuilder = Gtk::Builder::create_from_resource("/main/ui/ShortcutsWindow.ui");
            refBuilder->get_widget("shortcuts_window", m_shortcutsWindow);
        }

        m_shortcutsWindow->set_transient_for(*this);
        m_shortcutsWindow->show_all();
    }

    void MainWindow::onAbout()
    {
        auto aboutDialog = Gtk::AboutDialog{};

        aboutDialog.set_title(_("About"));
        aboutDialog.set_version(WFL_VERSION);
        aboutDialog.set_program_name(_("WasIstLos"));
        aboutDialog.set_comments(_("An unofficial WhatsApp desktop application for Linux"));
        aboutDialog.set_website(WFL_HOMEPAGE);
        aboutDialog.set_website_label(_("GitHub Repository"));
        aboutDialog.set_license_type(Gtk::LICENSE_GPL_3_0);

        aboutDialog.set_transient_for(*this);
        aboutDialog.run();
    }
}
