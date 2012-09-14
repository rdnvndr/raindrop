#ifndef PLUGINVIEWDLG_H
#define PLUGINVIEWDLG_H

#include "ui_pluginviewdlg.h"
#include "pluginmodel.h"
#include "common.h"
#include "iplugin.h"

#include <QStringListModel>
#include <QPointer>

class PluginViewDlg : public QDialog, private Ui::PluginViewDlg
{
    Q_OBJECT

public:
    explicit PluginViewDlg(QWidget *parent = 0);
    ~PluginViewDlg();

    void setModel(PluginModel *model);
    void cleanUserInterface();
    void fillUserInterface(IPlugin *plugin);
private slots:
    void currentItemChanged(QModelIndex index);

private:
    QPointer<QStringListModel> m_dependModel;
    QPointer<QStringListModel> m_clientModel;
};

#endif // PLUGINVIEWDLG_H
