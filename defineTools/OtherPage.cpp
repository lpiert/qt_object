#include "OtherPage.h"
#include <QLabel>
#include <QPainter>
#include <QLineEdit>
#include <QStyle>
#include <QBoxLayout>
#include <QtCore/QVariant>
#include <QMouseEvent>
#include <QListWidget>
#include <QPalette>
#include <QDesktopServices>
#include <QUrl>
#include "Typedef.h"
#include <QMessageBox>
#include <QTextEdit>
#include <QTextStream>
#include <QLineEdit>
#include <eco/Cast.h>
#include <QFileDialog>
#include <fstream>
#include "Front.h"
#include "PrecHeader.h"
#include "App.h"
#include "ParameterXmlHandel.h"
#include "OrderInsertPage.h"
#include <QTextCodec>

namespace itstation{

//##############################################################################
//##############################################################################
// 结算单
SettlementList::SettlementList(QWidget* parent) :QWidget(parent)
{
	//字体处理
	QPalette pe;
	pe.setColor(QPalette::WindowText,Qt::blue);
	//还需根据日结单和月结单做切换
	m_edit = new QLabel(tr(LOC(v19k11)),this);
	m_edit->setGeometry(7,60,132,27*5);
	m_edit->setPalette(pe);
	m_edit->setWordWrap(true);
	m_edit->setAlignment(Qt::AlignTop);
	//添加查询/打印 按钮
	auto* query_btn = new PushButton(tr(LOC(v19k8)),this);
	query_btn->setGeometry(7,150,82,25);
	connect(query_btn,SIGNAL(clicked(int)),this,SLOT(onClicked(int)));

	// 日结单/月结单
	ComboBox*  dayOrmonth_list = new ComboBox(this);
	dayOrmonth_list->setGeometry(7, 7, 135, 20);
	dayOrmonth_list->setEditable(false);
	dayOrmonth_list->addItem(tr(LOC(v19k3)));
	dayOrmonth_list->addItem(tr(LOC(v19k4)));
	dayOrmonth_list->setProperty("class", "trade_edit");
 	//添加显示框
	m_line_edit = new QLineEdit(this);
	m_line_edit->setGeometry(146,7,64,20);
	//添加 逐笔/盯市
	format_choose = new SingleOption(this);
	format_choose->addItem(tr(LOC(v19k6)), this, 1,true)
		->setGeometry(7, 40, 200, 15);
	format_choose->addItem(tr(LOC(v19k7)), this, 2,false)
		->setGeometry(86, 40, 200, 15);
	// 添加列表 
	QHBoxLayout* layout = new QHBoxLayout;
	setLayout(layout);
	m_list = new QTextEdit(this);
	layout->addSpacing(230);
	layout->setMargin(2);
	layout->addWidget(m_list);
	m_list->setReadOnly(true);
	m_list->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	m_list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn); 
	m_list->setWordWrapMode(QTextOption::NoWrap);
	connect(dayOrmonth_list,SIGNAL(currentIndexChanged(int)),this,SLOT(onChanged(int)));
}

////////////////////////////////////////////////////////////////////////////////
void SettlementList::onChanged(int index)
{
	QString str =tr(LOC(v19k11));
	QString stry = tr(LOC(v19k12));
	if(index == 1)
	{
		m_edit->setText(stry);
	}else{
		m_edit->setText(str);
	}
}

////////////////////////////////////////////////////////////////////////////////
void SettlementList::resetPromptInfo()
{
	m_edit->clear();
}

////////////////////////////////////////////////////////////////////////////////
void SettlementList::onClicked(int flag)
{
	// 设置流控提示
	boost::posix_time::ptime time_now;
	boost::posix_time::millisec_posix_time_system_config::time_duration_type
		time_elapse;
	// 这里精确到秒second_clock
	time_now = boost::posix_time::second_clock::universal_time();
	time_elapse = time_now - m_time_just;
	// 得到两个时间间隔的秒数; 
	if (time_elapse.total_seconds() < 10)
	{
		// 提示
		QTimer::singleShot(10000, this, SLOT(resetPromptInfo()));
		m_edit->setText(LOC(v19k13));
		return;
	}
	m_list->clear();
	m_edit->setText(tr(LOC(v19k10)));
	std::string trading_day = m_line_edit->text().toStdString();
	get_front().get_settlement(trading_day);
	// 记录上次查询时间
	m_time_just = time_now;
}

////////////////////////////////////////////////////////////////////////////////
void SettlementList::onUpdateList(std::string& current)
{
	m_edit->setText(tr(LOC(v19k11)));
	//	结算单查询
	QString& result = QString("%1\n%2").
		arg(lang::get_lang().get_license()).arg(current.c_str());
	m_list->append(result);
}
////////////////////////////////////////////////////////////////////////////////
// 持仓明细
HdwareHouseDetail::HdwareHouseDetail(QWidget* parent) :QWidget(parent)
{
	m_text_edit = new QLabel(this);
	QPalette pe;
	pe.setColor(QPalette::WindowText, Qt::red);
	m_text_edit->setPalette(pe);
	m_text_edit->setWordWrap(true);
	m_text_edit->setGeometry(7, 20, 150, 80);
	auto* m_query_btn = new PushButton(tr(LOC(v19k8)),this);
	m_query_btn->setGeometry(7,150,82,25);
	connect(m_query_btn,SIGNAL(clicked(int)),this,SLOT(onClicked(int)));

	// 添加列表
	QHBoxLayout* layout = new QHBoxLayout;
	setLayout(layout);
	m_list = new QTextEdit(this);
	m_list->setReadOnly(true);
	layout->addSpacing(158);
	layout->setMargin(2);
	layout->addWidget(m_list);
	m_list->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	m_list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	m_list->setWordWrapMode(QTextOption::NoWrap);
	connect(this, SIGNAL(selectFinished()), this, SLOT(onSelectFinished()));
}

////////////////////////////////////////////////////////////////////////////////
void HdwareHouseDetail::resetPromptInfo()
{
	m_text_edit->clear();
}

////////////////////////////////////////////////////////////////////////////////
void HdwareHouseDetail::onClicked(int flag)
{
	// 设置流控提示
	boost::posix_time::ptime time_now;
	boost::posix_time::millisec_posix_time_system_config::time_duration_type 
		time_elapse;
	// 这里精确到秒second_clock
	time_now = boost::posix_time::second_clock::universal_time();
	time_elapse = time_now - m_time_just;
	// 得到两个时间间隔的秒数; 
	if (time_elapse.total_seconds()< 10)
	{
		// 提示
		m_text_edit->setText(LOC(v19k13));
		QTimer::singleShot(10000, this, SLOT(resetPromptInfo()));
		return;
	}
	//持仓明细查询
	get_front().get_position_detail();
	// 记录上次查询时间
	m_time_just = time_now;
}
void HdwareHouseDetail::onSelectFinished()
{
	// 清空列表
	m_list->clear();
	m_text_edit->clear();
	auto& iterm = get_front().m_detail_set;
	if (!iterm.size()) {return;}
	bool is_zh = BROADEN;
	//数据处理
	const char* title = is_zh ?
		"%-20s|%-14s|%-13s|%-15s|%-14s|%-15s|%-12s|"
		"%-16s|%-20s|%-17s|%-17s|%-17s|%-17s|%-22s|%-17s|%-14s" : 
		"%-20s|%-14s|%-13s|%-15s|%-14s|%-15s|%-12s|"
		"%-16s|%-20s|%-40s|%-40s|%-40s|%-40s|%-22s|%-28s|%-14s";

	char buffer[1024] = {0};
	sprintf(buffer,title,
		LOC(v19tk1), LOC(v19tk2), LOC(v19tk3),
		LOC(v19tk4), LOC(v19tk5), LOC(v19tk6),
		LOC(v19tk7), LOC(v19tk8), LOC(v19tk9),
		LOC(v19tk10),//(std::string(LANG(lang_markingmarket)) + LOC(k240)).c_str(),
		LOC(v19tk11),//(std::string(LANG(lang_hedging)) + LOC(k240)).c_str(),
		LOC(v19tk12),//(std::string(LANG(lang_markingmarket)) + LOC(k239)).c_str(),
		LOC(v19tk13),//(std::string(LANG(lang_hedging)) + LOC(k239)).c_str(),
		LOC(v19tk14),
		LOC(v19tk15),//(std::string(LOC(k7)) + LANG(lang_price)).c_str(),
		LOC(v19tk16));
	m_list->append(buffer);
	const char* splite = 
		"----------------------------------------------------------"\
		"----------------------------------------------------------"\
		"----------------------------------------------------------"\
		"----------------------------------------------------------"\
		"----------------------------------------------------------"\
		"----------------------------------------------------------"\
		"----------------------------------------------------------";
	m_list->append(splite);
	const char* contens = is_zh ?
		"%-16s|%-12s|%-12s|%-12s|%-12s|%-12s|%-12s|"
		"%-16s|%-16s|%-16s|%-16s|%-16s|%-16s|%-16s|%-12s|%-12s" : 
		"%-20s|%-14s|%-13s|%-15s|%-14s|%-15s|%-12s|"
		"%-16s|%-20s|%-40s|%-40s|%-40s|%-40s|%-22s|%-28s|%-14s";
 	for(auto it = iterm.begin();it!= iterm.end();++it)
 	{	
		
		const char* m_direction = 
			(it->get_id().get_direction()  == vision::direct_buy) ? 
			LOC(v13p3k9):LOC(v13p3k10);
		const char* m_tradeType =  LOC(v19tk8);
		const char* m_hedgeFlag = (it->get_id().get_hedge() == '1') ?
			LOC(v19tk17) : LOC(v19tk18);
		char trVal[50] = "";
		itoa(it->get_volume(),trVal,10);
		char trCloseVal[50] = "";
		itoa(it->get_close_volume(),trCloseVal,10);
		sprintf(buffer, contens,
			it->get_open_date(), it->get_id().get_target_id(), m_direction, trVal,
			eco::Double(it->get_open_price(), 2).c_str(),
			trCloseVal, m_hedgeFlag, m_tradeType, it->get_trade_id(),
			eco::Double(it->get_position_profit_by_date(), 2).c_str(),
			eco::Double(it->get_position_profit_by_trade(), 2).c_str(),
			eco::Double(it->get_close_profit_by_date(), 2).c_str(),
			eco::Double(it->get_close_profit_by_trade(), 2).c_str(),
			eco::Double(it->get_user_margin(), 2).c_str(),
			eco::Double(it->get_last_settlement_price(), 2).c_str(),
			eco::Double(it->get_settlement_price(), 2).c_str());
		m_list->append(buffer);
	}
	iterm.clear();
}


//##############################################################################
//##############################################################################
QueryPage::QueryPage(QWidget* parent) :QFrame(parent)
{	
	this->setWindowFlags(Qt::FramelessWindowHint);  
	this->setAttribute(Qt::WA_TranslucentBackground);  
	auto mini_h = QPixmap(":/image/flash_order_page.png").height();
	setMinimumHeight(mini_h);
	// 下单Tab页签
	QHBoxLayout* h_layout = new QHBoxLayout;
	h_layout->setSpacing(0);
	h_layout->setMargin(0);
	m_tab = new TabPage(this);
	m_tab->addPage(0,tr(LOC(v19k1)),m_settle_list = new SettlementList(this));
	m_tab->addPage(1,
		tr(LOC(v19k2)),m_detail = new HdwareHouseDetail(this));
	// 导出按钮
	m_tab->getTabCtr()->setBtn(m_export_btn =
						new PushButton(tr(LOC(v19k9)),this));
	m_tab->showPage(0);
	h_layout->addWidget(m_tab);
	setLayout(h_layout);
	connect(m_export_btn,SIGNAL(clicked(int)),this,SLOT(onBtnExport(int)));
}


////////////////////////////////////////////////////////////////////////////////
void QueryPage::init()
{}


////////////////////////////////////////////////////////////////////////////////
void QueryPage::onBtnExport(int flag)
{
	QTextEdit* iterm = NULL;
	int flag_t = m_tab->getFlagPage();
	switch(flag_t)
	{
		//结算单/持仓明细
		case 0:
			iterm = m_settle_list->getList();
			//m_settle_list
			break;
		case 1:
			iterm = m_detail->getList();
			//m_detail;
			break;
		default:
			break;
	}
	//导出文件
	QString fileName =
		QFileDialog::getSaveFileName(this, getApp().login_page()->Title(),
			QString("C:\\%1.txt").arg(tr(LOC(v21k14))), tr("txt"));
	if(!fileName.isEmpty())
	{
		try
		{
			//将文本框数据取出并按行排列  
			QFile file(fileName, this);
			if (!file.open(QFile::WriteOnly | QFile::Text))
			{
				QMessageBox::information(this, tr(LOC(v21k17)),
					tr(LOC(v21k16)));
				return;
			}
			QTextStream out(&file);
			out.setCodec(QTextCodec::codecForName("utf-8"));
			if (iterm != nullptr)
			{
				out << iterm->toPlainText();
			}
			file.close();
		}
		catch (const std::exception& e)
		{
			ECO_FUNC(error) << e.what();

		}
	}

}


////////////////////////////////////////////////////////////////////////////////	
ModifyPage::ModifyPage(QWidget* parent) :QFrame(parent)
{
	m_bgr = QPixmap(":/image/modify_background.png");
	auto mini_h = QPixmap(":/image/flash_order_page.png").height();
	setMinimumHeight(mini_h);
	m_label = new QLabel(this);
	m_label->setPixmap(m_bgr);
	m_label->setScaledContents(true);
	m_label->setGeometry(0,0,m_bgr.width(),m_bgr.height());
	this->setStyleSheet("background: rgb(255, 255, 255)");
	// 交易密码
	QLabel* label = new QLabel(LOC(v21k1) + tr(":"),this);
	label->setGeometry(10, 0, 110, 30);
	ComboBox*  speculate_list = new ComboBox(this);
	speculate_list->setGeometry(127, 3, 150, 20);
	speculate_list->setEditable(false);
	speculate_list->addItem(tr(LOC(v21k5)));
	//添加密码输入框
	label = new QLabel(LOC(v21k2) + tr(":"),this);
	label->setGeometry(10, 28, 110, 30);
	m_old_pwd_edit = new QLineEdit(this);
	m_old_pwd_edit->setEchoMode(QLineEdit::Password);
	m_old_pwd_edit->setGeometry(127,31,150,20);
	
	label = new QLabel(LOC(v21k3) + tr(":"), this);
	label->setGeometry(10, 56, 110, 30);
	m_new_pwd_edit = new QLineEdit(this);
	m_new_pwd_edit->setEchoMode(QLineEdit::Password);
	m_new_pwd_edit->setGeometry(127,59,150,20);
	
	label = new QLabel(LOC(v21k4) + tr(":"), this);
	label->setGeometry(10, 84, 100, 30);
	m_again_pwd_edit = new QLineEdit(this);
	m_again_pwd_edit->setEchoMode(QLineEdit::Password);
	m_again_pwd_edit->setGeometry(127,87,150,20);
	//添加修改确认按钮
	m_modify_btn = new PushButton(tr(LOC(v21k6)),this);
	m_modify_btn->setGeometry(169,116,79,25);
	connect(m_modify_btn, SIGNAL(clicked(int)), this, SLOT(onClicked(int)));
	connect(this, SIGNAL(modifyError(QString)),
		this, SLOT(onModifyError(QString)));
}


////////////////////////////////////////////////////////////////////////////////
void ModifyPage::init()
{}

//////////////////////////////////////////////////////////////////////////////
void ModifyPage::onClicked(int flag)
{
	if(m_old_pwd_edit->text().isEmpty())
	{
 		// 请输入当前密码
		QMessageBox box(QMessageBox::Critical,
			tr(LOC(v21k12)), tr(LOC(v21k7)),NULL);
		box.addButton(tr(LOC(v21k13)), QMessageBox::AcceptRole);
		box.exec();
	}
	else if (m_new_pwd_edit->text().isEmpty())
	{
		// 请输入新密码
		QMessageBox box(QMessageBox::Critical, tr(LOC(v21k12)),
			tr(LOC(v21k8)), NULL);
		QPushButton* okBtn = box.addButton(
			tr(LOC(v21k13)), QMessageBox::AcceptRole);
		box.exec();
	}
	else if (m_new_pwd_edit->text()!=m_again_pwd_edit->text())
	{
		// 两次输入的新密码不一致，请重新输入
		QMessageBox box(QMessageBox::Critical, tr(LOC(v21k12)),
			tr(LOC(v21k9)), NULL);
		box.addButton(tr(LOC(v21k13)),QMessageBox::AcceptRole);
		box.exec();
	}
	else
	{
		auto new_pwd = m_new_pwd_edit->text().toStdString();
		auto old_pwd = m_old_pwd_edit->text().toStdString();
		// 用户口令更改
		get_front().update_user_passwd(new_pwd, old_pwd);
		
	}
}


////////////////////////////////////////////////////////////////////////////////
void ModifyPage::onModifyError(QString msg)
{
	clear();
	msg += "\t\n\r";
	QMessageBox::information(nullptr, tr(LOC(v21k12)), msg,tr(LOC(v21k13)));
}

////////////////////////////////////////////////////////////////////////////////

TradeSetPage::TradeSetPage(QWidget* parent) : QWidget(parent)
{
	setFocusPolicy(Qt::ClickFocus);
	m_one_click_order = new CheckBox(tr(LOC(v20k4)),this, close_Prompt);
    m_key_order = new CheckBox(tr(LOC(v20k21)),this,open_key_order);
	auto lable_text = new QLabel(tr(LOC(v20k3)),this);
    lable_text->setWordWrap(true);
	lable_text->setGeometry(15,15,400,40);
	lable_text = new QLabel(QString("%1:").arg(LOC(v20k5)),this);
    lable_text->setAlignment(Qt::AlignRight);
    lable_text->setGeometry(305, 65, 160, 20);
	m_default_price = new ComboBox(this);
	m_default_price->setMaxVisibleItems(20);
	m_default_price->setProperty("class", "u_target");
	m_default_price->addItem(tr(LOC(v13k16)));
	m_default_price->addItem(tr(LOC(v13k17)));
	m_default_price->addItem(tr(LOC(v13k18)));
	m_default_price->addItem(tr(LOC(v13k19)));
	m_default_price->addItem(tr(LOC(v13k20)));
	m_default_price->addItem(tr(LOC(v13k21)));
	m_default_price->setGeometry(470, 65, 160, 20);
	m_one_click_order->setGeometry(15,65,200,15);
    m_key_order->setGeometry(15,85,200,15);
	// 设置初始值
	std::string default_price = 
		get_para().at("tradingParameter/tradingInterface/priceType").c_str();
	if (!default_price.empty())
	{
		m_default_price->setCurrentIndex(atoi(default_price.c_str()));
	}
	m_one_click_order->setCheck(
		get_para().at("tradingParameter/tradingInterface/oneClickOrder"));
     m_key_order->setCheck(
         get_para().at("tradingParameter/tradingInterface/keyOrder"));
}

SystemSetPage::SystemSetPage(QWidget* parent) : QWidget(parent)
{
	QLabel* label = new QLabel(tr(LOC(v20k9)),this);
	label->setGeometry(10,10,100,30);
	m_lang_box = new ComboBox(this, false);
	m_lang_box->setMaxVisibleItems(10);
	m_lang_box->setStyleSheet("color:rgb(4,60,109);");
	m_lang_box->setProperty("class", "u_target");
	m_lang_box->setGeometry(10,50,160,20);
	init();
	connect(m_lang_box,SIGNAL(currentTextChanged(const QString &)),
		this,SLOT(onChanged(const QString &)));
}

void SystemSetPage::init()
{
	int size = vision::lang::get_lang().get_lang_type_size();
	for (int i = 0; i < size ;i++)
	{
		vision::lang::LangType& lang = vision::lang::get_lang().lang_type(i);
		m_lang_box->addItem(tr(lang.get_name()));
		m_lang_map[lang.get_type()] = lang;
	}
	auto def_lang = lang::get_lang().get_lang();
	if (!def_lang.empty()) set_curr_lang(def_lang.c_str());
	
}

void SystemSetPage::onChanged(const QString & text)
{
	QMessageBox box(QMessageBox::Information,
		tr(LOC(v20k14)), tr(LOC(v20k11)));
	QPushButton* cancel =
		box.addButton(LOC(v20k13), QMessageBox::NoRole);
	QPushButton* ok =
		box.addButton(LOC(v20k12), QMessageBox::ApplyRole);
	box.exec();
	if (box.clickedButton() == reinterpret_cast<QAbstractButton *>(ok))
	{
		get_para().m_lang_type.default_lang = curr_lang();
		if (get_para().setFeild("langType"))
		{
			getApp().restart();
		}
		else
		{
			QMessageBox::information(nullptr,tr(LOC(v21k19)),tr(LOC(v21k17)));
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
std::string SystemSetPage::curr_lang()
{
	for (auto& v: m_lang_map)
	{
		if (v.second.get_name() == m_lang_box->currentText()) return v.first;
	}
	return eco::empty_str;
}
void SystemSetPage::set_curr_lang(std::string text)
{
	auto it = m_lang_map.find(text);
	if (it != m_lang_map.end())
	{
		m_lang_box->setCurrentText(tr(it->second.get_name()));
	}
}

////////////////////////////////////////////////////////////////////////////////
ParameterSetPage::ParameterSetPage(QWidget* parent) 
	: QDialog(), m_system_set_page(nullptr)
{
	Qt::WindowFlags flags = Qt::Dialog;
	flags |= Qt::WindowMinMaxButtonsHint;
	setWindowFlags(flags);
	setFocusPolicy(Qt::ClickFocus);
	setProperty("class", "u_panel");
	m_make_sure_btn = new PushButton(tr(LOC(v20k8)), this);
	m_make_sure_btn->setGeometry(560, 440, 70, 20);
	m_labe_title = new QLabel(this);
	m_labe_title->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
	m_labe_title->setOpenExternalLinks(true);
	std::string help_site = getApp().get_config().get("main/help_site");
	auto& url = QString("%1<a href =%2>%3</a>").arg(LOC(v20k6)).
		arg(help_site.c_str()).arg(LOC(v20k7));
	m_labe_title->setText(url);
	m_labe_title->setGeometry(15, 440, 400, 20);
	resize(654, 472);
	setWindowFlags(Qt::WindowCloseButtonHint);
	setWindowTitle(tr(LOC(v20k10)));
	setProperty("class", "u_panel");
	setFocusPolicy(Qt::ClickFocus);
	m_tab = new TabPage(this);
	m_tab->addPage(0,
		tr(LOC(v20k1)), m_trade_set_page = new TradeSetPage(this));
	m_tab->addPage(1,
		tr(LOC(v20k2)), m_system_set_page = new SystemSetPage(this));
	m_tab->showPage(0);
	m_tab->setGeometry(0,0,this->width(),this->height()-40);
	connect(m_trade_set_page->m_one_click_order,
		SIGNAL(checked(int, bool)), this, SLOT(onChecked(int, bool)));
    connect(m_trade_set_page->m_key_order,
        SIGNAL(checked(int, bool)), this, SLOT(onChecked(int, bool)));
	connect(m_make_sure_btn,SIGNAL(clicked(int)), this, SLOT(onClicked(int)));
	if (parent) setWindowIcon(parent->windowIcon());
	
}

////////////////////////////////////////////////////////////////////////////////
ParameterSetPage::~ParameterSetPage()
{}

////////////////////////////////////////////////////////////////////////////////
void ParameterSetPage::onChecked(int flag, bool check)
{
    if ((flag == TradeSetPage::close_Prompt) && check)
    {
        QMessageBox::warning(nullptr, tr(LOC(v20k14)),
            tr(LOC(v20k15)), tr(LOC(v20k12)));
    }
    else if((flag == TradeSetPage::open_key_order))
    {
        getApp().insert_page()->showKeyOrder(check);
    }
	
}

////////////////////////////////////////////////////////////////////////////////
void ParameterSetPage::onClicked(int flag)
{
	int index = m_trade_set_page->m_default_price->currentIndex();
	bool one_order = m_trade_set_page->m_one_click_order->isChecked();
    bool key_order = m_trade_set_page->m_key_order->isChecked();
	getApp().insert_page()->modifyTraderSet(index, one_order, key_order);
	get_para().m_info.default_price = std::to_string(index);
	get_para().m_info.is_one_click_order = one_order;
    get_para().m_info.is_key_order = key_order;
	if (m_system_set_page != nullptr)
	{
		get_para().m_lang_type.default_lang = m_system_set_page->curr_lang();
	}
	get_para().save();
	close();
}

//////////////////////////////////////////////////////////////////////////////
void ParameterSetPage::closeEvent(QCloseEvent* e)
{
	getApp().order_page()->revertFocus();
}

//////////////////////////////////////////////////////////////////////////////
}
