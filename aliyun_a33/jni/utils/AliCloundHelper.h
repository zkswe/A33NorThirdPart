/*
 * AliCloundHelper.h
 *
 *  Created on: 2019年9月20日
 *      Author: Administrator
 */

#ifndef UTILS_ALICLOUNDHELPER_H_
#define UTILS_ALICLOUNDHELPER_H_
#include "iot_export.h"
#include "iot_import.h"
#include "iot_export_linkkit.h"
#include<string>

#define EXAMPLE_TRACE(...)                                          \
    do {                                                            \
        HAL_Printf("\033[1;32;40m%s.%d: ", __func__, __LINE__);     \
        HAL_Printf(__VA_ARGS__);                                    \
        HAL_Printf("\033[0m\r\n");                                  \
    } while (0)

#define EXAMPLE_MASTER_DEVID            (0)
#define EXAMPLE_YIELD_TIMEOUT_MS        (200)

#define PRODUCT_KEY                 "product_key"
#define DEVICE_NAME                 "device_name"
#define DEVICE_SECRET               "device_secret"
#define PRODUCT_SECRET              "product_secret"

typedef struct {
    int master_devid;
    int cloud_connected;
    int master_initialized;
} USER_CTX_T;

typedef int (*SET_PROPERTY_CALLBACK)(const int devid, const char *request, const int request_len);
typedef int (*SET_SERVICE_CALLBACK)(const int devid, const char *serviceid, const int serviceid_len,
        							const char *request, const int request_len, char **response, int *response_len);

class AliCloundHelper{
public:
	AliCloundHelper();
	~AliCloundHelper();
	int setProductKey(char *product_key);
	int setDeviceName(char *device_name);
	int setDeviceSecret(char *device_secret);
	int setProductSecret(char *product_secret);
	std::string getProductKey();
	std::string getDeviceName();
	std::string getDeviceSecret();
	std::string getProductSecret();
	int connectToClound();
	void disconnectToClound();
	void receiveMsgFormClound(int timeOut);
	int sendPropertyToClound(const char *payLoad);
	int sendTriggerEventToClound(int devid, char *eventid, int eventid_len, char *payload, int payload_len);
	void registerPropertyCallback(SET_PROPERTY_CALLBACK func);
	void registerServiceCallback(SET_SERVICE_CALLBACK func);

private:

	static int userConnectedEventHandler();
	static int userDisconnectedEventHandler();
	static int userServiceRequestEventHandler(const int devid, const char *serviceid, const int serviceid_len,
	                                   const char *request, const int request_len, char **response, int *response_len);
	static int userPropertySetEventHandler(const int devid, const char *request, const int request_len);
	static int userReportReplyEventHandler(const int devid, const int msgid, const int code, const char *reply, const int reply_len);
	static int userTriggerEventReplyEventHandler(const int devid, const int msgid, const int code, const char *eventid,
	        							  const int eventid_len, const char *message, const int message_len);
	static int userTimestampReplyEventHandler(const char *timestamp);
	static int userInitialized(const int devid);
	static int userFotaEventHandler(int type, const char *version);
	static int userCotaEventHandler(int type, const char *config_id, int config_size, const char *get_type,
	                                   const char *sign, const char *sign_method, const char *url);

	std::string mProductKey;
	std::string mProductSecret;
	std::string mDeviceName;
	std::string mDeviceSrcret;
    static USER_CTX_T mUserCtx;
	iotx_linkkit_dev_meta_info_t master_meta_info;
	static SET_PROPERTY_CALLBACK mPropertyCallback;
	static SET_SERVICE_CALLBACK mServiceCallback;
};

#endif /* UTILS_ALICLOUNDHELPER_H_ */
