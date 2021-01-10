
#include <omnetpp.h>
#include <typeinfo>
#include "TraceUtils.cc"
#include "Packet_m.h"
#include "MPLSLabel.cc"
#include <vector>

using namespace omnetpp;
using namespace std;

static int label = 20;

class Router: public cSimpleModule{

private:
    int nodeIndex;

    typedef map<int, int> RoutingTable;  // destaddr -> gateindex
    RoutingTable rtable;

    typedef map<int, MPLSLabelByInLabel> CacheByInLabel;
    CacheByInLabel labelCache;

    typedef map<string, vector<int>> CacheByIP;
    CacheByIP ipCache;

    int buildLsp(string prefix, int inGate, int dstIndex);

    cTopology *topo;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
};
Define_Module(Router)

void Router::finish() {
    delete topo;
}

void Router::initialize() {
    nodeIndex = par("nodeIndex");
    topo = new cTopology("topo");
    vector<string> nedTypes;
    nedTypes.push_back("Router");
    nedTypes.push_back("Switch");
    nedTypes.push_back("Host");
    topo->extractByNedTypeName(nedTypes);
    EV << "cTopology found " << topo->getNumNodes() << " nodes\n";

    // 得到当前节点
    cTopology::Node *thisNode = topo->getNode(nodeIndex);

    // 动态路由生成
    for (int i = 0; i < topo->getNumNodes(); i++) {
        cTopology::Node* node = topo->getNode(i);
        EV <<"target node type: " << node->getModule()->getClassName() << endl;
        if (strncmp("Host", node->getModule()->getClassName(), 5)) {
            EV <<"target node is not host, skip" << endl;
            continue;
        }

        topo->calculateUnweightedSingleShortestPathsTo(node);
        if (thisNode->getNumPaths() == 0)
            continue;  // not connected

        //得到最短路径并记录在路由表中
        cGate *parentModuleGate = thisNode->getPath(0)->getLocalGate();
        EV << thisNode->getPath(0)->getRemoteNode()->getModule()->getName() << endl;
        int gateIndex = parentModuleGate->getIndex();
        rtable[i] = gateIndex;
        EV << "  towards address " << i << " gateIndex is " << gateIndex << endl;
    }
}

void Router::handleMessage(cMessage *msg) {
    int inGate = msg->getArrivalGate()->getIndex();

    Packet *pk = check_and_cast<Packet *>(msg);
    string destAddr = pk->getDestAddr();
    string prefix = destAddr.substr(0, destAddr.size() - 1).append("*");

    int dstIndex = Singleton::get_instance().getIndex(destAddr);

    int inLabel = pk->getForwardLabel();
    // inlabel为0，代表是入口路由器，尚未分配标签
    if (inLabel == 0) {
        CacheByIP::iterator it = ipCache.find(prefix);
        if (it != ipCache.end()) {
			for (auto inL : it->second) {
				CacheByInLabel::iterator it = labelCache.find(inL);
				if (inGate == it->second.inIf) {
				    EV << "find same lsp" <<endl;
					inLabel = inL;
					break;
				}
			}
        }
        // inLabel仍未0.代表当前lsp尚未建立，首先建立lsp，分发标签
        if (inLabel == 0) {
            EV <<"no lsp available, need to set lsp first" << endl;
            inLabel = buildLsp(prefix, inGate, dstIndex);
        }
    }

    CacheByInLabel::iterator it = labelCache.find(inLabel);
    int outGateIndex = it->second.outIf;
    pk->setForwardLabel(it->second.outLabel);

    bool b = inGate==it->second.inIf;
    if (b) {
        EV << "*****************true"<< endl;
    } else {
        EV << "*****************false"<< endl;
    }

    send(pk, "out", outGateIndex);
}

// lsp尚未建立，建立lsp
int Router::buildLsp(string prefix, int inGate, int dstIndex) {
    int inLabel = 0;
    CacheByIP::iterator ipIt = ipCache.find(prefix);
    // 如果当前cache中有对应缓存，代表已经建立了从当前路由器到dst的lsp，可以合并标签
    if (ipIt != ipCache.end()) {
        EV <<"try to find same lsp in buildLsp" << endl;
        vector<int> list = ipIt->second;
        for (auto inL : list) {
            CacheByInLabel::iterator it = labelCache.find(inL);
            if (inGate == it->second.inIf) {
                EV <<"find same lsp in buildLsp" << endl;
                inLabel = inL;
                break;
            }
        }
    } else {
        EV <<"insert new vector"<<endl;
		vector<int> vv;
		ipCache.insert(make_pair(prefix, vv));
	}

    if (inLabel == 0) {
        EV <<"have to build new lsp" << endl;
        // 从当前路由器开始建立到dst的lsp
        //首先获取出接口outGate
        RoutingTable::iterator it = rtable.find(dstIndex);
        int outGate = (*it).second;

        topo->calculateUnweightedSingleShortestPathsTo(topo->getNode(dstIndex));
        cTopology::Node *thisNode = topo->getNode(nodeIndex);
        cModule* next = thisNode->getPath(0)->getRemoteNode()->getModule();
        Router* nextRouter = dynamic_cast<Router *>(next);
        int outLabel;
        // 下一跳仍为router，需要递归，以下一跳的inLabel作为本路由的outLabel
        if (nextRouter != NULL) {
            // 帮助下一跳计算它的inGate
            int nextGate = thisNode->getPath(0)->getRemoteGate()->getIndex();
            outLabel = nextRouter->buildLsp(prefix, nextGate, dstIndex);
        } else {
            //如果下一跳不为router，代表当前路由器为边界路由，无须再递归，直接设置outLabel为0
            outLabel = 0;
        }

        inLabel = label++;
        ipCache.find(prefix)->second.push_back(inLabel);

        MPLSLabelByInLabel* mm = new MPLSLabelByInLabel(inGate, outLabel, outGate);
        labelCache.insert(make_pair(inLabel, *mm));
    }
    return inLabel;
}


