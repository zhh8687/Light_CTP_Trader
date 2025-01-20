#include <iostream>
#include "CustomziedMdSpi.h"
#include "CustomziedTraderSpi.h"

using namespace std;

// 链接库
#pragma comment (lib, "thostmduserapi_se.lib")
#pragma comment (lib, "thosttraderapi_se.lib")

// 公共参数
TThostFtdcBrokerIDType gBrokerID = "9999";                         // 经纪商代码
TThostFtdcInvestorIDType gInvesterID = "";                         // 投资者账户名
TThostFtdcPasswordType gInvesterPassword = "";                     // 投资者密码
bool loginFlag = false;									    		// 登录标志

// 行情参数
CThostFtdcMdApi* g_pMdUserApi = nullptr;                           // 行情指针
char gMdFrontAddr[] = "tcp://180.168.146.187:10211";               // 行情前置地址
char* g_pInstrumentID[] = {"au2502"};			                   // 行情合约代码列表，中金所、上期所、大商所、郑商所、广期所各选一种
int instrumentNum = 1;                                             // 行情合约订阅数量

// 交易参数
CThostFtdcTraderApi* g_pTradeUserApi = nullptr;                    // 交易指针
char gTradeFrontAddr[] = "tcp://180.168.146.187:10201";            // 交易前置地址
TThostFtdcInstrumentIDType g_pTradeInstrumentID = "au2502";        // 所交易的合约代码
TThostFtdcDirectionType gTradeDirection;                           // 买卖方向
TThostFtdcPriceType	Price;                                         // 交易价格
TThostFtdcVolumeType gVolumeTotalOriginal = 1;                     // 交易数量
TThostFtdcCombOffsetFlagType gCombOffsetFlag;                      // 组合开平标志
TThostFtdcFrontIDType	trade_front_id;                            // 前置编号
TThostFtdcSessionIDType	session_id;                                // 会话编号
TThostFtdcOrderRefType	order_ref;                                 // 报单引用

int main() {
	// 账号密码
	std::cout << "请输入账号： " << std::endl;
	std::cin >> gInvesterID;
	std::cout << "请输入密码： " << std::endl;
	std::cin >> gInvesterPassword;

	// 初始化行情工作线程，向CTP建立连接
	cout << "初始化行情..." << endl;
	g_pMdUserApi = CThostFtdcMdApi::CreateFtdcMdApi();             // 创建行情实例
	CThostFtdcMdSpi* pMdUserSpi = new CustomziedMdSpi;             // 创建行情回调实例
	g_pMdUserApi->RegisterSpi(pMdUserSpi);                         // 注册事件类
	// 连接建立通知
	g_pMdUserApi->RegisterFront(gMdFrontAddr);                     // 设置行情前置地址
	g_pMdUserApi->Init();                                          // 连接运行

	// 初始化交易工作线程，向CTP建立连接
	cout << "初始化交易..." << endl;
	g_pTradeUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi(); // 创建交易实例
	CThostFtdcTraderSpi* pTradeSpi = new CustomziedTraderSpi;     // 创建交易回调实例
	g_pTradeUserApi->RegisterSpi(pTradeSpi);                      // 注册事件类
	// 连接建立通知
	g_pTradeUserApi->RegisterFront(gTradeFrontAddr);              // 设置交易前置地址
	g_pTradeUserApi->SubscribePublicTopic(THOST_TERT_QUICK);      // 订阅公共流
	g_pTradeUserApi->SubscribePrivateTopic(THOST_TERT_QUICK);     // 订阅私有流
	g_pTradeUserApi->Init();                                      // 连接运行
	system("pause");

	// 向CTP发起行情登录
	CThostFtdcReqUserLoginField loginReq;
	memset(&loginReq, 0, sizeof(loginReq));
	strcpy_s(loginReq.BrokerID, gBrokerID);
	strcpy_s(loginReq.UserID, gInvesterID);
	strcpy_s(loginReq.Password, gInvesterPassword);
	static int requestID1 = 0;
	int rt1 = g_pMdUserApi->ReqUserLogin(&loginReq, ++requestID1);	   // 登录应答
	if (!rt1) {
		std::cout << "<<<<<<发送行情登录请求成功<<<<<<" << std::endl;
	}
	else {
		std::cerr << "<<<<<<发送行情登录请求失败<<<<<<" << std::endl;
	}	

	// 向CTP发起交易登录
	static int requestID2 = 0;
	int rt2 = g_pTradeUserApi->ReqUserLogin(&loginReq, ++requestID2);	   // 登录应答
	if (!rt2){
		std::cout << "<<<<<<发送交易登录请求成功<<<<<<" << std::endl; 
	}
	else{ 
		std::cerr << "<<<<<<发送交易登录请求失败<<<<<<" << std::endl; 
	}
	system("pause");

	// 投资者结算结果确认
	CThostFtdcSettlementInfoConfirmField settlementConfirmReq;
	memset(&settlementConfirmReq, 0, sizeof(settlementConfirmReq));
	strcpy_s(settlementConfirmReq.BrokerID, gBrokerID);
	strcpy_s(settlementConfirmReq.InvestorID, gInvesterID);
	static int requestID3 = 0;
	int rt3 = g_pTradeUserApi->ReqSettlementInfoConfirm(&settlementConfirmReq, ++requestID3);
	if (!rt3) { 
		std::cout << "<<<<<<发送投资者结算结果确认请求成功<<<<<<" << std::endl; 
	}
	else { 
		std::cerr << "<<<<<<发送投资者结算结果确认请求失败<<<<<<" << std::endl; 
	}
	system("pause");

	// 请求查询合约
	CThostFtdcQryInstrumentField instrumentReq;
	memset(&instrumentReq, 0, sizeof(instrumentReq));
	strcpy_s(instrumentReq.InstrumentID, g_pTradeInstrumentID);
	static int requestID4 = 0; // 请求编号
	int rt4 = g_pTradeUserApi->ReqQryInstrument(&instrumentReq, ++requestID4);
	if (!rt4)
		std::cout << "<<<<<<发送合约查询请求成功<<<<<<" << std::endl;
	else
		std::cerr << "<<<<<<发送合约查询请求失败<<<<<<" << std::endl;
	system("pause");

	// 请求查询投资者持仓
	CThostFtdcQryInvestorPositionField postionReq;
	memset(&postionReq, 0, sizeof(postionReq));
	strcpy_s(postionReq.BrokerID, gBrokerID);
	strcpy_s(postionReq.InvestorID, gInvesterID);
	strcpy_s(postionReq.InstrumentID, g_pTradeInstrumentID);
	static int requestID5 = 0; // 请求编号
	int rt5 = g_pTradeUserApi->ReqQryInvestorPosition(&postionReq, ++requestID5);
	if (!rt5) { 
		std::cout << "<<<<<<发送投资者持仓查询请求成功<<<<<<" << std::endl; 
	}
	else {
		std::cerr << "<<<<<<发送投资者持仓查询请求失败<<<<<<" << std::endl;
	}
	system("pause");

	// 请求查询投资者资金账户
	CThostFtdcQryTradingAccountField tradingAccountReq;
	memset(&tradingAccountReq, 0, sizeof(tradingAccountReq));
	strcpy_s(tradingAccountReq.BrokerID, gBrokerID);
	strcpy_s(tradingAccountReq.InvestorID, gInvesterID);
	static int requestID6 = 0; // 请求编号
	int rt6 = g_pTradeUserApi->ReqQryTradingAccount(&tradingAccountReq, ++requestID6);
	if (!rt6) { 
		std::cout << "<<<<<<发送投资者资金账户查询请求成功<<<<<<" << std::endl; 
	}
	else { 
		std::cerr << "<<<<<<发送投资者资金账户查询请求失败<<<<<<" << std::endl; 
	}
	system("pause");
	
	/////////////////////////////////////////////////////////////////////////
	// 向CTP订阅行情
	int rt7 = g_pMdUserApi->SubscribeMarketData(g_pInstrumentID, instrumentNum);	   // 行情订阅应答（失败 / 成功），持续推送行情
	if (!rt7) {
		std::cout << "<<<<<<发送订阅请求成功<<<<<<" << std::endl;
	}
	else {
		std::cerr << "<<<<<<发送订阅请求失败<<<<<<" << std::endl;
	}
	std::cout << "下一步进入交易环节，可不继续往下... " << std::endl;
	system("pause");

	// 交易价格、交易方向、开平方向
	char temp1[] = "";
	char temp2;
	char temp3;
	std::cout << "请输入交易价格： " << std::endl;
	std::cin >> temp1;
	Price = atof(temp1);
	std::cout << "请输入交易方向：0代表买入，1代表卖出" << std::endl;
	std::cin >> temp2;
	if (temp2 == '0') {
		gTradeDirection = THOST_FTDC_D_Buy;
	}
	else {
		gTradeDirection = THOST_FTDC_D_Sell;
	}
	std::cout << "请输入开平方向：0代表开仓，1代表平仓，2代表强平，3代表平今，4代表平昨，5代表强减，6代表本地强平" << std::endl;
	std::cin >> temp3;
	if (temp3 == '0') {
		*gCombOffsetFlag = THOST_FTDC_OF_Open;
	}
	else if (temp3 == '1'){
		*gCombOffsetFlag = THOST_FTDC_OF_Close;
	}
	else if (temp3 == '2'){
		*gCombOffsetFlag = THOST_FTDC_OF_ForceClose;
	}
	else if (temp3 == '3') {
		*gCombOffsetFlag = THOST_FTDC_OF_CloseToday;
	}
	else if (temp3 == '4') {
		*gCombOffsetFlag = THOST_FTDC_OF_CloseYesterday;
	}
	else if (temp3 == '5') {
		*gCombOffsetFlag = THOST_FTDC_OF_ForceOff;
	}
	else if (temp3 == '6') {
		*gCombOffsetFlag = THOST_FTDC_OF_LocalForceClose;
	}
	else {
		std::cerr << "输入错误" << std::endl;
	}

	// 报单、处理报单、撤单
	CThostFtdcInputOrderField orderInsertReq;
	memset(&orderInsertReq, 0, sizeof(orderInsertReq));
	///经纪公司代码
	strcpy_s(orderInsertReq.BrokerID, gBrokerID);
	///投资者代码
	strcpy_s(orderInsertReq.InvestorID, gInvesterID);
	///合约代码
	strcpy_s(orderInsertReq.InstrumentID, g_pTradeInstrumentID);
	///报单引用，未设置递增模式
	strcpy_s(orderInsertReq.OrderRef, order_ref);
	///报单价格条件: 限价
	orderInsertReq.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	///买卖方向: 
	orderInsertReq.Direction = gTradeDirection;
	///组合开平标志: 开仓
	orderInsertReq.CombOffsetFlag[0] = *gCombOffsetFlag;
	///组合投机套保标志
	orderInsertReq.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	///价格
	orderInsertReq.LimitPrice = Price;
	///数量：1
	orderInsertReq.VolumeTotalOriginal = gVolumeTotalOriginal;
	///有效期类型: 当日有效
	orderInsertReq.TimeCondition = THOST_FTDC_TC_GFD;
	///成交量类型: 任何数量
	orderInsertReq.VolumeCondition = THOST_FTDC_VC_AV;
	///最小成交量: 1
	orderInsertReq.MinVolume = 1;
	///触发条件: 立即
	orderInsertReq.ContingentCondition = THOST_FTDC_CC_Immediately;
	///强平原因: 非强平
	orderInsertReq.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///自动挂起标志: 否
	orderInsertReq.IsAutoSuspend = 0;
	///用户强评标志: 否
	orderInsertReq.UserForceClose = 0;

	static int requestID8 = 0; // 请求编号
	int rt8 = g_pTradeUserApi->ReqOrderInsert(&orderInsertReq, ++requestID8);
	if (!rt8)
		std::cout << ">>>>>>发送报单录入请求成功" << std::endl;
	else
		std::cerr << "--->>>发送报单录入请求失败" << std::endl;
	system("pause");
	
	/////////////////////////////////////////////////////////////////////////
	// 向CTP取消订阅行情
	int rt9 = g_pMdUserApi->UnSubscribeMarketData(g_pInstrumentID, instrumentNum);
	if (!rt9)
	{
		std::cout << "<<<<<<发送取消订阅行情请求成功<<<<<<" << std::endl;
	}
	else
	{
		std::cerr << "<<<<<<发送取消订阅行情请求失败<<<<<<" << std::endl;
	}
	system("pause");
	
	// 向CTP发起行情登出
	CThostFtdcUserLogoutField logoutReq;
	memset(&logoutReq, 0, sizeof(logoutReq));
	strcpy_s(logoutReq.BrokerID, gBrokerID);
	strcpy_s(logoutReq.UserID, gInvesterID);
	static int requestID10 = 0;
	int rt10 = g_pMdUserApi->ReqUserLogout(&logoutReq, ++requestID10);	   // 登出应答
	if (!rt10) {
		std::cout << "<<<<<<发送行情登出请求成功<<<<<<" << std::endl;
	}
	else {
		std::cerr << "<<<<<<发送行情登出请求失败<<<<<<" << std::endl;
	}
	system("pause");

	// 向CTP发起交易登出
	static int requestID11 = 0;
	int rt11 = g_pTradeUserApi->ReqUserLogout(&logoutReq, ++requestID11);	   // 登出应答
	if (!rt11) {
		std::cout << "<<<<<<发送交易登出请求成功<<<<<<" << std::endl;
	}
	else {
		std::cerr << "<<<<<<发送交易登出请求失败<<<<<<" << std::endl;
	}
	system("pause");

	// 线程退出
	g_pMdUserApi->Join();
	delete pMdUserSpi;
	g_pMdUserApi->Release();

	g_pTradeUserApi->Join();
	delete pTradeSpi;
	g_pTradeUserApi->Release();
	system("pause");

	return 0;
}