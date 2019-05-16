#include "showwindow.h"

ShowWindow::ShowWindow(QWidget *parent, QString c1, QString c2) : QDialog(parent)
{
    times = c1.split("\n");
    numbs = c2.split("\n");

    initView();
}

void ShowWindow::initView()
{
    this->setMinimumSize(100, 200);

    QHBoxLayout * main_layout = new QHBoxLayout;
    QVBoxLayout* vlayout = new QVBoxLayout;
    week_combo = new QComboBox(this);
    time_list = new QListWidget(this);
    numb_edit = new QTextEdit(this);
    vlayout->addWidget(week_combo);
    vlayout->addWidget(time_list);
    main_layout->addLayout(vlayout);
    main_layout->addWidget(numb_edit);
    this->setLayout(main_layout);

    // 添加周数
    QStringList sl;
    for (int i = 1; i < 30; i++)
    {
        sl << QString::number(i);
    }
    week_combo->addItems(sl);

    // 添加每星期
    time_list->addItem("周日");
    time_list->addItem("周一");
    time_list->addItem("周二");
    time_list->addItem("周三");
    time_list->addItem("周四");
    time_list->addItem("周五");
    time_list->addItem("周六");

    // 链接信号槽
    connect(week_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(slotWeekChanged(int)));
    connect(time_list, SIGNAL(currentRowChanged(int)), this, SLOT(slotDateChanged(int)));

    // 开始统计分离的课程
    analyze(times, numbs, clss);
}

/**
 * 根据所有的内容进行解析，拆分成多个数组
 * @param times 课程时间数组：周一第1,2节{第1-8周};周四第6,7节{第2-8周|双周};周四第8节{第2-8周|双周}
 * @param numbs 上课人数数组，为一堆整数
 * @param clss  进行统计的课程数量，如果有多个时间段就算作多个课程（因为部分具体那个名字）
 */
void ShowWindow::analyze(QStringList times, QStringList numbs, QList<Cls> &clss)
{
    clss.clear();
    for (int i = 0; i < times.size(); i++)
    {
        QList<Cls> temps = clssFromString(times.at(i), numbs.at(i));
        if (temps.size() > 0)
        {
            for (int j = 0; j < temps.size(); j++)
            {
                clss.append(temps.at(j));
            }
        }
    }
}

/**
 * 针对某一行，添加一个课程对象数组（因为每个课程可能有多个时间段）
 * @param time 时间字符串
 * @param numb 人数
 * @return     课程对象数组
 */
QList<ShowWindow::Cls> ShowWindow::clssFromString(QString time, QString numb)
{
    QList<Cls> clss;
    //if (time.indexOf(";") > -1) // 多个时间段
    //{
    QStringList times = time.split(";");
    for (int i = 0; i < times.size(); i++)
    {
        clss.append(clsFromString(times.at(i), numb));
    }
    //}
    //else ;
    return clss;
}

/**
 * 添加一个具体的课程对象
 * @param time 唯一时间
 * @param numb 唯一人数
 * @return     课程对象
 */
ShowWindow::Cls ShowWindow::clsFromString(QString time, QString numb)
{
    Cls cls;
    cls.member = numb.toInt();

    // 去掉“星期”
    time = time.replace("星期", "周");

    // 判断单双周
    if (time.indexOf("|双周"))
    {
        cls.dual = true;
        time = time.replace("|双周", "");
    }
    if (time.indexOf("|单周"))
    {
        cls.dual = true;
        time = time.replace("|单周", "");
    }

    // 判断第几周
    QRegExp re("周(.)第([\\d,]+)节\\{第(\\d+)-(\\d+)周\\}");
    if (!re.exactMatch(time))
    {
        qDebug() << "无法正则匹配时间：" << time;
        return cls;
    }


}

/**
 * 学期的第几周被改变
 * @param x 第几周
 */
void ShowWindow::slotWeekChanged(int x)
{

}

/**
 * 一周的第几天被改变
 * @param x 周几
 */
void ShowWindow::slotDateChanged(int x)
{

}
