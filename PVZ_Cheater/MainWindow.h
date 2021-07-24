#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <windows.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void findGameProcess();

    void on_pbChangeSunCount_clicked();

    void on_rbPlantDoNotSubSunCount_toggled(bool checked);

    void on_rbPlantAddSunCount_toggled(bool checked);

    void on_rbPlantNormalSunCount_toggled(bool checked);

    void on_cbLockSunCount_stateChanged(int arg1);

    void on_rbCollectNormalSunCount_toggled(bool checked);

    void on_rbCollectSubSunCount_toggled(bool checked);

    void on_rbCollectDoNotAddSunCount_toggled(bool checked);

    void on_pbCardCDDelta_clicked();

    void on_pbCardCDDeltaDefault_clicked();

    void on_rbQuickAutoCollectSun_toggled(bool checked);

    void on_rbSlowAutoCollectSun_toggled(bool checked);

    void on_rbPlantBulletContinuously_toggled(bool checked);

    void on_rbPlantBulletContinuouslyHasZ_toggled(bool checked);

    void on_cbAllCardNoCD_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;

    HANDLE gameHandle;  // ProcessHandle

    HANDLE lastTimeGameHandle;

    QTimer gameProcessListener;
};
#endif // MAINWINDOW_H
