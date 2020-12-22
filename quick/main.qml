import QtQuick 2.5
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.4
import QtWebChannel 1.0
import QtWebEngine 1.1
import WebControl 1.0

// 构建类型： 窗体
Window {
    // 标志
    id: main_window
    // 设置可显示
    visible: false
    // 宽
    width: 800
    // 最小宽度
    minimumWidth: 600
    // 高
    height: 500
    // 最小高度
    minimumHeight: 400
    // 标题
    title: qsTr("WebChannel example") //countDown.start();
    // 成员变量声明及初始化
    property int y_: 1320

    // 布局： 列布局
    ColumnLayout {
        // 填充设置
        anchors.fill: parent
        anchors.margins: 5

        // 一个具有属性、信号和方法的对象——就像任何普通的Qt对象一样
        QtObject {
            // 当前对象ID
            id: jsHelper
            // 管道ID与 WebEngineView(.html)一致
            WebChannel.id: "backend"
            // 信号声明
            signal signal_insert_data(int y, int x)
            signal signal_update_option(string opt)
        }

        Connections {
            target: WebControl
            onDataChanged: {
                console.log("data changed" + WebControl.data)
            }
            onStyleChanged: {
                console.log("data changed" + WebControl.style)
            }
        }

        // 构建矩形
        Rectangle {
            // 使用布局填充宽高
            Layout.fillWidth: true
            Layout.fillHeight: true
            // 边界样式设置：宽度&颜色
            border.width: 1
            border.color: "#5d486b"
            // web 引擎加载显示
            WebEngineView {
                // 当前视图ID
                id: webView
                // 填充设置
                anchors.fill: parent
                anchors.margins: 5
                // 设置需要加载的资源地址
                url: "qrc:/web/index.html"
                // 响应加载变化
                onLoadingChanged: {
                    if (loadRequest.errorString) {
                        console.error(loadRequest.errorString)
                    }
                }
                // 构建管道成员
                webChannel: channel
            }
            // 构建管道对象
            WebChannel {
                // 当前对象ID
                id: channel
                // 注册助手
                registeredObjects: [jsHelper]
            }
        }
        // 可操作按钮：功能，点击为当前视图添加数据
        Button {
            id: button_web_channel
            text: qsTr("添加数据")
            onClicked: {
                //jsHelper.signal_insert_data(y_++)
                var style = {
                    "title": {
                        "text": "test"
                    }
                }
                jsHelper.signal_update_option(JSON.stringify(style))
            }
        }
        // 可操作按钮：功能，点击为当前视图添加数据
        Button {
            id: button_do_js
            text: qsTr("改变样式")
            onClicked: {
                console.log("button_do_js::onClicked")
                var title = "update_chart_option(" + "{ title: { text: 'fuck' } }" + ");"
                webView.runJavaScript(title)
            }
        }

        // 可操作按钮：功能，点击调用单例
        Button {
            id: button_do_cpp
            property int index: 1
            text: qsTr("总控")
            onClicked: {
                console.log("button_do_cpp::onClicked")
                WebControl.setData(index++)
                WebControl.setStyle(index++)
            }
        }
    }
}
