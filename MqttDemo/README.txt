1. Apollo服务器搭建
	参考博客：https://blog.csdn.net/ddxxii/article/details/80890293
	添加用户步骤：
		1.到之前创建的服务器 etc目录下，打开groups.properties
			默认用户admin ：admins=admin
			添加一个新用户：admins=admin|zkswe
		2.打开users.properties，添加：zkswe=123	// 表示用户zkswe的密码为123

2. 移植mqtt到工程项目中，jni/mqtt源码目录

3. 使用样例见 logic/mainLogic.cc

4. 链接（链接libcrypto.so和libssl.so，openssl头文件在jni/include/目录下），修改Android.mk
	添加：-L. -DOPENSSL -lssl -lcrypto
	LOCAL_LDFLAGS := -L$(ZKSW_PREBUILT_LIB_PATH)/system -lstlport -L. -DOPENSSL -lssl -lcrypto