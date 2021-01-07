
#include <omnetpp.h>
#include "TraceUtils.cc"
#include "Packet_m.h"

using namespace omnetpp;
using namespace std;

class Router: public cSimpleModule{

private:
    int nodeIndex;
    typedef map<int, int> RoutingTable;  // destaddr -> gateindex
    RoutingTable rtable;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};
Define_Module(Router)

void Router::initialize() {
    nodeIndex = par("nodeIndex");
    cTopology *topo = new cTopology("topo");
    vector<string> nedTypes;
    nedTypes.push_back("Router");
    nedTypes.push_back("Switch");
    nedTypes.push_back("Host");
    topo->extractByNedTypeName(nedTypes);
    EV << "cTopology found " << topo->getNumNodes() << " nodes\n";

    // �õ���ǰ�ڵ�
    cTopology::Node *thisNode = topo->getNode(nodeIndex);

    // ��̬·������
    for (int i = 0; i < topo->getNumNodes(); i++) {
        // Ŀ��ڵ��뵱ǰ�ڵ�Ϊͬһ�ڵ� ����
        if (topo->getNode(i) == thisNode)
            continue;  // skip ourselves

        topo->calculateUnweightedSingleShortestPathsTo(topo->getNode(i));
        if (thisNode->getNumPaths() == 0)
            continue;  // not connected

        //�õ����·������¼��·�ɱ���
        cGate *parentModuleGate = thisNode->getPath(0)->getLocalGate();
        int gateIndex = parentModuleGate->getIndex();
        rtable[i] = gateIndex;
        EV << "  towards address " << i << " gateIndex is " << gateIndex << endl;
    }
    delete topo;
}

void Router::handleMessage(cMessage *msg) {
    Packet *pk = check_and_cast<Packet *>(msg);
    string destAddr = pk->getDestAddr();
    int index = Singleton::get_instance().getIndex(destAddr);

    RoutingTable::iterator it = rtable.find(index);
    int outGateIndex = (*it).second;
    send(pk, "out", outGateIndex);
}


