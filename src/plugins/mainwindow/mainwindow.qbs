import "../main.qbs" as PluginProduct

PluginProduct {
    name: "mainwindow"

    productname: "Главное окно"
    version:     "1.0.0"
    description: "MainWindow Plugin"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines:          base.concat(["MAINWINDOW_LIBRARY"])
    cpp.includePaths:     base.concat(['.'])

    Depends { name: "Qt"; submodules: ["widgets"] }
    Depends { name: "plugin" }
    Depends { name: "mdiextarea" }
    Depends { name: "pushbuttonimage" }

    files: [
        "actiongroupmodel.h",
        "actiongrouptreeview.h",
        "actionprop.h",
        "hotkeydelegate.h",
        "hotkeylineedit.h",
        "iconsizespinbox.h",
        "mainwindow.h",
        "mainwindowglobal.h",
        "mainwindowoptions.h",
        "mdisubwindow.h",
        "menu.h",
        "menubar.h",
        "mimedataobject.h",
        "toolbar.h",
        "toolbarmodel.h",
        "toolbarprop.h",
        "version.h",
        "actiongroupmodel.cpp",
        "actiongrouptreeview.cpp",
        "actionprop.cpp",
        "hotkeydelegate.cpp",
        "hotkeylineedit.cpp",
        "iconsizespinbox.cpp",
        "mainwindow.cpp",
        "mainwindowoptions.cpp",
        "mdisubwindow.cpp",
        "menu.cpp",
        "menubar.cpp",
        "mimedataobject.cpp",
        "toolbar.cpp",
        "toolbarmodel.cpp",
        "toolbarprop.cpp",
        "actionprop.ui",
        "mainwindow.ui",
        "mainwindowoptions.ui",
        "toolbarprop.ui",
        "mainwindow.qrc"
    ]
}
