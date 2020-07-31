#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "heuristic.h"
#include "chat.h"
const int BOARD_GRAD_SIZE = 14;
const int MARGIN = 60;
const int MARK_SIZE = 6;
const int BLOCK_SIZE = 40;
const int CHESS_SIZE = BLOCK_SIZE * 0.8;
const int HEAD_HEIGHT = 100;
const int BOARD_SIZE = 620;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAutoFillBackground(true);
    setPalette(QPalette(QColor(231,254,245)));
    ui->offensive->hide();
    ui->defensive->hide();
    ui->back->hide();
    ui->change->hide();
    ui->givein->hide();
    ui->retract->hide();
    ui->restart->hide();
    ui->broadcast->setText("");
    srand(1);
    ui->chatBox->setText(before_game_start[get_random_under_three()]);
    setMouseTracking(true);
    setFixedSize(
        MARGIN * 2 + BLOCK_SIZE * BOARD_GRAD_SIZE + 200,
        MARGIN * 2 + BLOCK_SIZE * BOARD_GRAD_SIZE + HEAD_HEIGHT);
    hover = click = AI_on = false;
    currentPlayer = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor(245,222,179));
    painter.setBrush(brush);
    painter.drawRect(MARGIN / 2, MARGIN / 2 + HEAD_HEIGHT,
                     BOARD_SIZE, BOARD_SIZE);
    painter.setRenderHint(QPainter::Antialiasing, true);
    for (int i = 0; i <= BOARD_GRAD_SIZE; ++i)
    {
        painter.drawLine(MARGIN + BLOCK_SIZE * i, HEAD_HEIGHT + MARGIN,
                         MARGIN + BLOCK_SIZE * i, HEAD_HEIGHT + MARGIN + BOARD_GRAD_SIZE * BLOCK_SIZE);
        painter.drawLine(MARGIN, HEAD_HEIGHT + MARGIN + BLOCK_SIZE * i,
                         MARGIN + BOARD_GRAD_SIZE * BLOCK_SIZE, HEAD_HEIGHT + MARGIN + BLOCK_SIZE * i);
    }


    if (hover) {
        if (currentPlayer == 0)
            brush.setColor(Qt::black);
        else
            brush.setColor(Qt::white);
        painter.setBrush(brush);
        painter.drawRect(MARGIN + BLOCK_SIZE * hoverCol - MARK_SIZE / 2,
                     MARGIN + HEAD_HEIGHT + BLOCK_SIZE * hoverRow - MARK_SIZE / 2,
                     MARK_SIZE,
                     MARK_SIZE);
    }
    if (click) {
        for (int i = 0; i < 15; ++i)
            for (int j = 0; j < 15; ++j) {
                if (board[i][j] != -1) {
                    int x = MARGIN + i * BLOCK_SIZE - CHESS_SIZE / 2;
                    int y = MARGIN + HEAD_HEIGHT + j * BLOCK_SIZE - CHESS_SIZE / 2;
                    if (board[i][j] == 0)
                        brush.setColor(Qt::black);
                    else
                        brush.setColor(Qt::white);
                    painter.setBrush(brush);
                    painter.drawEllipse(x, y, CHESS_SIZE, CHESS_SIZE);
                }
            }
    } else if (mode == Mode::PVE and currentPlayer == ai_side) {
        for (int i = 0; i < 15; ++i)
            for (int j = 0; j < 15; ++j) {
                if (board[i][j] != -1) {
                    int x = MARGIN + i * BLOCK_SIZE - CHESS_SIZE / 2;
                    int y = MARGIN + HEAD_HEIGHT + j * BLOCK_SIZE - CHESS_SIZE / 2;
                    if (board[i][j] == 0)
                        brush.setColor(Qt::black);
                    else
                        brush.setColor(Qt::white);
                    painter.setBrush(brush);
                    painter.drawEllipse(x, y, CHESS_SIZE, CHESS_SIZE);
                }
            }
        currentPlayer = 1 - currentPlayer;
        click = hover = true;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    int x = event->x();
    int y = event->y();
    x -= MARGIN; y -= HEAD_HEIGHT + MARGIN;
    x += BLOCK_SIZE / 2; y += BLOCK_SIZE / 2;
    int col = x / BLOCK_SIZE;
    int row = y / BLOCK_SIZE;
    if (col < 15 and col >=0 and row < 15 and row >= 0 and board[col][row] == -1) {
        hoverCol = col; hoverRow = row;
        update();
    }

}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    int x = event->x();
    int y = event->y();
    x -= MARGIN; y -= HEAD_HEIGHT + MARGIN;
    x += BLOCK_SIZE / 2; y += BLOCK_SIZE / 2;
    int col = x / BLOCK_SIZE;
    int row = y / BLOCK_SIZE;
    if (col < 15 and col >=0 and row < 15 and row >= 0 and board[col][row] == -1) {
        ++turnPVP;
        board[col][row] = currentPlayer;
        clickCol = col; clickRow = row;
        update();
        if (judge(col, row, currentPlayer)) return;
        if (mode == Mode::PVP) {
            if (currentPlayer == 0) {
                ui->broadcast->insertPlainText("黑");
            } else {
                ui->broadcast->insertPlainText("白");
            }
            ui->broadcast->insertPlainText("方在(");
            ui->broadcast->insertPlainText(tr("%1").arg(clickCol));
            ui->broadcast->insertPlainText(", ");
            ui->broadcast->insertPlainText(tr("%1").arg(clickRow));
            ui->broadcast->insertPlainText("）处落子\n");
        } else {
            ui->broadcast->insertPlainText("你在（");
            ui->broadcast->insertPlainText(tr("%1").arg(clickCol));
            ui->broadcast->insertPlainText(", ");
            ui->broadcast->insertPlainText(tr("%1").arg(clickRow));
            ui->broadcast->insertPlainText(")处落子\n");
        }
        currentPlayer = 1 - currentPlayer;
        if (mode == Mode::PVP and turnPVP == 3) {
            ui->change->show();
        } else ui->change->hide();
        if (mode == Mode::PVE and currentPlayer == ai_side) {
            std::pair<int, int> AIaction = action(std::make_pair(row, col));
            int x = AIaction.first, y = AIaction.second;
            board[y][x] = ai_side;
            if (judge(y, x, ai_side)) return;
            currentPlayer = 1 - currentPlayer;
            ui->broadcast->insertPlainText("AI在(");
            ui->broadcast->insertPlainText(tr("%1").arg(y));
            ui->broadcast->insertPlainText(", ");
            ui->broadcast->insertPlainText(tr("%1").arg(x));
            ui->broadcast->insertPlainText(")处落子\n");
            update();
            if (turnPVE == 2 and ai_side == 0) {
                ui->change->show();
            } else ui->change->hide();
        }
        if (mode == Mode::PVP) {
            if (turnPVP <= 8) {
                ui->chatBox->setText(PVPopening[get_random_under_three()]);
            } else {
                ui->chatBox->setText(PVPchat[get_random_under_three()]);
            }
        } else if (mode == Mode::PVE) {
            if (turnPVE <= 3) {
                ui->chatBox->setText(first_three_turn[get_random_under_three()]);
            } else if (score >= 3000) {
                ui->chatBox->setText(advance_a_lot[get_random_under_three()]);
            } else if (score >= 1000) {
                ui->chatBox->setText(advance_a_little[get_random_under_three()]);
            } else if(score >= -1000) {
                ui->chatBox->setText(draw[get_random_under_three()]);
            } else if(score >= -3000) {
                ui->chatBox->setText(lose_a_little[get_random_under_three()]);
            } else {
                ui->chatBox->setText(lose_a_lot[get_random_under_three()]);
            }
        }
    }
}


void MainWindow::initGame(){
    for (int i = 0; i < 15; ++i)
        for (int j = 0; j < 15; ++j)
            board[i][j] = -1;
    update();
}

void MainWindow::on_PVE_clicked()
{
    ui->PVE->hide();
    ui->PVP->hide();
    ui->offensive->show();
    ui->defensive->show();
    ui->back->show();
}

void MainWindow::on_PVP_clicked()
{
    initGame();
    turnPVP = 0;
    mode = Mode::PVP;
    ui->PVE->hide();
    ui->PVP->hide();
    ui->givein->show();
    ui->retract->show();
    ui->back->show();
    hover = click = true;
    currentPlayer = 0;
}

void MainWindow::on_back_clicked()
{
    ui->PVE->show();
    ui->PVP->show();
    ui->offensive->hide();
    ui->defensive->hide();
    ui->back->hide();
    ui->givein->hide();
    hover = click = false;
    currentPlayer = 0;
    initGame();
    refresh();
}

void MainWindow::on_offensive_clicked()
{
    initGame();
    ai_side = 1;
    turnPVE = 0;
    ui->givein->show();
    ui->offensive->hide();
    ui->defensive->hide();
    ui->back->hide();
    ui->restart->show();
    myAI = new AI;
    hover = click = true;
    mode = Mode::PVE;
    currentPlayer = 0;
}

void MainWindow::on_defensive_clicked()
{
    initGame();
    hover = click = false;
    ai_side = 0;
    ui->givein->show();
    ui->offensive->hide();
    ui->defensive->hide();
    ui->back->hide();
    ui->restart->show();
    myAI = new AI;
    turnPVE = 0;
    std::pair<int, int> AIaction = action(std::make_pair(-1, -1));
    board[AIaction.first][AIaction.second] = ai_side;
    mode = Mode::PVE;
    ui->broadcast->insertPlainText("AI在(");
    ui->broadcast->insertPlainText(tr("%1").arg(AIaction.first));
    ui->broadcast->insertPlainText(", ");
    ui->broadcast->insertPlainText(tr("%1").arg(AIaction.second));
    ui->broadcast->insertPlainText(")处落子\n");
    update();
    hover = click = true;
    currentPlayer = 1;
}

void MainWindow::on_change_clicked()
{
    if (mode == Mode::PVP) {
        for (int i = 0; i < 15; ++i) {
            for (int j = 0; j < 15; ++j){
                if (board[i][j] >= 0)
                    board[i][j] = 1 - board[i][j];
            }
        }
        if (currentPlayer == 0)
            ui->broadcast->insertPlainText("黑");
        else ui->broadcast->insertPlainText("白");
        ui->broadcast->insertPlainText("翻转了棋盘\n");
        currentPlayer = !currentPlayer;
        update();
        ui->change->hide();
    } else {
        std::pair<int, int> AIaction = action(std::make_pair(-1, -1));
        for (int i = 0; i < 15; ++i) {
            for (int j = 0; j < 15; ++j){
                if (board[i][j] >= 0)
                    board[i][j] = 1 - board[i][j];
            }
        }
        int x = AIaction.first, y = AIaction.second;
        board[y][x] = ai_side;
        ui->broadcast->insertPlainText("你翻转了棋盘\n");
        ui->broadcast->insertPlainText("AI在(");
        ui->broadcast->insertPlainText(tr("%1").arg(y));
        ui->broadcast->insertPlainText(", ");
        ui->broadcast->insertPlainText(tr("%1").arg(x));
        ui->broadcast->insertPlainText(")处落子\n");
        update();
        ui->change->hide();
    }
}


void MainWindow::on_givein_clicked()
{
    QMessageBox::StandardButton rb = QMessageBox::question(NULL, "confirm", "你确定要认输吗?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if (rb == QMessageBox::Yes) {
        if (mode == Mode::PVP) {
            if (currentPlayer == 0) {
                QMessageBox message(QMessageBox::NoIcon, "confirm", "黑子认输，白子获胜!");
                message.exec();
            } else {
                QMessageBox message(QMessageBox::NoIcon, "confirm", "白子认输，黑子获胜!");
                message.exec();
            }
        } else {
            QMessageBox message(QMessageBox::NoIcon, "confirm", "很遗憾，你认输了，AI获胜!");
            message.exec();
        }
        initGame();
        refresh();
    }
}

void MainWindow::on_retract_clicked()
{
    QMessageBox::StandardButton rb = QMessageBox::question(NULL, "confirm", "你确定要悔棋吗?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if (rb == QMessageBox::Yes) {
        board[clickCol][clickRow] = -1;
        update();
        if (currentPlayer == 1) {
            ui->broadcast->insertPlainText("黑");
        } else {
            ui->broadcast->insertPlainText("白");
        }
        ui->broadcast->insertPlainText("方悔棋\n");
        currentPlayer = !currentPlayer;
    }
}

void MainWindow::refresh(){
    ui->PVP->show();
    ui->PVE->show();
    ui->offensive->hide();
    ui->defensive->hide();
    ui->back->hide();
    ui->change->hide();
    ui->givein->hide();
    ui->retract->hide();
    ui->restart->hide();
    hover = click = false;
    if (myAI) {
        delete myAI;
        myAI = NULL;
    }
    ui->chatBox->setText(before_game_start[get_random_under_three()]);
    ui->broadcast->setText("");
}

void MainWindow::showWin(int color) {
    if (mode == Mode::PVP) {
        if (color == 0) {
            QMessageBox message(QMessageBox::NoIcon, "confirm", "黑子获胜!");
            message.exec();

        } else {
            QMessageBox message(QMessageBox::NoIcon, "confirm", "白子获胜!");
            message.exec();
        }
    } else {
        if (color == ai_side) {
            QMessageBox message(QMessageBox::NoIcon, "confirm", "AI 获胜!");
            message.exec();
        } else {
            QMessageBox message(QMessageBox::NoIcon, "confirm", "Congratulations! 你战胜了AI！");
            message.exec();
        }
    }
    initGame();
    refresh();

}

bool MainWindow::judge(int a, int b, int color){
    int x = a, y = b;
    int len = 0;
    while (--x >= 0 and board[x][y] == color) ++len;
    x = a;
    while (++x < 15 and board[x][y] == color) ++len;
    if (len >= 4) {
        showWin(color);
        return true;
    }

    len = 0; x = a;
    while(--y >= 0 and board[x][y] == color) ++len;
    y = b;
    while(++y < 15 and board[x][y] == color) ++len;
    if (len >= 4) {
        showWin(color);
        return true;
    }

    len = 0; y = b;
    while (++x < 15 and ++y < 15 and board[x][y] == color) ++len;
    x = a; y = b;
    while (--x >= 0 and --y >= 0 and board[x][y] == color) ++len;
    if (len >= 4) {
        showWin(color);
        return true;
    }

    len = 0; x = a; y = b;
    while (++x < 15 and --y >= 0 and board[x][y] == color) ++len;
    x = a; y = b;
    while (--x >= 0 and ++y < 15 and board[x][y] == color) ++len;
    if (len >= 4) {
        showWin(color);
        return true;
    }

    return false;
}

void MainWindow::on_restart_clicked()
{
    initGame();
    refresh();
}

int MainWindow::get_random_under_three() {
    return rand() % 3;
}
