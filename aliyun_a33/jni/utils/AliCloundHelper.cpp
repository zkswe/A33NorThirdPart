/*
 * AliCloundHelper.cpp
 *
 *  Created on: 2019年9月20日
 *      Author: Administrator
 */
#include"AliCloundHelper.h"
#include "storage/StoragePreferences.h"
#include"utils/log.h"

USER_CTX_T AliCloundHelper::mUserCtx;
SET_PROPERTY_CALLBACK AliCloundHelper::mPropertyCallback = NULL;
SET_SERVICE_CALLBACK AliCloundHelper::mServiceCallback = NULL;

AliCloundHelper::AliCloundHelper(){

}

AliCloundHelper::~AliCloundHelper(){

}

/**
 * @brief   设置设备的`ProductKey`, 用于标识设备的品类, 三元组之一
 *
 * @param   product_key : 用来存放ProductKey字符串的数组
 * @return  写到product_key[]数组中的字符长度, 单位是字节(Byte)
 */
int AliCloundHelper::setProductKey(char *product_key){
	int len = strlen(product_key);
	if(len <= PRODUCT_KEY_LEN){
		HAL_SetProductKey(product_key);
		if(StoragePreferences::putString(PRODUCT_KEY, product_key)){
			mProductKey = product_key;
			return len;
		}
	}
	return 0;
}

/**
 * @brief   设置设备的`DeviceName`, 用于标识设备单品的名字, 三元组之一
 *
 * @param   device_name : 用来存放DeviceName字符串的数组
 * @return  写到device_name[]数组中的字符长度, 单位是字节(Byte)
 */
int AliCloundHelper::setDeviceName(char *device_name){
	int len = strlen(device_name);
	if(len <= DEVICE_NAME_LEN){
		HAL_SetDeviceName(device_name);
		if(StoragePreferences::putString(DEVICE_NAME, device_name)){
			mDeviceName = device_name;
			return len;
		}
	}
	return 0;
}

/**
 * @brief   设置设备的`DeviceSecret`, 用于标识设备单品的密钥, 三元组之一
 *
 * @param   device_secret : 用来存放DeviceSecret字符串的数组
 * @return  写到device_secret[]数组中的字符长度, 单位是字节(Byte)
 */
int AliCloundHelper::setDeviceSecret(char *device_secret){
	int len = strlen(device_secret);
	if(len <= DEVICE_SECRET_LEN){
		HAL_SetDeviceSecret(device_secret);
		if(StoragePreferences::putString(DEVICE_SECRET, device_secret)){
			mDeviceSrcret = device_secret;
			return len;
		}
	}
	return 0;
}

/**
 * @brief   设置设备的`ProductSecret`, 用于标识设备单品的密钥, 三元组之一
 *
 * @param   product_secret : 用来存放ProductSecret字符串的数组
 * @return  写到product_secret[]数组中的字符长度, 单位是字节(Byte)
 */
int AliCloundHelper::setProductSecret(char *product_secret){
	int len = strlen(product_secret);
	if(len <= PRODUCT_SECRET_LEN){
		HAL_SetProductSecret(product_secret);
		if(StoragePreferences::putString(PRODUCT_SECRET, product_secret)){
			mProductSecret = product_secret;
			return len;
		}
	}
	return 0;
}

std::string AliCloundHelper::getProductKey(){
	return StoragePreferences::getString(PRODUCT_KEY, "");
}

std::string AliCloundHelper::getDeviceName(){
	return StoragePreferences::getString(DEVICE_NAME, "");
}

std::string AliCloundHelper::getDeviceSecret(){
	return StoragePreferences::getString(DEVICE_SECRET, "");
}

std::string AliCloundHelper::getProductSecret(){
	return StoragePreferences::getString(PRODUCT_SECRET, "");
}

/*
 * 连接到阿里云
 */
int AliCloundHelper::connectToClound(){

	memset(&master_meta_info, 0, sizeof(iotx_linkkit_dev_meta_info_t));
	memcpy(master_meta_info.product_key, mProductKey.c_str(), mProductKey.length());
	memcpy(master_meta_info.product_secret, mProductSecret.c_str(), mProductSecret.length());
	memcpy(master_meta_info.device_name, mDeviceName.c_str(), mDeviceName.length());
	memcpy(master_meta_info.device_secret, mDeviceSrcret.c_str(), mDeviceSrcret.length());

	IOT_SetLogLevel(IOT_LOG_DEBUG);

	/* Register Callback */
	IOT_RegisterCallback(ITE_CONNECT_SUCC, userConnectedEventHandler);
	IOT_RegisterCallback(ITE_DISCONNECTED, userDisconnectedEventHandler);
	IOT_RegisterCallback(ITE_SERVICE_REQUEST, userServiceRequestEventHandler);
	IOT_RegisterCallback(ITE_PROPERTY_SET, userPropertySetEventHandler);
	IOT_RegisterCallback(ITE_REPORT_REPLY, userReportReplyEventHandler);
	IOT_RegisterCallback(ITE_TRIGGER_EVENT_REPLY, userTriggerEventReplyEventHandler);
	IOT_RegisterCallback(ITE_TIMESTAMP_REPLY, userTimestampReplyEventHandler);
	IOT_RegisterCallback(ITE_INITIALIZE_COMPLETED, userInitialized);
	IOT_RegisterCallback(ITE_FOTA, userFotaEventHandler);
	IOT_RegisterCallback(ITE_COTA, userCotaEventHandler);

	int res = 0, domain_type = 0, dynamic_register = 0, post_reply_need = 0;

	domain_type = IOTX_CLOUD_REGION_SHANGHAI;
	IOT_Ioctl(IOTX_IOCTL_SET_DOMAIN, (void *)&domain_type);

	/* Choose Login Method */
	dynamic_register = 0;
	IOT_Ioctl(IOTX_IOCTL_SET_DYNAMIC_REGISTER, (void *)&dynamic_register);

	/* post reply doesn't need */
	post_reply_need = 1;
	IOT_Ioctl(IOTX_IOCTL_RECV_EVENT_REPLY, (void *)&post_reply_need);

	/* Create Master Device Resources */
	mUserCtx.master_devid = IOT_Linkkit_Open(IOTX_LINKKIT_DEV_TYPE_MASTER, &master_meta_info);
	if (mUserCtx.master_devid < 0) {
		LOGE("IOT_Linkkit_Open Failed");
		return -1;
	}

	/* Start Connect Aliyun Server */
	do {
		res = IOT_Linkkit_Connect(mUserCtx.master_devid);
		if (res < 0) {
			LOGE("IOT_Linkkit_Connect failed, retry after 5s...");
			HAL_SleepMs(5000);
		}
	} while (res < 0);

	IOT_SetLogLevel(IOT_LOG_NONE);
	return res;
}

void AliCloundHelper::disconnectToClound(){

}

/**
 * @brief try to receive message from cloud and dispatch these message to user event callback
 *
 * @param timeout_ms. timeout for waiting new message arrived
 *
 * @return void.
 *
 */
void AliCloundHelper::receiveMsgFormClound(int timeOut){
	IOT_Linkkit_Yield(timeOut);
}

/**
 * @brief Report message to cloud
 *
 * @param devid. device identifier.
 * @param msg_type. message type. see iotx_linkkit_msg_type_t, as follows:
 *        ITM_MSG_POST_PROPERTY
 *        ITM_MSG_DEVICEINFO_UPDATE
 *        ITM_MSG_DEVICEINFO_DELETE
 *        ITM_MSG_POST_RAW_DATA
 *        ITM_MSG_LOGIN
 *        ITM_MSG_LOGOUT
 *
 * @param payload. message payload.
 * @param payload_len. message payload length.
 *
 * @return success: 0 or message id (>=1), fail: -1.
 *
 */
int AliCloundHelper::sendPropertyToClound(const char *payLoad){
	return IOT_Linkkit_Report(EXAMPLE_MASTER_DEVID, ITM_MSG_POST_PROPERTY,
            (unsigned char *)payLoad, strlen(payLoad));
}

/**
 * @brief post event to cloud
 *
 * @param devid. device identifier.
 * @param eventid. tsl event id.
 * @param eventid_len. length of tsl event id.
 * @param payload. event payload.
 * @param payload_len. event payload length.
 *
 * @return success: message id (>=1), fail: -1.
 *
 */
int AliCloundHelper::sendTriggerEventToClound(int devid, char *eventid, int eventid_len, char *payload, int payload_len){
	return IOT_Linkkit_TriggerEvent(devid, eventid, eventid_len, payload, payload_len);
}

/*
 * 注册属性消息回调
 */
void AliCloundHelper::registerPropertyCallback(SET_PROPERTY_CALLBACK func){
	mPropertyCallback = func;
}

void AliCloundHelper::registerServiceCallback(SET_SERVICE_CALLBACK func){
	mServiceCallback = func;
}

/*
 * 连接阿里云成功回调函数
 */
int AliCloundHelper::userConnectedEventHandler(){
	LOGD("Cloud Connected");
	mUserCtx.cloud_connected = 1;

	return 0;
}

/*
 * 阿里云断开回调函数
 */
int AliCloundHelper::userDisconnectedEventHandler(){
	LOGD("Cloud Disconnected");
	mUserCtx.cloud_connected = 0;

    return 0;
}

/*
 * 处理阿里云平台下发的服务消息
 * devid  连接云平台时返回的devid
 * serviceid  服务消息ID
 * request  消息内容， json格式
 * response  返回给云平台的消息，json格式
 */
int AliCloundHelper::userServiceRequestEventHandler(const int devid, const char *serviceid, const int serviceid_len,
                                   const char *request, const int request_len, char **response, int *response_len){
	if(mServiceCallback != NULL){
		(*mServiceCallback)(devid, serviceid, serviceid_len, request, request_len, response, response_len);
	}
	return 0;
}

/*
 * 处理云平台下发的属性消息
 * devid  连接云平台时返回的devid
 * request  消息内容，json格式
 */
int AliCloundHelper::userPropertySetEventHandler(const int devid, const char *request, const int request_len){
    int res = 0;
    LOGD("Property Set Received, Request: %s", request);
    if(mPropertyCallback != NULL)
    (*mPropertyCallback)(devid, request, request_len);
    res = IOT_Linkkit_Report(EXAMPLE_MASTER_DEVID, ITM_MSG_POST_PROPERTY,
                                (unsigned char *)request, request_len);
    EXAMPLE_TRACE("Post Property Message ID: %d", res);
    return 0;
}

/**
 * 接收云平台收到属性上报后的返回消息
 */
int AliCloundHelper::userReportReplyEventHandler(const int devid, const int msgid, const int code, const char *reply, const int reply_len){
	EXAMPLE_TRACE("Message Post Reply Received, Message ID: %d, Code: %d, Reply: %.*s", msgid, code,
				  reply_len,
				  (reply == NULL)? ("NULL") : (reply));
	return 0;
}

/*
 * 接收云平台收到事件上报后的返回消息
 */
int AliCloundHelper::userTriggerEventReplyEventHandler(const int devid, const int msgid, const int code, const char *eventid,
        							  const int eventid_len, const char *message, const int message_len){
	EXAMPLE_TRACE("Trigger Event Reply Received, Message ID: %d, Code: %d, EventID: %.*s, Message: %.*s",
				  msgid, code,
				  eventid_len,
				  eventid, message_len, message);

	return 0;
}

/*
 * 当收到查询时间戳请求的应答
 */
int AliCloundHelper::userTimestampReplyEventHandler(const char *timestamp){
	LOGD("Current Timestamp: %s", timestamp);

	return 0;
}

/*
 * 设备初始化完成回调
 */
int AliCloundHelper::userInitialized(const int devid){
	LOGD("Device Initialized");
	mUserCtx.master_initialized = 1;

	return 0;
}

/*
 * fota升级相关，暂未使用
 */
int AliCloundHelper::userFotaEventHandler(int type, const char *version){
    char buffer[128] = {0};
    int buffer_length = 128;

    /* 0 - new firmware exist, query the new firmware */
//    if (type == 0) {
//        LOGD("New Firmware Version: %s", version);
//
//        IOT_Linkkit_Query(EXAMPLE_MASTER_DEVID, ITM_MSG_QUERY_FOTA_DATA, (unsigned char *)buffer, buffer_length);
//    }

    return 0;
}

/*
 * 远程配置相关，暂未使用
 */
int AliCloundHelper::userCotaEventHandler(int type, const char *config_id, int config_size, const char *get_type,
                                   const char *sign, const char *sign_method, const char *url){
//    char buffer[128] = {0};
//    int buffer_length = 128;
//
//    /* type = 0, new config exist, query the new config */
//    if (type == 0) {
//    	LOGD("New Config ID: %s", config_id);
//    	LOGD("New Config Size: %d", config_size);
//    	LOGD("New Config Type: %s", get_type);
//    	LOGD("New Config Sign: %s", sign);
//    	LOGD("New Config Sign Method: %s", sign_method);
//    	LOGD("New Config URL: %s", url);
//
//        IOT_Linkkit_Query(EXAMPLE_MASTER_DEVID, ITM_MSG_QUERY_COTA_DATA, (unsigned char *)buffer, buffer_length);
//    }
    return 0;
}



