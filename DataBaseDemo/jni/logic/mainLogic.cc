#pragma once
#include "uart/ProtocolSender.h"
/*
*此文件由GUI工具生成
*文件功能：用于处理用户的逻辑相应代码
*功能说明：
*========================onButtonClick_XXXX
当页面中的按键按下后系统会调用对应的函数，XXX代表GUI工具里面的[ID值]名称，
如Button1,当返回值为false的时候系统将不再处理这个按键，返回true的时候系统将会继续处理此按键。比如SYS_BACK.
*========================onSlideWindowItemClick_XXXX(int index) 
当页面中存在滑动窗口并且用户点击了滑动窗口的图标后系统会调用此函数,XXX代表GUI工具里面的[ID值]名称，
如slideWindow1;index 代表按下图标的偏移值
*========================onSeekBarChange_XXXX(int progress) 
当页面中存在滑动条并且用户改变了进度后系统会调用此函数,XXX代表GUI工具里面的[ID值]名称，
如SeekBar1;progress 代表当前的进度值
*========================ogetListItemCount_XXXX() 
当页面中存在滑动列表的时候，更新的时候系统会调用此接口获取列表的总数目,XXX代表GUI工具里面的[ID值]名称，
如List1;返回值为当前列表的总条数
*========================oobtainListItemData_XXXX(ZKListView::ZKListItem *pListItem, int index)
 当页面中存在滑动列表的时候，更新的时候系统会调用此接口获取列表当前条目下的内容信息,XXX代表GUI工具里面的[ID值]名称，
如List1;pListItem 是贴图中的单条目对象，index是列表总目的偏移量。具体见函数说明
*========================常用接口===============
*LOGD(...)  打印调试信息的接口
*mTextXXXPtr->setText("****") 在控件TextXXX上显示文字****
*mButton1Ptr->setSelected(true); 将控件mButton1设置为选中模式，图片会切换成选中图片，按钮文字会切换为选中后的颜色
*mSeekBarPtr->setProgress(12) 在控件mSeekBar上将进度调整到12
*mListView1Ptr->refreshListView() 让mListView1 重新刷新，当列表数据变化后调用
*mDashbroadView1Ptr->setTargetAngle(120) 在控件mDashbroadView1上指针显示角度调整到120度
*
* 在Eclipse编辑器中  使用 “alt + /”  快捷键可以打开智能提示
*/

#include <vector>
#include "db/CppSQLite3.h"

// 数据库名称
#define DATA_BASE_FILE_NAME			"/mnt/extsd/zkswe.db"
// 表名
#define TABLE_NAME					"zkswe"

// sqlite封装类
static CppSQLite3DB sDBHelper;

struct SInfo {
	char name[20];
	int age;
};

static std::vector<SInfo> sInfoList;


/**
 * 注册定时器
 * 填充数组用于注册定时器
 * 注意：id不能重复
 */
static S_ACTIVITY_TIMEER REGISTER_ACTIVITY_TIMER_TAB[] = {
	//{0,  6000}, //定时器id=0, 时间间隔6秒
	//{1,  1000},
};

/**
 * 当界面构造时触发
 */
static void onUI_init(){
    //Tips :添加 UI初始化的显示代码到这里,如:mText1Ptr->setText("123");

}

/**
 * 当切换到该界面时触发
 */
static void onUI_intent(const Intent *intentPtr) {
    if (intentPtr != NULL) {
        //TODO
    }
}

/*
 * 当界面显示时触发
 */
static void onUI_show() {

}

/*
 * 当界面隐藏时触发
 */
static void onUI_hide() {

}

/*
 * 当界面完全退出时触发
 */
static void onUI_quit() {

}

/**
 * 串口数据回调接口
 */
static void onProtocolDataUpdate(const SProtocolData &data) {

}

/**
 * 定时器触发函数
 * 不建议在此函数中写耗时操作，否则将影响UI刷新
 * 参数： id
 *         当前所触发定时器的id，与注册时的id相同
 * 返回值: true
 *             继续运行当前定时器
 *         false
 *             停止运行当前定时器
 */
static bool onUI_Timer(int id){
	switch (id) {

		default:
			break;
	}
    return true;
}

/**
 * 有新的触摸事件时触发
 * 参数：ev
 *         新的触摸事件
 * 返回值：true
 *            表示该触摸事件在此被拦截，系统不再将此触摸事件传递到控件上
 *         false
 *            触摸事件将继续传递到控件上
 */
static bool onmainActivityTouchEvent(const MotionEvent &ev) {
    switch (ev.mActionStatus) {
		case MotionEvent::E_ACTION_DOWN://触摸按下
			//LOGD("时刻 = %ld 坐标  x = %d, y = %d", ev.mEventTime, ev.mX, ev.mY);
			break;
		case MotionEvent::E_ACTION_MOVE://触摸滑动
			break;
		case MotionEvent::E_ACTION_UP:  //触摸抬起
			break;
		default:
			break;
	}
	return false;
}

static bool onButtonClick_CreateButton(ZKButton *pButton) {
    //LOGD(" ButtonClick CreateButton !!!\n");
	bool isOK = true;
	try {
		// 打开数据库
		sDBHelper.open(DATA_BASE_FILE_NAME);
		LOGD("open %s ok !\n", DATA_BASE_FILE_NAME);

		// 检测表是否已创建，如果没有则需要创建
		if (!sDBHelper.tableExists(TABLE_NAME)) {
			LOGD("%s table is not exists, need to create!\n", TABLE_NAME);
			char sqlStr[256] = { 0 };
			// 创建表SQL语句
			sprintf(sqlStr, "create table %s (id integer primary key, name char(30), age int);", TABLE_NAME);
			LOGD("sqlStr: %s\n", sqlStr);

			// 创建表
			sDBHelper.execDML(sqlStr);
		}
	} catch (CppSQLite3Exception &e) {	// 数据库操作失败会抛异常，走下面流程
		LOGE("error: %s\n", e.errorMessage());
		isOK = false;
	}

	mTipTextviewPtr->setText(isOK ? "数据库创建成功" : "数据库创建失败");
	mTipWindowPtr->showWnd();

	sDBHelper.close();

    return false;
}

static bool onButtonClick_InsertButton(ZKButton *pButton) {
    //LOGD(" ButtonClick Button1 !!!\n");
	bool isOK = true;
	try {
		// 打开数据库
		sDBHelper.open(DATA_BASE_FILE_NAME);
//		LOGD("open %s ok !\n", DATA_BASE_FILE_NAME);

		char sqlStr[128] = { 0 };
		sprintf(sqlStr, "insert into %s(name, age) values ('%s', %s);", TABLE_NAME, mNameEdittextPtr->getText().c_str(), mAgeEdittextPtr->getText().c_str());
		sDBHelper.execDML(sqlStr);
	} catch (CppSQLite3Exception &e) {	// 数据库操作失败会抛异常，走下面流程
		LOGE("error: %s\n", e.errorMessage());
		isOK = false;
	}

	mTipTextviewPtr->setText(isOK ? "插入数据成功" : "插入数据失败");
	mTipWindowPtr->showWnd();

	sDBHelper.close();

    return false;
}

static void onEditTextChanged_NameEdittext(const std::string &text) {
    //LOGD(" onEditTextChanged_ NameEdittext %s !!!\n", text.c_str());
}

static void onEditTextChanged_AgeEdittext(const std::string &text) {
    //LOGD(" onEditTextChanged_ AgeEdittext %s !!!\n", text.c_str());
}

static void onEditTextChanged_Edittext1(const std::string &text) {
    //LOGD(" onEditTextChanged_ Edittext1 %s !!!\n", text.c_str());
}

static int getListItemCount_QueryListview(const ZKListView *pListView) {
    //LOGD("getListItemCount_QueryListview !\n");
    return sInfoList.size();
}

static void obtainListItemData_QueryListview(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index) {
    //LOGD(" obtainListItemData_ QueryListview  !!!\n");
	ZKListView::ZKListSubItem *pItem = pListItem->findSubItemByID(ID_MAIN_IDSubItem);
	if (pItem) {
		pItem->setText(index + 1);
	}
	pItem = pListItem->findSubItemByID(ID_MAIN_NameSubItem);
	if (pItem) {
		pItem->setText(sInfoList[index].name);
	}
	pItem = pListItem->findSubItemByID(ID_MAIN_AgeSubItem);
	if (pItem) {
		pItem->setText(sInfoList[index].age);
	}
}

static void onListItemClick_QueryListview(ZKListView *pListView, int index, int id) {
    //LOGD(" onListItemClick_ QueryListview  !!!\n");
}

static bool onButtonClick_QueryButton(ZKButton *pButton) {
    //LOGD(" ButtonClick QueryButton !!!\n");
	sInfoList.clear();

	try {
		// 打开数据库
		sDBHelper.open(DATA_BASE_FILE_NAME);

		char sqlStr[128];
		sprintf(sqlStr, "select * from %s where name = '%s';", TABLE_NAME, mQueryNameEdittextPtr->getText().c_str());

		CppSQLite3Query q = sDBHelper.execQuery(sqlStr);

		while (!q.eof()) {
			SInfo info;
			strcpy(info.name, q.fieldValue(1));
			info.age = atoi(q.fieldValue(2));
			sInfoList.push_back(info);

			q.nextRow();
		}
	} catch (CppSQLite3Exception &e) {
		LOGE("error: %s\n", e.errorMessage());
	}

	mQueryListviewPtr->refreshListView();

	sDBHelper.close();

    return false;
}

static void onEditTextChanged_QueryNameEdittext(const std::string &text) {
    //LOGD(" onEditTextChanged_ QueryNameEdittext %s !!!\n", text.c_str());
}
