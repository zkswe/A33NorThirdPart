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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"
#include "utils/AliCloundHelper.h"

typedef enum{
	AIR_QUALITY_EXCELLENT = 1,               //优
	AIR_QUALITY_GOOD,                        //良
	AIR_QUALITY_MILD_POLLUTION,              //轻度污染
	AIR_QUALITY_MODERATE_POLLUTION,          //中度污染
	AIR_QUALITY_BAD_POLLUTION,               //重度污染
	AIR_QUALITY_SEVERE_POLLUTION,            //严重污染
}AIR_LEVEL;

typedef enum{
	WIND_SPEED_AUTO,      //自动
	WIND_SPEED_QUIET,     //静音档
	WIND_SPEED_LOW,       //低档
	WIND_SPEED_MEDIUM,    //中档
	WIND_SPEED_HIGH,      //高档
	WIND_SPEED_HIGHEST,   //最高档
}WIND_SPEED;

static int sPM25Value;      	//PM2.5值
static AIR_LEVEL sAirLevel; 	//空气质量等级
static bool sPowerStatus;		//开关
static WIND_SPEED sWindSpeed;   //风速
static bool sWorkMode;			//工作模式
static int  sCurHumidity;       //当前湿度
static int sCurTmep;          //当前温度
static bool sHumified;			//加湿开关
static bool sChildLock;         //童锁开关
static bool sUVStatus;          //紫外线

static bool sLightStatus = false;
AliCloundHelper sCloundHelper;

class MessageThread : public Thread {
protected:
	virtual bool readyToRun() {
		// init something

		// return false 将退出线程
		return true;
	}

	virtual bool threadLoop() {
		// do something
		sCloundHelper.receiveMsgFormClound(EXAMPLE_YIELD_TIMEOUT_MS);
		// return false 将退出线程
		return true;
	}
};
static MessageThread messageTHread;

typedef struct {
    int master_devid;
    int cloud_connected;
    int master_initialized;
} user_example_ctx_t;

/**
 * These PRODUCT_KEY|PRODUCT_SECRET|DEVICE_NAME|DEVICE_SECRET are listed for demo only
 *
 * When you created your own devices on iot.console.com, you SHOULD replace them with what you got from console
 *
 */

#define PRODUCT_KEY_VALUE             "a1NGqZgXitq"
#define PRODUCT_SECRET_VALUE          ""
#define DEVICE_NAME_VALUE             "airtest1"
#define DEVICE_SECRET_VALUE           "8l04lqwR7b2cUKQwFZ0CuAlGCCguNhvV"

static void initUI(){
	char str[30];
	sPowerStatus = true;
	mButton4Ptr->setSelected(sPowerStatus);
	sprintf(str, "{\"PowerSwitch\":%d}", sPowerStatus);
	sCloundHelper.sendPropertyToClound(str);
	sWorkMode = true;
	mModeButtonPtr->setSelected(sWorkMode);
	sprintf(str, "{\"WorkMode\":%d}", sWorkMode?1:0);
	sCloundHelper.sendPropertyToClound(str);
	sUVStatus = false;
	mUVButtonPtr->setSelected(sUVStatus);
	sprintf(str, "{\"IonsSwitch\":%d}", sUVStatus);
	sCloundHelper.sendPropertyToClound(str);
	sChildLock = false;
	mChildLockButtonPtr->setSelected(sChildLock);
	sprintf(str, "{\"ChildLockSwitch\":%d}", sChildLock);
	sCloundHelper.sendPropertyToClound(str);
	sWindSpeed = 1;
	mSeekbar1Ptr->setProgress(sWindSpeed);
	sprintf(str, "{\"WindSpeed\":%d}", sWindSpeed);
	sCloundHelper.sendPropertyToClound(str);
	sPM25Value = 0;
	mPM25ButtonPtr->setText(sPM25Value);
	sprintf(str, "{\"PM25\":%d}", sPM25Value);
	sCloundHelper.sendPropertyToClound(str);\
	sCurTmep = 0;
	sprintf(str, "%d℃", sCurTmep);
	mCurTempTextPtr->setText(str);
	sprintf(str, "{\"CurrentTemperature\":%d}", sCurTmep);
	sCloundHelper.sendPropertyToClound(str);
	sCurHumidity = 0;
	sprintf(str, "%d%%", sCurHumidity);
	mHumidityTextPtr->setText(str);
	sprintf(str, "{\"CurrentHumidity\":%d}", sCurHumidity);
	sCloundHelper.sendPropertyToClound(str);
}

/** recv event post response message from cloud **/
static int user_property_set_event_handler(const int devid, const char *request, const int request_len)
{
    int res = 0;
    LOGD("Property Set Received, Request: %s", request);

    cJSON *root = NULL, *iterPower = NULL, *iterWindSpeed = NULL, *iterWorkMode = NULL, *iterUVStatus = NULL, *iterChildLock = NULL;
	/* Parse Root */
	root = cJSON_Parse(request);
	if (root == NULL || !cJSON_IsObject(root)) {
		LOGD("property_set: JSON Parse Error");
		return -1;
	}
	iterPower = cJSON_GetObjectItem(root, "PowerSwitch");
	iterWindSpeed = cJSON_GetObjectItem(root, "WindSpeed");
	iterWorkMode = cJSON_GetObjectItem(root, "WorkMode");
	iterUVStatus = cJSON_GetObjectItem(root, "IonsSwitch");
	iterChildLock = cJSON_GetObjectItem(root, "ChildLockSwitch");
	if(iterPower != NULL){
		if(cJSON_IsNumber(iterPower)){
			sPowerStatus = iterPower->valueint;
			mButton4Ptr->setSelected(sPowerStatus);
		}
	}else if(iterWindSpeed != NULL){
		if(cJSON_IsNumber(iterWindSpeed)){
			sWindSpeed = iterWindSpeed->valueint;
			mSeekbar1Ptr->setProgress(sWindSpeed);
		}
	}else if(iterWorkMode != NULL){
		if(cJSON_IsNumber(iterWorkMode)){
			sWorkMode = iterWorkMode->valueint;
			mModeButtonPtr->setSelected(sWorkMode);
		}
	}else if(iterUVStatus != NULL){
		if(cJSON_IsNumber(iterUVStatus)){
			sUVStatus = iterUVStatus->valueint;
			mUVButtonPtr->setSelected(sUVStatus);
		}
	}else if(iterChildLock != NULL){
		if(cJSON_IsNumber(iterChildLock)){
			sChildLock = iterChildLock->valueint;
			mChildLockButtonPtr->setSelected(sChildLock);
			mSeekbar1Ptr->setInvalid(sChildLock);
			mModeButtonPtr->setInvalid(sChildLock);
			mUVButtonPtr->setInvalid(sChildLock);
		}
	}

    cJSON_Delete(root);
    return 0;
}


static int user_service_request_event_handler(const int devid, const char *serviceid, const int serviceid_len,
                                              const char *request, const int request_len,
                                              char **response, int *response_len)
{
    return 0;
}

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
	case 0:{
		if(sPowerStatus){
			static int index;
			sCurTmep++;
			sCurHumidity++;
			sPM25Value++;
			if(sCurTmep > 50){
				sCurTmep = -20;
			}
			if(sCurHumidity > 100){
				sCurHumidity = 0;
			}
			if(sPM25Value > 500){
				sPM25Value = 0;
			}
			char str[30];
			if(index %2 == 0){
				mPM25ButtonPtr->setText(sPM25Value);
				sprintf(str, "{\"PM25\":%d}", sPM25Value);
				sCloundHelper.sendPropertyToClound(str);
			}

			if(index %5 == 0){
				sprintf(str, "%d%%", sCurHumidity);
				mHumidityTextPtr->setText(str);
				sprintf(str, "{\"CurrentHumidity\":%d}", sCurHumidity);
				sCloundHelper.sendPropertyToClound(str);
			}

			if(index %7 == 0){
				sprintf(str, "%d℃", sCurTmep);
				mCurTempTextPtr->setText(str);
				sprintf(str, "{\"CurrentTemperature\":%d}", sCurTmep);
				sCloundHelper.sendPropertyToClound(str);
			}
			index++;
		}

	}
	break;
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
static bool onButtonClick_Button1(ZKButton *pButton) {
    //LOGD(" ButtonClick Button1 !!!\n");

	//设置设备三元组信息
	sCloundHelper.setProductKey(PRODUCT_KEY_VALUE);
	sCloundHelper.setDeviceName(DEVICE_NAME_VALUE);
	sCloundHelper.setDeviceSecret(DEVICE_SECRET_VALUE);
	if(sCloundHelper.connectToClound() >= 0){
		messageTHread.run();    //获取云平台下发的信息
		mActivityPtr->registerUserTimer(0, 10000);
		sCloundHelper.registerPropertyCallback(user_property_set_event_handler);
		sCloundHelper.registerServiceCallback(user_service_request_event_handler);
		initUI();
		mButton1Ptr->setText("已连接");
	}
	return 0;
    return false;
}
static bool onButtonClick_Button2(ZKButton *pButton) {
    //LOGD(" ButtonClick Button2 !!!\n");

}
static bool onButtonClick_Button3(ZKButton *pButton) {
    //LOGD(" ButtonClick Button3 !!!\n");

    return false;
}
static bool onButtonClick_PM25Button(ZKButton *pButton) {
    //LOGD(" ButtonClick PM25Button !!!\n");
    return false;
}

static void onProgressChanged_Seekbar1(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged Seekbar1 %d !!!\n", progress);
	sWindSpeed = progress;
	char str[30];
	sprintf(str, "{\"WindSpeed\":%d}", sWindSpeed);
	sCloundHelper.sendPropertyToClound(str);

}
static bool onButtonClick_Button4(ZKButton *pButton) {
    //LOGD(" ButtonClick Button4 !!!\n");
	sPowerStatus = !sPowerStatus;
	pButton->setSelected(sPowerStatus);
	char str[30];
	sprintf(str, "{\"PowerSwitch\":%d}", sPowerStatus);
	sCloundHelper.sendPropertyToClound(str);
	if(sPowerStatus){
		mSeekbar1Ptr->setInvalid(sChildLock);
		mModeButtonPtr->setInvalid(sChildLock);
		mUVButtonPtr->setInvalid(sChildLock);
	}else{
		sWorkMode = false;
		sUVStatus = false;
		mSeekbar1Ptr->setInvalid(true);
		mModeButtonPtr->setInvalid(true);
		mUVButtonPtr->setInvalid(true);
	}
    return false;
}

static bool onButtonClick_ModeButton(ZKButton *pButton) {
    //LOGD(" ButtonClick ModeButton !!!\n");
	if(sPowerStatus && !sChildLock){
		sWorkMode = !sWorkMode;
		pButton->setText(sWorkMode?"手动":"自动");
		char str[30];
		sprintf(str, "{\"WorkMode\":%d}", sWorkMode?1:0);
		sCloundHelper.sendPropertyToClound(str);
	}
    return false;
}
static bool onButtonClick_UVButton(ZKButton *pButton) {
    //LOGD(" ButtonClick UVButton !!!\n");
	if(sPowerStatus && !sChildLock){
		sUVStatus = !sUVStatus;
		pButton->setSelected(sUVStatus);
		char str[30];
		sprintf(str, "{\"IonsSwitch\":%d}", sUVStatus);
		sCloundHelper.sendPropertyToClound(str);
	}
    return false;
}

static bool onButtonClick_ChildLockButton(ZKButton *pButton) {
    //LOGD(" ButtonClick ChildLockButton !!!\n");
	if(sPowerStatus){
		sChildLock = !sChildLock;
		pButton->setSelected(sChildLock);
		char str[30];
		sprintf(str, "{\"ChildLockSwitch\":%d}", sChildLock);
		sCloundHelper.sendPropertyToClound(str);
		mSeekbar1Ptr->setInvalid(sChildLock);
		mModeButtonPtr->setInvalid(sChildLock);
		mUVButtonPtr->setInvalid(sChildLock);
	}
    return false;
}
static bool onButtonClick_Button5(ZKButton *pButton) {
    //LOGD(" ButtonClick Button5 !!!\n");
	EASYUICONTEXT->openActivity("NetSettingActivity");
    return false;
}
