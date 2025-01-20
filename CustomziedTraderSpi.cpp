#include <iostream>
#include "CustomziedTraderSpi.h"

extern TThostFtdcBrokerIDType gBrokerID;
extern TThostFtdcInvestorIDType gInvesterID;
extern TThostFtdcPasswordType gInvesterPassword;
extern CThostFtdcTraderApi* g_pTradeUserApi;
extern TThostFtdcInstrumentIDType g_pTradeInstrumentID;
extern TThostFtdcDirectionType gTradeDirection;
extern TThostFtdcPriceType gLimitPrice;
extern TThostFtdcVolumeType gVolumeTotalOriginal;
extern bool loginFlag;

extern TThostFtdcFrontIDType	trade_front_id;	//前置编号
extern TThostFtdcSessionIDType	session_id;	    //会话编号
extern TThostFtdcOrderRefType	order_ref;	    //报单引用

void CustomziedTraderSpi::OnFrontConnected() {
	std::cout << "<<<<<<建立网络连接成功<<<<<<" << std::endl;
};

void CustomziedTraderSpi::OnFrontDisconnected(int nReason) {
	std::cerr << "<<<<<<网络连接断开<<<<<<" << std::endl;
	std::cerr << "错误码：" << nReason << std::endl;
};

void CustomziedTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		std::cout << "<<<<<<账户登录成功<<<<<<" << std::endl;
		loginFlag = true;
		std::cout << "交易日：" << pRspUserLogin->TradingDay << std::endl;
		std::cout << "登录时间：" << pRspUserLogin->LoginTime << std::endl;
		std::cout << "经纪商：" << pRspUserLogin->BrokerID << std::endl;
		std::cout << "帐户名：" << pRspUserLogin->UserID << std::endl;
		// 保存会话参数
		trade_front_id = pRspUserLogin->FrontID;
		session_id = pRspUserLogin->SessionID;
		strcpy_s(order_ref, pRspUserLogin->MaxOrderRef);
	}
};

void CustomziedTraderSpi::OnRspError(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	isErrorRspInfo(pRspInfo);
};

bool CustomziedTraderSpi::isErrorRspInfo(CThostFtdcRspInfoField* pRspInfo) {
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (bResult) {
		std::cerr << "返回错误--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
	}
	return bResult;
};

void CustomziedTraderSpi::OnHeartBeatWarning(int nTimeLapse) {
	std::cerr << "<<<<<<网络心跳超时<<<<<<" << std::endl;
	std::cerr << "距上次连接时间：" << nTimeLapse << std::endl;
};

void CustomziedTraderSpi::OnRspUserLogout(CThostFtdcUserLogoutField* pUserLogout, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	if (!isErrorRspInfo(pRspInfo))
	{
		loginFlag = false; // 登出就不能再交易了
		std::cout << "<<<<<<账户登出成功<<<<<<" << std::endl;
		std::cout << "经纪商： " << pUserLogout->BrokerID << std::endl;
		std::cout << "帐户名： " << pUserLogout->UserID << std::endl;
	}
};

void CustomziedTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	if (!isErrorRspInfo(pRspInfo))
	{
		std::cout << "<<<<<<投资者结算结果确认成功<<<<<<" << std::endl;
		std::cout << "确认日期：" << pSettlementInfoConfirm->ConfirmDate << std::endl;
		std::cout << "确认时间：" << pSettlementInfoConfirm->ConfirmTime << std::endl;
	};
};

void CustomziedTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField* pInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	if (!isErrorRspInfo(pRspInfo))
	{
		std::cout << "<<<<<<查询合约结果成功<<<<<<" << std::endl;
		std::cout << "交易所代码：" << pInstrument->ExchangeID << std::endl;
		std::cout << "合约代码：" << pInstrument->InstrumentID << std::endl;
		std::cout << "合约在交易所的代码：" << pInstrument->ExchangeInstID << std::endl;
		std::cout << "执行价：" << pInstrument->StrikePrice << std::endl;
		std::cout << "到期日：" << pInstrument->EndDelivDate << std::endl;
		std::cout << "当前交易状态：" << pInstrument->IsTrading << std::endl;
	};
};

void CustomziedTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField* pTradingAccount, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	if (!isErrorRspInfo(pRspInfo))
	{
		std::cout << "<<<<<<查询投资者资金账户成功<<<<<<" << std::endl;
		std::cout << "投资者账号：" << pTradingAccount->AccountID << std::endl;
		std::cout << "可用资金：" << pTradingAccount->Available << std::endl;
		std::cout << "可取资金：" << pTradingAccount->WithdrawQuota << std::endl;
		std::cout << "当前保证金:" << pTradingAccount->CurrMargin << std::endl;
		std::cout << "平仓盈亏：" << pTradingAccount->CloseProfit << std::endl;
	}
};

void CustomziedTraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField* pInvestorPosition, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	if (!isErrorRspInfo(pRspInfo))
	{
		std::cout << "<<<<<<查询投资者持仓成功<<<<<<" << std::endl;
		if (pInvestorPosition)
		{
			std::cout << "合约代码： " << pInvestorPosition->InstrumentID << std::endl;
			std::cout << "开仓价格： " << pInvestorPosition->OpenAmount << std::endl;
			std::cout << "开仓量： " << pInvestorPosition->OpenVolume << std::endl;
			std::cout << "开仓方向： " << pInvestorPosition->PosiDirection << std::endl;
			std::cout << "占用保证金：" << pInvestorPosition->UseMargin << std::endl;
		}
		else
		{
			std::cout << "----->该合约未持仓" << std::endl;
		}
	}
};

void CustomziedTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField* pInputOrder, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	if (!isErrorRspInfo(pRspInfo))
	{
		std::cout << "<<<<<<报单录入成功<<<<<<" << std::endl;
		std::cout << "合约代码： " << pInputOrder->InstrumentID << std::endl;
		std::cout << "价格： " << pInputOrder->LimitPrice << std::endl;
		std::cout << "数量： " << pInputOrder->VolumeTotalOriginal << std::endl;
		std::cout << "开仓方向： " << pInputOrder->Direction << std::endl;
	}
};

void CustomziedTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField* pInputOrderAction, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	if (!isErrorRspInfo(pRspInfo))
	{
		std::cout << "=====报单操作成功=====" << std::endl;
		std::cout << "合约代码： " << pInputOrderAction->InstrumentID << std::endl;
		std::cout << "操作标志： " << pInputOrderAction->ActionFlag;
	}
};

void CustomziedTraderSpi::OnRtnOrder(CThostFtdcOrderField* pOrder) {
	char str[10];
	sprintf_s(str, "%d", pOrder->OrderSubmitStatus);
	int orderState = atoi(str) - 48;	//报单状态0=已经提交，3=已经接受

	std::cout << "=====收到报单应答=====" << std::endl;

	if (isMyOrder(pOrder))
	{
		if (isTradingOrder(pOrder))
		{
			std::cout << "--->>> 等待成交中！" << std::endl;
			//reqOrderAction(pOrder); // 这里可以撤单
			//reqUserLogout(); // 登出测试
		}
		else if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled)
			std::cout << "--->>> 撤单成功！" << std::endl;
	}
};

void CustomziedTraderSpi::OnRtnTrade(CThostFtdcTradeField* pTrade) {
	std::cout << "=====报单成功成交=====" << std::endl;
	std::cout << "成交时间： " << pTrade->TradeTime << std::endl;
	std::cout << "合约代码： " << pTrade->InstrumentID << std::endl;
	std::cout << "成交价格： " << pTrade->Price << std::endl;
	std::cout << "成交量： " << pTrade->Volume << std::endl;
	std::cout << "开平仓方向： " << pTrade->Direction << std::endl;
};

bool CustomziedTraderSpi::isMyOrder(CThostFtdcOrderField* pOrder)
{
	return ((pOrder->FrontID == trade_front_id) &&
		(pOrder->SessionID == session_id) &&
		(strcmp(pOrder->OrderRef, order_ref) == 0));
}

bool CustomziedTraderSpi::isTradingOrder(CThostFtdcOrderField* pOrder)
{
	return ((pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
		(pOrder->OrderStatus != THOST_FTDC_OST_Canceled) &&
		(pOrder->OrderStatus != THOST_FTDC_OST_AllTraded));
}