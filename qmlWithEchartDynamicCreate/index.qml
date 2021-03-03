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
    color: "red"
    // 一个具有属性、信号和方法的对象——就像任何普通的Qt对象一样
    QtObject {
        // 当前对象ID
        id: jsHelper
        // 管道ID与 WebEngineView(.html)一致
        WebChannel.id: "backend"
        // 信号声明
        signal signal_init_option(string opt)
        signal signal_update_option_x(string id, string data)
        signal signal_update_option_y(string id, string data)
        signal signal_update_option_s(string id, string data)
        signal signal_draw
        // 管道连接成功后回调： echart加载
        function requestLoadChart() {
            webView.draw()
        }
    }

    // 构建矩形
    Rectangle {
        id: background
        // 使用布局填充宽高
        x: 0
        y: 0
        width: main_window.width
        height: main_window.height
        // 边界样式设置：宽度&颜色
        border.width: 1
        border.color: "#d33e6d"

        // 构建管道对象
        WebChannel {
            // 当前对象ID
            id: channel
            // 注册助手
            registeredObjects: [jsHelper]
        }

        // web 引擎加载显示
        WebEngineView {
            // 当前视图ID
            id: webView
            anchors.fill: parent
            objectName: "web"
            // 构建管道成员
            webChannel: channel
            // 设置需要加载的资源地址
            url: "qrc:/web/index.html"
            // 响应加载变化
            onLoadingChanged: {
                if (loadRequest.errorString) {
                    console.error(loadRequest.errorString)
                }
            }
            // 绘图数据，构建时外部传入
            property var option: _option
            // 绘制图表
            function draw() {
                jsHelper.signal_init_option(JSON.stringify(webView.option))
                jsHelper.signal_draw()
                console.log("draw,type:", webView.option.series[0].type)
            }
        }
    }
}
