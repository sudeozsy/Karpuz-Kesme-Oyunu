// SUDE ÖZSOY 22100011074
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QFile>
#include <QStringList>
#include <QDebug>
#include <QRandomGenerator>
#include <QPushButton>
#include <QTimer>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    QStringList DosyaOkuma();
    QPushButton *karpuz,*ucsn;
    QTimer *timer,*timer2,*timer3,*timer30,*timerEk,*timerEk3;
    QLabel *Label,*LabelSure,*LabelKesilen,*LabelKesilmeyen,*LabelEk;

private slots:
    void KarpuzOlustur(QStringList);
    void EkSureOlustur(QStringList);
    void karpuzTiklandi();
    void EkSureTiklandi();
    void SkorHesapla();
    void SureAzalt();

};
#endif // MAINWINDOW_H
