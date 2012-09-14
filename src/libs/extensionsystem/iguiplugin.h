#ifndef IGUIPLUGIN_H
#define IGUIPLUGIN_H

#include "extensionsystemglobal.h"
#include "iplugin.h"
#include <QtGui>

class  EXTENSIONSYSTEMSHARED_EXPORT IGuiPlugin : public IPlugin
{
    Q_OBJECT
    Q_INTERFACES(IPlugin)
public:
    IGuiPlugin(QObject *parent = 0);
    virtual ~IGuiPlugin();

    QIcon icon();
    void setIcon(QIcon icon);
signals:

public slots:

private:
    QIcon m_pluginIcon;
};

Q_DECLARE_INTERFACE(IGuiPlugin, "com.RTPTechGroup.UniversalPlatform.IGuiPlugin/1.0")
#endif // IGUIPLUGIN_H
