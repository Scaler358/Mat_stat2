import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.3
import QtQuick.Dialogs 1.2
import QXisq 1.0

ApplicationWindow {
    id:wind
    visible: true
    property bool state: width>=height?1:0
    property int type: 1
    width: 400
    height: 800
    Material.theme: Material.Light
    Material.accent: Material.Red
    Material.elevation: 5
    Rectangle{
        id:rect_tools
        anchors.top: parent.top
        anchors.left: parent.left
        height: wind.state?parent.height:parent.height/2
        width: !wind.state?parent.width:parent.width/2
        Rectangle{
            id:rect_but
            anchors.top: parent.top
            anchors.left: parent.left
            height: !wind.state?parent.height:parent.height/2
            width: wind.state?parent.width:parent.width/2
            TextField{
                id:text_path
                anchors.leftMargin: 10
                anchors.topMargin: 10
                anchors.top: parent.top
                anchors.left: parent.left
                width: parent.width-80
                height: 50>parent.height/5?parent.height:50
            }
            Button{
                id:but_select_path
                anchors.topMargin: 10
                anchors.rightMargin: 20
                anchors.top: parent.top
                anchors.right: parent.right
                highlighted: true
                width: 50
                height: 50
                text: "\u003F"
                font.pixelSize: width/2
                onClicked: {
                    file_dial.open();
                }
            }
            Label{
             id:lab_a
             anchors.leftMargin: 10
             anchors.topMargin: 10
             anchors.top: text_path.bottom
             anchors.left: parent.left
             width: 10>parent.width/5?10:parent.width/5
             height: width/3>parent.height/5?parent.height/5:width/3
             font.pixelSize: lab_a.height
             text: "\u237A = "
            }
            SpinBox{
             id: spin_a
             anchors.rightMargin: 10
             anchors.leftMargin: 10
             anchors.top: text_path.bottom
             anchors.left: lab_a.right
             width: parent.width-lab_a.width-30
             height: width/3>parent.height/5?parent.height/5:width/3
             from: 1
             to: 100
             font.pixelSize: lab_a.font.pixelSize
             stepSize: spin_a.value>9?5:1
             value: 5
             editable: true
            }
            Button{
                id:but_read
                anchors.margins: 10
                anchors.top: spin_a.bottom
                anchors.left:parent.left
                highlighted: true
                width: parent.width-30
                height: width/3>parent.height/5?parent.height/5:width/3
                text: "Зчитати"
                enabled: false
                onClicked: {
                    if(qxisq.slotRead(text_path.text,wind.type)){
                        text_path.enabled=false;
                        but_select_path.enabled=false;
                        but_read.enabled=false;
                        but_go.enabled=true;
                        but_select_path.text="\u2713";
                        but_cancel.enabled=true;
                    }
                    else{
                        text_res.text="Cannot open or read file!";
                    }
                }
            }
            Button{
                id:but_cancel
                anchors.margins: 10
                anchors.top: but_read.bottom
                anchors.left:parent.left
                highlighted: true
                width: parent.width-30
                height: width/3>parent.height/5?parent.height/5:width/3
                text: "Зброс"
                enabled: false
                onClicked: {
                    text_path.enabled=true;
                    text_path.text="";
                    but_select_path.enabled=true;
                    but_select_path.text="\u003F";
                    but_go.enabled=false;
                    but_read.enabled=false;
                    but_cancel.enabled=false;
                    qxisq.slotClear();
                }
            }
            Button{
                id:but_go
                anchors.margins: 10
                anchors.top: but_cancel.bottom
                anchors.left: parent.left
                highlighted: true
                width: parent.width-30
                height: width/3>parent.height/5?parent.height/5:width/3
                text: "Аналізувати"
                enabled: false
                onClicked: {
                    text_res.text=qxisq.result();
                }
            }
        }
        Rectangle{
            id:rect_types
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            height: !wind.state?parent.height:parent.height/2
            width: wind.state?parent.width:parent.width/2
            Rectangle{
                id:rect_type_main
                anchors.leftMargin: 10
                anchors.top: parent.top
                anchors.left: parent.left
                height: wind.state?parent.height:parent.height/2
                width: (!wind.state?parent.width:parent.width/2)-10
                RadioButton{
                    id:but_dyskr
                    anchors.left: parent.left
                    anchors.top:parent.top
                    width: parent.width
                    height: parent.height/2
                    checked: true
                    text: "Дискретний"
                    onClicked: {
                        types_if_dyskr.visible=true;
                        types_if_neper.visible=false;
                        but_binom.checked=true;
                        wind.type=1;
                    }
                }
                RadioButton{
                    id:but_neper
                    anchors.left: parent.left
                    anchors.top: but_dyskr.bottom
                    width: parent.width
                    height: parent.height/2
                    text: "Неперервний"
                    onClicked: {
                        types_if_dyskr.visible=false;
                        types_if_neper.visible=true;
                        but_norm.checked=true;
                        wind.type=3;
                    }
                }
            }
            Rectangle{
                id:rect_type_second
                anchors.leftMargin: 10
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                height: wind.state?parent.height:parent.height/2
                width: (!wind.state?parent.width:parent.width/2)-10
                Rectangle{
                    id:types_if_dyskr
                    anchors.fill: parent
                    RadioButton{
                        id:but_binom
                        anchors.left: parent.left
                        anchors.top:parent.top
                        width: parent.width
                        height: parent.height/2
                        checked: true
                        text: "Біномний"
                        onClicked: {
                            wind.type=1;
                        }
                    }
                    RadioButton{
                        id:but_puas
                        anchors.left: parent.left
                        anchors.top: but_binom.bottom
                        width: parent.width
                        height: parent.height/2
                        text: "Пуасона"
                        onClicked: {
                            wind.type=2;
                        }
                    }
                }
                Rectangle{
                    id:types_if_neper
                    anchors.fill: parent;
                    visible: false
                    RadioButton{
                        id:but_norm
                        anchors.left: parent.left
                        anchors.top:parent.top
                        width: parent.width
                        height: parent.height/3
                        checked: true
                        text: "Нормальний"
                        onClicked: {
                            wind.type=3;
                        }
                    }
                    RadioButton{
                        id:but_rivnom
                        anchors.left: parent.left
                        anchors.top: but_norm.bottom
                        width: parent.width
                        height: parent.height/3
                        text: "Рівномірний"
                        onClicked: {
                            wind.type=4;
                        }
                    }
                    RadioButton{
                        id:but_ekspon
                        anchors.left: parent.left
                        anchors.top: but_rivnom.bottom
                        width: parent.width
                        height: parent.height/3
                        text: "Експонентний"
                        onClicked: {
                            wind.type=5;
                        }
                    }
                }
            }
        }
    }
    Rectangle{
        id:rect_res
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        height: wind.state?parent.height:parent.height/2
        width: !wind.state?parent.width:parent.width/2
        property int smallersize: height<width?height:width
        Flickable{
            anchors.fill: parent
            contentHeight: text_res.contentHeight+50
            clip: true
            boundsBehavior: Flickable.StopAtBounds
            TextEdit{
                id:text_res
                anchors.margins: 10
                anchors.fill: parent
                wrapMode: Text.Wrap
                font.pixelSize: rect_res.smallersize/22
                readOnly: true
            }
        }
    }
    FileDialog{
        id:file_dial
        title: "Select file"
        nameFilters: ["Stat file (*.stat)"]
        onAccepted: {
            but_read.enabled=true;
            text_path.text=fileUrl.toString().substring(7);
        }
    }
    QXisq{
        id: qxisq
        a : spin_a.value/100
    }
}
