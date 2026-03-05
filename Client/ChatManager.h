#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include <QPlainTextEdit>
#include <QTextEdit>
#include <QString>
#include <string>

/**
 * @brief Manages chat functionality including display and input handling.
 * 
 * This class handles chat message display, scrolling, and provides
 * methods to update chat content and manage text input readability.
 */
class ChatManager 
{
public:
    explicit ChatManager(QPlainTextEdit* chatDisplay, QTextEdit* chatInput) noexcept;

    // Chat operations
    void UpdateChat(const std::string& encodedChat) noexcept;
    void ClearInput() noexcept;
    void SetInputReadOnly(bool readOnly) noexcept;
    [[nodiscard]] QString GetInputText() const noexcept;

private:
    void ScrollToBottom() noexcept;

private:
    QPlainTextEdit* m_chatDisplay;
    QTextEdit* m_chatInput;
};

#endif // CHATMANAGER_H
