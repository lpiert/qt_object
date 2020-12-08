#ifndef VISION_SLIDE_PUSH_H_
#define VISION_SLIDE_PUSH_H_
#include <QObject>



QT_BEGIN_NAMESPACE
class QPropertyAnimation;
class QParallelAnimationGroup;
class QHBoxLayout;
QT_END_NAMESPACE
////////////////////////////////////////////////////////////////////////////////
// ������̬Ч��
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
	// ����
	explicit SlidePush(
		QWidget* parent = nullptr,
		QObject* target = nullptr,
		const QByteArray &propertyName = "pos",
		RunMode model = fromTop,
		double speed = 0.5);
	// ��ʼ��
	void init();
	// ����
	void slidePop();
	// ����
	void slideBack();
	// ���ö�������
	void setTargetObject(QObject *target, RunMode m = RunMode::fromLeft);
	// λ����ʾģ��
	void setMode(RunMode m);
	
private:
	// �ٶ�
	double m_speed;
	// λ�����Զ���
	QPropertyAnimation* m_pos;
	// ��ʾλ��
	RunMode m_pos_mode;
	// ������ɱ��
	bool m_back_state;
	
};

////////////////////////////////////////////////////////////////////////////////
#endif 
