#ifndef APPMAIN_HPP
#define APPMAIN_HPP

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class AppMain; }
QT_END_NAMESPACE

class AppMain : public QMainWindow {
Q_OBJECT

public:
    explicit AppMain(QWidget *parent = nullptr);
    ~AppMain() override;

private:
    Ui::AppMain *ui;
};


#endif //APPMAIN_HPP
