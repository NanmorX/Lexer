#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

protected:
//    void closeEvent(QCloseEvent *event) override;

private slots:
    void open();
    void save();
    void startLexer();
    void pretreat();

private:
    int gettok(QString s);
    int inKeyWord(QString target);
    int inSign(char target);
    void deal(QString s);

    int i = 0;
    QString IdentifierStr;
    int Num_Id = 0;
    QVector <QString> Identify;

    QString KeyWord[17] = {
        "int", "void", "if", "else", "while", "return",
        "float", "double", "char", "bool", "do", "switch",
        "case", "class", "private", "public", "struct"
    };

    char Sign[18] = {
        '=', '+', '-', '*', '/', '=', '>', '>', '<', '<',
        '!', ';', ',', '(', ')', '{', '}', '#'
    };

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
