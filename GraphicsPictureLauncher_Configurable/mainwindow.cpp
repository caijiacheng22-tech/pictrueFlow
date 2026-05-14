/*
 * 图片跑马灯模块，功能未完善。
 * version 1.0
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMainWindow>
#include <pictrueitem.h>
#include <pictruebutton.h>
#include <pictrueview.h>
#include <pictruewidget.h>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QTimer>
#include <QPainterPath>
#include <QGraphicsView>
#include <QDebug>
#include <QQueue>
#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include <cmath>
#include <ctime>
#include <QTransform>
#include <QButtonGroup>
#include <QCoreApplication>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QPainter>
#include <QProcess>
#include <QPushButton>
#include <QUrl>

 //static int itemIdex = 0;
 //static int testIndex = 0;//调试用，可注释或删除
static int dir = 0;//记录方向
static QList<qreal> spaceList;
static QList<qreal> unitList;
static QList<qreal> transScaleList;//缩放比例表
static QSize pictrueBigSize = RAW_VIEW_SIZE / SCALE_VIEW_PIXMAP;
static QSize pictrueSmallSize = RAW_VIEW_SIZE / SCALE_VIEW_PIXMAP / SCALE_BIG_SMALL;
static QList<PictrueItem*> itemList;
static QList<int> finishList;


MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    m_timer(new QTimer(this)),
    m_scene(new QGraphicsScene(this)),
    m_index(0),
    currentRule(RuleA),
    m_rollCount(0),
    btnMoveEnable(true)
{
    ui.setupUi(this);
    setWindowTitle("Game Launcher");
    loadGames();

    btnGroup = new QButtonGroup(this);
    btnGroup->addButton(ui.picBtn0, 0);
    btnGroup->addButton(ui.picBtn1, 1);
    btnGroup->addButton(ui.picBtn2, 2);
    btnGroup->addButton(ui.picBtn3, 3);
    btnGroup->addButton(ui.picBtn4, 4);
    btnGroup->addButton(ui.picBtn5, 5);
    btnGroup->addButton(ui.picBtn6, 6);
    btnGroup->addButton(ui.picBtn7, 7);
    btnGroup->setExclusive(true);
    btnGroup->button(MID_TYPE)->setChecked(true);
    for (int i = 0; i < 8; i++)
    {
        static_cast<PictrueButton*>(btnGroup->button(i))->setId(i);
    }
#if 0
    QGraphicsItem* item = new PictrueItem(QPixmap(":/picture/testPart/character0.png")
        .scaled(300, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    QTimeLine* timer = new QTimeLine(1000);
    timer->setFrameRange(0, 60);
    timer->setUpdateInterval(1);


    QGraphicsScene* scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, 500, 500);
    scene->addItem(item);

    ui.graphicsView->setScene(scene);
    ui.graphicsView->show();

    connect(ui.goBtn, &QPushButton::clicked, timer, &QTimeLine::start);
    //    connect(ui.goBtn,&QPushButton::clicked,[=]()
    //    {
    //        QTransform transf(item->transform());
    //        transf.rotate(90.0/60.0,Qt::ZAxis);
    //        item->setTransform(transf);
    //        //scene->update();
    //    });
    static int previou;
    static int current;
    connect(timer, &QTimeLine::frameChanged, [=](int frame)
        {
            qDebug() << frame;
            QTransform transf(item->transform());
            current = frame;
            for (int i = 0; i < current - previou; i++)
                transf.rotate(90.0 / 60.0, Qt::ZAxis);
            previou = frame;
            item->setTransform(transf);
            //scene->update();
        });
    //timer->start();
#else 
    pointList << P1 << P2 << P3 << P4 << P5 << P6 << P7 << P8;
    zValueList << 1 << 2 << 3 << 2 << 1 << 0 << 0 << 0;
    pixmapScaleList << (qreal)1 << (qreal)1 << (qreal)1 << (qreal)1 << (qreal)1
        << (qreal)1 / SCALE_BIG_SMALL << (qreal)1 / SCALE_BIG_SMALL << (qreal)1 / SCALE_BIG_SMALL;

    ui.graphicsView->setStyleSheet("background: transparent; padding: 0px; border: 0px;");


    ui.graphicsView->setScene(m_scene);
    m_scene->setSceneRect(0, 0, RAW_VIEW_SIZE.width(), RAW_VIEW_SIZE.height());
    midLine.setPoints(QPointF(0, RAW_VIEW_SIZE.height() / 2), \
        QPointF(RAW_VIEW_SIZE.width(), RAW_VIEW_SIZE.height() / 2));

    //添加图片
    for (int i = 0; i < 8; i++)
        pixmapList.append(loadCoverPixmap(m_games[i]));

    //图元添加图片
    for (int i = 0; i < 8; i++)
    {
        itemList.append(new PictrueItem(pixmapList[i]
            .scaled(pictrueBigSize, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        ));
        itemList[i]->setScale(pixmapScaleList[i]);
        itemList[i]->setType(i);
        itemList[i]->setItemId(i);
        itemList[i]->setOffset(QPointF(0, 0));
    }

    for (int i = 0; i < 8; i++)
    {
        m_scene->addItem(itemList[i]);//添加图元
        itemList[i]->setPos(midLine.pointAt(pointList[i]));//设置位置
        itemList[i]->setZValue(zValueList[i]);//设置显示优先级
        itemList[i]->setTransformationMode(Qt::SmoothTransformation);//设置缩放模式
    }



    //利用持续时间和帧数计算出定时时间,持续时间/（帧数*持续时间），这里乘1000是转为秒
    m_timer->setInterval(DURATION_MS / (FPS * DURATION_MS / 1000));

    connect(m_timer, &QTimer::timeout, this, &MainWindow::timerOutFunc);

    //连接右按钮单机信号和处理函数
    connect(ui.nextBtn, &QPushButton::clicked, this, &MainWindow::nextItem);

    //连接左按钮单机信号和处理函数
    connect(ui.previousBtn, &QPushButton::clicked, this, &MainWindow::previousItem);

    //连接View大小改变信号和处理函数
    connect(ui.graphicsView, &PictrueView::sizeChanged, [=](const QSize& size) {
        //重新设置场景变换后的矩形
        if (m_scene->isActive())
            m_scene->setSceneRect(0, 0, size.width(), size.height());
        //重新计算变换后的大小
        pictrueBigSize = size / SCALE_VIEW_PIXMAP;
        for (int i = 0; i < 8; i++)
        {
            itemList[i]->setPixmap(pixmapList[i].scaled(pictrueBigSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            //重新设置Line坐标
            if (i == 0)
                midLine.setPoints(QPointF(0, \
                    size.height() / 2 - itemList[i]->boundingRect().height() / 2), \
                    QPointF(size.width() - itemList[i]->boundingRect().width(), \
                        size.height() / 2 - itemList[i]->boundingRect().height() / 2));
            //重新设置位置
            itemList[i]->setPos(midLine.pointAt(pointList[i]));
        }
        });

    for (int i = 0; i < 8; i++)
    {
        connect(itemList[i], &PictrueItem::clickedId, this, &MainWindow::clickedItemRoll);
        void (PictrueButton:: * funcPtr)(int) = &PictrueButton::entered;
        connect(static_cast<PictrueButton*>(btnGroup->button(i)), funcPtr, [this](int id)
            {
                //            if(m_timer->isActive())
                //            {
                //                return;
                //            }
                int i = 0;
                while (id != itemList[i]->itemId()) {
                    i++;
                    if (i > 7)
                    {
                        qDebug() << "无匹配的单元的编号（id）。";
                        return;
                    }
                }
                btnMoveEnable = false;
                clickedItemRoll(itemList[i]->type());
                qDebug() << itemList[i]->type();
            });
    }
#endif
}

MainWindow::~MainWindow()
{

}

int MainWindow::getIndexByRules(int oldIndex, int rule)
{
    switch (rule) {
    case 1:
        return (oldIndex + 1) % 8;
    case -1:
        return oldIndex == 0 ? 7 : oldIndex - 1;
    default:
        return 0;
    }
}

void MainWindow::timerOutFunc()
{
    for (int i = 0; i < 8; i++)
    {
        if (qAbs(midLine.pointAt(pointList[getIndexByRules(i, dir)]).x() - itemList[i]->pos().x()) < qAbs(unitList[i]))
        {
            if (finishList.contains(i))
                continue;
            itemList[i]->setPos(midLine.pointAt(pointList[getIndexByRules(i, dir)]));
            //设置新的显示优先级
            itemList[i]->setScale(pixmapScaleList[getIndexByRules(i, dir)]);
            //设置新的类型
            itemList[i]->setType(getIndexByRules(i, dir));
            //i==7-->最后一个图元移动完成
            finishList.append(i);
            if (finishList.size() == 8)
            {
                //循环旋转图元表和图片表
                rollList(itemList, dir, 1);
                rollList(pixmapList, dir, 1);
                for (int i = 0; i < 8; i++)
                    itemList[i]->setZValue(zValueList[i]);
                m_timer->stop();
                finishList.clear();
                if (btnGroup->checkedId() != itemList[MID_TYPE]->itemId() && btnMoveEnable)
                    btnGroup->button(getIndexByRules(btnGroup->checkedId(), -dir))->setChecked(true);
                if (--m_rollCount)
                {
                    rollItem(dir, m_rollCount);
                }
                if (m_rollCount == 0)
                    btnMoveEnable = true;
                break;
            }
        }
        else
        {
            //按计算好的移动单位移动一次
            itemList[i]->setPos(QPointF(itemList[i]->pos().x() + unitList[i], itemList[i]->pos().y()));
            //转换因子不是1.0时进行转换设置
            if (transScaleList[i] != (qreal)1.0)
            {
                itemList[i]->setScale(itemList[i]->scale() * transScaleList[i]);
            }
        }
        m_scene->invalidate();
    }
}

void MainWindow::rollItem(int rollDir, unsigned rollCount)
{
    if (m_timer->isActive())
        return;
    //清除之前的空间差列表和移动单位列表
    m_rollCount = rollCount;
    spaceList.clear();
    unitList.clear();
    transScaleList.clear();
    dir = rollDir;
    qDebug() << "rollCount" << rollCount;
    for (int i = 0; i < 8; i++)
    {
        spaceList.append(midLine.pointAt(pointList[getIndexByRules(i, dir)]).x() - itemList[i]->pos().x());//计算移动总距离
        unitList.append(spaceList[i] / (FPS * DURATION_MS / 1000));//计算移动单个距离
        transScaleList.append(pow(pixmapScaleList[getIndexByRules(i, dir)] / pixmapScaleList[i], \
            (qreal)1 / (FPS * DURATION_MS / 1000)));//计算增长倍数
    }

    //启动定时器
    m_timer->start();

}

void MainWindow::nextItem()
{
    rollItem(1, 1);
}

void MainWindow::previousItem()
{
    rollItem(-1, 1);
}

void MainWindow::clickedItemRoll(int type)
{
    if (m_timer->isActive())
        return;
    //如果点击的图片处于中间，则返回
    //int midType = itemList[MID_TYPE]->type();
    int ret = MID_TYPE - type;
    switch (ret) {
    case 0:
        launchCurrentGame();
        return;
        break;
    case 1:
    case 2:
        dir = 1;
        m_rollCount = ret;
        rollItem(dir, m_rollCount);
        break;
    case -1:
    case -2:
    case -3:
    case -4:
    case -5:
        dir = -1;
        m_rollCount = -ret;
        rollItem(dir, m_rollCount);
        break;
    default:
        return;
        break;
    }
}

static QList<GameInfo> defaultGames()
{
    QList<GameInfo> games;

    games.append({ "Sayonara o Oshiete", "covers/76933.jpg", "", "", QStringList() });
    games.append({ "Steins;Gate", "covers/77819.jpg", "", "", QStringList() });
    games.append({ "Full Metal Daemon Muramasa", "covers/77925.jpg",
        QString::fromUtf8("F:/rar     gallllllllllll/恶鬼/【PC】装甲恶鬼村正.part1/装甲恶鬼村正/Muramasa_chs.exe"),
        "", QStringList() });
    games.append({ "Nukitashi", "covers/78282.jpg", "", "", QStringList() });
    games.append({ "Sabbat of the Witch", "covers/79952.jpg", "", "", QStringList() });
    games.append({ "Subarashiki Hibi", "covers/90017.jpg", "", "", QStringList() });
    games.append({ "Magical Girl Witch Trial", "covers/100334.jpg", "", "", QStringList() });
    games.append({ "Saya no Uta", "covers/132772.jpg", "", "", QStringList() });

    return games;
}

QString MainWindow::configFilePath() const
{
    return QDir(QCoreApplication::applicationDirPath()).filePath("games.json");
}

QString MainWindow::resolveConfiguredPath(const QString& path) const
{
    const QString trimmedPath = path.trimmed();
    if (trimmedPath.isEmpty())
        return QString();

    QFileInfo fileInfo(trimmedPath);
    if (fileInfo.isAbsolute())
        return QDir::cleanPath(trimmedPath);

    return QDir::cleanPath(QDir(QCoreApplication::applicationDirPath()).filePath(trimmedPath));
}

bool MainWindow::createDefaultGamesConfig(const QString& path) const
{
    QJsonArray gamesArray;
    const QList<GameInfo> games = defaultGames();

    for (const GameInfo& game : games)
    {
        QJsonObject object;
        object["name"] = game.name;
        object["coverPath"] = game.coverPath;
        object["exePath"] = game.exePath;
        object["workingDir"] = game.workingDir;

        QJsonArray arguments;
        for (const QString& argument : game.arguments)
            arguments.append(argument);
        object["arguments"] = arguments;

        gamesArray.append(object);
    }

    QFileInfo fileInfo(path);
    QDir().mkpath(fileInfo.absolutePath());

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;

    file.write(QJsonDocument(gamesArray).toJson(QJsonDocument::Indented));
    return true;
}

bool MainWindow::loadGames()
{
    m_games = defaultGames();

    const QString path = configFilePath();
    if (!QFileInfo::exists(path) && !createDefaultGamesConfig(path))
    {
        showConfigMessage("Config Error",
            QString("Could not create games.json.\n\nExpected location:\n%1").arg(path));
        return false;
    }

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        showConfigMessage("Config Error",
            QString("Could not read games.json.\n\nLocation:\n%1").arg(path));
        return false;
    }

    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &parseError);
    if (parseError.error != QJsonParseError::NoError)
    {
        showConfigMessage("Config Error",
            QString("games.json is not valid JSON.\n\n%1\n\nLocation:\n%2")
                .arg(parseError.errorString(), path));
        return false;
    }

    QJsonArray gamesArray;
    if (document.isArray())
    {
        gamesArray = document.array();
    }
    else if (document.isObject())
    {
        gamesArray = document.object().value("games").toArray();
    }

    for (int i = 0; i < gamesArray.size() && i < m_games.size(); ++i)
    {
        const QJsonObject object = gamesArray[i].toObject();
        if (object.contains("name"))
            m_games[i].name = object.value("name").toString(m_games[i].name);
        if (object.contains("coverPath"))
            m_games[i].coverPath = object.value("coverPath").toString(m_games[i].coverPath);
        if (object.contains("exePath"))
            m_games[i].exePath = object.value("exePath").toString();
        if (object.contains("workingDir"))
            m_games[i].workingDir = object.value("workingDir").toString();
        if (object.contains("arguments"))
        {
            m_games[i].arguments.clear();
            const QJsonArray arguments = object.value("arguments").toArray();
            for (const QJsonValue& argument : arguments)
                m_games[i].arguments.append(argument.toString());
        }
    }

    return true;
}

QPixmap MainWindow::loadCoverPixmap(const GameInfo& game) const
{
    QPixmap pixmap(resolveConfiguredPath(game.coverPath));
    if (!pixmap.isNull())
        return pixmap;

    QPixmap fallback(580, 860);
    fallback.fill(QColor(36, 39, 45));

    QPainter painter(&fallback);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QColor(230, 234, 240));
    QFont font = painter.font();
    font.setPointSize(28);
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(fallback.rect().adjusted(40, 40, -40, -40),
        Qt::AlignCenter | Qt::TextWordWrap,
        QString("Cover Not Found\n\n%1").arg(game.name));

    return fallback;
}

void MainWindow::showConfigMessage(const QString& title, const QString& message)
{
    QMessageBox messageBox(this);
    messageBox.setWindowTitle(title);
    messageBox.setText(message);
    messageBox.setIcon(QMessageBox::Warning);
    QPushButton* openConfigButton = messageBox.addButton("Open games.json", QMessageBox::ActionRole);
    messageBox.addButton(QMessageBox::Ok);
    messageBox.exec();

    if (messageBox.clickedButton() == openConfigButton)
        QDesktopServices::openUrl(QUrl::fromLocalFile(configFilePath()));
}

void MainWindow::launchCurrentGame()
{
    if (itemList.size() <= MID_TYPE)
        return;

    const int gameIndex = itemList[MID_TYPE]->itemId();
    if (gameIndex < 0 || gameIndex >= m_games.size())
        return;

    const GameInfo& game = m_games[gameIndex];
    const QString exePath = resolveConfiguredPath(game.exePath);

    if (exePath.isEmpty())
    {
        showConfigMessage("Executable Path Not Configured",
            QString("No executable path is configured for:\n%1\n\nEdit games.json and set exePath.")
                .arg(game.name));
        return;
    }

    const QFileInfo exeInfo(exePath);

    if (!exeInfo.exists() || !exeInfo.isFile())
    {
        showConfigMessage("Executable Path Does Not Exist",
            QString("The configured executable path does not exist for:\n%1\n\nPath:\n%2")
                .arg(game.name, exePath));
        return;
    }

    QString workingDir = resolveConfiguredPath(game.workingDir);
    if (workingDir.isEmpty())
        workingDir = exeInfo.absolutePath();

    const bool started = QProcess::startDetached(exeInfo.absoluteFilePath(),
        game.arguments,
        workingDir);

    if (!started)
    {
        showConfigMessage("Launch Failed",
            QString("The executable exists, but Windows could not start it.\n\nGame:\n%1\n\nPath:\n%2")
                .arg(game.name, exePath));
    }
}

template<typename T>
void MainWindow::rollList(QList<T>& oldList, int dir, int count)
{
    for (int i = 0; i < count; i++)
    {
        if (dir == 1)
        {
            oldList.prepend(oldList.last());
            oldList.removeLast();
        }
        else if (dir == -1)
        {
            oldList.append(oldList.first());
            oldList.removeFirst();
        }
    }
}
