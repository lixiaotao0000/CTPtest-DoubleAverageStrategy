笔试题要求：用C++接上期CTP 实现均线策略：5分钟均线上穿20日均线做多，反之做空，交易标的为螺纹钢RB

下面是调试过程中一些出现的问题，有的已经解决，有的还没有解决。
注：为了方便调试，使用分钟K线来构造5均线和20均线。由于没有维护历史数据库，所以每次程序启动之后需要等待20min以收取大于20个K线数据。

	done	1 vscode里面task.json，launch.json是配置什么的，一般怎么设置？


	done	2 回调函数那个逻辑大概是什么样子的，还有就是怎么写。


	done	3 c++的事件驱动概念是啥，还有就是大概怎么写。属于编程结构的一种，不属于编程语言本身概念。


	done	4 具体运行的时候发生的问题

	done	5 函数注册与编译过程。注册事件类？函数或者类自带一个register子函数，在外界调用该函数，实现类似回调函数的功能。

	done	6 main.cpp没有将iostream等头文件，包含进来。是路径设置的问题，还是库安装的问题。

	done	7 动态链接和静态链接是类似python里面的库吗？

	done	8 每个动作函数对应一个对应动作的请求响应函数？答：具体查看Spi中的函数，里面的函数都是以On开头的，但是数量多于Api内部的函数。

9 VS studio中不包含src文件，即来自ctp官方的接口文件，从哪里查看啊？

	done	10 类CThostFtdcMdSpi和CThostFtdcTraderApi中以Api和Spi结尾的两个类的作用是什么？官方文档：客户端使用API向CTP后台发送请求，CTP后台则使用SPI向客户端回传响应及回报。

10.5 定义CThostFtdcMdApi的时候，MD_API_EXPORT CThostFtdcMdApi，定义这个类是一个导出类，目的是什么？

	done	11 作者在Custom.h中，以CustomMdSpi继承CThostFtdcTraderSpi，并在Custom.cpp中复写了全部的函数，目的？规范的代码习惯？原来的dll，lib文件不是已经写好了代码实现了吗？为了添加额外的功能，例如
std::cout << "=====账户登录成功=====" << std::endl;
		
std::cout << "交易日： " << pRspUserLogin->TradingDay << std::endl;
		
std::cout << "登录时间： " << pRspUserLogin->LoginTime << std::endl;
		
std::cout << "经纪商： " << pRspUserLogin->BrokerID << std::endl;
		
std::cout << "帐户名： " << pRspUserLogin->UserID << std::endl;

//
g_pMdUserApi = CThostFtdcMdApi::CreateFtdcMdApi();   // 创建行情实例

CThostFtdcMdSpi *pMdUserSpi = new CustomMdSpi;       // 创建行情回调实例
///
Spi感觉像是客户端自己编写的回调类，这个类里面的函数在哪里触发呢？
答：类CThostFtdcMdApi中有个virtual void RegisterSpi(CThostFtdcMdSpi *pSpi) = 0的函数，这里进行将Spi输入Api中。具体代码如下：
//注册回调接口
//@param pSpi 派生自回调接口类的实例
virtual void RegisterSpi(CThostFtdcMdSpi *pSpi) = 0;

Spi里面都是On____函数的编写，所以这里的函数基本上在Api.Register函数注册之后就可以使用。现在问题的关键就集中到了问题14。

Spi（包括MdSpi和TraderSpi）类的回调函数基本上可以分为以下四种：

1）. 以On...开头，这种回调函数通常是返回API连接相关的信息内容，与业务逻辑无关，返回值（即回调函数的参数）通常为空或是简单的整数类型。

2）. 以OnRsp...开头，这种回调函数通常是针对用户的某次特定业务逻辑操作返回信息内容，返回值通常会包括4个参数：业务逻辑相关结构体的指针，错误信息结构体的指针，本次操作的请求号整数，是否是本次操作最后返回信息的布尔值。其中OnRspError主要用于一些通用错误信息的返回，因此返回的值中不包含业务逻辑相关结构体指针，只有3个返回值。

3）. 以OnRtn...开头，这种回调函数返回的通常是由柜台向用户主动推送的信息内容，如客户报单状态的变化、成交情况的变化、市场行情等等，因此返回值通常只有1个参数，为推送信息内容结构体的指针。

4）. 以OnErrRtn...开头，这种回调函数通常由于用户进行的某种业务逻辑操作请求（挂单、撤单等等）在交易所端触发了错误，如用户发出撤单指令、但是该订单在交易所端已经成交，返回值通常是2个参数，即业务逻辑相关结构体的指针和错误信息的指针。

	done	12 三种不同通讯模式下的函数动作情况是什么样的？通讯协议。CTP-API与CTP后台，采用何种通讯协议，如何通讯？
CTP-API使用建立在TCP协议、FTD协议与CTP后台进行通讯，FTD协议的所有通讯都基于某个通讯模式。通讯模式实际上就是通讯双方协同工作的方式。三种通讯模式：1）对话通讯（对应对话数据流和查询数据流），是由客户端主动发起通话请求，该请求被CTP后台接受并且处理，并给与响应，如保单撤单，查询等。这种通讯模式和普通的客户/服务器的模式相同。2）私有通讯模式（对应私有数据流），是指CTP后台主动向某个特定的客户端发出信息，如报单回报，成交回报等。3）广播通讯模式（对应公共数据流），是指CTP后台向所有的客户端发出相同的信息，如合约成交状态通知。答：目前不需要理解通讯机理。

	done	13 客户端和交易托管系统的通讯过程分为2个阶段：初始化阶段和功能调用阶段。
初始化阶段：1）产生一个CThostFtdcTraderApi实例；2）产生一个时间处理的实例；3）注册一个时间处理的实例；4）订阅私有流；5）订阅公共流。
功能调用阶段，程序可以任意调用接口中的请求方法，如ReqOrderInsert等，同时按照需要响应回调接口中的函数。


	done	14 触发信号的价格，是交易平台才有私有或者广播通讯的方式主动实时给客户端发送，还是需要客户端请求？通讯方式是tcp点播 or UDP方式。答：先不管他了。

网上已有相关问答：如何介入CTP提供的UDP行情？
答：创建行情API实力函数CreateFtdcMdApi（const char *pszFlowPath = “”， const bool blsUsingUdp = false）中参数blsUsingUdp：为行情模式，该参数缺省或者为false时使用TCP行情，否则为UDP行情。无论使用TCP或是UDP行情，都必须注册相对应的TCP或者UDP行情服务器地址。一般来说，CTP的普通行情都前置位TCP行情服务器，使用UDP行情服务器需要向证券（期货）公司申请，且仅限专线或者内网的投资设使用。在注册行情前置服务器时，无论是TCP还是UDP，都必须使用ResiterFront（“tcp://行情前置服务器IP地址或域名：端口”）的格式，因为udp传输存在不可靠性，所以在登录、订阅以及接受第一次行情的时候仍然使用tcp方式；并且无需为udp方式提供配置节点参数，udp仍然使用相同的地址和端口号。

	done	15 基于CTP-API的客户端交易软件在每个交易日开始之前需要执行哪些必要的步骤？
答：1）创建API实例（CreateFtdcTraderApi）。2）订阅私有流（SubscribePrivateTopic）。3）订阅私有流（SubscribePublicTopic）。4）注册名字服务器网络地址（RegisterNameServer），或者注册前置机网络地址（RegisterFront）。5）发起客户端仍正请求（ReqAuthenticate），客户端认证结果由OnRspAuthenticate返回；在CTP后台开启强制客户端认证（或者客户端在本次回话主动发起人证）后，只有通过客户点认证才能介入CTP后台。6）发起用户登录请求（ReqUserLogin），登录请求响应（OnRspUserLogin）返回登录结果。7）登录成功后，客户端在当前交易日必须成功执行一次“投资者结算结果确认（ReqSettlementInfoConfirm）”后才能开始交易。

	done	16 上面第5，6步涉及到的函数是否需要在main函数里面调用，感觉应该在CThostFtdcTraderApi.Init（）中已经调用，无法看到thosttraderapi.dll的情况下是不是就没办法判断了？答：只能从说明文档，或者别人做好的案例里面来确认。

	done	17 以请求登录为例，解释一下命令行界面出现的内容，与ReqUserLogin，OnRspUserLogin内部内容的关系？OnRspUserLogin的类型为void，表明不输出，那么把Spi利用Api.register（）注册之后，如何调用？是循环持续调用，来读取网络传输过来的信息还是其他的什么方式？答：循环体现在，CTP通过OnRtnDepthMarketData（）函数通过私有通讯的方式占用一个进程（线程），然后实时输送数据。

	done	18 通讯协议相关。
交易员API使用建立在TCP协议之上的FTD协议，FTD协议中的所有通讯都基于某个通讯模式。通讯模式实际上就是双方协同工作的方式，有三种通讯模式：对话通讯模式，私有通讯模式，广播通讯模式。

	done	19 目前推测MdApi.Init（）函数里面包含FrontConnected()，ReqUserLogin（），MdSpi里面的OnFrontConnected()函数响应FrontConnected()，OnRspUserLogin（）函数响应ReqUserLogin（），其内部调用了MdApi.SubscribeMarketData（），SubscribeMarketData应该是不带循环运行。然后CTP主动利用函数OnRtnDepthMarketData（）推送了深度数据。

20 删除掉线程退出之后的代码，基本会出现exe一闪而过。是跟线程有关？ 网上相关回答：CTP是基于多线程的，至少有两个线程在工作，一个是程序主线程，另一个是API工作线程。

	done	21 交易Api和行情Api是否需要分别登录？现在的情况是，行情和交易两个线程应该是同时运行的，但是会报错，报错原因不详。根据调试结果来看，确实是这个样子的。

	done	22 SIMNOW提供两类数据，一为交易时段的地址，如09:00-15:00和21:00-02:30(大概，夜盘真心没怎么关心)，使用第一套地址，这些数据是真实的行情数据，只是时间上比真实的行情会有延迟30秒左右(SIMNOW从交易所接收后转发出来的)。二为非交易时段，这时的数据是历史行情的播放，比如昨天的数据之类的，可以用来做程序调试。
第一套：
标准CTP：

        第一组：Trade Front：180.168.146.187:10000，Market Front：180.168.146.187:10010；【电信】

        第二组：Trade Front：180.168.146.187:10001，Market Front：180.168.146.187:10011；【电信】

        第三组：Trade Front：218.202.237.33 :10002，Market Front：218.202.237.33 :10012；【移动】

        交易阶段(服务时间)：与实际生产环境保持一致

第二套CTP：
	交易前置：180.168.146.187:10030，行情前置：180.168.146.187:10031；【7x24】

    第二套环境仅服务于CTP API开发爱好者，仅为用户提供CTP API测试需求，不提供结算等其它服务。

    新注册用户，需要等到第二个交易日才能使用第二套环境。

    账户、钱、仓跟第一套环境上一个交易日保持一致。

    交易阶段(服务时间)：交易日，16：00～次日09：00；非交易日，16：00～次日15：00。

    用户通过SimNow的账户（上一个交易日之前注册的账户都有效）接入环境，建议通过商业终端进行模拟交易的用户使用第一套环境。

23 目前的K线结果是没有对齐的，随机120个连续数据进行聚合。

	done	24 typedef 和 define的区别。
 typedef是用来声明类型别名的，用来增加代码的可读性，相当于一个数据类型起一个新的别名。#define只是作简单的字符串替换，不表达 任何含义。例如
#define INTPTR1 int*
typedef int* INTPTR2;

INTPTR1 p1,p2;
INTPTR2 p3,p4;

INTPTR1 p1,p2;和INTPTR2 p3,p4;这两句的效果决然不同。INTPTR1 p1,p2;进行字符串替换后变成int* p1,p2;要表达的意义是声明一个指针变量p1和一个整型变量p2；而INTPTR2 p3,p4;由于INTPTR2是具有含义的，告诉我们是一个指向整型数据的指针，那么p3和p4都为指针变量，这句相当于int* p1,*p2;从这里可以看出，进行宏替换是不含任何意义的替换，仅仅为字符串替换；而用typedef为一种数据类型起的别名是带有一定含义的。

25 订阅的私有流，公有流的主要内容是什么？私有流的内容是深度数据吗？

26 交易系统支持的交易指令：报单是通过ReqOrderInsert来执行，通过对函数不同参数的设置，实现买卖，申购，赎回，市价单，限价单等各种类型的报单。报单会由CTP返回一个唯一的标识Ref，撤单通过ReqOrderAction函数来使用两种数据序列来实现。撤单会由CTP返回一个唯一的标识RefAction









