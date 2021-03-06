#ifndef SHOWWINDOW_H
#define SHOWWINDOW_H

#include <QObject>
#include <QDialog>
#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QTextEdit>
#include <QComboBox>
#include <QList>
#include <QRegExp>
#include <QDebug>
#include <QPushButton>
#include <QMessageBox>
#include <QClipboard>

class ShowWindow : public QDialog
{
    Q_OBJECT

struct Cls {
    int start_week, end_week;      // 第几周单位
    int day;                       // 周日~周六，0~6
    int start_course, end_course;  // 课程范围：第1~12节课
    bool dual;                     // 双周
    bool single;                   // 单周
    int member;                    // 人数

    Cls()
    {
        dual = single = false;
        start_week = end_week = 0; // 判断失败设置为0
    }

    QString toString()
    {
        return QString("第 %1 ~ %2 周，周 %3 ，第 %4 ~ %5 节，%6 人 %7 %8").arg(start_week)
                .arg(end_week).arg(day).arg(start_course).arg(end_course).arg(member)
                .arg(dual?"双周":"").arg(single?"单周":"");
    }
};

struct DayNums {
    int m4, m5, a9, n11, n12;

    DayNums()
    {
        m4 = m5 = a9 = n11 = n12 = 0;
    }

    QString toString()
    {
        return QString("%1\t%2\t%3\t%4\t%5").arg(m4).arg(m5).arg(a9).arg(n11).arg(n12);
    }
};

public:
    ShowWindow(QWidget* parent, QString c1, QString c2);

protected:
    QString weekDayTrans(int x);
    int weekDayTrans(QString);
    DayNums getDayNums(int week, int day);

private:
    void initView();
    void analyze(QStringList times, QStringList numbs, QList<Cls> &clss);
    QList<Cls> clssFromString(QString time, QString numb);
    Cls clsFromString(QString time, QString numb);
    void refreshInfomation();

signals:

public slots:
    void slotWeekChanged(int x);
    void slotDayChanged(int x);
    void slotCopyAll();

private:
    QComboBox* week_combo;
    QListWidget* time_list;
    QTextEdit* numb_edit;
    QPushButton* copy_btn;

    QList<Cls> clss;   // 课程时间人数，从字符串数组转换为数组对象
    bool Monday_first; // 一周从星期一开始还是星期日开始
    int current_week;
    int current_day;
};

#endif // SHOWWINDOW_H
