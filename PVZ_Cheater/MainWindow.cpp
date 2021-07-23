#include <QMessageBox>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "MemoryUtil.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&gameProcessListener, SIGNAL(timeout()), this, SLOT(findGameProcess()));
    gameProcessListener.setInterval(1000);
    gameProcessListener.start();
}

MainWindow::~MainWindow()
{
    gameProcessListener.stop();
    delete ui;
}

void MainWindow::findGameProcess()
{
    lastTimeGameHandle = gameHandle;
    gameHandle = MemoryUtil::getProcessHandle();
    if (gameHandle != lastTimeGameHandle) {
        if (gameHandle) {
            ui->lbGameIsRunningColor->setStyleSheet("QLabel{background-color:rgb(0, 255, 0);}");
            ui->lbGameIsRunning->setText("PVZ汉化第二版正在运行!");
        } else {
            ui->lbGameIsRunningColor->setStyleSheet("QLabel{background-color:rgb(255, 0, 0);}");
            ui->lbGameIsRunning->setText("PVZ汉化第二版未运行!");
        }
        int tabCount = ui->tabWidget->count();
        for (int i = 0; i < tabCount; i++)
            ui->tabWidget->widget(i)->setEnabled(gameHandle);
    }
}

//更改阳光数值
void MainWindow::on_pbChangeSunCount_clicked()
{
    DWORD sunCount = ui->leSunCount->text().toInt();
    int pBase = 0x496BC8;    //基址
    DWORD ptr1, ptr2;
    ReadProcessMemory(gameHandle, (LPVOID)pBase, &ptr1, 4, 0);
    ReadProcessMemory(gameHandle, (LPVOID)(ptr1 + 0x8), &ptr2, 4, 0);
    if (!WriteProcessMemory(gameHandle, (LPVOID)(ptr2 + 0x5560), &sunCount, 4, 0))
        QMessageBox::critical(this, tr("警告"), tr("修改阳光数值失败！"), QMessageBox::Ok);
}

//种植物不减阳光
void MainWindow::on_rbPlantDoNotSubSunCount_toggled(bool checked)
{
    if (checked) {
        LPVOID codeAddress = (LPVOID)0x41BA74;        //sub esi,ebx  减阳光指令  29 DE
        BYTE instruction[2] = { 0x90, 0x90 };       //0x90是nop指令
        if (!WriteProcessMemory(gameHandle, codeAddress, &instruction, sizeof(instruction), 0))
            QMessageBox::critical(this, tr("警告"), tr("修改失败！"), QMessageBox::Ok);
    }
}

//种植物加阳光
void MainWindow::on_rbPlantAddSunCount_toggled(bool checked)
{
    if (checked) {
        LPVOID codeAddress = (LPVOID)0x41BA74;        //sub esi,ebx  减阳光指令  29 DE
        BYTE instruction[2] = { 0x01, 0xDE };       //0x01 0xDE是add esi,ebx加阳光指令
        if (!WriteProcessMemory(gameHandle, codeAddress, &instruction, sizeof(instruction), 0))
            QMessageBox::critical(this, tr("警告"), tr("修改失败！"), QMessageBox::Ok);
    }
}

//阳光正常
void MainWindow::on_rbPlantNormalSunCount_toggled(bool checked)
{
    if (checked) {
        LPVOID codeAddress = (LPVOID)0x41BA74;        //sub esi,ebx  减阳光指令  29 DE
        BYTE instruction[2] = { 0x29, 0xDE };
        if (!WriteProcessMemory(gameHandle, codeAddress, &instruction, sizeof(instruction), 0))
            QMessageBox::critical(this, tr("警告"), tr("修改失败！"), QMessageBox::Ok);
    }
}

//锁定阳光数值
void MainWindow::on_cbLockSunCount_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked) {
        ui->rbPlantDoNotSubSunCount->setChecked(true);
        ui->rbCollectDoNotAddSunCount->setChecked(true);
        ui->gbPlantSunCount->setEnabled(false);
        ui->gbCollectSunCount->setEnabled(false);
    } else {
        ui->gbPlantSunCount->setEnabled(true);
        ui->gbCollectSunCount->setEnabled(true);
        ui->rbPlantNormalSunCount->setChecked(true);
        ui->rbCollectNormalSunCount->setChecked(true);
    }
}

//正常加减收集阳光数量
void MainWindow::on_rbCollectNormalSunCount_toggled(bool checked)
{
    if (checked) {
        LPVOID codeAddress = (LPVOID)0x430A11;        //add [eax+00005560],ecx  加阳光指令  01 88 60550000
        BYTE instruction[6] = { 0x01, 0x88, 0x60, 0x55, 0x00, 0x00 };       //0x01是add
        if (!WriteProcessMemory(gameHandle, codeAddress, &instruction, sizeof(instruction), 0))
            QMessageBox::critical(this, tr("警告"), tr("修改失败！"), QMessageBox::Ok);
    }
}

//收集阳光后减去相应阳光数值
void MainWindow::on_rbCollectSubSunCount_toggled(bool checked)
{
    if (checked) {
        LPVOID codeAddress = (LPVOID)0x430A11;        //add [eax+00005560],ecx  加阳光指令  01 88 60550000
        BYTE instruction[6] = { 0x29, 0x88, 0x60, 0x55, 0x00, 0x00 };       //0x29是sub
        if (!WriteProcessMemory(gameHandle, codeAddress, &instruction, sizeof(instruction), 0))
            QMessageBox::critical(this, tr("警告"), tr("修改失败！"), QMessageBox::Ok);
    }
}

//收集阳光不加相应阳光数值
void MainWindow::on_rbCollectDoNotAddSunCount_toggled(bool checked)
{
    if (checked) {
        LPVOID codeAddress = (LPVOID)0x430A11;        //add [eax+00005560],ecx  加阳光指令  01 88 60550000
        BYTE instruction[6] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };       //0x90是nop
        if (!WriteProcessMemory(gameHandle, codeAddress, &instruction, sizeof(instruction), 0))
            QMessageBox::critical(this, tr("警告"), tr("修改失败！"), QMessageBox::Ok);
    }
}

//每次冷却刷新加的delta值，越大冷却越快，可以是0和负数。0停止冷却，负数反向冷却
void MainWindow::on_pbCardCDDelta_clicked()
{
    LPVOID codeAddress = (LPVOID)0x48728C;        //add dword ptr [edi+24],01  加冷却进度值  83 47 24 01
    int delta = ui->sbCardCDDelta->value();
    if (delta < 0) {
        if (QMessageBox::question(this, tr("警告"), tr("修改负数值可能会导致游戏崩溃，是否继续修改？"),
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) != QMessageBox::Yes) {
            return;
        }
    }
    BYTE instruction[4] = { 0x83, 0x47, 0x24, (BYTE)(0x000000ff & delta) };       //delta转1字节的16进制
    if (!WriteProcessMemory(gameHandle, codeAddress, &instruction, sizeof(instruction), 0))
        QMessageBox::critical(this, tr("警告"), tr("修改失败！"), QMessageBox::Ok);
}

//恢复冷却delta
void MainWindow::on_pbCardCDDeltaDefault_clicked()
{
    ui->sbCardCDDelta->setValue(1);
    ui->pbCardCDDelta->click();
}

//快速自动收集阳光
void MainWindow::on_rbQuickAutoCollectSun_toggled(bool checked)
{
    if (checked) {
        //快速收集
        LPVOID codeAddress = (LPVOID)0x43158B;      //cmp byte ptr [ebx+50],00  判断阳光是否收集指令，0为没有点击被收集  80 7B 50 00
        BYTE instruction[4] = { 0x80, 0x7B, 0x50, 0x01 };
        if (!WriteProcessMemory(gameHandle, codeAddress, &instruction, sizeof(instruction), 0))
            QMessageBox::critical(this, tr("警告"), tr("修改失败！"), QMessageBox::Ok);
    } else {
        //取消快速收集
        LPVOID codeAddress1 = (LPVOID)0x43158B;      //cmp byte ptr [ebx+50],00  判断阳光是否收集指令，0为没有点击被收集  80 7B 50 00
        BYTE instruction1[4] = { 0x80, 0x7B, 0x50, 0x00 };
        if (!WriteProcessMemory(gameHandle, codeAddress1, &instruction1, sizeof(instruction1), 0))
            QMessageBox::critical(this, tr("警告"), tr("修改失败！"), QMessageBox::Ok);
    }
}

//慢速自动收集阳光
void MainWindow::on_rbSlowAutoCollectSun_toggled(bool checked)
{
    if (checked) {
        //较慢速收集
        LPVOID codeAddress = (LPVOID)0x430AD0;      //jne PlantsVsZombies.exe+30B10  没被收集则跳转 75 3E
        BYTE instruction[2] = { 0x90, 0x90 };
        if (!WriteProcessMemory(gameHandle, codeAddress, &instruction, sizeof(instruction), 0))
            QMessageBox::critical(this, tr("警告"), tr("修改失败！"), QMessageBox::Ok);
    } else {
        //取消较慢速收集
        LPVOID codeAddress2 = (LPVOID)0x430AD0;      //jne PlantsVsZombies.exe+30B10  没被收集则跳转 75 3E
        BYTE instruction2[2] = { 0x75, 0x3E };
        if (!WriteProcessMemory(gameHandle, codeAddress2, &instruction2, sizeof(instruction2), 0))
            QMessageBox::critical(this, tr("警告"), tr("修改失败！"), QMessageBox::Ok);
    }
}
