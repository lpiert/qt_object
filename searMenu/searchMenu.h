#ifndef SEARCH_MENU_H_
#define SEARCH_MENU_H_

#include <QLineEdit>
#include <QStandardItemModel>
#include <QItemDelegate>
#include <QItemSelectionModel>
////////////////////////////////////////////////////////////////////////////////
class QStandardItemModel;
class QPushButton;
class QCompleter;
class QTreeView;
////////////////////////////////////////////////////////////////////////////////
namespace itstation {
    ;
////////////////////////////////////////////////////////////////////////////////
class ItermDelegate : public QItemDelegate
{
public:
    explicit ItermDelegate(QObject *parent = nullptr) : QItemDelegate(parent)
    {}
    QSize sizeHint(const QStyleOptionViewItem &option,
        const QModelIndex &index) const override
    {
        QSize size = QItemDelegate::sizeHint(option, index);
        return QSize(size.width(),size.height() * 1.3);
    }
};

////////////////////////////////////////////////////////////////////////////////
class ItermModel : public QStandardItemModel
{
    Q_OBJECT
signals:
    void updateData(const QList<QStringList>&);
private slots:
    void setData(const QList<QStringList>&);
public:
    explicit ItermModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &index, 
        int role = Qt::DisplayRole) const override;
    int row() const { return m_data_list.size(); }
    QStringList atRow(int row) const;
private:
    QList<QStringList> m_data_list;
};

////////////////////////////////////////////////////////////////////////////////
class MarketPage;
class searchMenu : public QLineEdit
{
    Q_OBJECT
private slots:
    void onTextChanged(const QString&);   
    void onActivated(const QModelIndex&);
    void onClear(bool);
public:
    // 枚举值用于状态判断
    enum SymbolScope
    {
        SCOPE_STOCK = 0X0001,
        SCOPE_FUTURE = 0X0010,
        SCOPE_SPOT = 0X0100,
        SCOPE_OPTION = 0X1000,
        SCOPE_ALL = SCOPE_STOCK | SCOPE_FUTURE | SCOPE_SPOT | SCOPE_OPTION
    };
    // 构造
    explicit searchMenu(QWidget *parent,MarketPage* ctl);
    // 初始化
    bool Init(int scope = SCOPE_ALL);
    // 解析
    void formatFactory(QModelIndex index);
    // 焦点事件
    virtual void focusInEvent(QFocusEvent *) override;
    virtual void focusOutEvent(QFocusEvent *) override;
    // 键盘事件
    virtual void keyPressEvent(QKeyEvent *) override;

public:
    // 侧边栏与行情列表   
    MarketPage* m_maket_page;
private:
    QList<QStringList> m_comands;
    QList<QStringList> m_words;
    QCompleter* m_completer;
    ItermModel* m_model;
    QTreeView* m_pop_view;
    QPushButton* m_serch_back;
    QString m_current_text;  
};

////////////////////////////////////////////////////////////////////////////////
;}
#endif