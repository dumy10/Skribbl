#include "ChatManager.h"
#include "utils.h"
#include <QScrollBar>

ChatManager::ChatManager(QPlainTextEdit* chatDisplay, QTextEdit* chatInput) noexcept
    : m_chatDisplay(chatDisplay)
    , m_chatInput(chatInput)
{
}

void ChatManager::UpdateChat(const std::string& encodedChat) noexcept
{
    if (encodedChat.empty() || !m_chatDisplay) {
        return;
    }

    std::string decodedString = urlDecode(encodedChat);
    QString chat = QString::fromUtf8(decodedString.data(), static_cast<int>(decodedString.size()));
    
    m_chatDisplay->setPlainText(chat);
    ScrollToBottom();
}

void ChatManager::ClearInput() noexcept
{
    if (m_chatInput) {
        m_chatInput->clear();
    }
}

void ChatManager::SetInputReadOnly(bool readOnly) noexcept
{
    if (m_chatInput) {
        m_chatInput->setReadOnly(readOnly);
    }
}

QString ChatManager::GetInputText() const noexcept
{
    return m_chatInput ? m_chatInput->toPlainText() : QString();
}

void ChatManager::ScrollToBottom() noexcept
{
    if (m_chatDisplay && m_chatDisplay->verticalScrollBar()) {
        m_chatDisplay->verticalScrollBar()->setValue(m_chatDisplay->verticalScrollBar()->maximum());
    }
}
