#ifndef VISION_SLIDE_PUSH_H_
#define VISION_SLIDE_PUSH_H_
#include <QObject>



QT_BEGIN_NAMESPACE
class QPropertyAnimation;
class QParallelAnimationGroup;
class QHBoxLayout;
QT_END_NAMESPACE
////////////////////////////////////////////////////////////////////////////////
// 弹窗动态效果
class SlidePush : public QObject
{
	Q_OBJECT
public:
	enum RunMode
	{
		fromTop = 0x001,
		fromRight = 0x002,
		fromBottom = 0x003,
		fromLeft = 0x004,
	};
	// 构造
	explicit SlidePush(
		QWidget* parent = nullptr,
		QObject* target = nullptr,
		const QByteArray &propertyName = "pos",
		RunMode model = fromTop,
		double speed = 0.5);
	// 初始化
	void init();
	// 弹出
	void slidePop();
	// 隐藏
	void slideBack();
	// 设置动画主题
	void setTargetObject(QObject *target, RunMode m = RunMode::fromLeft);
	// 位置显示模型
	void setMode(RunMode m);
	
private:
	// 速度
	double m_speed;
	// 位置属性动画
	QPropertyAnimation* m_pos;
	// 显示位置
	RunMode m_pos_mode;
	// 隐藏完成标记
	bool m_back_state;
	
};

////////////////////////////////////////////////////////////////////////////////
#endif 
