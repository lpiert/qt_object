#ifndef ITSTATION_OTHERPAG_H_
#define ITSTATION_OTHERPAG_H_
#include "Page.h"
#include <boost/date_time/posix_time/posix_time.hpp> 
#include <QDesktopWidget>
#include <QApplication>
#include <lang/LangPack.h>

class QWidget;
class QLabel;
class QPixmap;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QTextEdit;
class QLineEdit;
namespace itstation{
////////////////////////////////////////////////////////////////////////////////
// 结算单
class SettlementList : public QWidget
{
	Q_OBJECT
private slots:
	void onChanged(int);
	void onClicked(int);
	void resetPromptInfo();
public slots:
	void onUpdateList(std::string&);

public:
	SettlementList(QWidget* parent);
	inline 	QTextEdit* getList(){ return m_list; }
	
private:
	boost::posix_time::ptime m_time_just;
	QLineEdit* m_line_edit;
	QTextEdit* m_list;
	SingleOption* format_choose;
	QLabel* m_edit;
};


////////////////////////////////////////////////////////////////////////////////
// 持仓库明细
class HdwareHouseDetail : public QWidget
{
	Q_OBJECT
signals:
	void selectFinished();
private slots:
	void onClicked(int);
	void onSelectFinished();
	void resetPromptInfo();
public:
	HdwareHouseDetail(QWidget* parent);
	inline 	QTextEdit* getList()
	{
		return m_list;
	}

private:
	QTextEdit* m_list;
	boost::posix_time::ptime m_time_just;
	QLabel* m_text_edit;
};


////////////////////////////////////////////////////////////////////////////////
// 查询功能
class QueryPage :public QFrame
{
	Q_OBJECT
private slots:
	void onBtnExport(int);
signals:
	void updateList(QString);

public:
	QueryPage(QWidget* parent);

	// 绑定事件与加载数据
	void init();

	inline void setUpdateInfo(QString current) { emit updateList(current);}

public:
	SettlementList* m_settle_list;
	HdwareHouseDetail* m_detail;
	PushButton* m_export_btn;
	TabPage* m_tab;
	QRect  m_rect;
};


////////////////////////////////////////////////////////////////////////////////
// 密码修改 
class ModifyPage :public QFrame
{
	Q_OBJECT
signals:
	void modifyError(QString);
private slots:
	void onClicked(int);
	void onModifyError(QString);

public:
	ModifyPage(QWidget* parent);

	// 绑定事件与加载数据
	void init();

	inline void setModifyError(QString msg)
	{
		emit modifyError(msg);
	}
	inline void clear()
	{
		m_old_pwd_edit->clear();
		m_new_pwd_edit->clear();
		m_again_pwd_edit->clear();
	}

private:
	QLineEdit* m_old_pwd_edit;
	QLineEdit* m_new_pwd_edit;
	QLineEdit* m_again_pwd_edit;
	QLabel* m_label;
	QPixmap m_bgr;
	PushButton* m_modify_btn;
	QRect m_rect;
};
////////////////////////////////////////////////////////////////////////////////
// 交易界面设置
class TradeSetPage : public QWidget
{	
	Q_OBJECT
public:
    enum 
    {
        close_Prompt = 1,
        open_key_order = 2
    };
	TradeSetPage(QWidget* parent = nullptr);		  
	CheckBox * m_one_click_order;
    CheckBox * m_key_order;
	ComboBox* m_default_price;
	
};
// 系统参数设置
class SystemSetPage : public QWidget
{
	Q_OBJECT
private slots:
	void onChanged(const QString &);
public:
	SystemSetPage(QWidget* parent = nullptr);
	std::string curr_lang();
	void set_curr_lang(std::string text);
	void init();
private:
	ComboBox* m_lang_box;
	std::unordered_map<std::string, vision::lang::LangType> m_lang_map;
};

////////////////////////////////////////////////////////////////////////////////
// 参数设置			
class ParameterSetPage : public QDialog
{
	Q_OBJECT
private slots:
	void onChecked(int, bool);
	void onClicked(int);
public:
	ParameterSetPage(QWidget* parent=nullptr);
	~ParameterSetPage();
	void closeEvent(QCloseEvent *) override;

public:
	TabPage* m_tab;
	TradeSetPage* m_trade_set_page;
	SystemSetPage* m_system_set_page;
	PushButton* m_make_sure_btn;
	QLabel* m_labe_title;
};

////////////////////////////////////////////////////////////////////////////////
};

////////////////////////////////////////////////////////////////////////////////
#endif