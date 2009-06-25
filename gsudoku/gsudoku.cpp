#include "config.h"
#include "gettext.h"
#include <iostream>
#include "mainwindow.hpp"

#define _(S) gettext(S)

int main(int argc, char *argv[])
{
    setlocale (LC_ALL, "");
    bindtextdomain (PACKAGE, LOCALEDIR);
    textdomain (PACKAGE);

    Gtk::Main m(argc, argv);
    MainWindow window;
    Gtk::Main::run(window);

    return 0;
}
