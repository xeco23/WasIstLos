#include "PhoneNumberDialog.hpp"

namespace wil::ui
{
    PhoneNumberDialog::PhoneNumberDialog(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& refBuilder)
        : Gtk::Dialog{cobject}
        , m_buttonOk{nullptr}
        , m_entryPhoneNumber{nullptr}
    {
        refBuilder->get_widget("button_ok", m_buttonOk);

        refBuilder->get_widget("entry_phone_number", m_entryPhoneNumber);
        m_entryPhoneNumber->signal_changed().connect(sigc::mem_fun(*this, &PhoneNumberDialog::onTextChange));

        signal_show().connect(sigc::mem_fun(*this, &PhoneNumberDialog::onShow));
        signal_response().connect(sigc::hide(sigc::mem_fun(*this, &PhoneNumberDialog::hide)));

        onTextChange();
    }

    void PhoneNumberDialog::onShow()
    {
        m_entryPhoneNumber->set_text({});
    }

    void PhoneNumberDialog::onTextChange()
    {
        auto const text = m_entryPhoneNumber->get_text();

        m_buttonOk->set_sensitive(!text.empty() && std::all_of(text.begin(), text.end(), [](unsigned char c) { return std::isdigit(c); }));
    }

    Glib::ustring PhoneNumberDialog::getPhoneNumber()
    {
        return m_entryPhoneNumber->get_text();
    }
}
