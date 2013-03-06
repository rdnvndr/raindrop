#ifndef HOTKEYLINEEDIT_H
#define HOTKEYLINEEDIT_H

#include <QLineEdit>

//! Класс ввода горячих клавиш
/*!  Класс ввода горячих клавиш предназначен для ввода клавиатурных сокращений
     с клавиаьуры
 */

class HotKeyLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    //! Конструктор
    explicit HotKeyLineEdit(QWidget *parent = 0);

    //! Получение введенной горячей клавиши
    const QKeySequence& keySequence() const;

    //! Установка горячей клавиши по умолчанию
    void setKeySequence(const QKeySequence& keySequence);

    //! Сброс горячей клавиши
    void resetKeySequence();

signals:

    //! Сигнал изменения горячей клавиши
    void changeHotKey(QKeySequence keySequence);

protected:

    //!  Обработчик события нажатия клавиш
    virtual void keyPressEvent(QKeyEvent* event);

    //! Обработчик события при показе QWidget
    virtual void showEvent(QShowEvent* event);
    
private slots:
    //! Слот обработки изменения текста
    void onTextChanged(const QString &);

private:

    //! Введенная горячая клавиша
    QKeySequence m_keySequence;

    //! Горячая клавиша по умолчанию
    QKeySequence m_oldKeySequence;
};

#endif // HOTKEYLINEEDIT_H
