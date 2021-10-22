#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <QDebug>
#include <QVector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->openButton, &QPushButton::clicked, this, &MainWindow::open);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::save);
    connect(ui->lexerButton, &QPushButton::clicked, this, &MainWindow::startLexer);
    connect(ui->pretreatButton, &QPushButton::clicked, this, &MainWindow::pretreat);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::open()
{
    QString fileName=QFileDialog::getOpenFileName(this,"打开文件",
                                     ".","files(*)");
    //getOPenFilename 他会返回一个选中文件的名字

    QFile file(fileName);
    file.setFileName(fileName);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray t;
        while(!file.atEnd())
        {
            t += file.readLine();
        }
        ui->resource->setText(QString(t));
        file.close();
    }
}

void MainWindow::save()
{
    QString e = ui->result->toPlainText();
    QFile file;
    file.setFileName(QFileDialog::getSaveFileName(this, QString("保存路径"), QString("/"),QString("TEXT(*.txt)")));
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(e.toUtf8());
    file.close();
}

void MainWindow::startLexer()
{
    QString resource = ui->resource->toPlainText();
    ui->result->clear();
    Identify.clear();
    IdentifierStr.clear();
    Num_Id = 0;
    i = 0;
    while(i < resource.size())
    {
        int re = gettok(resource);
        if(re == 0)
            break;
        else if(re == -1)
        {
            QMessageBox::warning(NULL,  "warning",  "代码存在不符合规则的部分（如数字以0开头、存在不属于规则的符号等）", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            ui->resource->clear();
            break;
        }
    }
}

int MainWindow::gettok(QString s)
{
    int LastChar = s.at(i).toLatin1();

    while(isspace(LastChar) || LastChar == 10)
    {
        i++;
        if(i >= s.size())
            return 0;
        LastChar = s.at(i).toLatin1();
    }

    if(isalpha(LastChar))
    {
        IdentifierStr = LastChar;
        if(i + 1 >= s.size())
            return 0;
        while(isalnum(LastChar = s.at(++i).toLatin1()))
            IdentifierStr += LastChar;

        int t = inKeyWord(IdentifierStr);
        if(t)
        {
            QString res = "<$"+IdentifierStr+", ->";
            ui->result->append(res);
            return t;
        }

        if(Identify.indexOf(IdentifierStr) == -1)
        {
            Num_Id++;
            Identify.push_back(IdentifierStr);
//            QString res = "<"+IdentifierStr+", "+QString::number(Num_Id)+">";
            QString res = "<$ID, " + QString::number(Num_Id) + ">";
            ui->result->append(res);
        }
        else
        {
            int num = Identify.indexOf(IdentifierStr) + 1;
//            QString res = "("+IdentifierStr+", "+QString::number(num)+")";
            QString res = "<$ID, " + QString::number(num) + ">";
            ui->result->append(res);
        }
        return 7;
    }
    // 不考虑.d形式
    else if(isdigit(LastChar))
    {
        QString NumStr;
        NumStr += LastChar;
        if(LastChar != '0')
        {
            if(i + 1 >= s.size())
            {
                QString res = "<$" + NumStr + ", ->";
                ui->result->append(res);
                return 0;
            }
            LastChar = s.at(++i).toLatin1();
            while(isdigit(LastChar))
            {
                NumStr += LastChar;
                if(i + 1 >= s.size())
                {
                    QString res = "<$" + NumStr + ", ->";
                    ui->result->append(res);
                    return 0;
                }
                LastChar = s.at(++i).toLatin1();
            }
        }
        else
        {
            if(i + 1 >= s.size())
            {
                ui->result->append("<$0, ->");
                return 0;
            }
            LastChar = s.at(++i).toLatin1();
            if(isdigit(LastChar))
                return -1;
        }

        QString res = "<$" + NumStr + ", ->";
        ui->result->append(res);
        return 8;
    }
    // 先特殊处理=和==， <和<=, >和>=, !=
    else if(LastChar == '!')
    {
        if(i + 1 >= s.size())
            return 0;
        LastChar = s.at(++i).toLatin1();
        ui->result->append("<$!=, ->");
        if(i + 1 >= s.size())
            return 0;
        LastChar = s.at(++i).toLatin1();
        return 19;
    }
    else if(LastChar == '=')
    {
        if(i + 1 >= s.size())
            return 0;
        LastChar = s.at(++i).toLatin1();
        if(LastChar == '=')
        {
            ui->result->append("<$==, ->");
            if(i + 1 >= s.size())
                return 0;
            LastChar = s.at(++i).toLatin1();
            return 14;
        }
        else
        {
            ui->result->append("<$=, ->");
            return 9;
        }
    }
    else if(LastChar == '<')
    {
        if(i + 1 >= s.size())
            return 0;
        LastChar = s.at(++i).toLatin1();
        if(LastChar == '=')
        {
            ui->result->append("<$<=, ->");
            if(i + 1 >= s.size())
                return 0;
            LastChar = s.at(++i).toLatin1();
            return 18;
        }
        else
        {
            ui->result->append("<$<, ->");
            return 17;
        }
    }
    else if(LastChar == '>')
    {
        if(i + 1 >= s.size())
            return 0;
        LastChar = s.at(++i).toLatin1();
        if(LastChar == '=')
        {
            ui->result->append("<$>=, ->");
            if(i + 1 >= s.size())
                return 0;
            LastChar = s.at(++i).toLatin1();
            return 16;
        }
        else
        {
            ui->result->append("<$>, ->");
            return 15;
        }
    }
    else if(inSign(LastChar))
    {
        int t = inSign(LastChar);
        QString res = "<$"+QString(LastChar)+", ->";
        ui->result->append(res);
        if(i + 1 >= s.size())
            return 0;
        LastChar = s.at(++i).toLatin1();
        return t;
    }
    else
        return -1;
}

int MainWindow::inKeyWord(QString target)
{
    for(int i = 0; i < 17; i++)
    {
        if(target == KeyWord[i])
            return i + 1;
    }
    return 0;
}

int MainWindow::inSign(char target)
{
    for(int i = 0; i < 18; i++)
    {
        if(target == Sign[i])
            return i + 9;
    }
    return 0;
}

void MainWindow::pretreat()
{
    QString resource = ui->resource->toPlainText();
    ui->resource->clear();
    deal(resource);
}

void MainWindow::deal(QString s)
{
    QVector <int> vec;
    QString res;
    for(int i = 0; i < s.size(); i++)
    {
        if(s.at(i) == '/')
        {
            if(s.at(i + 1) == '/')
            {
                i += 1;
                while(s.at(++i) != '\n' && i < s.size());
            }
            else if(s.at(i + 1) == '*')
            {
                vec.push_back(0);
                i++;
                while(i < s.size() && !vec.empty())
                {
                    i++;
                    if(s.at(i) == '/' && s.at(i+1) == '*')
                    {
                        i++;
                        vec.push_back(0);
                    }
                    else if(s.at(i) == '*' && s.at(i+1) == '/')
                    {
                        i++;
                        vec.pop_back();
                    }
                }
            }
//            else
//            {
//                QMessageBox::warning(NULL,  "warning",  "'/'的使用不符合语法", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
//                break;
//            }
        }
        else
        {
            res += s.at(i);
        }
    }
    ui->resource->append(res);
}
