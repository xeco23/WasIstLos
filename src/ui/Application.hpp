#pragma once

#include <gtkmm/application.h>
#include "MainWindow.hpp"

namespace wil::ui
{
    class Application : public Gtk::Application
    {
        public:
            static Application& getInstance();

        public:
            Application(int argc, char** argv);

        public:
            void keepAlive();
            void endKeepAlive();

        protected:
            virtual void on_startup() override;
            virtual void on_activate() override;
            virtual void on_open(type_vec_files const& files, Glib::ustring const&) override;

        private:
            static Application* m_instance;

        private:
            bool                        m_onHold;
            std::unique_ptr<MainWindow> m_mainWindow;
    };
}
