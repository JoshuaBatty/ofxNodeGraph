//
//  NodeGraph.h
//  gtf
//
//  Created by David Gallardo on 31/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#pragma once

#include <functional>
#include <list>
#include <vector>
#include <map>

#include <imgui.h>

#include "UniqueId.h"

namespace gtf 
{
class NodeConnectionDescBase
{
public:
    inline NodeConnectionDescBase( UniqueId _t, std::string const & _n)
    : unique_id(_t)
    ,  displayName(_n){}
    
    std::string displayName;
    UniqueId unique_id;
    bool checkType(NodeConnectionDescBase const * other) const { return other->unique_id == unique_id; }
    
    virtual class NodeConnectionBase* createConnectionInstance(bool _input) = 0;
};

class NodeTypeBase
{
public:
    inline NodeTypeBase(UniqueId uid, std::string name)
    : unique_id(uid)
    , displayName(name){};
    
	UniqueId unique_id;
    std::string displayName;
    std::vector<NodeConnectionDescBase*> inputConnectionsDesc;
	std::vector<NodeConnectionDescBase*> outputConnectionsDesc;
    virtual class Node* createNodeInstance() = 0;
    
};

class NodeConnectionBase
{
public:
	inline NodeConnectionBase(NodeConnectionDescBase const * _desc, bool _input)
    : desc(_desc)
    , isInput(_input){};
    
    virtual ~NodeConnectionBase(){};
    
    float posX, posY;
    bool isInput { false };
    bool isReady { true };
    bool isDirty { false };
    
    NodeConnectionDescBase const * desc;
    class Node* parentNode;
    
    //connections
    
    NodeConnectionBase * input { nullptr };
    std::list<NodeConnectionBase*> output;
    
};

template <typename T> class NodeConnection : public NodeConnectionBase
{
public:
    inline NodeConnection(NodeConnectionDescBase const * _desc, bool _input)
    : NodeConnectionBase(_desc, _input){};
    
    using PTR_TYPE = NodeConnection<T>*;
    using PTR_TYPE_CONST = NodeConnection<T> const *;
    
    T data { 0 };
    
    static PTR_TYPE CAST(NodeConnectionBase* con){ return dynamic_cast<PTR_TYPE>(con); };
    static PTR_TYPE_CONST CAST(NodeConnectionBase const * con){ return dynamic_cast<PTR_TYPE_CONST>(con); };
};

using NodeConnectionI32 = NodeConnection<int32_t>;
using NodeConnectionU32 = NodeConnection<uint32_t>;
using NodeConnectionF32 = NodeConnection<float>;
using NodeConnectionF32V3 = NodeConnection<float[3]>;

class Node
{
public:
    virtual ~Node();
    virtual void update() = 0;
    
    float posX, posY;
	float width, height;
    bool dirty { true };
    bool selected { false };
    bool mouseClicking { false };
    unsigned int nodeId;
    NodeTypeBase const * type;
    std::string name;
	std::vector<NodeConnectionBase*> inputConnections;
	std::vector<NodeConnectionBase*> outputConnections;
};

template <typename CON_T> class NodeConnectionDesc : public NodeConnectionDescBase
{
public:
    inline NodeConnectionDesc(UniqueId _t, std::string const & _dn)
    : NodeConnectionDescBase(_t, _dn){};
    
    NodeConnectionBase* createConnectionInstance(bool _input) override
    {
        NodeConnectionBase * con = new CON_T(this, _input);
        con->desc = this;
        return con;
    };
};

template <typename NODE_T> class NodeType : public NodeTypeBase
{
public:
    inline NodeType(UniqueId uid, std::string name) : NodeTypeBase(uid, name){};
    Node* createNodeInstance() override
    {
        Node* node = new NODE_T();
        node->type = this;
        return node;
    };
};

class NodeGraphType
{
public:
    NodeGraphType(UniqueId graphId) : unique_id(graphId){};
    
    void registerNodeType(NodeTypeBase* nodeType);

    std::map<UniqueId, NodeTypeBase*> nodeTypeRegistry;
    
    UniqueId unique_id;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum class GTFDragState : uint8_t
{
	DS_DEFAULT,
	DS_HOVER,
	DS_BEGIN_DRAG,
	DS_DRAGING,
	DS_CONNCECT,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct GTFDragNode
{
	ImVec2 pos;
	NodeConnectionBase* con;
};

class NodeGraph
{
public:
    NodeGraph(NodeGraphType* type) : graphType(type){};
    virtual ~NodeGraph();
    virtual Node* createNode(UniqueId typeId, float atX, float atY);
    virtual void updateNodes();
    virtual void updateGUI();
    
    NodeGraphType * graphType { nullptr };
    std::list<Node*> nodeList;
    std::list<Node*> selectedNodes;
    Node* lastClickedNode { nullptr };
    
protected:
    virtual void drawHermite(ImDrawList* drawList, ImVec2 p1, ImVec2 p2, int STEPS);
    virtual bool isConnectorHovered(NodeConnectionBase* c, ImVec2 offset);
    virtual NodeConnectionBase* getHoverCon(ImVec2 offset, ImVec2* pos, bool resetCon = true);
    virtual void updateDraging(ImVec2 offset);
    virtual Node* findNodeByCon(NodeConnectionBase const * findCon);
    virtual void renderLines(ImDrawList* drawList, ImVec2 offset);
    virtual void displayNode(ImDrawList* drawList, ImVec2 offset, Node* node, int& node_selected);
    
    
    
    
    unsigned int nodeCounter {0};
    
    GTFDragNode dragNode;
    GTFDragState dragState { GTFDragState::DS_DEFAULT };
};

namespace NodeGRAPH
{
    static const float GTF_NODE_SLOT_RADIUS = 6.0f;
    static const ImVec2 GTF_NODE_SSELECTED_MARGIN {5.0f, 5.0f};
    static const ImVec2 GTF_NODE_WINDOW_PADDING {8.0f, 8.0f};
};
} //namespace gtf
