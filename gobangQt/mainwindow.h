#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPainter>
#include <QMessageBox>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    int board[15][15];
    int currentPlayer;
    int hoverCol, hoverRow;
    bool hover;
    int clickCol, clickRow;
    bool click;
    enum Mode{PVP, PVE} mode;
    int turnPVP;
    bool AI_on;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_PVE_clicked();

    void on_PVP_clicked();

    void on_back_clicked();

    void on_offensive_clicked();

    void on_defensive_clicked();

    void on_change_clicked();

    void on_givein_clicked();

    void on_retract_clicked();

    void on_restart_clicked();

private:
    Ui::MainWindow *ui;

    void paintEvent(QPaintEvent *event);

    void mouseMoveEvent(QMouseEvent* event);

    void mouseReleaseEvent(QMouseEvent* event);

    void refresh();

    bool judge(int x, int y, int color);

    void showWin(int color);

    void initGame();

    int get_random_under_three();

};
#endif // MAINWINDOW_H
