#include "searchMenu.h"
#include "LangDefine.h"
#include "Front.h"
#include "MarketPage.h"
////////////////////////////////////////////////////////////////////////////////
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QPushButton>
#include <QCompleter>
#include <QLineEdit>
#include <QTreeView>
#include <QHeaderView>
#include <QScrollBar>
namespace itstation {;
    ;
////////////////////////////////////////////////////////////////////////////////
ItermModel::ItermModel(QObject *parent) :QStandardItemModel(parent)
{
    connect(this, SIGNAL(updateData(const QList<QStringList>&)),
        this, SLOT(setData(const QList<QStringList>&)));
}

////////////////////////////////////////////////////////////////////////////////
QVariant ItermModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (role == Qt::TextAlignmentRole)
    {
        if (index.column() == 2) {
            return int(Qt::AlignRight | Qt::AlignVCenter);
        }
        else {
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        }
    }
    else if (role == Qt::FontRole) 
    {
        return QFont(tr("微软雅黑"), 10, 60);
    }
    else if (role == Qt::DisplayRole)
    {
        return m_data_list.at(index.row()).at(index.column());
    }
    return QVariant();
}

////////////////////////////////////////////////////////////////////////////////
QStringList ItermModel::atRow(int r) const
{
    if (r >= row()) return QStringList();
    return m_data_list.at(r);
}

void ItermModel::setData(const QList<QStringList>& data)
{   
    beginResetModel();
    m_data_list.clear();
    m_data_list.reserve(data.size());
    m_data_list = data;
    setRowCount(m_data_list.size());
    endResetModel();   
}

////////////////////////////////////////////////////////////////////////////////
searchMenu::searchMenu(QWidget *parent, MarketPage* ctl) 
    : QLineEdit(parent),m_maket_page(ctl)
{   
    setFixedSize(280, 30);
    // 设置搜索框的样式
    auto layout = new QHBoxLayout;
    m_serch_back = new QPushButton(this);
    m_serch_back->setFixedSize(13, 13);
    m_serch_back->setCursor(Qt::PointingHandCursor);
    m_serch_back->setStyleSheet(
        "QPushButton{border-image:url(:image/search.png);"
        "background:transparent;cursor:pointer;}");
    setPlaceholderText(tr(LOC(v6k10)));
    layout->addStretch();
    layout->addWidget(m_serch_back);
    setTextMargins(3, 0, m_serch_back->width() * 2 + 10, 0);
    setLayout(layout);
    setContentsMargins(0, 0, 0, 0);
    // 设置搜索框背景颜色
    setStyleSheet("QLineEdit{background-color: rgb(32, 33, 42); "
        /*"border:1px solid #eaeaea;border-radius:14px;"*/
        "color:rgb(218, 222, 222);};");
    // 设置自动补全
    m_model = new ItermModel(this);
    m_model->setColumnCount(3);
    m_pop_view = new QTreeView(this);
    m_pop_view->setRootIsDecorated(false);
    m_pop_view->setUniformRowHeights(true);
    m_pop_view->setAllColumnsShowFocus(true);
    m_pop_view->setHeaderHidden(true);
    m_pop_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pop_view->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pop_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pop_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pop_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   
    m_pop_view->setMouseTracking(true); 
    auto delegate = new ItermDelegate(this);
    m_pop_view->setItemDelegateForColumn(0, delegate);
    m_pop_view->setItemDelegateForColumn(1, delegate);
    m_pop_view->setItemDelegateForColumn(2, delegate);
    m_pop_view->header()->setSectionResizeMode(QHeaderView::ResizeToContents);   
    m_pop_view->setStyleSheet("QTreeView{color: rgb(222, 222, 222);"        
        "selection-color: rgb(208, 202, 202);"
        "selection-background-color: rgb(84, 84, 93);"
        "background-color: rgb(32, 33, 42);}");  
    m_completer = new QCompleter(m_model,this);
    m_completer->setCaseSensitivity(Qt::CaseInsensitive);
    m_completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    m_completer->setPopup(m_pop_view);
    m_completer->setMaxVisibleItems(10);
    setCompleter(m_completer); 
    // 事件监听
    connect(m_serch_back, SIGNAL(clicked(bool)), this, SLOT(onClear(bool)));
    connect(this, SIGNAL(textChanged(const QString&)),
        this, SLOT(onTextChanged(const QString&)));        
    connect(m_pop_view, SIGNAL(clicked(const QModelIndex&)),
        this, SLOT(onActivated(const QModelIndex&))); 
}

////////////////////////////////////////////////////////////////////////////////
bool searchMenu::Init(int scope)
{
    // 获取合约信息
    auto product_set = get_front().get_product_set();
    if (!product_set.size()) { ECO_ERROR << "product_set.size == 0"; false; }

    // 初始化命令
    QStringList proxy_list;
    proxy_list << "1" << tr("1") + LOC(v6k2) << tr(LOC(v6k9));
    m_comands.push_back(proxy_list);
    proxy_list.clear();
    proxy_list << "3" << tr("3") + LOC(v6k2) << tr(LOC(v6k9));
    m_comands.push_back(proxy_list);
    proxy_list.clear();
    proxy_list << "5" << tr("5") + LOC(v6k2) << tr(LOC(v6k9));
    m_comands.push_back(proxy_list);
    proxy_list.clear();
    proxy_list << "15" << tr("15") + LOC(v6k2) << tr(LOC(v6k9));
    m_comands.push_back(proxy_list);
    proxy_list.clear();
    proxy_list << "30" << tr("30") + LOC(v6k2) << tr(LOC(v6k9));
    m_comands.push_back(proxy_list);
    proxy_list.clear();
    proxy_list << "60" << tr("60") + LOC(v6k2) << tr(LOC(v6k9));
    m_comands.push_back(proxy_list);
    proxy_list.clear();
    proxy_list << "d" << tr(LOC(v6k4)) << tr(LOC(v6k9));
    m_comands.push_back(proxy_list);
    proxy_list.clear();
    proxy_list << "w" << tr(LOC(v6k5)) << tr(LOC(v6k9));
    m_comands.push_back(proxy_list);
    proxy_list.clear();
    proxy_list << "m" << tr(LOC(v6k6)) << tr(LOC(v6k9));
    m_comands.push_back(proxy_list);
    proxy_list.clear();
    proxy_list << "q" << tr(LOC(v6k7)) << tr(LOC(v6k9));
    m_comands.push_back(proxy_list);
    proxy_list.clear();
    proxy_list << "y" << tr(LOC(v6k8)) << tr(LOC(v6k9));
    m_comands.push_back(proxy_list);

    // 初始化字典: 期货
    if (scope & SCOPE_FUTURE)
    {
        for (auto it = product_set.begin(); it != product_set.end(); ++it)
        {
            auto targets = get_front().get_target_list((*it)->pid());
            if (!targets) { continue; }

            for (auto itt = targets->begin(); itt != targets->end(); ++itt)
            {
                QString target_id = (**itt).target_id().c_str();
                char buffer[1024] = "";
                sprintf(buffer, "%-3s", (**itt).target_name().c_str());
                QString target_name = buffer;
                auto exchange = 
                    get_front().get_product_exchange((**itt).pid());
                if (exchange == nullptr)
                {
                    ECO_ERROR << "exchange_info is nullptr";
                    return false;
                }
                sprintf(buffer, "%-3s", exchange->name().c_str());
                QString exchange_name = buffer;
                QStringList proxy_list;
                proxy_list << target_id << target_name << exchange_name
                    << QString("%1").arg((**itt).tid());
                m_words.push_back(proxy_list);
            }
        }
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
void searchMenu::onActivated(const QModelIndex&)
{
    formatFactory(m_pop_view->currentIndex());
}

////////////////////////////////////////////////////////////////////////////////
void searchMenu::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    if (Qt::Key_Enter == key ||
        Qt::Key_Return == key)  onActivated(QModelIndex());
    if (Qt::Key_Escape == key)  onClear(true);
    QLineEdit::keyPressEvent(event);
}

////////////////////////////////////////////////////////////////////////////////
void searchMenu::formatFactory(QModelIndex index)
{
    if (!index.isValid())
    {
        if (!m_model->row()) return;
        index = m_model->index(0, 0);
    }
    int row = index.row();
    if (row >= m_model->row() || m_maket_page == nullptr) return;
    auto& current_list = m_model->atRow(row);
    if (current_list.size() < 4) return;
    onClear(true);
    auto function = current_list.at(2).toStdString();
    auto tid = current_list.at(3).toULongLong();   
    if (function == std::string(LOC(v6k9)))
    {
        // K线
        std::string target_id = current_list.at(0).toStdString();
        int func_no = eco::cast<int>(target_id);
        if (func_no != 0)
            m_maket_page->showMarket(vision::ktype_minute, func_no);
        else if (target_id == "y")
            m_maket_page->showMarket(vision::ktype_year, 1);
        else if (target_id == "s")
            m_maket_page->showMarket(vision::ktype_season, 1);
        else if (target_id == "m")
            m_maket_page->showMarket(vision::ktype_month, 1);
        else if (target_id == "w")
            m_maket_page->showMarket(vision::ktype_week, 1);
        else if (target_id == "d")
            m_maket_page->showMarket(vision::ktype_day, 1);
    }
    else
    {
        // 切换合约
        m_maket_page->selectTarget(tid);
    }
}

void searchMenu::focusInEvent(QFocusEvent* e)
{      
    QLineEdit::focusInEvent(e);
#if QT_CONFIG(completer)
    // 以下槽函数不需要链接，但Qt QLineEdit（基类）做了很多地方做了链接，
    // 且当前方法每次都会重新连接一次，无奈只好这样处理。
    m_completer->disconnect(m_completer, SIGNAL(activated(QString)),
        this, SLOT(setText(QString)));
    m_completer->disconnect(m_completer, SIGNAL(highlighted(QString)),
        this, SLOT(_q_completionHighlighted(QString)));
#endif        
}
void searchMenu::focusOutEvent(QFocusEvent* e)
{   
    setText("");
    QLineEdit::focusOutEvent(e);
}

////////////////////////////////////////////////////////////////////////////////
void searchMenu::onTextChanged(const QString& current_text)
{
    // 样式处理
    if (current_text.isEmpty()) {
        m_serch_back->setStyleSheet(
            "QPushButton{border-image:url(:image/search.png);"
            "background:transparent;cursor:pointer;}");
        return; 
    }
    else {
        m_serch_back->setStyleSheet(
            "QPushButton{border-image:url(:image/clear.png);"
            "background:transparent;cursor:pointer;}");
    }
    QList<QStringList> data_set;
    // 命令处理
    foreach(QStringList comand, m_comands)
    {
        if (comand[0].startsWith(current_text, Qt::CaseInsensitive))
        {
            QStringList proxy_list;
            proxy_list << comand[0] << comand[1] << comand[2] << "";
            data_set.push_back(proxy_list);
            if (data_set.size() > 9) { break; }
        }
    }
    // 文字泛型处理
    foreach(QStringList word, m_words)
    {
        if (word[0].startsWith(current_text, Qt::CaseInsensitive) ||
            !current_text.at(0).isNumber() &&
            word[1].startsWith(current_text, Qt::CaseInsensitive))
        {
            QStringList proxy_list;
            proxy_list << word[0] << word[1] << word[2] << word[3];
            data_set.push_back(proxy_list);
            if (data_set.size() > 9) { break; }
        }
    }
    // 文字泛型处理
    foreach(QStringList word, m_words)
    {
        QString item;
        QChar first_char = current_text.at(0);
        if (word[0].contains(current_text, Qt::CaseInsensitive) ||
            word[1].contains(current_text, Qt::CaseInsensitive))
        {
            bool is_exist = false;
            foreach(QStringList proxy_list, data_set)
            {
                if (word[0] == proxy_list.at(0) && word[1] == proxy_list.at(1))
                {
                    is_exist = true;
                    break;
                }
            }
            if (is_exist) { continue; }
            QStringList proxy_list;
            proxy_list << word[0] << word[1] << word[2] << word[3];
            data_set.push_back(proxy_list);
            if (data_set.size() > 9) { break; }
        }
    }
    emit m_model->updateData(data_set);
}

////////////////////////////////////////////////////////////////////////////////
void searchMenu::onClear(bool)
{
    setText("");
    clearFocus();
}
////////////////////////////////////////////////////////////////////////////////
; }
